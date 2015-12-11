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
					syn_eh()
					syn_printe()
					gen_incode()
					program();
					opt_statments();
					statements();
					statement();
					statements_p();
					assignment_statement();
					assignment_expression();
					selection_statement();
					iteration_statement();
					input_statement();
					variable_list();
					variable_list_p();
					variable_identifier();
					output_statement();
					output_list();
					arithmetic_expression();
					unary_arithmetic_expression();
					additive_arithmetic_expression();
					additive_arithmetic_expression_p();
					multiplicative_arithmetic_expression();
					multiplicative_arithmetic_expression_p();
					primary_arithmetic_expression();
					string_expression();
					string_expression_p();
					primary_string_expression();
					conditional_expression();
					logical_OR_expression();
					logical_OR_expression_p();
					logical_AND_expression();
					logical_AND_expression_p();
					relational_expression();
					primary_a_relational_expression();
					primary_a_relational_expression_p();
					primary_s_relational_expression();
					primary_s_relational_expression_p();
*******************************************************************************/

/* project header files */
#include <stdlib.h>
#include <string.h>
#include "parser.h"

/* global variables */
Token lookahead;
Buffer *par_buf;
int synerrno;

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
	par_buf = in_buf;
	lookahead = mlwpar_next_token(par_buf);
	program(); match(SEOF_T, NO_ATTR);
	gen_incode("PLATY: Source file parsed");
}

/*******************************************************************************
Purpose:
Author:				Bryden Lacelle
History/Versions:	Version 1.0, 2015/12/07
Called Functions:
Parameters:			int, int
Return Value:		N/A
Algorithm:
*******************************************************************************/
void match(int pr_token_code, int pr_token_attribute)
{
	if (lookahead.code == pr_token_code && (pr_token_attribute == NO_ATTR || lookahead.attribute.int_value == pr_token_attribute))
	{
		if (lookahead.code == SEOF_T)
			return;
		lookahead = mlwpar_next_token(par_buf);
		while (lookahead.code == ERR_T)
		{
			++synerrno;
			syn_printe();
			lookahead = mlwpar_next_token(par_buf);
		}
	}
	else
		syn_eh(EOS_T);
}

/*******************************************************************************
Purpose:
Author:				Bryden Lacelle & Justin Farinaccio
History/Versions:	Version 1.0, 2015/12/07
Called Functions:	sizeof(), exit()
Parameters:			int
Return Value:		N/A
Algorithm:
*******************************************************************************/
void syn_eh(int sync_token_code) {
	/* compares  */
	while (lookahead.code != sync_token_code) {
		syn_printe();
		++synerrno;
		lookahead = mlwpar_next_token(par_buf);
		/* if matching token found and token IS SEOF */
		if (lookahead.code == SEOF_T)
		{
			return;
		}
	}
}

