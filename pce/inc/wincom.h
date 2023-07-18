/*
 * pce_wincom.h
 *
 */
#ifndef PCE_WINCOM
#define PCE_WINCOM
#include "..\pce.h"





/*
 * PathGetFile
 *   Retrieves the file name of a path.
 *
 * _PARAMETERS_
 *   [in] 'pPath' -- Pointer to the path.
 *
 * _RETURN_
 *   Returns a pointer to the file-name component of 'pPath'
 *   
 */
char* PathGetFile(const char* pPath);


/*
 * PathGetFileExt
 *   Retrieves the file extension of a path.
 *
 * _PARAMETERS_
 *   [in] 'pPath' -- Pointer to the path.
 *
 * _RETURN_
 *   Returns a pointer to the file-extension of the file specified by'pPath'
 *   excluding the extension deliminator.
 *
 *   If the path does not specify a file with an extension the return value
 *   is NULL.
 *   
 */
char* PathGetFileExt(const char* pPath);


/*
 * PathCreate
 *   Creates a file path.
 *
 * _PARAMETERS_
 *   [in] 'pPath' -- Pointer to the path to create.
 *
 * _RETURN_
 *   If the function succeeds the return value is non-zero, elsewise if the
 *   function fails the return value is zero.
 *   
 */
uib PathCreate(char* pPath);


/*
 * ipstr
 *   Converts an IPv4 address to standard a dotted-decimal string.
 *
 * _PARAMETERS_
 *   [in] 'ip' -- Address to be converted.
 *
 * _RETURN_
 *   Returns a pointer to the converted string.
 *   
 */
char* ipstr(uid ip);


/*
 * IsPrivateAddress
 *   Determines if an IPv4 address is a private address (RFC 1918)
 *
 * _PARAMETERS_
 *   [in] 'Addr' -- Address to be tested.
 *
 * _RETURN_
 *   If the given address is a private address the return value is non-zero,
 *   elsewise the return value is zero.
 *   
 */
uib IsPrivateAddress(uid Addr);

#endif