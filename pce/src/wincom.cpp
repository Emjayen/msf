/*
 * pce_wincom.cpp
 *
 */
#include "..\pce.h"
#include <memory.h>
#include <string.h>
#include <intrin.h>
#include <Windows.h>



#define IS_PATH_DELIM(_c) (_c == '\\' || _c == '/')
#define IS_EXT_DELIM(_c) (_c == '.')



char* PathGetFile(const char* pPath)
{
	const char* p = pPath+strlen(pPath);

	while(p >= pPath && !IS_PATH_DELIM(*p))
		p--;

	return (char*) p+1;
}


char* PathGetFileExt(const char* pPath)
{
	const u32 len = strlen(pPath);
	char* p = (char*) pPath + len;

	while(p >= pPath && !IS_EXT_DELIM(*p))
		p--;

	return p < pPath ? (char*) pPath+len : (char*) p+1;
}


uib PathCreate(char* pPath)
{
	char Path[MAX_PATH];
	char Tmp[MAX_PATH];

	GetFullPathName(pPath, MAX_PATH, Path, NULL);

	char* p = Path;
	uid Len;

	while(!IS_PATH_DELIM(*p))
		p++;

	p++;

	while(*p)
	{
		while(*p && !IS_PATH_DELIM(*p))
			p++;

		if(!*p)
			break;

		if(!(Len = p-Path))
			continue;

		memcpy(Tmp, Path, Len);
		Tmp[Len] = '\0';

		if(!CreateDirectory(Tmp, NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
			return FALSE;

		while(*p && IS_PATH_DELIM(*p))
			p++;
	}

	return TRUE;
}

char* ipstr(uid ip)
{
	static char tmp[8][16];
	static uid i;

	if(i == 8)
		i = 0;

	ipxs(tmp[i], 16, ip);

	return tmp[i++];
}


uib IsPrivateAddress(uid Addr)
{
	if((Addr & 0xFF) == 192 && (Addr & 0xFF00) == (168<<8))
		return TRUE;

	if((Addr & 0xFF) == 172 && (Addr & 0xFF00) == (16<<8))
		return TRUE;

	if((Addr & 0xFF) == 127)
		return TRUE;

	if((Addr & 0xFF) == 10)
		return TRUE;

	return FALSE;
}