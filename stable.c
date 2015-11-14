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

/* project header files */
#include "buffer.h"
#include "token.h"
#include "table.h"
#include "stable.h"

/*******************************************************************************
Purpose:			
Author:				
History/Versions:	
Called Functions:	
Parameters:			
Return Value:		
Algorithm:			
*******************************************************************************/
STD st_create(int st_size) {



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
int st_install(STD sym_table, char *lexeme, char type, int line) {



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
Author:				
History/Versions:	
Called Functions:	
Parameters:			
Return Value:		
Algorithm:			
*******************************************************************************/
int st_update_type(STD sym_table, int vid_offset, char v_type) {



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
int st_update_value(STD sym_table, int vid_offset, InitialValue i_value) {



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
char st_get_type (STD sym_table, int vid_offset) {



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
void st_destroy(STD sym_table) {



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
int st_print(STD sym_table) {



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
Author:				
History/Versions:	
Called Functions:	
Parameters:			
Return Value:		
Algorithm:			
*******************************************************************************/
int st_sort(STD sym_table, char s_order) {
	return 0;
}
