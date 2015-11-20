/* ASK SVILLEN ABOUT CHECKING FOR INVALID STD. IS CHECKING FOR SIZE = 0 OR CHEKING FOR NULL BUFFER? 
	Does everytime the symbol table get updated does it copy that change to the global symbol table?*/

/*******************************************************************************
* File Name:		scanner.c
* Compiler:			MS Visual Studio 2012
* Authors:			Bryden Lacelle & Justin Farinaccio
* Course:			CST8152 - Compilers, Lab Section 011
* Assignment:		Assignment 3: The Symbol Table
* Date:				November 20, 2015
* Professor:		Svillen Ranev
* Purpose:			
* Function List:	
*******************************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#define ISSTRING		6				/* 0000000000000110 */
#define STRINGFLAG		6				/* 0000000000000110 */
#define ISINT			4				/* 0000000000000100 */
#define INTFLAG			4				/* 0000000000000100 */
#define ISFLOAT			2				/* 0000000000000010 */ 
#define FLOATFLAG		2				/* 0000000000000010 */
#define UPDATEFLAG		1				/* 0000000000000001 */
#define UFLOATFLAG		65531			/* 1111111111111011 */
#define UINTFLAG		65533			/* 1111111111111101 */
#define SSTRINGFLAG		65535			/* 1111111111111111 */
#define DEFAULT_STATUS	65528			/* 1111111111111000 */

/* project header files */
#include <string.h>
#include "buffer.h"
#include "stable.h"
extern STD sym_table;

/*******************************************************************************
Purpose:			Creates a symbol table
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/11/14
Called Functions:	malloc(), sizeof(), b_create()
Parameters:			int
Return Value:		STD [range?]
Algorithm:			
*******************************************************************************/
STD st_create(int st_size) {

	/* local symbol table descriptor */
	STD s_table;
	/* variable identifier records */
	STVR *temp_s_table_vid_record;
	/* temporary buffer */
	Buffer *temp_buf;

	/* memory allocation for array of STVR with st_size number of elements */
	if(!(temp_s_table_vid_record = (STVR*)malloc(st_size * sizeof(STVR)))) {
		s_table.st_size = 0;
		return s_table;
	};

	/* create self-incrementing buffer */
	if(!(temp_buf = b_create(1, 1, 'a'))) {	// are these parameters correct?
		s_table.st_size = 0;
		return s_table;
	}

	/* initializes pointer to array of STVR */
	s_table.pstvr = temp_s_table_vid_record;
	/* initializes pointer to lexeme buffer */
	s_table.plsBD = temp_buf;

	s_table.st_offset = 0;

	/* set size of local STD structure to st_size */
	s_table.st_size = st_size;

	return s_table;
}

