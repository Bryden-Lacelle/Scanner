/*******************************************************************************
* File Name:		scanner.c
* Compiler:			MS Visual Studio 2012
* Authors:			Bryden Lacelle & Justin Farinaccio
* Course:			CST8152 - Compilers, Lab Section 011
* Assignment:		Assignment 2: The Scanner
* Date:				October 27, 2015
* Professor:		Svillen Ranev
* Purpose:			Building a lexical analyzer (scanner) for the Platypus language
* Funciton List:	scanner_init();
					mlwpar_next_token();
					get_next_state();
					char_class();
					aa_func02();
					aa_func03();
					aa_func08();
					aa_func05();
					aa_func10();
					aa_func12();
					atool();
					iskeyword();
					getString();
					copyString();
					errorString();
					runError();
					errSymbol();
					err_VID_LEN_atrbt()
*******************************************************************************/

/* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
* to suppress the warnings about using "unsafe" functions like fopen()
* and standard sting library functions defined in string.h.
* The define does not have any effect in Borland compiler projects.
*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>	/* standard input / output */
#include <ctype.h>	/* conversion functions */
#include <stdlib.h>	/* standard library functions and constants */
#include <string.h>	/* string functions */
#include <limits.h>	/* integer types constants */
#include <float.h>	/* floating-point types constants */

/*#define NDEBUG	to suppress assert() call */
#include <assert.h>	/* assert() prototype */

/* project header files */
#include "buffer.h"
#include "token.h"
#include "table.h"

#define DEBUG	/* for conditional processing */
#undef	DEBUG

/* Global objects - variables */
/* This buffer is used as a repository for string literals.
It is defined in platy_st.c */
extern Buffer *str_LTBL;	/*String literal table */
int line;					/* current line number of the source code */
extern int scerrnum;		/* defined in platy_st.c - run-time error number */

/* Local(file) global objects - variables */
static Buffer *lex_buf;/*pointer to temporary lexeme buffer*/

/* No other global variable declarations/definitiond are allowed */

/* scanner.c static(local) function  prototypes */ 
static int char_class(char c);					/* character class function */
static int get_next_state(int, char, int *);	/* state machine function */
static int iskeyword(char * kw_lexeme);			/*keywords lookup functuion */
static long atool(char * lexeme);				/* converts octal string to decimal value */

/*******************************************************************************
Purpose:			Initializes scanner input buffer, mark, and line number
Author:				Svillen Ranev
History/Versions:	1.15.02, 2015/09/29
Called Functions:	b_isempty(), b_setmark(), b_retract_to_mark(), b_reset()
Parameters:			Buffer*
Return Value:		int
*******************************************************************************/
int scanner_init(Buffer *sc_buf) {
	if(b_isempty(sc_buf)) return EXIT_FAILURE;/*1*/
	/* in case the buffer has been read previously  */
	b_setmark(sc_buf, 0);
	b_retract_to_mark(sc_buf);
	b_reset(str_LTBL);
	line = 1;
	return EXIT_SUCCESS;/*0*/
	/*	scerrnum = 0;  *//*no need - global ANSI C */
}

