/*
 * pce_crt.h
 *
 */
#ifndef PCE_CRT_H
#define PCE_CRT_H
#include "..\pce.h"








/*
 * memzero
 *   Fills a buffer with zeroes.
 *
 * _PARAMETERS_
 *   [in/out] 'pd' -- Pointer to the buffer to zero.
 *   [in]     'sz' -- Size of the buffer pointed to by 'pd, in bytes.
 *
 * _RETURN_
 *   None.
 *
 */
void pce_fastcall memzero(void* pd, uint sz);


/*
 * ixh
 *   Converts an integer to a hexadecimal, ASCII digit.
 *
 * _PARAMETERS_
 *   [in] 'v' -- Integer to be converted.
 *
 * _RETURN_
 *   Returns the hexadecimal character representation of 'v'
 *
 */
char pce_fastcall ixh(uib v);


/*
 * ixd
 *   Converts an integer to a decimal, ASCII digit.
 *
 * _PARAMETERS_
 *   [in] 'v' -- Integer to be converted.
 *
 * _RETURN_
 *   Returns the decimal character representation of 'v'
 *
 */
char pce_fastcall ixd(uib v);


/*
 * dxi
 *   Converts an ASCII decimal digit to an integer.
 *
 * _PARAMETERS_
 *   [in] 'c' -- Digit character to be converted.
 *
 * _RETURN_
 *   Returns the integer representation of 'c'
 *
 */
uib pce_fastcall dxi(char c);


/*
 * hxi
 *   Converts an ASCII hexadecimal digit to an integer.
 *
 * _PARAMETERS_
 *   [in] 'c' -- Digit character to be converted.
 *
 * _RETURN_
 *   Returns the integer representation of 'c'
 *
 */
uib pce_fastcall hxi(char c);


/*
 * uixs
 *   Converts an unsigned integer to a null-terminated ASCII decimal string.
 *
 * _PARAMETERS_
 *   [in/out] 'pd'     - Pointer to a buffer where the resulting string is written.
 *   [in]     'sz'     - Size of the buffer pointed to by 'pd', in bytes.
 *   [in]     'value'  - Value to be converted.
 *
 * _RETURN_
 *   Returns the length of the converted string (excluding the null-terminator)
 *
 *   If the buffer pointed to by 'p' is insufficient to store the converted string the
 *   return value is 0.
 *
 */
uib pce_fastcall uixs(char* pd, uint sz, uint value);


/*
 * uiqxs
 *   Unsigned 64-bit integer to decimal string.
 *
 */
uib pce_fastcall uiqxs(char* pd, uint sz, uiq value);


/*
 * uixsh
 *   Converts an unsigned integer to a null-terminated ASCII hexadecimal string.
 *
 * _PARAMETERS_
 *   [in/out] 'pd'     - Pointer to a buffer where the resulting string is written.
 *   [in]     'sz'     - Size of the buffer pointed to by 'pd', in bytes.
 *   [in]     'value'  - Value to be converted.
 *
 * _RETURN_
 *   Returns the length of the converted string (excluding the null-terminator)
 *
 *   If the buffer pointed to by 'p' is insufficient to store the converted string the
 *   return value is 0.
 *
 */
uib pce_fastcall uixsh(char* pd, uint sz, uint value);



/*
 * ixs
 *   Converts a 32-bit two's complement signed integer to an null-terminated ASCII decimal string.
 *
 * _PARAMETERS_
 *   [in/out] 'pd'     - Pointer to a buffer where the resulting string is written.
 *   [in]     'sz'     - Size of the buffer pointed to by 'pd', in bytes.
 *   [in]     'value'  - Value to be converted.
 *
 * _RETURN_
 *   Returns the length of the converted string (excluding the null-terminator)
 *
 *   If the buffer pointed to by 'pd' is insufficient to store the converted string the
 *   value is 0.
 *
 */
uib pce_fastcall ixs(char* pd, uint sz, sint value);


/*
 * sxui
 *   Converts an ASCII decimal string representing an unsigned integer to a string.
 *
 * _PARAMETERS_
 *   [in] 'p' -- Pointer to the string to be converted.
 *
 * _RETURN_
 *   Returns the integer representation of the string pointed to by 'p'
 *   
 */
uint pce_fastcall sxui(const char* p);


/*
 * shxui
 *   Hexadecimal string to unsigned integer.
 *
 */
uint pce_fastcall shxui(const char* p);


/*
 * sxi
 *   Converts an ASCII decimal string representing a signed integer to a string.
 *
 * _PARAMETERS_
 *   [in] 'p' -- Pointer to the string to be converted.
 *
 * _RETURN_
 *   Returns the integer representation of the string pointed to by 'p'
 *   
 */
sint pce_fastcall sxi(const char* p);


/*
 * ipxs
 *   Converts an IPv4 address to a null-terminated string in dot-decimal notation.
 *
 * _PARAMETERS_
 *   [in/out] 'pd'  - Pointer to a buffer where the resulting string is written.
 *   [in]     'sz'  - Size of the buffer pointed to by 'pd', in bytes.
 *   [in]     'ip4' - IPv4 address in network order.
 *
 * _RETURN_
 *   Returns the length of the converted string (not inclusive of null-terminator)
 *
 *   If the buffer pointed to by 'p' is insufficient to store the converted string the
 *   value is 0.
 *   
 */