/*******************************************************************************
Purpose:			Add a new lexeme to the symbol table
Author:				Bryden Lacelle & Justin Farinaccio
History/Versions:	Version 1.0, 2015/11/20
Called Functions:	st_lookup(), strlen(), b_addc(), st_incoffset(), b_setmark(), b_rflag()
Parameters:			STD, char*, char, int
Return Value:		int
Algorithm:			Add the lexeme and end-of-string character to the symbol table buffer
					If the memory location of the buffer changes set all plex pointers to new
					memory addresses in the buffer. If the memory address of the buffer is 
					unchanged only set the last plex pointer to it's corisponding lexeme in
					the buffer
*******************************************************************************/
int st_install(STD sym_table, char *lexeme, char type, int line) {
	int i = -1, st_lookup_val, offset = sym_table.plsBD->addc_offset; /* Set offset to the current addc_offset in plsBD */
	char flag = 0;
	/* Check for NULL sym_table */
	if (!sym_table.st_size)
		return R_FAIL_1;
	/* Check for full symbol table */
	if(sym_table.st_offset == sym_table.st_size) { return R_FAIL_1; }
	
	/* Check if the lexeme exists in the sym_table */
	if ((st_lookup_val = st_lookup(sym_table, lexeme)) != R_FAIL_1)		{ return st_lookup_val; }

	/* Add lexeme to sym_table buffer */
	while (++i <= strlen(lexeme)) 
	{ 
		b_addc(sym_table.plsBD, lexeme[i]);
		if (b_rflag(sym_table.plsBD)) /* Check if memory location of buffer changed */
			flag = 1;
	}
	if (flag)
	{
		i = -1;
		while (++i <= sym_table.st_offset)
		{
			offset = 0; /* Set offset to 0 ie. plsBD->cb_head */
			sym_table.pstvr[i].plex = b_setmark(sym_table.plsBD, offset); /* Set new pointer location */ 
			offset += strlen(b_setmark(sym_table.plsBD, offset) + 1); /* Set offset to the start of the next lexeme */
		}
	}
	else
		sym_table.pstvr[sym_table.st_offset].plex = b_setmark(sym_table.plsBD, offset); /* Set plex to the newly added lexeme in the buffer*/
	sym_table.pstvr[sym_table.st_offset].o_line = line;
	sym_table.pstvr[sym_table.st_offset].status_field = DEFAULT_STATUS;
	
	if (type == 'F')
	{ sym_table.pstvr[sym_table.st_offset].status_field |= FLOATFLAG; sym_table.pstvr[sym_table.st_offset].i_value.fpl_val = 0.0; }
	if (type == 'I')
	{ sym_table.pstvr[sym_table.st_offset].status_field |= INTFLAG; sym_table.pstvr[sym_table.st_offset].i_value.int_val = 0; }
	if (type == 'S')
	{ sym_table.pstvr[sym_table.st_offset].status_field |= SSTRINGFLAG; sym_table.pstvr[sym_table.st_offset].i_value.str_offset = -1; }
	
	st_incoffset(); /* Increment st_offset */
	
	/* Return offset of lexeme entered */
	return sym_table.st_offset;
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/11/18
Called Functions:	strcmp()
Parameters:			STD, char*
Return Value:		int
Algorithm:			
*******************************************************************************/
int st_lookup(STD sym_table, char *lexeme) {
	/* begin from last entry in symbol table */
	int i = sym_table.st_offset;

	if(sym_table.st_size == 0)	{ return R_FAIL_1; }

	/* starting from the last entry in the symbol table, compare the current lexeme to each lexeme stored in the character array */
	while(--i >= 0) {
		/* if the lexeme is found, return its offset (from the start of the array of STVR) */
		if(strcmp(sym_table.pstvr[i].plex, lexeme) == 0) {
			return sym_table.st_offset;
		}
	}
	return R_FAIL_1;
}

/*******************************************************************************
Purpose:			
Author:				Bryden Lacelle
History/Versions:	Version 1.0, 2015/11/14
Called Functions:	
Parameters:			
Return Value:		
Algorithm:			
*******************************************************************************/
int st_update_type(STD sym_table, int vid_offset, char v_type) {
	if (!sym_table.st_size) /* Check for not NULL sym_table */
		return R_FAIL_1;
	if ((sym_table.pstvr[vid_offset].status_field & STRINGFLAG) == ISSTRING) /* Return error if type is string */
		return R_FAIL_1;
	if ((sym_table.pstvr[vid_offset].status_field & UPDATEFLAG) == 1) /* Return error if type has been changed already */
		return R_FAIL_1;
	if (v_type == 'F')
		sym_table.pstvr[vid_offset].status_field |= UFLOATFLAG; /* Change type to float and set UPDATEFLAG */
	if (v_type == 'I')
		sym_table.pstvr[vid_offset].status_field |= UINTFLAG; /* Change type to int and set UPDATEFLAG */
	return vid_offset;
}

/*******************************************************************************
Purpose:			Update the initial value of a lexeme
Author:				Bryden Lacelle
History/Versions:	Version 1.0, 2015/11/14
Called Functions:	N/A
Parameters:			STD, int, InitialValue
Return Value:		int
Algorithm:			Set a single lexeme initial value to the passed in value i_value
*******************************************************************************/
int st_update_value(STD sym_table, int vid_offset, InitialValue i_value)
 {
	if (!sym_table.st_size)
		return R_FAIL_1;
	sym_table.pstvr[vid_offset].i_value = i_value;
	return vid_offset;
}

/*******************************************************************************
Purpose:			Get the type of the lexeme
Author:				Bryden Lacelle
History/Versions:	Version 1.0, 2015/11/14
Called Functions:	N/A
Parameters:			STD, int
Return Value:		char
Algorithm:			Use bitwise aritmatic to determine the type of the passed in
					lexeme type and return the type as a char associated with
					the type
*******************************************************************************/
char st_get_type (STD sym_table, int vid_offset) {
	if ((sym_table.pstvr[vid_offset].status_field & STRINGFLAG) == ISSTRING)
		return 'S';
	if ((sym_table.pstvr[vid_offset].status_field & INTFLAG) == ISINT)
		return 'I';
	if ((sym_table.pstvr[vid_offset].status_field & FLOATFLAG) == ISFLOAT)
		return 'F';
	return R_FAIL_1;
}

/*******************************************************************************
Purpose:			Free all allocated memory in the Symbol Table
Author:				Bryden Lacelle
History/Versions:	Version 1.0, 2015/11/20
Called Functions:	b_destroy(), free(), st_setsize();
Parameters:			STD
Return Value:		N/A
Algorithm:			Free pstvr, destroy the sty_table buffer, set the size of
					the sym_table to 0
*******************************************************************************/
void st_destroy(STD sym_table) {
	int i = -1;
	free(sym_table.pstvr);
	b_destroy(sym_table.plsBD);
	st_setsize();
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2105/11/14, 18
Called Functions:	ex(), printf()
Parameters:			STD
Return Value:		int
Algorithm:			
*******************************************************************************/
int st_print(STD sym_table) {
	/* forward declaration */
	int ex(int, int);
	
	/* maximum number of lines of code in the program */
	int max_lines = sym_table.pstvr[sym_table.st_offset - 1].o_line;
	
	/* initial magnitude of max. number of lines */
	int max_line_magnitude = 0;
	
	/* magnitude of current line */
	int current_line_magnitude = 0;
	
	/*  */
	int default_num_spaces = 11;
	
	/*  */
	int i = -1;
	
	/*  */
	int j = -1;

	while((max_lines /= 10) > 0) {
		max_line_magnitude++;
	}

	if(sym_table.plsBD == NULL)	// or size == 0?
		return R_FAIL_1;

	printf("\nSymbol Table\n");
	printf("____________\n\n");
	printf("Line Number Variable Identifier\n");

	
	while(++i < sym_table.st_offset)
	{
		while(sym_table.pstvr[i].o_line > (10 * ex(10, current_line_magnitude) - 1)) 
		{
			++current_line_magnitude;
		}
		while(++j < (max_line_magnitude - current_line_magnitude)) 
		{
			printf(" ");
		}
		j = -1;
		printf("%d", sym_table.pstvr[i].o_line);
		while(--default_num_spaces - max_line_magnitude >= 0) { printf(" "); }
		printf("%s\n", sym_table.pstvr[i].plex);
		default_num_spaces = 11;
	}
	return sym_table.st_offset;
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2105/11/15
Called Functions:	N/A
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
static void st_setsize(void) {
	sym_table.st_size = 0;
}

/*******************************************************************************
Purpose:			
Author:				
History/Versions:	
Called Functions:	
Parameters:			
Return Value:		
Algorithm:			
*******************************************************************************/
static void st_incoffset(void) {
	++sym_table.st_offset;
}

/*******************************************************************************
Purpose:			Stores symbol table in text file
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/11/18
Called Functions:	fopen(), fputc(), strlen(), st_get_type(), fprintf()
Parameters:			STD
Return Value:		int
Algorithm:			
*******************************************************************************/
int st_store(STD sym_table) {
	/* file pointer */
	FILE *fp;
	/* character variable for getting lexeme type */
	char c;
	/* counter */
	unsigned int i = 0;
	
	printf("\nError: The Symbol Table is full - install failed.\n");
	
	/* open designated file for writing, overwriting if it already exists */
	fp = fopen("$stable.out", "w");
	
	/* if file does not exist */
	if(fp == NULL) { return R_FAIL_1; }
	
	/* writes st_size to file */
	fprintf(fp, "%d", sym_table.st_size);
	
	/* for each entry in symbol table... */
	for(i = 0; i < sym_table.st_offset; i++) {
		/* print to file the following: variable record status field (in hexadecimal format); length of lexeme; 
		the lexeme; its line number */
		fprintf(fp, " %X %d %s %d ", sym_table.pstvr[i].status_field, strlen(sym_table.pstvr[i].plex), sym_table.pstvr[i].plex, sym_table.pstvr[i].o_line);
		
		/* finally, print to file the lexeme's initial value, depending on its type */
		c = st_get_type(sym_table, i);
		if(c == 'I')		{ fprintf(fp, "%d", sym_table.pstvr[i].i_value.int_val); }
		else if(c == 'F')	{ fprintf(fp, "%.2f", sym_table.pstvr[i].i_value.fpl_val); }
		else if(c == 'S')	{ fprintf(fp, "%d", sym_table.pstvr[i].i_value.str_offset); }
	}
	printf("\nSymbol Table stored.\n");
	st_setsize();
	return sym_table.st_offset-1;
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/11/20
Called Functions:	
Parameters:			STD, char
Return Value:		int
Algorithm:			
*******************************************************************************/
int st_sort(STD sym_table, char s_order) {
	/*unsigned int i = 0;

	STD copy_pstvr;
	STD 
	
	if(s_order == 'A') {
		for(i = 0; i < sym_table.st_offset - 1; i++) {
			qsort(sym_table.pstvr[i], sym_table.st_offset, sizeof(char*), strcmp(sym_table.pstvr[i].plex, sym_table.pstvr[i+1].plex));
		}
		qsort(sym_table.pstvr[i], sym_table.st_offset, sizeof(char*), strcmp(sym_table.pstvr[i].plex, sym_table.pstvr[0].plex));
	}


	return 1;*/

	return 0;
}

/*******************************************************************************
Purpose:			Calculate t to the power of x
Author:				Bryden Lacelle
History/Versions:	Version 1.0, 2015/10/20
Called Functions:	ex()
Parameters:			int, int
Return Value:		int
Algorithm:			Determine an interger power of an integer
*******************************************************************************/
int ex(int t, int x) { return (x > 0 ? t * ex(t, x-1) : 1); }
