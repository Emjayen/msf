/*
 * msf.cpp
 *
 */
#include "pce\pce.h"
#include <windows.h>
#include <stdio.h>




// Constants
#define PAGE_SIZE  0x1000
#define MAX_MSF_FILES  512 /* Maximum number of files within an MSF we support. */

// MFS constants
#define MSF_HDR_MAGIC  "Microsoft C/C++ MSF 7.00\r\n\x1A\x44\x53\x00\x00\x00"

// LBA
typedef u32 lba;

// Round up byte to block.
#define BLOCK_ROUND_UP(x) (ROUND_UP(x, PAGE_SIZE) / PAGE_SIZE)


#pragma pack(1)

// MSF file header
struct msf_hdr
{
	byte magic[0x20];
	u32 block_sz; /* Block size; LBA granularity. */
	lba bitmap; /* LBA of the first volume bitmap. */
	u32 vol_sz; /* Total volume size, in blocks. */
	u32 mft_sz; /* Size of the MFT, in bytes. */
	u32 unk;
	lba mft_rl; /* LBA of the MFT runlist. */
};

// MSF MFT
struct msf_mft
{
	u32 file_num; /* Total number of files. */
	u32 file_sz[]; /* File sizes, in bytes. */
};

#pragma pack()




#define LOG(fmt, ...) printf("\r\n" fmt, __VA_ARGS__)


bool EnablePrivilege(const TCHAR* lpPrivilegeName)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tp;


	if(!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken))
		return false;

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if(!LookupPrivilegeValue(NULL, lpPrivilegeName, &tp.Privileges[0].Luid))
	{
		CloseHandle(hToken);
		return false;
	}

	return AdjustTokenPrivileges(hToken, FALSE, &tp, 0, (PTOKEN_PRIVILEGES) NULL, 0) && GetLastError() == ERROR_SUCCESS;
}


// MSF state
static HANDLE hFile;
static ULONG_PTR* BlockPfn; // Translate LBA -> PFN
static LARGE_INTEGER FileSz; // MFS size, in bytes.
static ULONG_PTR MfsSz; // MFS size, in blocks/pages.
static msf_hdr* MsfHdr;
static msf_mft* Mft;

struct
{
	void* p; /* Pointer to file data. */
	u32 size; /* File size, in bytes. */
} static MsfFile[MAX_MSF_FILES];


/*
 * MfsMapLba
 *
 */
static void* MsfMapLba(lba* LBA, u32 count)
{
	ULONG_PTR* pfn = (ULONG_PTR*) alloca(count * sizeof(ULONG_PTR));
	void* VA;

	if(!(VA = VirtualAlloc(NULL, count * PAGE_SIZE, MEM_RESERVE | MEM_PHYSICAL, PAGE_READWRITE)))
		return NULL;

	for(u32 i = 0; i < count; i++)
		pfn[i] = BlockPfn[LBA[i]];

	if(!MapUserPhysicalPages(VA, count, pfn))
	{
		DWORD err = GetLastError();
		return NULL;
	}

	return VA;
}


static void* MsfMapLba(lba LBA)
{
	return MsfMapLba(&LBA, 1);
}



/*
 * MsfOpenFile
 *
 */
void* MsfOpenFile(u32 idx, u32* FileSz)
{
	if(idx >= Mft->file_num)
		return NULL;

	if(FileSz)
		*FileSz = MsfFile[idx].size;

	return MsfFile[idx].p;
}


/*
 * MsfFlush
 *
 */
bool MsfFlush(HANDLE hOutFile /* [optional] */)
{
	hOutFile = hOutFile ? hOutFile : hFile;

	// Shoot down any current mappings.
	if(!MapUserPhysicalPages(NULL, MfsSz, BlockPfn))
		return false;

	// Map the whole thing into memory again.
	void* VA;

	if(!(VA = VirtualAlloc(NULL, MfsSz * PAGE_SIZE, MEM_RESERVE | MEM_PHYSICAL, PAGE_READWRITE)))
		return false;

	if(!MapUserPhysicalPages(VA, MfsSz, BlockPfn))
		return false;

	DWORD Result;

	return WriteFile(hOutFile, VA, MfsSz * PAGE_SIZE, &Result, NULL) && Result == MfsSz * PAGE_SIZE;
}



