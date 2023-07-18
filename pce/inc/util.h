/*
 * pce_crt.h
 *
 */
#ifndef PCE_UTIL_H
#define PCE_UTIL_H
#include "..\pce.h"






/*
 * b64enc
 *   Encode data into base64.
 *
 * _PARAMETERS_
 *   [out] 'pdst'  -- Pointer to a buffer which will receive the resulting base64 encoded string.
 *   [in] 'dst_sz' -- Size of the buffer pointed to by 'pdst', in bytes.
 *   [in] 'psrc'   -- Pointer to a buffer containing the data to be encoded.
 *   [in] 'len'    -- Length of the input data pointed to by 'psrc', in bytes.
 *
 * _RETURN_
 *   Returns the length of the resulting base64 encoded string, in characters.
 *
 * _NOTES_
 *   The resulting string is not null-terminated.
 *
 */
uint b64enc(void* pd, uint dst_sz, const void* ps, uint len);


/*
 * b64dec
 *   Decode data from base64.
 *
 * _PARAMETERS_
 *   [out] 'pdst'  -- Pointer to a buffer which will receive the resulting decoded data.
 *   [in] 'dst_sz' -- Size of the buffer pointed to by 'pdst', in bytes.
 *   [in] 'psrc'   -- Pointer to a buffer containing the base64 string to be decoded.
 *   [in] 'len'    -- Length of the input data pointed to by 'psrc', in bytes.
 *
 * _RETURN_
 *   Returns the length of the resulting decoded data, in bytes.
 *
 */
uint b64dec(void* pdst, uint dst_sz, const void* psrc, uint len);


#endif