/*
 * pce.h
 *
 */
#ifndef PCE_H
#define PCE_H



// Compiler bootstrap
#ifdef _MSC_VER
#include "inc\msvc.h"
#else
#error "Unsupported compiler"
#endif

// Challenge our assumptions
COMPILE_ASSERT(sib, sizeof(sib) == 1);
COMPILE_ASSERT(siw, sizeof(siw) == 2);
COMPILE_ASSERT(sid, sizeof(sid) == 4);
COMPILE_ASSERT(siq, sizeof(siq) == 8);
COMPILE_ASSERT(uib, sizeof(uib) == 1);
COMPILE_ASSERT(uiw, sizeof(uiw) == 2);
COMPILE_ASSERT(uid, sizeof(uid) == 4);
COMPILE_ASSERT(uiq, sizeof(uiq) == 8);
COMPILE_ASSERT(byte, sizeof(byte) == 1);
COMPILE_ASSERT(uiptr, sizeof(uiptr) == sizeof(void*));


/*
 * Following are (should) be compiler-independent.
 *
 */

// Variable argument-list helpers.
#define __valist(_param) ((void*) (((uiptr) (&(_param))) + sizeof(void*)))

// Round integer up to multiple.
#define ROUND_UP(n, m) ((((n) + (m) - 1) / m) * m)

// Min/max
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

// Ect
#define TRUE 1
#define FALSE 0

// Standard includes
#include "inc\crt.h"
#include "inc\wincom.h"
#include "inc\freelist.h"
#include "inc\lnklist.h"
#include "inc\crypto.h"
#include "inc\util.h"



#endif