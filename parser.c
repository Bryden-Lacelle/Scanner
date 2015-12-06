/*******************************************************************************
* File Name:		parser.c
* Compiler:			MS Visual Studio 2012
* Authors:			Bryden Lacelle & Justin Farinaccio
* Course:			CST8152 - Compilers, Lab Section 011
* Assignment:		Assignment 4: The Parser
* Date:				December 11, 2015
* Professor:		Svillen Ranev
* Purpose:			Implement a parser
* Function List:	parser()
					match()
*******************************************************************************/

/* project header files */
#include <stdlib.h>
#include <string.h>
#include "buffer.h"
#include "token.h"
#include "table.h"
#include "stable.h"
#include "parser.h"

/* extern declaration */
extern Token lookahead;
extern Buffer *sc_buf;
extern int synerrno;

/*******************************************************************************
Purpose:			
Author:				Svillen Ranev
History/Versions:	Version 1.0, 2015/11/09
Called Functions:	mlwpar_next_token(), program(), match(), gen_incode()
Parameters:			Buffer*
Return Value:		N/A
Algorithm:			
*******************************************************************************/
void parser(Buffer *in_buf) {
	sc_buf = in_buf;
	lookahead = mlwpar_next_token(sc_buf);
	program(); match(SEOF_T, NO_ATTR);
	gen_incode("PLATY: Source file parsed");
}

/*******************************************************************************
Purpose:			
Author:				
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			int, int
Return Value:		N/A
Algorithm:			
*******************************************************************************/
void match(int pr_token_code, int pr_token_attribute) {

}

/*******************************************************************************
Purpose:			
Author:				
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			int
Return Value:		N/A
Algorithm:			
*******************************************************************************/
void syn_eh(int sync_token_code) {

}

