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
Called Functions:	calloc(), b_create()
Parameters:			int
Return Value:		STD [range?]
Algorithm:			
*******************************************************************************/
STD st_create(int st_size) {
	STD s_table;
	STVR *temp_s_table_vid_record;
	Buffer* temp_buf;
	// memory allocation for array of STVR with st_size number of elements
	if(!(temp_s_table_vid_record = (STVR*)malloc(st_size * sizeof(STVR)))) 
	{
		s_table.st_size = 0;
		return s_table;
	};
	// create self-incrementing buffer
	if(!(temp_buf = b_create(500, 1, 'a')))
	{	
		s_table.st_size = 0;
		return s_table;
	}
	s_table.pstvr = temp_s_table_vid_record;
	s_table.plsBD = temp_buf;
	s_table.st_offset = 0;
	s_table.st_size = st_size;

	return s_table;
}

/*******************************************************************************
Purpose:			
Author:				Bryden Lacelle & Justin Farinaccio
History/Versions:	Version 1.0, 2015/11/14, 15
Called Functions:	
Parameters:			
Return Value:		
Algorithm:			
*******************************************************************************/
int st_install(STD sym_table, char *lexeme, char type, int line) {
	int i = -1, st_lookup_val, offset = sym_table.plsBD->addc_offset;
	if(sym_table.st_offset == sym_table.st_size) { return R_FAIL_1; }
	if ((st_lookup_val = st_lookup(sym_table, lexeme)) != R_FAIL_1)
	{ return st_lookup_val; }
	while (++i < strlen(lexeme)) { b_addc(sym_table.plsBD, lexeme[i]); } /* Warning Dangling pointers ahead*/
	b_addc(sym_table.plsBD, '\0');
	++i;
	sym_table.pstvr[sym_table.st_offset].plex = (char*) malloc(sizeof(char) * i);
	strcpy(sym_table.pstvr[sym_table.st_offset].plex, sym_table.plsBD->cb_head + offset);
	sym_table.pstvr[sym_table.st_offset].o_line = line;
	sym_table.pstvr[sym_table.st_offset].status_field = DEFAULT_STATUS;
	if (type == 'F')
	{ sym_table.pstvr[sym_table.st_offset].status_field |= FLOATFLAG; sym_table.pstvr[sym_table.st_offset].i_value.fpl_val = 0.0; }
	if (type == 'I')
	{ sym_table.pstvr[sym_table.st_offset].status_field |= INTFLAG; sym_table.pstvr[sym_table.st_offset].i_value.int_val = 0; }
	if (type == 'S')
	{ sym_table.pstvr[sym_table.st_offset].status_field |= SSTRINGFLAG; sym_table.pstvr[sym_table.st_offset].i_value.str_offset = -1; }
	st_incoffset();
	return sym_table.st_offset;
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/11/15
Called Functions:	
Parameters:			
Return Value:		
Algorithm:			
*******************************************************************************/
int st_lookup(STD sym_table, char *lexeme) {
	int i = sym_table.st_offset;
	if(sym_table.st_size == 0) { return R_FAIL_1; }

	while(--i >= 0) {
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
	if (!sym_table.st_size)
		return R_FAIL_1;
	if ((sym_table.pstvr[vid_offset].status_field & STRINGFLAG) == ISSTRING)
		return R_FAIL_1;
	if ((sym_table.pstvr[vid_offset].status_field & UPDATEFLAG) == 1) 
		return R_FAIL_1;
	if (v_type == 'F')
		sym_table.pstvr[vid_offset].status_field |= UFLOATFLAG;
	if (v_type == 'I')
		sym_table.pstvr[vid_offset].status_field |= UINTFLAG;
	return vid_offset;
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
int st_update_value(STD sym_table, int vid_offset, InitialValue i_value) {
	if (!sym_table.plsBD)
		return R_FAIL_1;
	sym_table.pstvr[vid_offset].i_value = i_value;
	return vid_offset;
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
Purpose:			
Author:				Bryden Lacelle
History/Versions:	Version 1.0, 2015/11/14
Called Functions:	
Parameters:			
Return Value:		
Algorithm:			
*******************************************************************************/
void st_destroy(STD sym_table) {
	int i = -1;
	while(++i < sym_table.st_offset)
	{
		free(sym_table.pstvr[i].plex);
	}
	free(sym_table.pstvr);
	b_destroy(sym_table.plsBD);
	st_setsize();
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2105/11/14, 18
Called Functions:	
Parameters:			
Return Value:		
Algorithm:			
*******************************************************************************/
int st_print(STD sym_table) {
	int ex(int, int);
	
	int max_lines = sym_table.pstvr[sym_table.st_offset - 1].o_line;
	int max_line_magnitude = 0;
	int current_line_magnitude = 0;
	int default_num_spaces = 11;
	int i = -1;
	int cntr = -1;

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
		while(++cntr < (max_line_magnitude - current_line_magnitude)) 
		{
			printf(" ");
		}
		cntr = -1;
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
Called Functions:	
Parameters:			
Return Value:		
Algorithm:			
*******************************************************************************/
static void st_setsize(void) {
	sym_table.st_size = 0;
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2105/11/15
Called Functions:	
Parameters:			
Return Value:		
Algorithm:			
*******************************************************************************/
static void st_incoffset(void) {
	++sym_table.st_offset;
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/11/18
Called Functions:	
Parameters:			
Return Value:		
Algorithm:			
*******************************************************************************/
int st_store(STD sym_table) {
	FILE *fp;
	char c;
	unsigned int i = 0;
	printf("\nError: The Symbol Table is full - install failed.\n");
	
	fp = fopen("$stable.out", "w");

	if(fp == NULL) { return R_FAIL_1; }
	
	fprintf(fp, "%d", sym_table.st_size);

	for(i = 0; i < sym_table.st_offset; i++) {
		fprintf(fp, " %X %d %s %d ", sym_table.pstvr[i].status_field, strlen(sym_table.pstvr[i].plex), sym_table.pstvr[i].plex, sym_table.pstvr[i].o_line);
		c = st_get_type(sym_table, i);
		if(c == 'I')		{ fprintf(fp, "%d", sym_table.pstvr[i].i_value.int_val); }
		else if(c == 'F')	{ fprintf(fp, "%.2f", sym_table.pstvr[i].i_value.fpl_val); }
		else if(c == 'S')	{ fprintf(fp, "%d", sym_table.pstvr[i].i_value.str_offset); }
	}
	printf("\nSymbol Table stored.\n");
	return sym_table.st_offset-1;
}

/*******************************************************************************
Purpose:			
Author:				Bryden Lacelle & Justin Farinaccio
History/Versions:	Version 1.0, 2015/11/14
Called Functions:	
Parameters:			
Return Value:		
Algorithm:			
*******************************************************************************/
int st_sort(STD sym_table, char s_order) {
	return 0;
}

/*******************************************************************************
Function Name:		ex
Purpose:		Calculate t to the power of x
In Parameters:		double, int
Out Parameters:		double
Version:		1.0
Author:			Bryden Lacelle
*******************************************************************************/
int ex (int t, int x) { return (x > 0 ? t * ex(t, x-1) : 1); }
