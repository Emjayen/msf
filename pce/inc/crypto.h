/*
 * pce_crypto.h
 *
 */
#ifndef PCE_CRYPTO_H
#define PCE_CRYPTO_H
#include "..\pce.h"






/*
 * crc16
 *   Computes a standard additive 16-bit CRC
 *
 * _PARAMETERS_
 *   [in] 'pData'  -- Pointer to a buffer containing the data to be checked.
 *   [in] 'Length' -- Length of the buffer pointed to by 'pData', in bytes.
 *
 * _RETURN_
 *   Returns the CRC16 of 'pData'
 *
 */
uiw crc16(void* pData, uid Length);

u32 crc32(void* src, uint len);


/*
 * sha1
 *   Computes a SHA-1 hash.
 *
 * _PARAMETERS_
 *   [in]  'pMessage'  -- Pointer to a buffer containing the data to be hashed.
 *   [in]  'Length'    -- Length of the buffer pointed to by 'pMessage', in bytes.
 *   [out] 'pHash'     -- Pointer to a buffer which will receive the resulting hash.
 *
 * _RETURN_
 *   None.
 *
 */
void sha1(const void* pMessage, uid Length, void* pHash);


/*
 * sha256
 *   Computes a SHA-256 hash.
 *
 * _PARAMETERS_
 *   [in]  'pMessage'  -- Pointer to a buffer containing the data to be hashed.
 *   [in]  'Length'    -- Length of the buffer pointed to by 'pMessage', in bytes.
 *   [out] 'pHash'     -- Pointer to a buffer which will receive the resulting hash.
 *
 * _RETURN_
 *   None.
 *
 */
extern "C" void sha256(void* pMessage, uid Length, void* pHash);


/*
 * murmur
 *   Computes a Murmur hash.
 *
 * _PARAMETERS_
 *   [in]  'pMessage'  -- Pointer to a buffer containing the data to be hashed.
 *   [in]  'Length'    -- Length of the buffer pointed to by 'pMessage', in bytes.
 *   [in]  'Seed'      -- Hash seed.
 *   [out] 'pHash'     -- Pointer to a buffer which will receive the resulting hash.
 *
 * _RETURN_
 *   None.
 *
 */
void murmur32(void* pMessage, uid Length, uid Seed, void* pHash);
void murmur128(void* pMessage, uid Length, uid Seed, void* pHash);




#endif