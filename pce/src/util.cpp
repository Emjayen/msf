/*
 * pce_crt.cpp
 *
 */
#include "..\inc\util.h"












/*
 * b64enc
 *   Base64 encode
 *
 */
uint b64enc(void* pdst, uint dst_sz, const void* psrc, uint len)
{
	__declspec(align(64)) const byte table[] = 
	{
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
		'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
		'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
		'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
		'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
		'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
		'w', 'x', 'y', 'z', '0', '1', '2', '3',
		'4', '5', '6', '7', '8', '9', '+', '/'
	};

	byte* ps = (byte*) psrc;
	byte* pd = (byte*) pdst;


	while(len >= 3)
	{
		pd[0] = table[ps[0] >> 2];
		pd[1] = table[((ps[0] & 0x03) << 4) | (ps[1] >> 4)]; 
		pd[2] = table[((ps[1] & 0x0F) << 2) | (ps[2] >> 6)];
		pd[3] = table[ps[2] & 0x3F];

		pd += 4;
		ps += 3;
		len -= 3;
	}

	if(len)
	{
		*pd++ = table[ps[0] >> 2];

		if(len == 1)
		{
			*pd++ = table[(ps[0] & 0x03) << 4];
			*pd++ = '=';
		}

		else
		{
			*pd++ = table[((ps[0] & 0x03) << 4) | (ps[1] >> 4)];
			*pd++ = table[(ps[1] & 0x0F) << 2];
		}

		*pd++ = '=';
	}

	return pd - ((byte*) pdst);
}



/*
 * b64dec
 *   Base64 decode
 *
 * Parameters:
 *   [out] 'dst'    -- Pointer to a buffer which will receive the resulting decoded output.
 *   [in]  'dst_sz' -- Size of the buffer pointed to by 'pdst', in bytes.
 *   [in]  'src'    -- Pointer to a buffer containing base64-encoded data.
 *   [in]  'len'    -- Length of the base64 sequence contained in 'src'
 *
 * Return:
 *   If the function is successful, the return value specifies the length of the decoded
 *   data contained in 'dst'
 *
 *   If the function fails, the return value is zero indicating insufficient output buffer-
 *   space.
 * 
 * Remarks:
 *   Assumes correctly a correctly formed base64 sequence as input.
 * 
 */
uint b64dec(void* dst, uint dst_sz, const void* src, uint len)
{
	static const byte table[0x100] =
	{
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  62, 63, 62, 62, 63,
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0,  0,  0,  0,  0,  0,
		0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
		15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0,  0,  0,  0,  63,
		0,  26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
		41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
	};

	byte* pd = (byte*) dst;
	byte* ps = (byte*) src;


	if(len <= 1 || !dst_sz)
		return 0;

	if(ps[len-1] == '=')
	{
		if(ps[len-2] == '=')
			len--;

		len--;
	}

	if(dst_sz < (len*6)>>3)
		return 0;

	while(len >= 4)
	{
		u32 r = ((u32) table[ps[0]] << 18 | table[ps[1]] << 12 | table[ps[2]] << 6 | table[ps[3]]);

		pd[0] = (byte) (r >> 16);
		pd[1] = (byte) (r >> 8);
		pd[2] = (byte) (r);

		pd += 3;
		ps += 4;
		len -= 4;
	}

	if(len)
	{
		u32 r = table[ps[0]] << 18 | table[ps[1]] << 12;

		*pd++ = (byte) (r >> 16);

		if(len & 1)
		{
			*pd++ = (byte) (r >> 8 | table[ps[2]] >> 2);
		}
	}

	return (pd - ((byte*) dst));
}