/*
 * pce_crt.cpp
 *
 */
#include "..\inc\crt.h"







uib cmpstri(const char* p1, const char* p2)
{
	while(*p1 && *p2)
	{
		register char c1 = *p1 + (*p1 >= 'A' && *p1 <= 'Z' ? 0x20 : 0);
		register char c2 = *p2 + (*p2 >= 'A' && *p2 <= 'Z' ? 0x20 : 0);

		if(c1 != c2)
			return false;

		p1++;
		p2++;
	}

	if(*p1 || *p2)
		return false;

	return true;
}









/*
 * ixh
 *   Integer to hex digit.
 *
 */
char pce_fastcall ixh(uib v)
{
	const char HEX_TBL[] = "0123456789ABCDEF";

	return HEX_TBL[v & 0x0F];
}


/*
 * ixd
 *   Integer to decimal digit.
 *
 */
char pce_fastcall ixd(uib v)
{
	return ('0' + v);
}


/*
 * dxi
 *   Decimal digit to integer.
 *
 */
uib pce_fastcall dxi(char c)
{
	return (c & 0x0F);
}


/*
 * hxi
 *   Hexadecimal digit to integer.
 *
 */
uib pce_fastcall hxi(char c)
{
	return c & 0x40 ? (c & 0x0F) + 9 : (c & 0x0F);
}


/*
 * strcpyn
 *    Copy string.
 *
 */
uint pce_fastcall strcpyn(char* pd, uint sz, const char* ps)
{
	if(!sz)
		return 0;

	uint len = strlen(ps);

	len = len >= sz ? sz-1 : len;

	memcpy(pd, ps, len);
	pd[len] = '\0';

	return len;
}



/*
 * ipxs
 *
 */
uib pce_fastcall ipxs(char* pd, uint sz, uid ip4)
{
	const uib MIN_BUFFER = (4*3)+(3*1)+1;
	char* ptmp = pd;
	uib o;


	__likely(sz >= MIN_BUFFER);

	if(sz < MIN_BUFFER)
		return 0;

	if(!ip4)
	{
		*((uiq*) pd) = 0x00302E302E302E30;
		return 7;
	}

	o = (ip4&0xFF);

	for(uib i = 0; i < 4; i++)
	{
		if((o / 100))
		{
			*pd++ = '0' + (o/100);
			o -= (o/100)*100;

			*pd++ = '0' + (o/10);
			o -= (o/10)*10;
		}

		else if((o / 10))
		{
			*pd++ = '0' + (o/10);
			o -= (o/10)*10;
		}

		*pd++ = '0' + o;
		*pd++ = '.';

		ip4 >>= 8;
		o = (ip4 & 0xFF);
	}

	*--pd = '\0';

	return (uib) (pd-ptmp);
}


/*
 * sxip
 *
 */
uid pce_fastcall sxip(const char* ps)
{
	union
	{
		uid addr;
		byte addr_octet[4];
	};

	addr = 0;

	for(uib i = 0; i < 4 && *ps; i++)
	{
		byte octet = 0;

		while(*ps && *ps != '.')
			octet = (octet * 10) + dxi(*ps++);

		if(*ps == '.')
			ps++;

		addr_octet[i] = octet;
	}

	return addr;
}


/*
 * fxs
 *   Float to decimal string.
 *
 */
#define FLT_EXPONENT_BITS  8
#define FLT_MANTISSA_BITS  23
#define FLT_EXP_SHIFT  127
#define FLT_IMPLICIT_ONE  (((uid) 1) << FLT_MANTISSA_BITS)
#define FLT_UID(_float) (*((uid*) &_float))
#define FLT_EXP_MASK ((((uid) 1) << FLT_EXPONENT_BITS)-1)

#define FXS_PRECISION_BITS  64
#define FXS_PRECISION_DIGITS  8
#define FXS_EXPONENT(_float) (((sid) ((FLT_UID(_float) >> FLT_MANTISSA_BITS) & FLT_EXP_MASK)) - FLT_EXP_SHIFT)
#define FXS_MANTISSA_NLZ(_float) ((FLT_UID(_float) & (FLT_IMPLICIT_ONE-1)) | FLT_IMPLICIT_ONE)

