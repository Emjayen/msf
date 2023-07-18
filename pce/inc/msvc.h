/*
 * msvc.h
 *
 */
#ifndef PCE_MSVC_H
#define PCE_MSVC_H
#include <memory.h>
#include <string.h>
#include <intrin.h>



// Architecture identification
#ifdef _M_IX86
#define ARCH_X86
#elif _M_X64
#define ARCH_X64
#else
#error "Unknown architecture"
#endif

// Helper macros
#define COMPILE_ASSERT(label, exp) typedef int _ASSERT_##label[(exp) ? 1 : -1]

// Fixed sized types
typedef signed char sib;
typedef signed short siw;
typedef signed long sid;
typedef signed long long siq;
typedef unsigned char uib;
typedef unsigned short uiw;
typedef unsigned long uid;
typedef unsigned long long uiq;
typedef uib byte;

#ifdef ARCH_X86
typedef unsigned long uiptr;
typedef signed int sint;
#else
typedef unsigned long long uiptr;
typedef signed int sint;
#endif

// Machine native word. Maybe.
typedef size_t uint;

// Fixed sized types
typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef signed long long s64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;


// Calling conventions
#define pce_cdecl  __cdecl
#define pce_stdcall  __stdcall
#ifdef ARCH_X86
#define pce_fastcall __fastcall
#else
#define pce_fastcall __stdcall
#endif

// Intrinsics
#define bswap16(_bswap_v) _byteswap_ushort(_bswap_v)
#define bswap32(_bswap_v) _byteswap_ulong(_bswap_v)
#define bswap64(_bswap_v) _byteswap_uint64(_bswap_v)

#ifdef ARCH_X86
#define bswap bswap32
#else
#define bswap bswap64
#endif

#define bitscan32_msb(_mask, _pidx) _BitScanReverse(_pidx, _mask)
#define bitscan64_msb(_mask, _pidx) _BitScanReverse64(_pidx, _mask)
#define bitscan32_lsb(_mask, _pidx) _BitScanForward(_pidx, _mask)
#define bitscan64_lsb(_mask, _pidx) _BitScanForward64(_pidx, _mask)

#ifdef ARCH_X86
#define bitscan_msb bitscan32_msb
#define bitscan_lsb bitscan32_lsb
#else
#define bitscan_msb bitscan64_msb
#define bitscan_lsb bitscan64_lsb
#endif

// Macro-functions
#define TOUPPER(_c) ((_c) >= 'a' && (_c) <= 'z' ? (_c)-0x20 : (_c))
#define SWAP(_a, _b) (_a)^=(_b)^=(_a)^=(_b)
#define isdec(_c) ((_c) >= '0' && (_c) <= '9')
#define isc_alpha(_c) (((_c) >= 'a' && (_c) <= 'z') || ((_c) >= 'A' && (_c) <= 'Z'))
#define isc_lower(_c) ((_c) >= 'a' && (_c) <= 'z')

#define decxint(_c) ((_c) - '0')
#define intxhex(_v) ((_v) < 10 ? '0' + (_v) : ('A'-10) + (_v))

// Directives
#define ALIGN(_byte_alignment) __declspec(align(_byte_alignment))
#define __likely(_exp)

/*
* Deprecated
*
*/
#ifndef CDECL
#define CDECL  __cdecl
#endif

#ifndef STDCALL
#define STDCALL  __stdcall
#endif
#ifdef ARCH_X86
#ifndef FASTCALL
#define FASTCALL __fastcall
#endif
#else
#ifndef FASTCALL
#define FASTCALL __stdcall
#endif
#endif



/*
 * Compile-time offsetof
 *
 */
#ifdef __cplusplus
template <typename T, typename M> M get_member_type(M T::*);
template <typename T, typename M> T get_class_type(M T::*);

template <typename T,
	typename R,
	R T::*M
>


size_t offset_of()
{
	return reinterpret_cast<size_t>(&(((T*) 0)->*M));
}

#define __offsetof(m) offset_of<decltype(get_class_type(&m)), \
                     decltype(get_member_type(&m)), &m>()


#define __sizeof(_type, _member) sizeof(((_type*) 0)->_member)

#endif

// Absolute value of integer.
#define __abs(_v) (((_v) ^ ((_v) >> (sizeof(_v)*8-1))) - ((_v) >> (sizeof(_v)*8-1)))

// Get sign bit
#define __getsign(_v) ((_v) >> (sizeof((_v)) * 8 - 1))


#endif

