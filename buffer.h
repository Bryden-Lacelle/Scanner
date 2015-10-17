/*****************************************************************************************************
File Name:			buffer.h
Compiler:			MS Visual Studio 2012	
Author:				Bryden Lacelle 040754048
Course:				CST8152 - Compilers Lab Section 011
Assignment:			Assignment 1: The Buffer
Date:				2015/09/29
Professor:			Svillen Ranev
Purpose:			Header file for the Buffer containg includes, defines, struct declarations and forward declarations
Function List:		b_create ();
					b_addc ();
					b_reset ();
					b_destroy ();
					b_isfull ();
					b_size ();
					b_capacity();
					b_setmark ();
					b_mark ();
					b_mode ();
					b_inc_factor ();
					b_load ();
					b_isempty ();
					b_eob ();
					b_getc ();
					b_print ();
					b_pack ();
					b_rflag ();
					b_retract ();
					b_retract_to_mark ();
					b_getc_offset ();
*****************************************************************************************************/
#ifndef BUFFER_H_
#define BUFFER_H_

/*#pragma warning(1:4001) to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001) to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* constant definitions */
/* You may add your own constant definitions here */
#define R_FAIL_1 -1         /* fail return value */
#define R_FAIL_2 -2         /* fail return value */
#define LOAD_FAIL -2       /* load fail error */
#define SET_R_FLAG 1       /* realloc flag set value */
#define ADD_MODE_MAX 255	/* Maximum interger value of inc factor in additive mode */
#define MULT_MODE_MAX 100	/* Maximum interger value of inc factor in multiplicative mode */
#define ADD_MODE 1			/* integer value of the buffer in additive mode */
#define MULT_MODE -1		/* integer value of the buffer in multiplicative mode */
#define FIXED_MODE 0		/* integer value of the buffer in fixed mode */
#define RESET_R_FLAG 0		/* realloc flag reset or not set value */
#define INC_FACTOR_ERROR 256 /* Error code for an invalid inc factor */
#define B_ISFULL			/* Uses macro version of b_isfull when defined */
#undef B_ISFULL				/* Uses function version of b_isfull when undefined */

#if defined(B_ISFULL) 
#define b_isfull(pBD) (pBD && pBD->addc_offset == pBD->capacity) /* When a buffer exists and addc_offset == capacity the buffer is full **Macro for use within if statements only**/
#endif

/* user data type declarations */
typedef struct BufferDescriptor {
    char *cb_head;   /* pointer to the beginning of character array (character buffer) */
    short capacity;    /* current dynamic memory size (in bytes) allocated to character buffer */
    short addc_offset;  /* the offset (in chars) to the add-character location */
    short getc_offset;  /* the offset (in chars) to the get-character location */
    short mark_offset; /* the offset (in chars) to the mark location */
    char  inc_factor; /* character array increment factor */
    char  r_flag;     /* character array reallocation flag */
    char  mode;       /* operational mode indicator*/
    int   eob;       /* end-of-buffer reached flag */
} Buffer, *pBuffer;
/*typedef Buffer *pBuffer;*/

/* function declarations */
Buffer* b_create (short, char, char);
pBuffer b_addc (pBuffer const, char);
int b_reset (Buffer* const);
void b_destroy (Buffer* const);
#ifndef B_ISFULL
int b_isfull (Buffer* const);
#endif
short b_size (Buffer* const);
short b_capacity(Buffer* const);
char* b_setmark (Buffer* const, short);
short b_mark (Buffer* const);
int b_mode (Buffer* const);
size_t b_inc_factor (Buffer* const);
int b_load (FILE* const, Buffer* const);
int b_isempty (Buffer* const);
int b_eob (Buffer* const);
char b_getc (Buffer* const);
int b_print (Buffer* const);
Buffer* b_pack (Buffer* const);
char b_rflag (Buffer* const);
short b_retract (Buffer* const);
short b_retract_to_mark (Buffer* const);
short b_getc_offset (Buffer* const);
#endif