/*******************************************************************************
Purpose:			Performs token recognition of lexemes from input buffer
Author:				Bryden Lacelle & Justin Farinaccio
History/Versions:	Version 1.0, 2015/10/24
Called Functions:	errorString(), copyString(), getString(), errSymbol(), 
b_getc(), b_setmark(), b_getc_offset(), getString(), 
b_addc(), isalpha(), isdigit(), get_next_state(), b_mark(), 
b_create(), b_retract_to_mark(), b_retract(), b_destroy()
Parameters:			Buffer*
Return Value:		Token
Algorithm:			
*******************************************************************************/
Token mlwpar_next_token(Buffer * sc_buf)
{
	Token t;			/* token to return after recognition */
	unsigned char c;	/* input symbol */
	int state = 0;		/* initial state of the FSM */
	short lexstart;		/* start offset of a lexeme in the input buffer */
	short lexend;		/* end offset of a lexeme in the input buffer */
	int accept = NOAS;	/* type of state - initially not accepting */

	/* 
	lexstart is the offset from the beginning of the char buffer of the
	input buffer (sc_buf) to the first character of the current lexeme,
	which is being processed by the scanner.
	lexend is the offset from the beginning of the char buffer of the
	input buffer (sc_buf) to the last character of the current lexeme,
	which is being processed by the scanner.
	*/ 


	/*  DECLARE YOUR VARIABLES HERE IF NEEDED */
#define SEOF 255
#define RUNERROR 254
	char vAND[] = {'A', 'N', 'D', '.'};
	char vOR[] = {'O', 'R', '.'};
	int i = -1;
	int switch_AND_OR = 0;
	short increment = 0;
	typedef enum {FALSE, TRUE} BOOL;
	BOOL validString = FALSE;
	Token errorString(Buffer*);
	int copyString(Buffer*, Buffer*, int);
	short getString(Buffer*, short);
	Token errSymbol(char);
	Token runError();
	char errComment[3] = {'!', ' ', '\0'};
	static short str_LTBL_mark = 0;

	while(1)
	{ /* endless loop broken by token returns it will generate a warning */
		c = b_getc(sc_buf);
		/* checks for end of source file */
		if (c == SEOF) { t.code = SEOF_T; return t; }
		if(c != SEOF)
		{
			/* checks for special characters */
			if(c == ' ' || c == '\n' || c == 'LF' || c == 'CR' || c == '\t' || !c) /* Ignore spaces, new lines, line feeds and NULL characters */
			{
				if (c == '\n' || c == 'LF' || c == 'CR') /* Increment line count on new line */
					++line;
				continue;
			}
			if (c == '#') { t.code = SCC_OP_T; return t; }
			if (c == ';') { t.code = EOS_T; return t;}
			if (c == ',') { t.code = COM_T; return t;}
			if (c == '(') { t.code = LPR_T; return t;}
			if (c == ')') { t.code = RPR_T; return t;}
			if(c == '{') { t.code = LBR_T; /*no attribute */ return t; }
			else if(c == '}') { t.code = RBR_T; return t; }

			/* arithmetic token attributes set */
			if(c == '+') { t.code = ART_OP_T; t.attribute.arr_op = PLUS; return t; }
			else if(c == '-') { t.code = ART_OP_T; t.attribute.arr_op = MINUS; return t; }
			else if(c == '*') { t.code = ART_OP_T; t.attribute.arr_op = MULT; return t; }
			else if(c == '/') { t.code = ART_OP_T; t.attribute.arr_op = DIV; return t; }
			/* determines whether character is part of an equals sign (relational token) or an assignment character */
			if(c == '=') 
			{
				c = b_getc(sc_buf);
				if(c == '=') { t.code = REL_OP_T; t.attribute.rel_op = EQ; return t; }
				else { b_retract(sc_buf); t.code = ASS_OP_T; return t; }
			}
			else if(c == '>') { t.code = REL_OP_T; t.attribute.rel_op = GT; return t; }
			else if(c == '<') 
			{
				/* determines whether character is part of a not-equals sign (relational operator) or a less-than character (relational operator) */
				c = b_getc(sc_buf);
				if(c == '>') { t.code = REL_OP_T; t.attribute.rel_op = NE; return t; }
				else { b_retract(sc_buf); t.code = REL_OP_T; t.attribute.rel_op = LT; return t; }
			}
			if (c == '.')
			{
				b_setmark(sc_buf, b_getc_offset(sc_buf));/* Set mark on the '.' char */
				while(1) /* TRIGGERS WARNING: Intended infinite loop*/
				{
					++i;
					c = b_getc(sc_buf);
					switch(switch_AND_OR)
					{
					case 0:
						if (c == vAND[i]) /* Checks next character for matching character for .AND. Token */
							switch_AND_OR = 1;
						else if (c == vOR[i]) /* Checks next character for matching character for .OR. Token */
							switch_AND_OR = 2;
						else 
						{ b_retract_to_mark(sc_buf); t = errSymbol('.'); return t;} /* Illegal character returns error code and retracts buffer */
						break;
					case 1:
						if (c == vAND[i])
							break;
						else 
						{ b_retract_to_mark(sc_buf); t = errSymbol('.'); return t;}	
					case 2:
						if (c == vOR[i])
							break;
						else 
						{ b_retract_to_mark(sc_buf); t = errSymbol('.'); return t;}
					}
					if (i == 2 && switch_AND_OR == 2)
					{ t.code = LOG_OP_T; t.attribute.log_op = OR; return t; } /* Set Token to .OR. Token */
					if(i == 3 && switch_AND_OR == 1)
					{ t.code = LOG_OP_T; t.attribute.log_op = AND; return t; } /* Set Token to .AND. Token */
				}
			}
		}

		/* determines if character denotes the start of a comment */
		if (c == '!')
		{
			if ((c = b_getc(sc_buf)) == '<')
			{
				while ((c = b_getc(sc_buf)) != '\n' && c != SEOF) {} /* Ignores characters until next line or SEOF is reached */
			}
			else 
			{ 
				t.code = ERR_T;
				errComment[1] = c;
				t.attribute.err_lex[0] = errComment[0]; /* Set to '"' */
				t.attribute.err_lex[1] = errComment[1]; /* Set erroneous character */
				t.attribute.err_lex[2] = errComment[2]; /* Set to \0 */
				while ((c = b_getc(sc_buf)) != '\n' && c != SEOF) {} /* Ignores characters until next line or SEOF is reached */
				return t; 
			}
			continue;
		}
		if (c == '"')
		{
			b_setmark(sc_buf, b_getc_offset(sc_buf));
			b_setmark(str_LTBL, str_LTBL_mark);
			validString = (BOOL) copyString(sc_buf, str_LTBL, increment = getString(sc_buf, 0)); /* Copy string from buffer to str_LTBL */
			if (!validString) { t = errorString(sc_buf); return t; } /* Return error token if string is illegal */
			t.code = STR_T; 
			t.attribute.str_offset = str_LTBL_mark; 
			str_LTBL_mark += increment + 1; /* Set new mark for first empty space in str_LTBL buffer */
			b_getc(sc_buf);
			if (increment == 0)
				b_retract(sc_buf);
			return t;
		}

		/* process runtime error */
		if (c == RUNERROR) { scerrnum = 100; t = runError(); return t; }

		/* implementation of finite state machine */
		if (!isalpha(c) && !isdigit(c)) { t = errSymbol(c); return t; }
		b_setmark(sc_buf, b_getc_offset(sc_buf)); /* Set mark on first character of lexeme */
		while(1)  /* TRIGGERS WARNING: Intended infinite loop*/
		{
			state = get_next_state(state, c, &accept);
			if(accept == NOAS) {
				c = b_getc(sc_buf);
				continue;
			}
			break;
		} 

		lexstart = b_mark(sc_buf);
		lexend = b_getc_offset(sc_buf);
		lex_buf = b_create(b_capacity(sc_buf), 0, 'f'); /* Create temporary buffer for lexeme */
		b_retract_to_mark(sc_buf); /* Retract to start of lexeme */
		b_retract(sc_buf);
		while (b_getc_offset(sc_buf) < (state == ES ? lexend : lexend - 1))
		{
			b_addc(lex_buf, c = b_getc(sc_buf));
		}
		b_addc(lex_buf,'\0');
		if (state == ES)
			t = aa_table[ES](b_setmark(lex_buf, 0));
		else
			t = aa_table[state](b_setmark(lex_buf, 0));
		b_destroy(lex_buf);
		if (t.code == SVID_T)
			b_getc(sc_buf);
		return t;
	}//end while(1)
}