uib fxs(char* pd, uint sz, float value)
{
	char* p = pd;
	

	// Special cases.
	if((*((uid*) &value) & 0x7F800000)-1 >= (0x7F800000-1))
	{

	}

	// Common
	const sid exp = FXS_EXPONENT(value) - FLT_MANTISSA_BITS;
	const sid shift = FXS_PRECISION_BITS + exp - 4;
	const uiq mantissa = FXS_MANTISSA_NLZ(value);
	

	/*
	 * Convert integer component
	 *
	 */
	uiq integer;

	if(exp > 0)
	{
		integer = mantissa << exp;
	}

	else if(exp < 0)
	{
		if(-exp > FLT_MANTISSA_BITS)
		{
			*p++ = '0';
			*p++ = '.';
			
			goto FRACTION;
		}

		integer = mantissa >> (-exp);
	}

	else
	{
		integer = mantissa;
	}

	p += uiqxs(p, 32, integer);
	*p++ = '.';

FRACTION:
	/*
	 * Convert fractional
	 *
	 */
	if(shift >= FXS_PRECISION_BITS-4 || shift <= 0)
	{
		*p++ = '0';
	}

	else
	{
		uiq fraction = mantissa << shift;
		uib digits = 0;
		char c;


		for(uib i = 0; fraction && i < FXS_PRECISION_DIGITS; i++)
		{
			fraction &= 0x0fffffffffffffffllu;
            fraction *= 10;

			p[i] = (c = '0' + (fraction >> (FXS_PRECISION_BITS-4)));

			if(c)
				digits++;
		}

		p += digits;
	}

	*p = '\0';

	return (p-pd);
}


/*
 * uixs
 *   Unsigned integer to decimal string.
 *
 */
uib pce_fastcall uixs(char* pd, uint sz, uint value)
{
	uib digits = 0;


	__likely(sz > 1);

	if(!sz)
		return 0;

	if(sz < 2)
	{
		*pd = '\0';
		return 0;
	}

	// Fast-path for common case.
	if(value < 10)
	{
		pd[0] = ixd((uib) value);
		pd[1] = '\0';

		return 1;
	}

	sz--;

	while(value && digits < sz)
	{
		*pd++ = '0' + (value % 10);
		value /= 10;
		digits++;
	} pd--;

	uib count = digits/2;

	for(uib i = 0; i < count; i++)
		SWAP(*((pd-digits)+i+1), *(pd-i));

	*(pd+1) = '\0';

	return digits;
}


/*
 * uiqxs
 *   Unsigned 64-bit integer to decimal string.
 *
 */
uib pce_fastcall uiqxs(char* pd, uint sz, uiq value)
{
	uib digits = 0;


	__likely(sz > 1);

	if(!sz)
		return 0;

	if(sz < 2)
	{
		*pd = '\0';
		return 0;
	}

	// Fast-path for common case.
	if(value < 10)
	{
		pd[0] = '0' + ((uib) value);
		pd[1] = '\0';

		return 1;
	}

	sz--;

	while(value && digits < sz)
	{
		*pd++ = '0' + (value % 10);
		value /= 10;
		digits++;
	} pd--;

	uib count = digits/2;

	for(uib i = 0; i < count; i++)
		SWAP(*((pd-digits)+i+1), *(pd-i));

	*(pd+1) = '\0';

	return digits;
}


/*
 * ixs
 *   Signed integer to decimal string.
 *
 */
uib pce_fastcall ixs(char* pd, uint sz, sint value)
{
	__likely(sz > 1);

	if(!sz)
		return 0;

	if(sz < 2)
	{
		*pd = '\0';
		return 0;
	}

	if(__getsign(value))
	{
		*pd = '-';

		return uixs(pd+1, sz-1, ~value+1) + 1;
	}

	return uixs(pd, sz, value);
}


/*
 * iqxs
 *   Signed 64-bit integer to decimal string.
 *
 */
uib pce_fastcall iqxs(char* pd, uint sz, siq value)
{
	__likely(sz > 1);

	if(!sz)
		return 0;

	if(sz < 2)
	{
		*pd = '\0';
		return 0;
	}

	if(value & (((uint) ~0) >> 1))
	{
		*pd = '-';

		return uiqxs(pd+1, sz-1, ~value+1) + 1;
	}

	return uiqxs(pd, sz, value);
}



