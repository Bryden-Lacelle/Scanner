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
		gen_incode("PLATY: Statement parsed");
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
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/*
<selection statement> ->
  IF (<conditional expression>)  THEN  <opt_statements> 
  ELSE { <opt_statements> } ;

FIRST(selection statement) = {KW_T(IF)}
*/

void selection_statement(void) {
	match(KW_T, IF); match(LPR_T, NO_ATTR); conditional_expression();
	match(KW_T, RPR_T); match(KW_T, THEN); opt_statements();
	match(KW_T, ELSE); match(LBR_T, NO_ATTR); opt_statements();
	match(RBR_T, NO_ATTR); match(EOS_T, NO_ATTR);
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/*
<iteration statement> ->
USING  (<assignment expression> , <conditional expression> , <assignment  expression> )
REPEAT {
     <opt_statements>
	};

FIRST(iteration statement) = { KW_T(USING)}
*/

void iteration_statement(void) {
	match(KW_T, USING); match(LPR_T, NO_ATTR); assignment_expression();
	match(COM_T, NO_ATTR); conditional_expression(); match(COM_T, NO_ATTR);
	assignment_expression(); match(RPR_T, NO_ATTR);
	match(KW_T, REPEAT); match(LBR_T, NO_ATTR); opt_statements();
	match(RBR_T, NO_ATTR); match(EOS_T, NO_ATTR);
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

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/*
<variable list> ->
	<variable identifier><variable list_p>

FIRST(variable list) = {AVID_T, SVID_T}
*/
void variable_list(void) {
	variable_identifier(); variable_list_p();
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/*
<variable identifier> ->
	AVID_T | SVID_T

FIRST(variable identifier) = {AVID_T, SVID_T}
*/
void variable_identifier(void) {
	switch(lookahead.code)
	{
	case AVID_T:
		match(AVID_T, lookahead.attribute.vid_offset);
	case SVID_T:
		match(SVID_T, lookahead.attribute.vid_offset);
	default:
		syn_printe();
	}
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/*
<variable list_p> ->
	, <variable identifier><variable list_p>
	| e

FIRST(variable list_p) = {, , e}
*/
void variable_list_p(void) {
	switch(lookahead.code)
	{
	case COM_T:
		variable_identifier();
	default:
		gen_incode("PLATY: Variable list parsed");
		break;
	}
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/*
<output statement> ->
	OUTPUT (<output list>);

FIRST(output statement) = {KW_T(OUTPUT)}
*/
void output_statement(void) {
	match(KW_T, OUTPUT); match(LPR_T, NO_ATTR); output_list();
	match(RPR_T, NO_ATTR); match(EOS_T, NO_ATTR);
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/*
<output list> ->
	<variable list>
	| STR_T
	| e

FIRST(output list) = {, , STR_T, e}
*/
void output_list(void) {
	switch(lookahead.code)
	{
	case COM_T:
		variable_identifier();
	case STR_T:
		match(STR_T, lookahead.attribute.str_offset);
	default:
		gen_incode("PLATY: Output list parsed");
		break;
	}
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/*
<arithmetic expression> - >
	  <unary arithmetic expression>  
	| <additive arithmetic expression>

FIRST(arithmetic expression) = {+, -, AVID_T, FPL_T, INL_T, ( }
*/
void arithmetic_expression(void) {
	switch(lookahead.code)
	{
	case ART_OP_T:
	case AVID_T:
	case FPL_T:
	case INL_T:
	case LPR_T:
		variable_identifier();
	default:
		syn_printe();
	}
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/*
<unary arithmetic expression> ->
	  -  <primary arithmetic expression> 
	| + <primary arithmetic expression>

FIRST(unary arithmetic expression) = {+, -}
*/
void unary_arithmetic_expression(void) {
	switch(lookahead.code)
	{
	case ART_OP_T:
		primary_arithmetic_expression();
	default:
		syn_printe();
	}
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/*
<additive arithmetic expression> ->
	  <multiplicative arithmetic expression><additive arithmetic expression_p>

FIRST(additive arithmetic expression) = {AVID_T, FPL_T, INL_T, ( }
*/

void additive_arithmetic_expression(void) {
	multiplicative_arithmetic_expression();
	multiplicative_arithmetic_expression_p();
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/*
<additive arithmetic expression_p> ->
	  + <multiplicative arithmetic expression><additive arithmetic expression_p>
	| - <multiplicative arithmetic expression><additive arithmetic expression_p>
	| e

FIRST(additive arithmetic expression_p) = {+, -, e}
*/

void additive_arithmetic_expression_p(void) {
	switch(lookahead.code)
	{
	case ART_OP_T:
		multiplicative_arithmetic_expression();
	default:
		gen_incode("PLATY: Additive arithmetic expression_p parsed");
		break;
	}
}

/*******************************************************************************
Purpose:			
Author:				
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/*
<multiplicative arithmetic expression> ->
	<primary arithmetic expression><multiplicative arithmetic expression_p>

FIRST(multiplicative arithmetic expression) = {AVID_T, FPL_T, INL_T, ( }
*/
void multiplicative_arithmetic_expression(void) {
	switch(lookahead.code)
	{
	case AVID_T:
	case FPL_T:
	case INL_T:
	case LPR_T:
		multiplicative_arithmetic_expression();
	default:
		syn_printe();
	}
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/*
<multiplicative arithmetic expression_p> ->
	  * <primary arithmetic expression><multiplicative arithmetic expression_p>
	| / <primary arithmetic expression><multiplicative arithmetic expression_p>
	| e

FIRST(multiplicative arithmetic expression_p) = {*, /, e}
*/
void multiplicative_arithmetic_expression_p(void) {
	switch(lookahead.code)
	{
	case ART_OP_T:
		primary_arithmetic_expression();
	default:
		gen_incode("PLATY: Multiplicative arithmetic expression_p parsed");
		break;
	}
}

/*******************************************************************************
Purpose:			
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/*
<primary arithmetic expression> ->
	  AVID_T
	| FPL_T
	| INL_T
	| (<arithmetic expression>)

FIRST(primary arithmetic expression) = {AVID_T, FPL_T, INL_T, ( }
*/
void primary_arithmetic_expression(void) {
	switch(lookahead.code)
	{
	case AVID_T:
		match(AVID_T, lookahead.attribute.vid_offset);
	case FPL_T:
		match(FPL_T, lookahead.attribute.vid_offset);
	case INL_T:
		match(INL_T, lookahead.attribute.vid_offset);
	case LPR_T:
		arithmetic_expression();
	default:
		syn_printe();
	}
}

/*******************************************************************************
Purpose:			
Author:				
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/*<string expression> ->
	<primary string expression><string expression_p>

FIRST(string expression) = {SVID_T, STR_T}*/

void string_expression(void);

/*******************************************************************************
Purpose:			
Author:				
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/* <string expression_p> ->
	# <primary string expression><string expression_p>
	| e

FIRST(string expression_p) = {#, e} */

void string_expression_p(void);

/*******************************************************************************
Purpose:			
Author:				
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/* <primary string expression> ->
  SVID_T
| STR_T

FIRST(primary string expression) = {SVID_T, STR_T}*/

void primary_string_expression(void);

/*******************************************************************************
Purpose:			
Author:				
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/* <logical OR expression> ->
	<logical AND expression><logical OR expression_p>

FIRST(logical OR expression) = {AVID_T, FPL_T, INL_T, SVID_T, STR_T}*/

void conditional_expression(void);

/*******************************************************************************
Purpose:			
Author:				
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/*<logical OR expression> ->
	<logical AND expression><logical OR expression_p>

FIRST(logical OR expression) = {AVID_T, FPL_T, INL_T, SVID_T, STR_T}*/

void logical_OR_expression(void);

/*******************************************************************************
Purpose:			
Author:				
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/* <logical OR expression_p> ->
	.OR. <logical AND expression><logical OR expression_p>
	| e

FIRST(logical OR expression_p) = {.OR., e}*/

void logical_OR_expression_p(void);

/*******************************************************************************
Purpose:			
Author:				
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/* <logical AND expression> ->
	<relational expression><logical AND expression_p>

FIRST(logical AND expression) = {AVID_T, FPL_T, INL_T, SVID_T, STR_T}*/
void logical_AND_expression(void);

/*******************************************************************************
Purpose:			
Author:				
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/*<logical AND expression_p> ->
	.AND. <relational expression><logical AND expression_p>
	| e

FIRST(logical AND expression_p) = {.AND., e}*/

void logical_AND_expression_p(void);

/*******************************************************************************
Purpose:			
Author:				
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/* <relational expression> ->
	<primary a_relational expression><primary a_relational expression_p>
	| <primary s_relational expression><primary s_relational expression_p>

FIRST(relational expression) = {AVID_T, FPL_T, INL_T, SVID_T, STR_T}*/

void relational_expression(void);

/*******************************************************************************
Purpose:			
Author:				
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/*   AVID_T
	| FPL_T
	| INL_T

FIRST(primary a_relational expression) = {AVID_T, FPL_T, INL_T}*/

void primary_a_relational_expression(void);

/*******************************************************************************
Purpose:			
Author:				
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/* <primary a_relational expression_p> ->
	== <primary a_relational expression>
	| <> <primary a_relational expression>
	| > <primary a_relational expression>
	| < <primary a_relational expression>

FIRST(primary a_relational expression_p) = {=, <, >, <>}*/
void primary_a_relational_expression_p(void);

/*******************************************************************************
Purpose:			
Author:				
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/* <primary s_relational expression> ->
<primary string expression>

FIRST(primary s_relational expression) = {SVID_T, STR_T}*/

void primary_s_relational_expression(void);

/*******************************************************************************
Purpose:			
Author:				
History/Versions:	Version 1.0, 2015/12/06
Called Functions:	
Parameters:			N/A
Return Value:		N/A
Algorithm:			
*******************************************************************************/
/* <primary s_relational expression_p> ->
	== <primary s_relational expression>
	| <> <primary s_relational expression>
	| > <primary s_relational expression>
	| < <primary s_relational expression>

FIRST(primary s_relational expression_p) = {=, <, >, <>}*/

void primary_s_relational_expression_p(void);