/*******************************************************************************
Purpose:			Gets the next state from the transition table
Author:				Svillen Ranev
History/Versions:	1.15.02, 2015/09/29
Called Functions:	char_class(), assert(), exit()
Parameters:			int, char, int*
Return Value:		int
*******************************************************************************/
int get_next_state(int state, char c, int *accept)
{
	int col;
	int next;
	col = char_class(c);
	next = st_table[state][col];
#ifdef DEBUG
	printf("Input symbol: %c Row: %d Column: %d Next: %d \n",c,state,col,next);
#endif
	assert(next != IS);

#ifdef DEBUG
	if(next == IS){
		printf("Scanner Error: Illegal state:\n");
		printf("Input symbol: %c Row: %d Column: %d\n",c,state,col);
		exit(1);
	}
#endif
	*accept = as_table[next];
	return next;
}

/*******************************************************************************
Purpose:			Returns the necessary column in transition table for character
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/10/23
Called Functions:	isalpha()
Parameters:			char
Return Value:		
Algorithm:			
*******************************************************************************/
int char_class(char c)
{
	int val;

	if(isalpha(c)) {
		val = 0;
	}
	else if(c == '0') {
		val = 1;
	}
	else if(c >= '1' && c <= '7') {
		val = 2;
	}
	else if(c == '8' || c == '9') {
		val = 3;
	}
	else if(c == '.') {
		val = 4;
	}
	else if(c == '%') {
		val = 5;
	}
	else
		val = 6;
	return val;
}

