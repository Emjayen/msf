#include "msf.h"


#pragma pack(1)

struct pdb_info_hdr
{
	u32 ver;
	u32 sig;
	u32 age;
};

#pragma pack()


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



void main()
{
	if(!EnablePrivilege(SE_LOCK_MEMORY_NAME))
		return;

	auto result = MsfOpen(L"C:\\Age.pdb");

	// Open the 'PDB Info Stream'
	pdb_info_hdr* pdb = (pdb_info_hdr*) MsfOpenFile(1, NULL);

	auto r = MsfFlush(NULL);
}