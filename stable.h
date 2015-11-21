/*****************************************************************************************************
 * File Name:		stable.h
 * Compiler:		MS Visual Studio 2012
 * Authors:			Bryden Lacelle & Justin Farinaccio
 * Course:			CST8152 - Compilers, Lab Section 011
 * Assignment:		Assignment 3: The Symbol Table
 * Date:			November 20, 2015
 * Professor:		Svillen Ranev
 * Purpose:			Header file for stable.c; includes definitions for SymbolTableDescriptor, 
					SymbolTableVidRecord, and InitialValue, as well as forward declartions for 
					necessary functions.
 * Function List:	st_create()
					st_install()
					st_lookup()
					st_update_type()
					st_update_value()
					st_get_type()
					st_destroy()
					st_print()
					st_setsize()
					st_incoffset()
					st_store()
					st_sort()
*****************************************************************************************************/

#ifndef  STABLE_H_
#define  STABLE_H_

#ifndef BUFFER_H_
#include "buffer.h"
#endif
#define _CRT_SECURE_NO_WARNINGS
#define ISSTRING			6				/* 0000000000000110 */
#define STRINGFLAG			6				/* 0000000000000110 */
#define ISINT				4				/* 0000000000000100 */
#define INTFLAG				4				/* 0000000000000100 */
#define ISFLOAT				2				/* 0000000000000010 */ 
#define FLOATFLAG			2				/* 0000000000000010 */
#define UPDATEFLAG			1				/* 0000000000000001 */
#define UFLOATFLAG			65531			/* 1111111111111011 */
#define UINTFLAG			65533			/* 1111111111111101 */
#define SSTRINGFLAG			65535			/* 1111111111111111 */
#define DEFAULT_STATUS		65528			/* 1111111111111000 */
#define DEFAULT_NUM_SPACES	11				/* default number of spaces for formating output */

typedef union InitialValue {
	int int_val;						/* integer variable initial value */
	float fpl_val;						/* floating-point variable initial value */
	int str_offset;						/* string variable initial value (offset) */
} InitialValue;

typedef struct SymbolTableVidRecord {
	unsigned short status_field;		/* variable record status field*/
	char *plex;							/* pointer to lexeme (VID name) in CA */
	int o_line;							/* line of first occurrence */
	InitialValue i_value;				/* variable initial value */
	size_t reserved;					/* reserved for future use */
} STVR;

typedef struct SymbolTableDescriptor {
	STVR *pstvr;						/* pointer to array of STVR */
	int st_size;						/* size in number of STVR elements */
	int st_offset;						/* offset in number of STVR elements */
	Buffer *plsBD;						/* pointer to the lexeme storage buffer descriptor */
} STD;

extern STD sym_table;

STD st_create(int);
int st_install(STD, char*, char, int);
int st_lookup(STD, char*);
int st_update_type(STD, int, char);
int st_update_value(STD, int, InitialValue);
char st_get_type (STD, int);
void st_destroy(STD);
int st_print(STD);
static void st_setsize(void);
static void st_incoffset(void);
int st_store(STD);
int st_sort(STD, char);

#endif