/*
 * uixsh
 *   Unsigned integer to hexadecimal string.
 *
 */
uib pce_fastcall uixsh(char* pd, uint sz, uint value)
{
	if(!sz)
		return 0;

	if(sz < 2)
	{
		*pd = '\0';
		return 0;
	}

	if(value == 0)
	{
		*((uiw*) pd) = 0x0030;
		return 1;
	}

	const char HEX_TBL[] = "0123456789ABCDEF";
	uid digits = 0;

	// ASSUME: Compiler should be able to optimize this significantly.
	bitscan_msb(value, &digits);

	digits = (digits / 4) + 1;

	__assume(digits <= sizeof(uint)*2);

	if(digits >= sz)
		return 0;

	for(uib i = 1; i <= digits; i++)
	{
		pd[digits-i] = HEX_TBL[(value & 0x0F)];
		value >>= 4;
	}

	pd[digits] = '\0';
	
	return (uib) digits;
}


/*
 * uiqxsh
 *   Unsigned 64-bit integer to hexadecimal string.
 *
 */
uib pce_fastcall uiqxsh(char* pd, uint sz, uiq value)
{
	if(!sz)
		return 0;

	if(sz < 2)
	{
		*pd = '\0';
		return 0;
	}

	if(value == 0)
	{
		*((uiw*) pd) = 0x0030;
		return 1;
	}

	const char HEX_TBL[] = "0123456789ABCDEF";
	uid digits = 0;


	// ASSUME: Compiler should be able to optimize this significantly.
//	bitscan64_msb(value, &digits);

	digits = (digits / 4) + 1;

	__assume(digits <= sizeof(uiq)*2);

	if(digits >= sz)
		return 0;

	for(uib i = 1; i <= digits; i++)
	{
		pd[digits-i] = HEX_TBL[(value & 0x0F)];
		value >>= 4;
	}

	pd[digits] = '\0';
	
	return (uib) digits;
}


/*
 * bufxstr
 *   Dump buffer.
 *
 */
uint pce_fastcall bufxstr(char* __restrict pd, uint dst_sz, void* __restrict ps, uint src_sz)
{
	if(!dst_sz)
		return 0;

	if(dst_sz <= 2 || dst_sz < (src_sz*2)+1)
	{
		*pd = '\0';
		return 0;
	}

	for(uint i = 0; i < src_sz; i++)
	{
		pd[(i*2)+0] = intxhex((((byte*) ps)[i]>>4) & 0x0F);
		pd[(i*2)+1] = intxhex(((byte*) ps)[i] & 0x0F);
	}

	pd[src_sz*2] = '\0';

	return (src_sz*2)+1;
}


/*
 * sxui
 *   Decimal string to unsigned integer.
 *
 */
uint pce_fastcall sxui(const char* p)
{
	uint v = 0;


	while(*p)
	{
		v = (v * 10) + decxint(*p);
		p++;
	}

	return v;
}


/*
 * shxui
 *   Hexadecimal string to unsigned integer.
 *
 */
uint pce_fastcall shxui(const char* p)
{
	uint v = 0;

	while(*p && (*p == '0' || *p == ' '))
		p++;

	while(*p)
	{
		v <<= 4;
		v += hxi(*p++);
	}

	return v;
}


/*
 * sxi
 *  Decimal string to signed integer.
 *
 */
sint pce_fastcall sxi(const char* p)
{
	if(*p == '-')
		return -((sint) sxui(p+1));

	else
		return (sint) sxui(p);
}


/*
 * strfmtv
 *    Write formatted string.
 *
 */
