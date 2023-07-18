/*
 * msvc.cpp
 *
 */
#include "..\pce.h"





/*
 * memzero
 *   Zero a block of memory.
 *
 */
void pce_fastcall memzero(void* pd, uint sz)
{
	memset(pd, 0, (size_t) sz);
}