#ifndef PCE_FREELIST_H
#define PCE_FREELIST_H
#include "..\pce.h"



// Freelist item
struct freelist
{
	freelist* next;

#ifdef PCE_FREELIST_DEBUG
	uint block_sz;
	uint pool_sz;
#endif
};

// Freelist functions
void* freelist_alloc(freelist* fl);
void freelist_free(freelist* fl, void* p);
void freelist_init(freelist* fl, void* pool, uid element_size, uid element_count);
void freelist_init_dbg(freelist* fl, void* pool, uid element_size, uid element_count);

#endif