/*******************************************************************************
Purpose:			Accepting function for arithmetic variable identifier and keywords
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/10/24
Called Functions:	iskeyword(), strlen()
Parameters:			char*
Return Value:		
Algorithm:			
*******************************************************************************/
Token aa_func02(char lexeme[]) {
	Token err_VID_LEN_atrbt (unsigned short, char*);
	int kw_Index;
	Token t;
	if((kw_Index = iskeyword(lexeme)) != -1) {
		t.code = KW_T;
		t.attribute.kwt_idx = kw_Index;
		return t;
	}
	else 
	{
		t = err_VID_LEN_atrbt (VID_LEN, lexeme);
		t.code = AVID_T;
	}
	return t;
}

/*******************************************************************************
Purpose:			Accepting function for string variable identifier
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/10/24
Called Functions:	strlen()
Parameters:			char*
Return Value:		
Algorithm:			
*******************************************************************************/
Token aa_func03(char lexeme[]) {
	Token err_VID_LEN_atrbt (unsigned short, char*);
	Token t;
	t = err_VID_LEN_atrbt (VID_LEN, lexeme);
	t.code = SVID_T;
	t.attribute.vid_lex[strlen(lexeme) >= VID_LEN-1 ? VID_LEN-1 : strlen(lexeme)] = '%';
	t.attribute.vid_lex[strlen(lexeme) >= VID_LEN ? VID_LEN  : strlen(lexeme) + 1] = '\0';
	return t;
}

/*******************************************************************************
Purpose:			Accepting function for floating-point literal; 
					converts string representing floating-point to float value
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/10/24
Called Functions:	atof(), strlen()
Parameters:			char*
Return Value:		
Algorithm:			
*******************************************************************************/
Token aa_func08(char lexeme[]) {
	Token err_VID_LEN_atrbt (unsigned short, char*);
	Token t;	
	double dLexeme = atof(lexeme);
	if(dLexeme >= FLT_MIN && dLexeme <= FLT_MAX || !dLexeme) {
		t.code = FPL_T;
		t.attribute.flt_value = (float) dLexeme;
		return t;
	}
	t = err_VID_LEN_atrbt(ERR_LEN, lexeme);
	t.code = ERR_T;
	return t;
}

/*******************************************************************************
Purpose:			Accepting function for decimal integer literal and 0; 
					converts string representing decimal constant and 0 to 
					decimal integer value
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/10/24
Called Functions:	atoi(), strlen()
Parameters:			char*
Return Value:		
Algorithm:			
*******************************************************************************/
Token aa_func05(char lexeme[]) {
	Token err_VID_LEN_atrbt (unsigned short, char*);
	Token t;
	int intLexeme = atol(lexeme);
	if(intLexeme >= 0 && intLexeme <= SHRT_MAX) {
		t.code = INL_T;
		t.attribute.int_value = intLexeme;
		return t;
	}
	t = err_VID_LEN_atrbt (ERR_LEN, lexeme);
	t.code = ERR_T;
	return t;	
}

/*******************************************************************************
Purpose:			Accepting function for 
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/10/xx
Called Functions:	atool(), strlen()
Parameters:			char*
Return Value:		
Algorithm:			
*******************************************************************************/
Token aa_func10(char lexeme[]) {
	Token err_VID_LEN_atrbt (unsigned short, char*);
	Token t;
	long intLexeme = atool(lexeme);

	if(intLexeme >= 0 && intLexeme <= SHRT_MAX) {
		t.code = INL_T;
		t.attribute.int_value = intLexeme;
		return t;
	}
	t = err_VID_LEN_atrbt (ERR_LEN, lexeme);
	t.code = ERR_T;
	return t;
}

