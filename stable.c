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
#define ISSTRING	6				//0000000000000110
#define STRINGFLAG	6				//0000000000000110
#define ISINT		4				//0000000000000100
#define INTFLAG		4				//0000000000000100
#define ISFLOAT		2				//0000000000000010
#define FLOATFLAG	2				//0000000000000010
#define UPDATEFLAG	1				//0000000000000001
#define UFLOATFLAG	65531			//1111111111111011
#define UINTFLAG	65533			//1111111111111101

/* project header files */
#include "buffer.h"
#include "token.h"
#include "table.h"
#include "stable.h"

STD sym_table;
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

	// memory allocation for array of STVR with st_size number of elements
	if(!(temp_s_table_vid_record = (STVR*)calloc(st_size, sizeof(STVR)))) {
		s_table.st_size = 0;
		return s_table;
	};

	// create self-incrementing buffer
	Buffer *temp_buf;
	if(!(temp_buf = b_create((short)st_size, 0, 'f'))) {	// are these parameters correct?
		s_table.st_size = 0;
		return s_table;
	}
	//temp_buf = b_create((short)st_size, 0, 'f');

	s_table.plsBD = temp_buf;

	s_table.st_offset = 0;

	s_table.st_size = st_size;

	return s_table;

}

/*******************************************************************************
Purpose:			
Author:				
History/Versions:	Version 1.0, 2015/11/14
Called Functions:	
Parameters:			
Return Value:		
Algorithm:			
*******************************************************************************/
int st_install(STD sym_table, char *lexeme, char type, int line) {
	if (st_lookup(sym_table, lexeme) == R_FAIL_1)
	{ /*TO-DO*/ }
	//sym_table.st_offset = sym_table.plsBD->getc_offset;
	sym_table.pstvr->plex = lexeme;
	sym_table.pstvr->o_line = line;


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
int st_lookup(STD sym_table, char *lexeme) {



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
	if (sym_table.pstvr[vid_offset].status_field & STRINGFLAG == ISSTRING)
		return R_FAIL_1;
	if ((sym_table.pstvr[vid_offset].status_field & UPDATEFLAG) == 1) 
		return R_FAIL_1;
	if (v_type = 'F')
		sym_table.pstvr[vid_offset].status_field |= UFLOATFLAG;
	if (v_type = 'I')
		sym_table.pstvr[vid_offset].status_field |= UINTFLAG;
	return vid_offset;

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
	if (sym_table.pstvr[vid_offset].status_field & STRINGFLAG == ISSTRING)
		return 'S';
	if (sym_table.pstvr[vid_offset].status_field & INTFLAG == ISINT)
		return 'I';
	if (sym_table.pstvr[vid_offset].status_field & FLOATFLAG == ISFLOAT)
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
	int i = sym_table.st_size;
	while(--i >= 0)
	{
		free(sym_table.pstvr[i].plex);
	}
	free(sym_table.pstvr);
	b_destroy(sym_table.plsBD);
}

/*******************************************************************************
Purpose:			
Author:				Bryden Lacelle & Justin Farinaccio
History/Versions:	Version 1.0, 2105/11/14
Called Functions:	
Parameters:			
Return Value:		
Algorithm:			
*******************************************************************************/
int st_print(STD sym_table) {
	sym_table.pstvr[0].o_line

	if(sym_table.plsBD == NULL)	// or size == 0?
		return R_FAIL_1;

	printf("Symbol Table\n");
	printf("____________\n");
	printf("____________\n");
	printf("Line Number   Variable Identifier\n");

	printf("%d %s\n", sym_table.pstvr->o_line, sym_table.pstvr->plex);
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
static void st_setsize(void) {



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
int st_store(STD sym_table) {



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