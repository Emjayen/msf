/*
 * msf.cpp
 *
 */
#include "msflib.h"
#include <windows.h>
#include <malloc.h>
#include <stdio.h>





// Constants
#define PAGE_SIZE  0x1000

// MFS constants
#define MSF_HDR_MAGIC  "Microsoft C/C++ MSF 7.00\r\n\x1A\x44\x53\x00\x00\x00"

// LBA
typedef u32 lba;

// Round up byte to block.
#define BLOCK_ROUND_UP(x) (ROUND_UP(x, PAGE_SIZE) / PAGE_SIZE)

// Round integer up to multiple.
#define ROUND_UP(n, m) ((((n) + (m) - 1) / m) * m)

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



// MSF state
static HANDLE hFile;
static ULONG_PTR* BlockPfn; // Translate LBA -> PFN
static LARGE_INTEGER FileSz; // MFS size, in bytes.
static ULONG_PTR MfsSz; // MFS size, in blocks/pages.
static msf_mft* Mft;

struct _MSF_FILE
{
	void* p; /* Pointer to file data. */
	u32 size; /* File size, in bytes. */
} *MsfFile;


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
		return NULL;

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
	for(u32 i = 0; i < Mft->file_num; i++)
	{
		if(MsfFile[i].p && !VirtualFree(MsfFile[i].p, 0, MEM_RELEASE))
			return false;
	}

	if(!VirtualFree(Mft, 0, MEM_RELEASE))
		return false;

	// Map the whole thing into memory again.
	void* VA;

	if(!(VA = VirtualAlloc(NULL, MfsSz * PAGE_SIZE, MEM_RESERVE | MEM_PHYSICAL, PAGE_READWRITE)))
		return false;

	if(!MapUserPhysicalPages(VA, MfsSz, BlockPfn))
		return false;

	DWORD Result;

	return SetFilePointerEx(hFile, { 0 }, NULL, FILE_BEGIN) && WriteFile(hOutFile, VA, MfsSz * PAGE_SIZE, &Result, NULL) && Result == MfsSz * PAGE_SIZE;
}



/*
 * MsfOpen
 *
 */
bool MsfOpen(const char* FileName)
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
	msf_hdr* MsfHdr = (msf_hdr*) MsfMapLba(0);

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

	// Release the MFT runlist and MSF header mapping as it's no longer needed.
	if(!VirtualFree(rl, 0, MEM_RELEASE) || !VirtualFree(MsfHdr, 0, MEM_RELEASE))
		return false;

	// Parse the MFT, mapping each file into contiguous address-space.
	if(!(MsfFile = (_MSF_FILE*) VirtualAlloc(NULL, Mft->file_num * sizeof(_MSF_FILE), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)))
		return false;

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