/*******************************************************************************
Purpose:			
Author:				Svillen Ranev
History/Versions:	Version 1.0, 2015/11/09
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/* Parser error printing function, Assignmet 4, F15 */
void syn_printe() {
	Token t = lookahead;

	printf("PLATY: Syntax error:  Line:%3d\n", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch(t.code) {
		case  ERR_T:	/* ERR_T	0	Error token */
			printf("%s\n", t.attribute.err_lex);
			break;
		case  SEOF_T:	/* SEOF_T	1	Source end-of-file token */
			printf("NA\n");
			break;
		case  AVID_T:	/* AVID_T	2	Arithmetic Variable identifier token */
		case  SVID_T :	/* SVID_T	3	String Variable identifier token */
			printf("%s\n", sym_table.pstvr[t.attribute.get_int].plex);
			break;
		case  FPL_T:	/* FPL_T	4	Floating point literal token */
			printf("%5.1f\n", t.attribute.flt_value);
			break;
		case INL_T:		/* INL_T	5	Integer literal token */
			printf("%d\n", t.attribute.get_int);
			break;
		case STR_T:		/* STR_T	6	String literal token */
			printf("%s\n", b_setmark(str_LTBL, t.attribute.str_offset));
			break;

		case SCC_OP_T:	/*			7	String concatenation operator token */
			printf("NA\n");
			break;

		case  ASS_OP_T:	/* ASS_OP_T	8	Assignment operator token */
			printf("NA\n");
			break;
		case  ART_OP_T:	/* ART_OP_T	9	Arithmetic operator token */
			printf("%d\n", t.attribute.get_int);
			break;
		case  REL_OP_T:	/* REL_OP_T	10	Relational operator token */ 
			printf("%d\n", t.attribute.get_int);
			break;
		case  LOG_OP_T:	/* LOG_OP_T	11	Logical operator token */
			printf("%d\n", t.attribute.get_int);
			break;

		case  LPR_T:	/* LPR_T	12	Left parenthesis token */
			printf("NA\n");
			break;
		case  RPR_T:	/* RPR_T	13	Right parenthesis token */
			printf("NA\n");
			break;
		case LBR_T:		/*			14	Left brace token */
			printf("NA\n");
			break;
		case RBR_T:		/*			15	Right brace token */
			printf("NA\n");
			break;

		case KW_T:		/*			16	Keyword token */
			printf("%s\n", kw_table[t.attribute.get_int]);
			break;

		case COM_T:		/*			17	Comma token */
			printf("NA\n");
			break;
		case EOS_T:		/*			18	End of statement *(semi - colon) */
			printf("NA\n");
			break; 		
		default:
			printf("PLATY: Scanner error: invalid token code: %d\n", t.code);
	} /*end switch*/
}/* end syn_printe()*/

/*******************************************************************************
Purpose:			
Author:				
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			char*
Return Value:		N/A
Algorithm:			
*******************************************************************************/
void gen_incode(char *c) {
	
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/12/05
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/*
<program>  ->
  	PLATYPUS {<opt_statements>}

FIRST(program) = {KW_T(PLATYPUS)}
*/
void program(void) {
	match(KW_T, PLATYPUS); match(LBR_T, NO_ATTR); opt_statements();
	match(RBR_T, NO_ATTR);
	gen_incode("PLATY: Program parsed");
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/12/05
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/*
<opt_statements> ->
	<statements> | e

FIRST(opt_statements) = {AVID_T, SVID_T, KW_T(IF), KW_T(USING), KW_T(INPUT), KW_T(OUTPUT), e}
*/
void opt_statements(void) {
	switch(lookahead.code)
	{
	case AVID_T:
	case SVID_T:
		statements();
		break;
	case KW_T:
		if(lookahead.attribute.get_int != PLATYPUS
			&& lookahead.attribute.get_int != ELSE
			&& lookahead.attribute.get_int != THEN
			&& lookahead.attribute.get_int != REPEAT) {
			statements();
			break;
		}
	default:
		gen_incode("PLATY: Opt_statements parsed");
	}
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/12/05
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/*
<statements> ->
	<statement><statements_p>

FIRST(statements) = {AVID_T, SVID_T, KW_T(IF), KW_T(USING), KW_T(INPUT), KW_T(OUTPUT)}
*/
void statements(void) {
	statement();
	gen_incode("PLATY: Statements parsed");
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/12/05
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/*
<statement> ->
	  <assignment statement>
	| <selection statement>
	| <iteration statement>
 	| <input statement>
	| <output statement>

FIRST(statement) = {AVID_T, SVID_T, KW_T(IF), KW_T(USING), KW_T(INPUT), KW_T(OUTPUT)}
*/
void statement(void) {
	switch(lookahead.code)
	{
	case AVID_T:
		match(AVID_T, lookahead.attribute.vid_offset);
		break;
	case SVID_T:
		match(SVID_T, lookahead.attribute.vid_offset);
		break;
	case KW_T:
		if(lookahead.attribute.get_int == IF) {
			match(KW_T, IF);
			break;
		}
		else if(lookahead.attribute.get_int == USING) {
			match(KW_T, USING);
			break;
		}
		else if(lookahead.attribute.get_int == INPUT) {
			match(KW_T, INPUT);
			break;
		}
		else if(lookahead.attribute.get_int == OUTPUT) {
			match(KW_T, OUTPUT);
			break;
		}
	default:
		syn_printe();
		break;
	}
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/12/05
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/*
<statements_p> ->
	<statement><statements_p>
	| e

FIRST(statements_p) = {AVID_T, SVID_T, KW_T(IF), KW_T(USING), KW_T(INPUT), KW_T(OUTPUT), e}
*/
void statements_p(void) {
	switch(lookahead.code)
	{
	case AVID_T:
	case SVID_T:
		assignment_statement();	// how to deal with these??
		selection_statement();
		iteration_statement();
		input_statement();
		output_statement();
	case KW_T:
		if(lookahead.attribute.get_int == IF
			|| lookahead.attribute.get_int == USING
			|| lookahead.attribute.get_int == INPUT
			|| lookahead.attribute.get_int == OUTPUT) {
			assignment_statement();
			selection_statement();
			iteration_statement();
			input_statement();
			output_statement();
		}
	default:
		gen_incode("PLATY: Statment parsed");
		break;
	}
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/12/05
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/*
<assignment statement> -> 
	<assignment expression>;

FIRST(assignment statement) = {AVID_T, SVID_T}
*/
void assignment_statement(void) {
	assignment_expression();
	match(EOS_T, NO_ATTR);
	gen_incode("PLATY: Assignment statement parsed");
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/12/05
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/*
<assignment expression> ->
	  AVID = <arithmetic expression>
	| SVID = <string expression>

FIRST(assignment expression) = {AVID_T, SVID_T}
*/
void assignment_expression(void) {
	switch(lookahead.code)
	{
	case AVID_T:
		match(AVID_T, lookahead.attribute.vid_offset);
		break;
	case SVID_T:
		match(SVID_T, lookahead.attribute.vid_offset);
		break;
	}
	gen_incode("PLATY: Assignment expression parsed");
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/12/05
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/*
<input statement> ->
	INPUT (<variable list>);

FIRST(input statement) = {KW_T(INPUT)}
*/
void input_statement(void) {
	match(KW_T, INPUT); match(LPR_T, NO_ATTR); variable_list();
	match(RPR_T, NO_ATTR); match(EOS_T, NO_ATTR);
	gen_incode("PLATY: Input statement parsed");
}

