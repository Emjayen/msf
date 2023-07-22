/*
 * msf.h
 *
 */
#pragma once
#include <stdint.h>


typedef uint32_t u32;
typedef unsigned char byte;


/*
 * MsfOpen
 *   Open MSF volume.
 * 
 * Parameters:
 *    [in] 'FileName' -- Local file containing an MSF volume.
 * 
 * Return:
 *    Returns non-zero on success; zero on failure.
 * 
 * Remarks:
 *    Only one MSF volume may be operated upon concurrently.
 * 
 */
bool MsfOpen(const char* FileName);



/*
 * MsfOpenFile
 *   Open MSF file ('stream') from volume previously opened with `MsfOpen`
 * 
 * Parameters:
 *   [in]      'idx'    -- The file index.
 *   [in, opt] 'FileSz' -- Receives the size of the file specified by `idx`, in bytes.
 * 
 * Return:
 *   Returns a pointer to a memory region containing the file's contents. Changes to this view
 *   are coherent with the volume only if explicitly flushed (see: `MsfFlush`)
 * 
 */
void* MsfOpenFile(u32 idx, u32* FileSz);



/*
 * MsfFlush
 *   Flushes the MSF volume previously opened with `MsfOpen`, committing any changes to file contents
 *   made through the memory returned by `MsfOpenFile` to disk.
 *
 * Parameters:
 *   [in, opt] 'hOutFile' -- Specifies the file to write the MSF volume to. Omitting will use the same file as used in `MsfOpen`
 *
 * Return:
 *    Returns non-zero on success; zero on failure.
 * 
 * Remarks:
 *    Flushing tears down all current state. All views acquired via `MSfOpenFile` are invalidated.
 *
 */
bool MsfFlush(void* hOutFile);