/*******************************************************************************
Purpose:
Author:				Svillen Ranev
History/Versions:	Version 1.0, 2015/11/09
Called Functions:	printf()
Parameters:			N/A
Return Value:		N/A
Algorithm:
*******************************************************************************/
/* Parser error printing function, Assignmet 4, F15 */
void syn_printe() {
	Token t = lookahead;

	printf("PLATY: Syntax error:  Line:%3d\n", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case  ERR_T:	/* ERR_T	0	Error token */
		printf("%s\n", t.attribute.err_lex);
		break;
	case  SEOF_T:	/* SEOF_T	1	Source end-of-file token */
		printf("NA\n");
		break;
	case  AVID_T:	/* AVID_T	2	Arithmetic Variable identifier token */
	case  SVID_T:	/* SVID_T	3	String Variable identifier token */
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
Author:				Bryden Lacelle & Justin Farinaccio
History/Versions:	Version 1.0, 2015/12/07
Called Functions:	printf()
Parameters:			char*
Return Value:		N/A
Algorithm:
*******************************************************************************/
void gen_incode(char *c) {
	printf("%s\n", c);
}

/*******************************************************************************
<program>  ->
PLATYPUS {<opt_statements>}

FIRST(program) = {KW_T(PLATYPUS)}

Author:	Justin Farinaccio
*******************************************************************************/
void program(void) {
	match(KW_T, PLATYPUS);
	match(LBR_T, NO_ATTR);
	opt_statements();
	match(RBR_T, NO_ATTR);
	gen_incode("PLATY: Program parsed");
}

/*******************************************************************************
<opt_statements> ->
<statements> | e

FIRST(opt_statements) = {AVID_T, SVID_T, KW_T(IF), KW_T(USING), KW_T(INPUT), KW_T(OUTPUT), e}

Author:	Justin Farinaccio
*******************************************************************************/
void opt_statements(void) {
	switch (lookahead.code)
	{
	case AVID_T:
	case SVID_T:
		statements();
		break;
	case KW_T:
		if (lookahead.attribute.kwt_idx != PLATYPUS
			&& lookahead.attribute.kwt_idx != ELSE
			&& lookahead.attribute.kwt_idx != THEN
			&& lookahead.attribute.kwt_idx != REPEAT) {
			statements();
			break;
		}
	default:
		gen_incode("PLATY: Opt_statements parsed");
	}
}

/*******************************************************************************
<statements> ->
<statement><statements_p>

FIRST(statements) = {AVID_T, SVID_T, KW_T(IF), KW_T(USING), KW_T(INPUT), KW_T(OUTPUT)}

Author:	Justin Farinaccio
*******************************************************************************/
void statements(void) {
	statement();
	statements_p();
	//gen_incode("PLATY: Statements parsed");
}

/*******************************************************************************
<statement> ->
<assignment statement>
| <selection statement>
| <iteration statement>
| <input statement>
| <output statement>

FIRST(statement) = {AVID_T, SVID_T, KW_T(IF), KW_T(USING), KW_T(INPUT), KW_T(OUTPUT)}

Author:	Justin Farinaccio
*******************************************************************************/
void statement(void) {
	switch (lookahead.code)
	{
	case AVID_T:
	case SVID_T:
		assignment_statement();
		break;
	case KW_T:
		if (lookahead.attribute.kwt_idx == IF) {
			selection_statement();
			break;
		}
		else if (lookahead.attribute.kwt_idx == USING) {
			iteration_statement();
			break;
		}
		else if (lookahead.attribute.kwt_idx == INPUT) {
			input_statement();
			break;
		}
		else if (lookahead.attribute.kwt_idx == OUTPUT) {
			output_statement();
			break;
		}
		else {
			syn_printe();
			break;
		}
	default:
		syn_printe();
	}
}

/*******************************************************************************
<statements_p> ->
<statement><statements_p>
| e

FIRST(statements_p) = {AVID_T, SVID_T, KW_T(IF), KW_T(USING), KW_T(INPUT), KW_T(OUTPUT), e}

Author:	Justin Farinaccio
*******************************************************************************/
void statements_p(void) {
	switch (lookahead.code)
	{
	case AVID_T:
	case SVID_T:
		assignment_statement();
		break;
	case KW_T:
		if (lookahead.attribute.kwt_idx == IF) {
			selection_statement();
			break;
		}
		else if (lookahead.attribute.kwt_idx == USING) {
			iteration_statement();
			break;
		}
		else if (lookahead.attribute.kwt_idx == INPUT) {
			input_statement();
			break;
		}
		else if (lookahead.attribute.kwt_idx == OUTPUT) {
			output_statement();
			break;
		}
		else {
			break;
		}
	default:
		return;
	}
	statements_p();
}

/*******************************************************************************
<assignment statement> ->
<assignment expression>;

FIRST(assignment statement) = {AVID_T, SVID_T}

Author:	Justin Farinaccio
*******************************************************************************/
void assignment_statement(void) {
	assignment_expression();
	match(EOS_T, NO_ATTR);
	gen_incode("PLATY: Assignment statement parsed");
}

/*******************************************************************************
<assignment expression> ->
AVID = <arithmetic expression>
| SVID = <string expression>

FIRST(assignment expression) = {AVID_T, SVID_T}

Author:	Justin Farinaccio
*******************************************************************************/
void assignment_expression(void) {
	switch (lookahead.code)
	{
	case AVID_T:
		match(AVID_T, NO_ATTR);
		match(ASS_OP_T, NO_ATTR);
		arithmetic_expression();
		gen_incode("PLATY: Assignment expression (arithmetic) parsed");
		break;
	case SVID_T:
		match(SVID_T, NO_ATTR);
		match(ASS_OP_T, NO_ATTR);
		string_expression();
		break;
	default:
		break;
	}
}

/*******************************************************************************
<selection statement> ->
IF (<conditional expression>)  THEN  <opt_statements>
ELSE { <opt_statements> } ;

FIRST(selection statement) = {KW_T(IF)}

Author:	Justin Farinaccio
*******************************************************************************/
void selection_statement(void) {
	match(KW_T, IF); match(LPR_T, NO_ATTR); conditional_expression();
	match(KW_T, RPR_T); match(KW_T, THEN); opt_statements();
	match(KW_T, ELSE); match(LBR_T, NO_ATTR); opt_statements();
	match(RBR_T, NO_ATTR); match(EOS_T, NO_ATTR);
	gen_incode("PLATY: IF statement parsed");
}

/*******************************************************************************
<iteration statement> ->
USING  (<assignment expression> , <conditional expression> , <assignment  expression> )
REPEAT {
<opt_statements>
};

FIRST(iteration statement) = {KW_T(USING)}

Author:	Justin Farinaccio
*******************************************************************************/
void iteration_statement(void) {
	match(KW_T, USING); match(LPR_T, NO_ATTR); assignment_expression();
	match(COM_T, NO_ATTR); conditional_expression(); match(COM_T, NO_ATTR);
	assignment_expression(); match(RPR_T, NO_ATTR);
	match(KW_T, REPEAT); match(LBR_T, NO_ATTR); opt_statements();
	match(RBR_T, NO_ATTR); match(EOS_T, NO_ATTR);
	gen_incode("PLATY: USING statement parsed");
}

/*******************************************************************************
<input statement> ->
INPUT (<variable list>);

FIRST(input statement) = {KW_T(INPUT)}

Author:	Justin Farinaccio
*******************************************************************************/
void input_statement(void) {
	match(KW_T, INPUT); 
	match(LPR_T, NO_ATTR); 
	variable_list();
	match(RPR_T, NO_ATTR);
	match(EOS_T, NO_ATTR);
	gen_incode("PLATY: INPUT statement parsed");
}

/*******************************************************************************
<variable list> ->
<variable identifier><variable list_p>

FIRST(variable list) = {AVID_T, SVID_T}

Author:	Justin Farinaccio
*******************************************************************************/
void variable_list(void) {
	variable_identifier(); 
	variable_list_p();
	gen_incode("PLATY: Variable list parsed");
}

/*******************************************************************************
<variable identifier> ->
AVID_T | SVID_T

FIRST(variable identifier) = {AVID_T, SVID_T}

Author:	Justin Farinaccio
*******************************************************************************/
void variable_identifier(void) {
	switch (lookahead.code)
	{
	case AVID_T:
		match(AVID_T, NO_ATTR);
		break;
	case SVID_T:
		match(SVID_T, NO_ATTR);
		break;
	default:
		syn_printe();
		break;
	}
	//gen_incode("PLATY: Variable identifier parsed");
}

/*******************************************************************************
<variable list_p> ->
, <variable identifier><variable list_p>
| e

FIRST(variable list_p) = {, , e}

Author:	Justin Farinaccio
*******************************************************************************/
void variable_list_p(void) {
	switch (lookahead.code)
	{
	case COM_T:
		match(COM_T, NO_ATTR);
		variable_identifier();
		variable_list_p();
		break;
	default:
		break;
	}
}

/*******************************************************************************
<output statement> ->
OUTPUT (<output list>);

FIRST(output statement) = {KW_T(OUTPUT)}

Author:	Justin Farinaccio
*******************************************************************************/
void output_statement(void) {
	match(KW_T, OUTPUT); match(LPR_T, NO_ATTR); output_list();
	match(RPR_T, NO_ATTR); match(EOS_T, NO_ATTR);
	gen_incode("PLATY: OUTPUT statement parsed");
}

/*******************************************************************************
<output list> ->
<variable list>
| STR_T
| e

FIRST(output list) = {, , STR_T, e}

Author:	Justin Farinaccio
*******************************************************************************/
void output_list(void) {
	switch (lookahead.code)
	{
	case COM_T:
		match(COM_T, NO_ATTR);
		variable_list();
		break;
	case STR_T:
		match(STR_T, NO_ATTR);
		primary_string_expression();
		gen_incode("PLATY: Output list (string literal) parsed");
		break;
	default:
		gen_incode("PLATY: Output list (empty) parsed");
		break;
	}
}

/*******************************************************************************
<arithmetic expression> - >
<unary arithmetic expression>
| <additive arithmetic expression>

FIRST(arithmetic expression) = {+, -, AVID_T, FPL_T, INL_T, ( }

Author:	Justin Farinaccio
*******************************************************************************/
void arithmetic_expression(void) {
	switch (lookahead.code)
	{
	case ART_OP_T:
		unary_arithmetic_expression();
		gen_incode("PLATY: Arithmetic expression parsed");
		break;
	case AVID_T:
		additive_arithmetic_expression();
		gen_incode("PLATY: Arithmetic expression parsed");
		break;
	case FPL_T:
	case INL_T:
		match(INL_T, NO_ATTR);
		gen_incode("PLATY: Arithmetic expression parsed");
		break;
	case LPR_T:
		//match(LPR_T, NO_ATTR);
		additive_arithmetic_expression();
		//match(RPR_T, NO_ATTR);
		gen_incode("PLATY: Arithmetic expression (arithmetic) parsed");
		break;
	default:
		syn_printe();
	}
}

/*******************************************************************************
<unary arithmetic expression> ->
	- <primary arithmetic expression>
	| + <primary arithmetic expression>

FIRST(unary arithmetic expression) = {+, -}

Author:	Justin Farinaccio
*******************************************************************************/
void unary_arithmetic_expression(void) {
	switch (lookahead.attribute.arr_op)
	{
	case PLUS:
		match(ART_OP_T, PLUS);
		primary_arithmetic_expression();
		break;
	case MINUS:
		match(ART_OP_T, MINUS);
		primary_arithmetic_expression();
		break;
	default:
		syn_printe();
	}
	gen_incode("PLATY: Unary arithmetic expression parsed");
}

/*******************************************************************************
<additive arithmetic expression> ->
	<multiplicative arithmetic expression><additive arithmetic expression_p>

FIRST(additive arithmetic expression) = {AVID_T, FPL_T, INL_T, ( }

Author:	Justin Farinaccio
*******************************************************************************/
void additive_arithmetic_expression(void) {
	multiplicative_arithmetic_expression();
	additive_arithmetic_expression_p();
	gen_incode("PLATY: Additive arithmetic expression parsed");
}

/*******************************************************************************
<additive arithmetic expression_p> ->
	+ <multiplicative arithmetic expression><additive arithmetic expression_p>
	| - <multiplicative arithmetic expression><additive arithmetic expression_p>
	| e

FIRST(additive arithmetic expression_p) = {+, -, e}

Author:	Justin Farinaccio
*******************************************************************************/
void additive_arithmetic_expression_p(void) {
	switch (lookahead.code)
	{
	case ART_OP_T:
		switch (lookahead.attribute.arr_op)
		{
		case PLUS:
			match(ART_OP_T, PLUS);
			multiplicative_arithmetic_expression();
			multiplicative_arithmetic_expression_p();
			break;
		case MINUS:
			match(ART_OP_T, MINUS);
			multiplicative_arithmetic_expression();
			multiplicative_arithmetic_expression_p();
			break;
		default:
			syn_printe();
		}
	}
}

/*******************************************************************************
<multiplicative arithmetic expression> ->
<primary arithmetic expression><multiplicative arithmetic expression_p>

FIRST(multiplicative arithmetic expression) = {AVID_T, FPL_T, INL_T, ( }

Author:	Justin Farinaccio
*******************************************************************************/
void multiplicative_arithmetic_expression(void) {
	primary_arithmetic_expression();
	multiplicative_arithmetic_expression_p();
	gen_incode("PLATY: Multiplicative arithmetic expression parsed");
}

/*******************************************************************************
<multiplicative arithmetic expression_p> ->
* <primary arithmetic expression><multiplicative arithmetic expression_p>
| / <primary arithmetic expression><multiplicative arithmetic expression_p>
| e

FIRST(multiplicative arithmetic expression_p) = {*, /, e}

Author:	Justin Farinaccio
*******************************************************************************/
void multiplicative_arithmetic_expression_p(void) {
	switch (lookahead.attribute.arr_op)
	{
	case MULT:
		match(ART_OP_T, MULT);
		primary_arithmetic_expression();
		multiplicative_arithmetic_expression_p();
		break;
	case DIV:
		match(ART_OP_T, DIV);
		primary_arithmetic_expression();
		multiplicative_arithmetic_expression_p();
		break;
	default:
		break;
	}
}

/*******************************************************************************
<primary arithmetic expression> ->
AVID_T
| FPL_T
| INL_T
| (<arithmetic expression>)

FIRST(primary arithmetic expression) = {AVID_T, FPL_T, INL_T, ( }

Author:	Justin Farinaccio
*******************************************************************************/
void primary_arithmetic_expression(void) {
	switch (lookahead.code)
	{
	case AVID_T:
		match(AVID_T, NO_ATTR);
		break;
	case FPL_T:
		match(FPL_T, NO_ATTR);
		break;
	case INL_T:
		match(INL_T, NO_ATTR);
		break;
	case LPR_T:
		match(LPR_T, NO_ATTR);
		additive_arithmetic_expression();
		match(RPR_T, NO_ATTR);
		break;
	default:
		syn_printe();
	}
	gen_incode("PLATY: Primary arithmetic expression parsed");
}

/*******************************************************************************
<string expression> ->
<primary string expression><string expression_p>

FIRST(string expression) = {SVID_T, STR_T}

Author:	Bryden Lacelle
*******************************************************************************/
void string_expression(void)
{
	primary_string_expression();
	string_expression_p();
	gen_incode("PLATY: String expression parsed\n");
}

/*******************************************************************************
<string expression_p> ->
# <primary string expression><string expression_p>
| e

FIRST(string expression_p) = {#, e}

Author:	Bryden Lacelle
*******************************************************************************/
void string_expression_p(void)
{
	lookahead = mlwpar_next_token(par_buf);
	switch (lookahead.code)
	{
	case SCC_OP_T:
		lookahead = mlwpar_next_token(par_buf);
		primary_string_expression();
		string_expression_p();
		break;
	default:
		break;
	}
}

/*******************************************************************************
<primary string expression> ->
SVID_T
| STR_T

FIRST(primary string expression) = {SVID_T, STR_T}

Author:	Bryden Lacelle
*******************************************************************************/
void primary_string_expression(void)
{
	switch (lookahead.code)
	{
	case SVID_T:
		match(SVID_T, NO_ATTR);
		break;
	case STR_T:
		match(STR_T, NO_ATTR);
		break;
	default:
		break;
	}
}

/*******************************************************************************
<conditional expression> ->
<logical OR expression>

FIRST(conditional expression) = {AVID_T, FPL_T, INL_T, SVID_T, STR_T}

Author:	Bryden Lacelle
*******************************************************************************/
void conditional_expression(void)
{
	logical_OR_expression();
}

/*******************************************************************************
<logical OR expression> ->
<logical AND expression><logical OR expression_p>

FIRST(logical OR expression) = {AVID_T, FPL_T, INL_T, SVID_T, STR_T}

Author:	Bryden Lacelle
*******************************************************************************/
void logical_OR_expression(void)
{
	logical_AND_expression();
	logical_OR_expression_p();
}

/*******************************************************************************
<logical OR expression_p> ->
.OR. <logical AND expression><logical OR expression_p>
| e

FIRST(logical OR expression_p) = {.OR., e}

Author:	Bryden Lacelle
*******************************************************************************/
void logical_OR_expression_p(void)
{
	lookahead = mlwpar_next_token(par_buf);
	switch (lookahead.attribute.log_op)
	{
	case OR:
		lookahead = mlwpar_next_token(par_buf);
		logical_AND_expression();
		logical_OR_expression_p();
		break;
	default:
		gen_incode("PLATY: Logical OR expression parsed\n");
		break;
	}
}

/*******************************************************************************
<logical AND expression> ->
<relational expression><logical AND expression_p>

FIRST(logical AND expression) = {AVID_T, FPL_T, INL_T, SVID_T, STR_T}

Author:	Bryden Lacelle
*******************************************************************************/
void logical_AND_expression(void)
{
	relational_expression();
	logical_AND_expression_p();
}

/*******************************************************************************
<logical AND expression_p> ->
.AND. <relational expression><logical AND expression_p>
| e

FIRST(logical AND expression_p) = {.AND., e}

Author:	Bryden Lacelle
*******************************************************************************/
void logical_AND_expression_p(void)
{
	while (1)
	{
		switch (lookahead.attribute.log_op)
		{
		case AND:
			match(REL_OP_T, AND);
			relational_expression();
			break;
		default:
			gen_incode("PLATY: Relational expression parsed\n");
			return;
		}
	}
}

/*******************************************************************************
<relational expression> ->
<primary a_relational expression><primary a_relational expression_p>
| <primary s_relational expression><primary s_relational expression_p>

FIRST(relational expression) = {AVID_T, FPL_T, INL_T, SVID_T, STR_T}

Author:	Bryden Lacelle
*******************************************************************************/
void relational_expression(void)
{
	if (lookahead.code == AVID_T || lookahead.code == INL_T || lookahead.code == FPL_T)
	{
		primary_a_relational_expression(); primary_a_relational_expression_p();
	}
	if (lookahead.code == SVID_T || lookahead.code == STR_T)
	{
		primary_s_relational_expression(); primary_s_relational_expression_p();
	}
}

/*******************************************************************************
<primary a_relational expression> ->
AVID_T
| FPL_T
| INL_T

FIRST(primary a_relational expression) = {AVID_T, FPL_T, INL_T}

Author:	Bryden Lacelle
*******************************************************************************/
void primary_a_relational_expression(void)
{
	switch (lookahead.code)
	{
	case AVID_T:
		match(AVID_T, NO_ATTR);
		break;
	case FPL_T:
		match(FPL_T, NO_ATTR);
		break;
	case INL_T:
		match(INL_T, NO_ATTR);
		break;
	default:
		/*Report error*/
		break;
	}
}

/*******************************************************************************
<primary a_relational expression_p> ->
== <primary a_relational expression>
| <> <primary a_relational expression>
| > <primary a_relational expression>
| < <primary a_relational expression>

FIRST(primary a_relational expression_p) = {=, <, >, <>}

Author:	Bryden Lacelle
*******************************************************************************/
void primary_a_relational_expression_p(void)
{
	switch (lookahead.attribute.rel_op)
	{
	case EQ:
		match(REL_OP_T, EQ);
		primary_a_relational_expression();
		break;
	case GT:
		match(REL_OP_T, GT);
		primary_a_relational_expression();
		break;
	case LT:
		match(REL_OP_T, LT);
		primary_a_relational_expression();
		break;
	case NE:
		match(REL_OP_T, NE);
		primary_a_relational_expression();
		break;
	default:
		/* Empty statement */
		break;
	}
}

/*******************************************************************************
<primary s_relational expression> ->
	<primary string expression>

FIRST(primary s_relational expression) = {SVID_T, STR_T}

Author:	Bryden Lacelle
*******************************************************************************/
void primary_s_relational_expression(void)
{
	switch (lookahead.code)
	{
	case SVID_T:
		match(SVID_T, NO_ATTR);
		break;
	case STR_T:
		match(STR_T, NO_ATTR);
		break;
	default:
		/*Report error*/
		break;
	}
}

/*******************************************************************************
<primary s_relational expression_p> ->
	== <primary s_relational expression>
	| <> <primary s_relational expression>
	| > <primary s_relational expression>
	| < <primary s_relational expression>

FIRST(primary s_relational expression_p) = {==, <, >, <>}

Author:	Bryden Lacelle
*******************************************************************************/
void primary_s_relational_expression_p(void)
{
	switch (lookahead.attribute.rel_op)
	{
	case EQ:
		match(REL_OP_T, EQ);
		primary_s_relational_expression();
		break;
	case GT:
		match(REL_OP_T, GT);
		primary_s_relational_expression();
		break;
	case LT:
		match(REL_OP_T, LT);
		primary_s_relational_expression();
		break;
	case NE:
		match(REL_OP_T, NE);
		primary_s_relational_expression();
		break;
	default:
		/* Empty statement */
		break;
	}
}
