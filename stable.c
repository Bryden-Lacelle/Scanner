/*******************************************************************************
* File Name:		stable.c
* Compiler:			MS Visual Studio 2012
* Authors:			Bryden Lacelle & Justin Farinaccio
* Course:			CST8152 - Compilers, Lab Section 011
* Assignment:		Assignment 3: The Symbol Table
* Date:				November 20, 2015
* Professor:		Svillen Ranev
* Purpose:			Implement and incorporate a symbol table in PLATYPUS compiler
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
					ex()
*******************************************************************************/

/* project header files */
#include <stdlib.h>
#include <string.h>
#include "buffer.h"
#include "stable.h"

/* extern declaration */
extern STD sym_table;

/*******************************************************************************
Purpose:			Creates a symbol table
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/11/14
Called Functions:	malloc(), sizeof(), b_create()
Parameters:			int
Return Value:		STD
Algorithm:			Sets parameters of new, empty symbol table
*******************************************************************************/
STD st_create(int st_size) {

	/* local symbol table descriptor */
	STD s_table;
	/* variable identifier records */
	STVR *temp_s_table_vid_record;
	/* temporary buffer */
	Buffer *temp_buf;

	/* memory allocation for array of STVR with st_size number of elements */
	if(!(temp_s_table_vid_record = (STVR*)malloc(st_size * sizeof(STVR)))) { /* TRIGGERS WARNING Intentional assignment inside expression */
		s_table.st_size = 0;
		return s_table;
	};

	/* create self-incrementing buffer */
	if(!(temp_buf = b_create(1, 1, 'a'))) { /* TRIGGERS WARNING Intentional assignment inside expression */
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
	while (++i <= strlen(lexeme)) /* TRIGGERS WARNING Intentional signed/unsigned mismatch */
	{ 
		b_addc(sym_table.plsBD, lexeme[i]);
		if (b_rflag(sym_table.plsBD)) /* Check if memory location of buffer changed */
			flag = 1;
	}
	if (flag)
	{
		i = -1;
		offset = 0; /* Set offset to 0 ie. plsBD->cb_head */
		while (++i <= sym_table.st_offset)
		{
			/* TRIGGERS WARNING Intentional conversion */
			sym_table.pstvr[i].plex = b_setmark(sym_table.plsBD, offset); /* Set new pointer location */ 
			/* TRIGGERS WARNING Intentional conversion */
			offset += strlen(b_setmark(sym_table.plsBD, offset)) + 1; /* Set offset to the start of the next lexeme */
		}
	}
	else
		/* TRIGGERS WARNING Intentional conversion */
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
Purpose:			Searches for VIDs in symbol table
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/11/18
Called Functions:	strcmp()
Parameters:			STD, char*
Return Value:		int
Algorithm:			Compares each VID name in symbol table with those in character array
*******************************************************************************/
int st_lookup(STD sym_table, char *lexeme) {
	/* begin from last entry in symbol table */
	int i = sym_table.st_offset;

	if(sym_table.st_size == 0)	{ return R_FAIL_1; }

	/* starting from the last entry in the symbol table, compare the current VID name to each VID name stored in the character array */
	while(--i >= 0) {
		/* if the VID name is found, return its offset (from the start of the array of STVR) */
		if(strcmp(sym_table.pstvr[i].plex, lexeme) == 0) {
			return sym_table.st_offset;
		}
	}
	return R_FAIL_1;
}

/*******************************************************************************
Purpose:			Update the type of a lexeme in the Symbol Table
Author:				Bryden Lacelle
History/Versions:	Version 1.0, 2015/11/14
Called Functions:	N/A
Parameters:			STD, int, char
Return Value:		int
Algorithm:			Change the type of a variable from float to int or from
					int to float. If the type of the variable is a string or
					has already been changed, return an error
*******************************************************************************/
int st_update_type(STD sym_table, int vid_offset, char v_type) {
	/* Check for not NULL sym_table */
	if (!sym_table.st_size)
		return R_FAIL_1;
	/* Return error if type is string */
	if ((sym_table.pstvr[vid_offset].status_field & STRINGFLAG) == ISSTRING)
		return R_FAIL_1;
	/* Return error if type has been changed already */
	if ((sym_table.pstvr[vid_offset].status_field & UPDATEFLAG) == 1)
		return R_FAIL_1;
	/* Change type to float and set UPDATEFLAG */
	if (v_type == 'F')
		sym_table.pstvr[vid_offset].status_field |= UFLOATFLAG;
	/* Change type to int and set UPDATEFLAG */
	if (v_type == 'I')
		sym_table.pstvr[vid_offset].status_field |= UINTFLAG;
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
int st_update_value(STD sym_table, int vid_offset, InitialValue i_value) {
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
Algorithm:			Use bitwise aritmetic to determine the type of the passed in
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
	if(!sym_table.st_size)
		return;
	free(sym_table.pstvr);
	b_destroy(sym_table.plsBD);
	st_setsize();
}

/*******************************************************************************
Purpose:			Prints contents of symbol table to terminal
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/11/18
Called Functions:	ex(), printf()
Parameters:			STD
Return Value:		int
Algorithm:			Calculates number of lines in program as well as the 
					magnitude of the number of lines; prints line numbers of 
					VID names along with the VID names
*******************************************************************************/
int st_print(STD sym_table) {
	/* forward declaration */
	int ex(int, int);
	
	/* maximum number of lines of code in the program */
	int max_lines;
	
	/* initial magnitude of max. number of lines */
	int max_line_magnitude = 0;
	
	/* magnitude of current line */
	int current_line_magnitude = 0;
	
	/* number of spaces between line number and lexeme name */
	int default_num_spaces = DEFAULT_NUM_SPACES;
	
	/* counters */
	int i = -1, j = -1;

	if(!sym_table.st_size)
		return R_FAIL_1;

	/* find the number of lines in the program */
	max_lines = sym_table.pstvr[sym_table.st_offset - 1].o_line;

	/* determine magnitude of number of lines of code */
	while((max_lines /= 10) > 0) {
		max_line_magnitude++;
	}

	printf("\nSymbol Table\n");
	printf("____________\n\n");
	printf("Line Number Variable Identifier\n");

	/* cycles through each element in symbol table */
	while(++i < sym_table.st_offset)
	{
		/* determine the magnitude of the number of lines in the program */
		while(sym_table.pstvr[i].o_line > (10 * ex(10, current_line_magnitude) - 1)) 
		{
			++current_line_magnitude;
		}

		/* print one space before the line number for each order of magnitude less than the maximum */
		while(++j < (max_line_magnitude - current_line_magnitude)) { printf(" "); }
		/* reset */
		j = -1;

		/* print the line number */
		printf("%d", sym_table.pstvr[i].o_line);

		/* print the corresponding number of spaces between the line number and VID name */
		while(--default_num_spaces - max_line_magnitude >= 0) { printf(" "); }

		/* print VID name */
		printf("%s\n", sym_table.pstvr[i].plex);

		/* reset */
		default_num_spaces = DEFAULT_NUM_SPACES;
	}
	return sym_table.st_offset;
}

/*******************************************************************************
Purpose:			Resets size of global symbol table to 0
Author:				Bryden Lacelle & Justin Farinaccio
History/Versions:	Version 1.0, 2015/11/15
Called Functions:	N/A
Parameters:			N/A
Return Value:		N/A
Algorithm:			N/A
*******************************************************************************/
static void st_setsize(void) {
	sym_table.st_size = 0;
}

/*******************************************************************************
Purpose:			Increments offset of global symbol table
Author:				Bryden Lacelle & Justin Farinaccio
History/Versions:	Version 1.0, 205/11/15
Called Functions:	N/A
Parameters:			N/A
Return Value:		N/A
Algorithm:			N/A
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
Algorithm:			Creates file for writing, writes to file the size of the 
					symbol table, as well as the characteristics of each VID
*******************************************************************************/
int st_store(STD sym_table) {
	/* file pointer */
	FILE *fp;
	/* character variable for getting VID type */
	char c;
	/* counter */
	unsigned int i = 0;

	if(!sym_table.st_size) { return R_FAIL_1; }
	
	printf("\nError: The Symbol Table is full - install failed.\n");
	
	/* open designated file for writing, overwriting if it already exists */
	fp = fopen("$stable.out", "w");
	
	/* if file does not exist */
	if(fp == NULL) { return R_FAIL_1; }
	
	/* writes st_size to file */
	fprintf(fp, "%d", sym_table.st_size);
	
	/* for each entry in symbol table... */
	for(i = 0; i < sym_table.st_offset; i++) { /* TRIGGERS WARNING Intentional sign mismatch */
		/* print to file the following: variable record status field (in hexadecimal format); length of VID name; 
		the VID name; its line number */
		fprintf(fp, " %X %d %s %d ", sym_table.pstvr[i].status_field, 
			strlen(sym_table.pstvr[i].plex), sym_table.pstvr[i].plex, sym_table.pstvr[i].o_line);
		
		/* finally, print to file the VID's initial value, depending on its type */
		c = st_get_type(sym_table, i);
		if(c == 'I')		{ fprintf(fp, "%d", sym_table.pstvr[i].i_value.int_val); }
		else if(c == 'F')	{ fprintf(fp, "%.2f", sym_table.pstvr[i].i_value.fpl_val); }
		else if(c == 'S')	{ fprintf(fp, "%d", sym_table.pstvr[i].i_value.str_offset); }
	}
	printf("\nSymbol Table stored.\n");
	fclose(fp);

	/* return number of records in ST */
	return ++i;
}

/*******************************************************************************
Purpose:			Nothing; in other circumstances, would be used to sort lexemes
Author:				Bryden Lacelle & Justin Farinaccio
History/Versions:	Version 1.0, 2015/11/20
Called Functions:	N/A
Parameters:			STD, char
Return Value:		int
Algorithm:			N/A
*******************************************************************************/
int st_sort(STD sym_table, char s_order) { /* TRIGGERS WARNING Default return */
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