/*******************************************************************************
Purpose:			Accepting function for error token
Author:				Bryden Lacelle
History/Versions:	Version 1.0, 2015/10/24
Called Functions:	strlen()
Parameters:			char*
Return Value:		err_VID_LEN_atrbt()
Algorithm:			Calls err_VID_LEN_atrbt() which returns Token attribute
					and sets Token code to ERR_T
*******************************************************************************/
Token aa_func12(char lexeme[]) {
	Token err_VID_LEN_atrbt (unsigned short, char*);
	Token t;
	t = err_VID_LEN_atrbt(ERR_LEN, lexeme);
	t.code = ERR_T;
	return t;
}

/*******************************************************************************
Purpose:			Converts an ASCII string representing a valid octal integer
					to a long integer value
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/10/22
Called Functions:	strtol()
Parameters:			char*
Return Value:		
Algorithm:			
*******************************************************************************/
long atool(char *lexeme) {
	char *ptr;
	long int x;
	x = strtol(lexeme, &ptr, 8);
	return x;
}

/*******************************************************************************
Purpose:			Determines if lexeme is a valid keyword in Platypus language
Author:				Justin Farinaccio
History/Versions:	Version 1.0, 2015/10/23
Called Functions:	strstr()
Parameters:			char*
Return Value:		
Algorithm:			
*******************************************************************************/
int iskeyword(char *kw_lexeme) {
	int i = 0;
	for(i = 0; i < KWT_SIZE; i++) {
		// if keyword is found
		if(strstr(kw_lexeme, kw_table[i]) != NULL) {
			return i;
		}
	}
	return -1;
}

/*******************************************************************************
Purpose:			Get the length of string token
Author:				Bryden Lacelle
History/Versions:	Version 1.0, 2015/10/27
Called Functions:	b_getc(), b_retract_to_mark(), getString()
Parameters:			Buffer*, short
Return Value:		short
Algorithm:			Calls the function recursivally incrementing counteruntil a 
					quotation mark is reached returning the value of counter
					If a string terminator is reached a negative value for the 
					length of the string is returned
					In all cases except when a quotation mark is the first char
					the buffer retracts to the beginning of the string
*******************************************************************************/
short getString(Buffer* tsc_Buf, short counter)
{
	char c = b_getc(tsc_Buf);
	if (c == '\n' || c == 'CR') /* Increment line count on new line*/
		++line;
	if (c == '\0') { b_retract_to_mark(tsc_Buf); return -1; }
	if (c != '"') { return getString(tsc_Buf, ++counter); }
	else if (counter == 0) { return counter; }
	b_retract_to_mark(tsc_Buf);
	return counter;
}

/*******************************************************************************
Purpose:			Copies valid string from scanner buffer into str_LTBL buffer
Author:				Bryden Lacelle
History/Versions:	Version 1.0, 2015/10/27
Called Functions:	b_addc(), b_getc(), copyString(), copyString()
Parameters:			Buffer*, Buffer*, int
Return Value:		int
Algorithm:			Recursivally call the function adding the next char in sc_buf
					to str_LTBL until counter(the length of the string being copied)
					reaches 0
*******************************************************************************/
int copyString(Buffer* sc_Buf, Buffer* str_LTBL, int counter)
{
	if (counter < 0) /* Invalid string length check */
		return 0;
	if (counter > 0 ) { b_addc(str_LTBL, b_getc(sc_Buf)); return copyString(sc_Buf, str_LTBL, --counter); }
	b_addc(str_LTBL, '\0'); /* When counter reaches 0 add string terminating character to str_LTBL*/
	return 1;
}

