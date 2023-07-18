#include "..\inc\freelist.h"

#ifdef PCE_FREELIST_DEBUG
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif




void* freelist_alloc(freelist* fl)
{
	void* p;


	if(!(p = fl->next)) return 0;

#ifdef PCE_FREELIST_DEBUG
	if(fl->block_sz)
	{
		DWORD r;
		if(!VirtualProtect(p, fl->block_sz, PAGE_READWRITE, &r))
			__asm int 3
	}
#endif 

	fl->next = ((freelist*) p)->next;

	*((void**) p) = 0;

	return p;
}

void freelist_free(freelist* fl, void* p)
{
	((freelist*) p)->next = fl->next;
	fl->next = ((freelist*) p);

#ifdef PCE_FREELIST_DEBUG
	if(fl->block_sz)
	{
		DWORD r;
		if(!VirtualProtect(p, fl->block_sz, PAGE_NOACCESS, &r))
			__asm int 3
	}
#endif
}

void freelist_init(freelist* fl, void* pool, uid element_size, uid element_count)
{
#ifdef PCE_FREELIST_DEBUG
	fl->block_sz = 0;
	fl->pool_sz = 0;
#endif

	for(int i = element_count-1; i >= 0; i--)
	{
		freelist_free(fl, &((byte*) pool)[element_size*i]);
	}

}

#ifdef PCE_FREELIST_DEBUG

void freelist_init_dbg(freelist* fl, void* pool, uid element_size, uid element_count)
{
	fl->block_sz = element_size;
	fl->pool_sz = element_count;
	
	for(int i = element_count-1; i >= 0; i--)
	{
		freelist_free(fl, &((byte*) pool)[element_size*i]);
	}
}

#endif