/*
 * MsfOpen
 *
 */
bool MsfOpen(const TCHAR* FileName)
{
	if((hFile = CreateFile(FileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL)) == INVALID_HANDLE_VALUE || !GetFileSizeEx(hFile, &FileSz))
		return false;

	MfsSz = ROUND_UP(FileSz.QuadPart, PAGE_SIZE) / PAGE_SIZE;

	if(!(BlockPfn = (ULONG_PTR*) VirtualAlloc(NULL, MfsSz * sizeof(ULONG_PTR), MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE)))
		return false;

	ULONG_PTR Pages = MfsSz;

	if(!AllocateUserPhysicalPages(GetCurrentProcess(), &Pages, BlockPfn) || Pages != MfsSz)
		return false;

	// Map the entire set of physical pages representing the file on disk into memory and dump the file from disk into it.
	void* VA;

	if(!(VA = VirtualAlloc(NULL, MfsSz * PAGE_SIZE, MEM_RESERVE | MEM_PHYSICAL, PAGE_READWRITE)))
		return false;

	if(!MapUserPhysicalPages(VA, MfsSz, BlockPfn))
		return false;

	DWORD Result;

	if(!ReadFile(hFile, VA, MfsSz * PAGE_SIZE, &Result, NULL) || Result != FileSz.QuadPart)
		return false;

	VirtualFree(VA, 0, MEM_RELEASE);

	// Parse the MSF header.
	MsfHdr = (msf_hdr*) MsfMapLba(0);

	// Some basic sanity checks first.
	if(memcmp(MsfHdr->magic, MSF_HDR_MAGIC, sizeof(MsfHdr->magic)) != 0)
	{
		LOG("Malformed MSF; bad header magic.");
		return false;
	}

	if(MsfHdr->block_sz != PAGE_SIZE)
	{
		LOG("Unsupported MSF block size of %u; only page-aligned supported.", MsfHdr->block_sz);
		return false;
	}

	if(MsfHdr->vol_sz * PAGE_SIZE != FileSz.QuadPart)
	{
		LOG("WARNING: MSF volume size does not match file-size; %u != %u", MsfHdr->vol_sz * PAGE_SIZE, (u32) FileSz.QuadPart);
	}

	// Okay now map the MFT runlist.
	lba* rl = (lba*) MsfMapLba(MsfHdr->mft_rl);

	// Now finally map the MFT itself.
	Mft = (msf_mft*) MsfMapLba(rl, BLOCK_ROUND_UP(MsfHdr->mft_sz));

	// Parse the MFT, mapping each file into contiguous address-space.
	rl = (lba*) &Mft->file_sz[Mft->file_num];

	for(u32 i = 0; i < Mft->file_num; i++)
	{
		const auto block_sz = BLOCK_ROUND_UP(Mft->file_sz[i]);

		MsfFile[i].size = Mft->file_sz[i];
		
		if(MsfFile[i].size && MsfFile[i].size != ((u32) -1) && !(MsfFile[i].p = MsfMapLba(rl, block_sz)))
		{
			LOG("Failed to map MSF file");
			return false;
		}

		rl += block_sz;
	}

	return true;
}


#pragma pack(1)

struct pdb_info_hdr
{
	u32 ver;
	u32 sig;
	u32 age;
};

#pragma pack()


void main()
{
	if(!EnablePrivilege(SE_LOCK_MEMORY_NAME))
		return;

	if(!MsfOpen(L"C:\\Age.pdb"))
		LOG("Failed to open input file");

	// Open the 'PDB Info Stream'
	pdb_info_hdr* pdb = (pdb_info_hdr*) MsfOpenFile(1, NULL);
}