/*******************************************************************************
Purpose:			Returns a Token for an invalid string
Author:				Bryden Lacelle
History/Versions:	Version 1.0, 2015/10/27
Called Functions:	b_getc()
Parameters:			Buffer*
Return Value:		Token
Algorithm:			Calls b_getc() and stores the value in the Token attribute
					until the end of string is reached or until the maximum
					amount of chars that can stored in the attribute is reached
					If so appended three dots to the string and loop through the 
					remaining chars in the string until \0 is reached
*******************************************************************************/
Token errorString(Buffer* tsc_Buf)
{
	Token errToken;
	char counter = -1;
	char c;
	errToken.code = ERR_T;
	b_retract(tsc_Buf); /* Retracts to the first quotation mark in the string */
	while (++counter < ERR_LEN - 3)
	{
		c = b_getc(tsc_Buf);
		if (c == '\0') { errToken.attribute.err_lex[counter] = c; return errToken; }
		errToken.attribute.err_lex[counter] = c;
	}
	for (counter; counter < ERR_LEN; ++counter)
		errToken.attribute.err_lex[counter] = '.';
	errToken.attribute.err_lex[ERR_LEN] = '\0'; /* Appended end of string character */
	while (b_getc(tsc_Buf)) {} /* Loop through the remaining chars in the string */
	return errToken;
}

/*******************************************************************************
Purpose:			Displays of runtime error attribute
Author:				Bryden Lacelle
History/Versions:	Version 1.0, 2015/10/27
Called Functions:	strcpy()
Return Value:		Token
Algorithm:			Sets Token code to an error and sets the Token to the declared
					char* variable runErrors
*******************************************************************************/
Token runError()
{
	char* runErrors = "RUN TIME ERROR: ";
	Token errToken;
	errToken.code = ERR_T;
	strcpy(errToken.attribute.err_lex, runErrors);
	return errToken;
}

/*******************************************************************************
Purpose:			Return a Token for an erroneous symbol
Author:				Bryden Lacelle
History/Versions:	Version 1.0, 2015/10/27
Called Functions:	N/A
Parameters:			char
Return Value:		Token
Algorithm:			Sets Token code to an Error and sets the attribute to the 
					erroneous symbol
*******************************************************************************/
Token errSymbol(char c)
{
	Token t;
	t.code = ERR_T;
	t.attribute.err_lex[0] = c;
	t.attribute.err_lex[1] = '\0';
	return t;
}

/*******************************************************************************
Purpose:			Return the attribute for an err_lex or vid_lex for a Token
Author:				Bryden Lacelle
History/Versions:	Version 1.0, 2015/10/27
Called Functions:	strlen()
Parameters:			unsigned short, char*
Return Value:		Token
Algorithm:			Accepts a length which is either equal to the maximum length
					of a VID token attribute or ERR token attribute
					If the length of the lexeme exceeds the value of length only
					the first length of characters are stored in the attribute
					with \0 being appended to the end of the lexeme
					If the length of the lexeme does not exceed length all 
					characters are stored and a \0 is appended to the end.
*******************************************************************************/
Token err_VID_LEN_atrbt (unsigned short length, char* lexeme)
{
	short i = -1;
	Token t;
	/* Set attribute to err_lex */
	if (length == ERR_LEN)
	{
		if(strlen(lexeme) >= length) /* TRIGGERS WARNING Intentional comparison between signed and unsigned values*/
		{
			for(i; i < length; ++i)
			{
				t.attribute.err_lex[i] = lexeme[i];
			}
			t.attribute.err_lex[length] = '\0';
		}
		else 
		{
			for(i ; i < strlen(lexeme); ++i) /* TRIGGERS WARNING Intentional comparison between signed and unsigned values*/
			{
				t.attribute.err_lex[i] = lexeme[i];
			}
			t.attribute.err_lex[strlen(lexeme)] = '\0';
		}
		return t;
	}
	/* Set attribute to vid_lex */
	if(strlen(lexeme) >= length) /* TRIGGERS WARNING Intentional comparison between signed and unsigned values*/
		{
			for(i; i < length; ++i)
			{
				t.attribute.vid_lex[i] = lexeme[i];
			}
			t.attribute.vid_lex[length] = '\0';
		}
		else 
		{
			for(i ; i < strlen(lexeme); ++i) /* TRIGGERS WARNING Intentional comparison between signed and unsigned values*/
			{
				t.attribute.vid_lex[i] = lexeme[i];
			}
			t.attribute.vid_lex[strlen(lexeme)] = '\0';
		}
		return t;
	
}
