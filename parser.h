/*****************************************************************************************************
* File Name:		parser.h
* Compiler:			MS Visual Studio 2012
* Authors:			Bryden Lacelle & Justin Farinaccio
* Course:			CST8152 - Compilers, Lab Section 011
* Assignment:		Assignment 4: The Parser
* Date:				December 11, 2015
* Professor:		Svillen Ranev
* Purpose:			Header file for parser.c; includes definitions for ___, as well as forward declartions for
					necessary functions.
* Function List:	parser()
					match()
					syn_eh()
					syn_printe()
					gen_incode()
*****************************************************************************************************/

#ifndef PARSER_H_
#define PARSER_H_

#ifndef TOKEN_H_
#include "token.h"
#endif

#ifndef STABLE_H_
#include "stable.h"
#endif

#ifndef BUFFER_H_
#include "buffer.h"
#endif

extern char* kw_table[];
/* definitions */
#define NO_ATTR		0
#define ELSE		0
#define IF			1
#define INPUT		2
#define OUTPUT		3
#define PLATYPUS	4
#define REPEAT		5
#define THEN		6
#define USING		7

/* parser functions */
void parser(Buffer*);
void match(int, int);
void syn_eh(int);
void syn_printe();
void gen_incode(char*);

/* forward declarations */
extern Token mlwpar_next_token(Buffer*);
extern int line;
extern Buffer *str_LTBL;

/* grammar productions */
void program(void);
void opt_statements(void);
void statements(void);
void statement(void);
void statements_p(void);
void assignment_statement(void);
void assignment_expression(void);
void selection_statement(void);
void iteration_statement(void);
void input_statement(void);
void variable_list(void);
void variable_list_p(void);
void variable_identifier(void);
void output_statement(void);
void output_list(void);
void arithmetic_expression(void);
void unary_arithmetic_expression(void);
void additive_arithmetic_expression(void);
void additive_arithmetic_expression_p(void);
void multiplicative_arithmetic_expression(void);
void multiplicative_arithmetic_expression_p(void);
void primary_arithmetic_expression(void);
void string_expression(void);
void string_expression_p(void);
void primary_string_expression(void);
void conditional_expression(void);
void logical_OR_expression(void);
void logical_OR_expression_p(void);
void logical_AND_expression(void);
void logical_AND_expression_p(void);
void relational_expression(void);
void primary_a_relational_expression(void);
void primary_a_relational_expression_p(void);
void primary_s_relational_expression(void);
void primary_s_relational_expression_p(void);

#endif
