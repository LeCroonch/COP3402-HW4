/* $Id: bof.h,v 1.14 2023/11/06 09:36:24 leavens Exp $ */
// FLOAT Binary File Format (for the FLOAT SRM)
#ifndef _BOF_H
#define _BOF_H
#include <stdio.h>
#include <stdint.h>
#include "machine_types.h"

#define MAGIC_BUFFER_SIZE 4

typedef struct { // Field magic should be "FBF" (with the null char)
    char     magic[MAGIC_BUFFER_SIZE];
    address_type text_start_address;  // byte address to start running (PC)
    address_type text_length;         // size of the text section in bytes
    address_type data_start_address;  // byte address of static data (GP)
    address_type ints_length;         // size of int data in bytes
    address_type floats_length;       // size of float data in bytes
    address_type stack_bottom_addr;   // byte address of stack "bottom" (FP)
} BOFHeader;

// a type for Binary Output Files
typedef struct {
    FILE *fileptr;
    const char *filename;
} BOFFILE;

// Open filename for reading as a binary file
// Exit the program with an error if this fails,
// otherwise return the FILE pointer to the open file.
extern BOFFILE bof_read_open(const char *filename);

// Requires: bf is open for reading in binary and has
//           at least BYTES_PER_WORD bytes in it
// Return the next int from bf
extern int_type bof_read_int(BOFFILE bf);

// Requires: bf is open for reading in binary and has
//           at least BYTES_PER_WORD bytes in it
// Return the next float from bf
extern float_type bof_read_float(BOFFILE bf);

// Requires: bf is open for reading in binary and
// buf is of size at least bytes
// Read the given number of bytes into buf and return the number of bytes read
size_t bof_read_bytes(BOFFILE bf, size_t bytes, void *buf);

// Requires: bf is open for reading in binary
// Read the header of bf as a BOFHeader and return that header
// If any errors are encountered, exit with an error message.
extern BOFHeader bof_read_header(BOFFILE);

// Open filename for writing as a binary file
// Exit the program with an error if this fails,
// otherwise return the BOFFILE for it.
extern BOFFILE bof_write_open(const char *filename);

// Requres: bf is open
// Close the given binary file
// Exit the program with an error if this fails.
extern void bof_close(BOFFILE bf);

// Requires: f (which is accessed through filename)
//           is open for writing in binary
//           and the size of buf is at least BYTES_PER_WORD bytes
// Write the given int into bf.
// Exit the program with an error if this fails.
extern void bof_write_int(BOFFILE bf, int_type w);

// Requires: f (which is accessed through filename)
//           is open for writing in binary
//           and the size of buf is at least BYTES_PER_WORD bytes
// Write the given float into bf.
// Exit the program with an error if this fails.
extern void bof_write_float(BOFFILE bf, float_type w);

// Requires: bf is open for writing in binary
//           and the size of buf is at least bytes
// Write the given number of bytes from buf into f.
// Exit the program with an error if this fails.
extern void bof_write_bytes(BOFFILE bf, size_t bytes,
			    const void *buf);

// Requires: bf is open for writing in binary
// Write the given header to f
// Exit the program with an error if this fails.
void bof_write_header(BOFFILE bf, const BOFHeader hdr);
// The following line is for the SRM manual document
// ...
#endif
