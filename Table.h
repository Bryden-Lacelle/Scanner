/*****************************************************************************************************
 * File Name:		table.h
 * Compiler:		MS Visual Studio 2012
 * Authors:			Bryden Lacelle & Justin Farinaccio
 * Course:			CST8152 - Compilers, Lab Section 011
 * Assignment:		Assignment 2: The Scanner
 * Date:			October 27, 2015
 * Professor:		Svillen Ranev
 * Purpose:			Header file for scanner.c; includes definitions for error/invalid and accepting states, 
					the transition table, accepting state table, and list of keywords, as well as other 
					necessary definitions and typedefs.
 * Function List:	aa_func02();
					aa_func03();
					aa_func05();
					aa_func08();
					aa_func10();
					aa_func12();
					aa_func13()
*****************************************************************************************************/

#ifndef  TABLE_H_
#define  TABLE_H_

#ifndef BUFFER_H_
#include "buffer.h"
#endif

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/* define error and invalid states */
#define ES 12	/* Error state */
#define IS -1	/* Invalid state */

/* State transition table definition */
#define TABLE_COLUMNS 7
#define COL_CHAR	0		/* defintion of column for letters */
#define COL_ZERO	1		/* defintion of column for 0 */
#define COL_1TO7	2		/* defintion of column for digits 1 - 7 */
#define COL_8TO9	3		/* defintion of column for digits 8 - 9 */
#define COL_DCML	4		/* defintion of column for decimal point */
#define COL_PERC	5		/* defintion of column for percent sign */
#define COL_OTHR	6		/* defintion of column for other characters */

/* Transition table - type of states defined in separate table */
int st_table[ ][TABLE_COLUMNS] = {
	/* State 0 */	{1, 6, 4, 4, IS, IS, IS}, 
	/* State 1 */	{1, 1, 1, 1, 2, 3, 2}, 
	/* State 2 */	{IS, IS, IS, IS, IS, IS, IS}, 
	/* State 3 */	{IS, IS, IS, IS, IS, IS, IS}, 
	/* State 4 */	{ES, 4, 4, 4, 7, 5, 5}, 
	/* State 5 */	{IS, IS, IS, IS, IS, IS, IS}, 
	/* State 6 */	{ES, ES, 9, ES, 7, ES, 5}, 
	/* State 7 */	{8, 7, 7, 7, 8, 8, 8}, 
	/* State 8 */	{IS, IS, IS, IS, IS, IS, IS}, 
	/* State 9 */	{ES, 9, 9, ES, ES, 10, 10}, 
	/* State 10 */	{IS, IS, IS, IS, IS, IS, IS}, 
	/* State 11 */	{IS, IS, IS, IS, IS, IS, IS}, 
	/* State 12 */	{IS, IS, IS, IS, IS, IS, IS}, 
	/* State 13 */	{IS, IS, IS, IS, IS, IS, IS}
};

/* Accepting state definitions */
#define ASWR 97	/* accepting state with retract */
#define ASNR 98	/* accepting state with no retract */
#define NOAS 99	/* not accepting state */

/* Accepting state table */
int as_table[ ] = {NOAS, NOAS, ASWR, ASWR, NOAS, ASWR, NOAS, NOAS, ASWR, NOAS, ASWR, ASNR, ASWR};

/* Accepting action function declarations */
Token aa_func02(char*);
Token aa_func03(char*);
Token aa_func05(char*);
Token aa_func08(char*);
Token aa_func10(char*);
Token aa_func12(char*);
Token aa_func13(char*);

/* define new type: pointer to function (of one char * argument) returning Token */
typedef Token (*PTR_AAF)(char *lexeme);

/* Accepting function callback table definition */
PTR_AAF aa_table[ ] = {
	NULL, NULL, *aa_func02, *aa_func03, NULL, *aa_func05, NULL, NULL, *aa_func08, NULL, *aa_func10, NULL, *aa_func12, *aa_func12
};

#define KWT_SIZE 8

/* Keyword lookup table (.AND. and .OR. are not keywords) */
char *kw_table[] = {
	"ELSE",
	"IF",
	"INPUT",
	"OUTPUT",
	"PLATYPUS",
	"REPEAT",
	"THEN",
	"USING"   
};

#endif