uint pce_fastcall strfmtv(char* __restrict pd, uint sz, const char* __restrict pFormat, void* __restrict pArgs)
{
	char* const pds = pd;

	union
	{
		const byte* __restrict pa;
		const char** __restrict pa_pstr;
		const uib* __restrict pa_uib;
		const sint* __restrict pa_sint;
		const uint* __restrict pa_uint;
		const uiq* __restrict pa_uiq;
		const double* __restrict pa_dbl;
	};

	pa = (byte*) pArgs;

	__likely(sz > 1);

	if(!sz)
		return 0;

	if(sz < 2)
	{
		*pd = '\0';
		return 0;
	}

	sz--;

	while(sz && *pFormat)
	{
		__likely(*pFormat != '%');

		if(*pFormat == '%')
		{
			uint len = 0;
			uint pad_sz = 0;
			char pad_char = ' ';
			uib arg_sz = sizeof(uint);


			pFormat++;

			// Flags
			if(*pFormat == '0')
			{
				pad_char = '0';
				pFormat++;
			}

			// Width
			if(isdec(*pFormat))
			{
				pad_sz = decxint(*pFormat++);

				while(isdec(*pFormat))
				{
					pad_sz *= 10;
					pad_sz += decxint(*pFormat++);
				}

				if(pad_sz > sz)
					break;

				__likely(pad_sz < 8);

				memset(pd, pad_char, pad_sz);
			}

			// Type
			switch(*pFormat)
			{
				case 'u':
					len = uixs(pd, sz, *pa_uint);
					break;

				case 'X':
					len = uixsh(pd, sz, *pa_uint);
					break;

				case 's':
					arg_sz = sizeof(*pa_pstr);
					len = strcpyn(pd, sz, *pa_pstr);
					break;

				case 'd':
					len = ixs(pd, sz, *pa_sint);
					break;

				case 'a':
					len = ipxs(pd, sz, *pa_uint);
					break;

				case 'f':
					len = fxs(pd, sz, (float) *pa_dbl);
					arg_sz = 8;
					break;

				case 'c':
					*pd = *pa;
					len = 1;
					break;

				case '%':
					*pd = '%';
					len = 1;
					break;

				default:
					// Size-qualified types.
					switch(*((uiw*) pFormat))
					{
						case 'uq':
							arg_sz = sizeof(uiq);
							len = uiqxs(pd, sz, *pa_uiq);
							break;

						case 'dq':
							arg_sz = sizeof(uiq);
							len = iqxs(pd, sz, *pa_uiq);
							break;

						case 'Xq':
							arg_sz = sizeof(pa_uiq);
							len = uiqxs(pd, sz, *pa_uiq);
							break;

						default:
							
							// Invalid type.
							goto DONE;
							
					}

					pFormat++;
			}

			pFormat++;

			// Pad to width if specified.
			if(len < pad_sz)
			{
				__likely(pad_sz < 8);

				memmove(pd+(pad_sz-len), pd, len);
				memset(pd, pad_char, pad_sz-len);

				len = pad_sz;
			}

			sz -= len;
			pd += len;
			pa += arg_sz;
		}

		else
		{
			*pd++ = *pFormat++;
			sz--;
		}
	}

	DONE:
	*pd = '\0';

	return (pd-pds);
}



/*
 * strfmt
 *    Write formatted string.
 *
 */
uint pce_cdecl strfmt(char* pd, uint sz, const char* pFormat, ...)
{
	return strfmtv(pd, sz, pFormat, __valist(pFormat));
}


char* pce_fastcall strstri(const char* pd, const char* ps)
{
	while(*pd)
	{
		if(TOUPPER(*pd) == TOUPPER(*ps))
		{
			const char* p1 = pd;
			const char* p2 = ps;

			while(*p1 && *p2)
			{
				if(TOUPPER(*p1) != TOUPPER(*p2))
					break;

				p1++;
				p2++;
			}

			if(!*p2)
				return (char*) pd;
		}

		pd++;
	}

	return 0;
}




void ListAppend(List* pList, Node* pNode)
{
	if(!pList->pHead)
	{
		pList->pHead = pNode;
		pList->pTail = pNode;
		pNode->pPrev = NULL;
	}

	else
	{
		pList->pTail->pNext = pNode;
		pNode->pPrev = pList->pTail;
		pList->pTail = pNode;
	}

	pNode->pNext = NULL;
	pList->Count++;
}



void ListRemove(List* pList, Node* pNode)
{
	if(!pNode->pPrev)
	{
		pList->pHead = pNode->pNext;
	}

	else
	{
		pNode->pPrev->pNext = pNode->pNext;
	}

	if(!pNode->pNext)
	{
		pList->pTail = pNode->pPrev;
	}

	else
	{
		pNode->pNext->pPrev = pNode->pPrev;
	}

	pNode->pNext = NULL;
	pNode->pPrev = NULL;
	pList->Count--;
}