uib pce_fastcall ipxs(char* pd, uint sz, uid ip4);


/*
 * fxs
 *   Converts a float to an ASCII string.
 *
 */
uib fxs(char* pd, uint sz, float value);


/*
 * bufxstr
 *   Dump buffer.
 *
 */
uint pce_fastcall bufxstr(char* pd, uint dst_sz, void* ps, uint src_sz);


/*
 * sxip
 *   Converts an IPv4 address in standard dot-decimal notation to it's internal representation.
 *
 * _PARAMETERS_
 *   [in] 'ps' -- Pointer to the IPv4 string.
 *
 * _RETURN_
 *   Returns the address represented by 'ps'.
 *
 */
uid pce_fastcall sxip(const char* ps);


/*
 * strcpyn
 *   Copies a null-terminated string.
 *
 * _PARAMETERS_
 *   [in/out] 'pd' -- Pointer to a buffer which will receive the string.
 *   [in]     'sz' -- Size of the buffer pointed to by 'pd', in bytes.
 *   [in]     'ps' -- Pointer to the string to copy from.
 *
 * _RETURN_
 *   Returns the length of resulting string pointed to by 'pd', in bytes (excluding
 *   the null-terminator)
 *
 * _REMARKS_
 *   Behaviour is undefined if the buffers pointed to by 'pd' and 'ps' overlap.
 *
 */
uint pce_fastcall strcpyn(char* pd, uint sz, const char* ps);


/*
 * strstri
 *   Case-insensitive, null-terminated string search
 *
 * _PARAMETERS_
 *   [in] 'pd' -- Pointer to the string that is to be searched.
 *   [in] 'ps' -- Pointer to the string to find.
 *
 * _RETURN_
 *   If the string is found, the return value is a pointer to the first character
 *   of 'ps' within the string pointed to by 'pd'.
 *
 *   If the string is not found the return value is zero.
 *
 */
char* pce_fastcall strstri(const char* pd, const char* ps);


/*
 * cmpstri
 *   Case-insensitive null-terminated ASCII string comparison.
 *
 * _PARAMETERS_
 *   [in] 'p1' -- Pointer to first string.
 *   [in] 'p2] -- Pointer to second string.
 *
 * _RETURN_
 *   If the given strings are equal the return value is non-zero, elsewise the
 *   return value is zero.
 *
 */
uib cmpstri(const char* p1, const char* p2);


/*
 * strfmtv
 *   Writes a formatted string to a buffer.
 *
 * _PARAMETERS_
 *   [out] 'pd'      -- Pointer to a buffer where the resulting string is written to.
 *   [in]  'sz'      -- Size of the buffer pointed to by 'pd', in bytes.
 *   [in]  'pFormat' -- Format control string.
 *   [in]  'pArgs'   -- Pointer to an argument-list used to format the string.
 *
 * _RETURN_
 *   Returns the length of the resulting string written to the buffer pointed to by
 *  'pd', excluding the null-terminator.
 *
 * _REMARKS_
 *   See 'strfmt'
 *
 */
uint pce_fastcall strfmtv(char* pd, uint sz, const char* pFormat, void* pArgs);


/*
 * strfmt
 *   Writes a formatted string to a buffer.
 *
 * _PARAMETERS_
 *   [out] 'pd'      -- Pointer to a buffer where the resulting string is written to.
 *   [in]  'sz'      -- Size of the buffer pointed to by 'pd', in bytes.
 *   [in]  'pFormat' -- Format control string.
 *   [in]  '...'     -- Supplied arguments used to format the string.
 *
 * _RETURN_
 *   Returns the length of the resulting string written to the buffer pointed to by
 *  'pd', excluding the null-terminator.
 *
 * _REMARKS_
 *   See 'strfmt'
 *
 */
uint pce_cdecl strfmt(char* pd, uint sz, const char* pFormat, ...);


struct Node
{
	Node* pNext;
	Node* pPrev;
};


struct List
{
	Node* pHead;
	Node* pTail;
	uid Count;
};


void ListAppend(List* pList, Node* pNode);
void ListRemove(List* pList, Node* pNode);


// List wrapper
#define LIST_ADD(_pList, _pNode) list_append(&(_pList), (list_node*) _pNode)
#define LIST_REM(_pList, _pNode) list_remove(&(_pList), (list_node*) _pNode)

// list helpers
#define LIST_ITERATE(_lst, _type, _node) \
	for(_type* ple = (_type*) _lst.head, *_ns = NULL; ple ? _ns = (_type*) ((list_node*) ple)->next, (ple = CONTAINING_RECORD(ple, _type, _node)) : 0; ple = _ns) \


// Object pool
template <typename t, uint Size> struct Pool
{
	t* next;
	t obj[Size];
	
	
	t* Allocate()
	{
		t* p = next;
		
		if(p)
			next = *((t**) p);
		
		return p;
	}
	
	void Free(t* p)
	{
		*((t**) p) = next;
		next = p;
	}
	
	void Initialize()
	{
		for(int i = Size-1; i >= 0; i--)
			Free(&obj[i]);
	}
	
	t& operator [](int i)
	{
		return obj[i];
	}
	
	uint operator -(t* p)
	{
		return p-obj;
	}
};


#endif