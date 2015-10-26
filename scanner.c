/* Filename: scanner.c
/* PURPOSE:
 *    SCANNER.C: Functions implementing a Lexical Analyzer (Scanner)
 *    as required for CST8152, Assignment #2
 *    scanner_init() must be called before using the scanner.
 *    The file is incomplete;
 *    Provided by: Svillen Ranev
 *    Version: 1.15.02
 *    Date: 29 September 2015
 *******************************************************************
 *    REPLACE THIS HEADER WITH YOUR HEADER
 *******************************************************************
 */

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
	char errComment[3] = {'!', NULL, '\0'};
	static short str_LTBL_mark = 0;

        
                
		while(1)
		{ /* endless loop broken by token returns it will generate a warning */
                
        /*GET THE NEXT SYMBOL FROM THE INPUT BUFFER */
        c = b_getc(sc_buf);



/* special cases or token driven processing */

/*WRITE YOUR CODE FOR PROCESSING THE SPECIAL CASES HERE. 
COMMENTS AND STRING LITERALS ARE ALSO PROCESSED HERE.

WHAT FOLLOWS IS A PSEUDO CODE. YOU CAN USE switch STATEMENT
INSTEAD OF if-else TO PROCESS THE SPECIAL CASES
DO NOT FORGET TO COUNT THE PROGRAM LINES*/
		if (c == 255) { t.code = SEOF_T; return t; }
			if(c != 255)
			{
				if(c == ' ' || c == '\n' || c == 'LF' || c == 'CR' || c == 'CRLF' || c == '\t' || !c)
				{
					continue;
				}
				if (c == '#') { t.code = SCC_OP_T; return t; }
				if (c == ';') { t.code = EOS_T; return t;}
				if (c == ',') { t.code = COM_T; return t;}
				if (c == '(') { t.code = LPR_T; return t;}
				if (c == ')') { t.code = RPR_T; return t;}
				if(c == '{') { t.code = LBR_T; /*no attribute */ return t; }
				else if(c == '}') { t.code = RBR_T; return t; }
			
				if(c == '+') { t.code = ART_OP_T; t.attribute.arr_op = PLUS; return t; }
				else if(c == '-') { t.code = ART_OP_T; t.attribute.arr_op = MINUS; return t; }
				else if(c == '*') { t.code = ART_OP_T; t.attribute.arr_op = MULT; return t; }
				else if(c == '/') { t.code = ART_OP_T; t.attribute.arr_op = DIV; return t; }
				if(c == '=') 
				{
					c = b_getc(sc_buf);
					if(c == '=') { t.code = REL_OP_T; t.attribute.rel_op = EQ; return t; }
					else { b_retract(sc_buf); t.code = ASS_OP_T; return t; }
				}
				else if(c == '>') { t.code = REL_OP_T; t.attribute.rel_op = GT; return t; }
				else if(c == '<') 
				{
					c = b_getc(sc_buf);
					if(c == '>') { t.code = REL_OP_T; t.attribute.rel_op = NE; return t; }
					else { b_retract(sc_buf); t.code = REL_OP_T; t.attribute.rel_op = LT; return t; }
				}
				if (c == '.')
				{
					b_setmark(sc_buf, b_getc_offset(sc_buf));
					while(1)
					{
						++i;
						c = b_getc(sc_buf);
						switch(switch_AND_OR)
						{
						case 0:
							if (c == vAND[i])
								switch_AND_OR = 1;
							else if (c == vOR[i])
								switch_AND_OR = 2;
							else 
								{ b_retract_to_mark(sc_buf); t = errSymbol('.'); return t;}
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
							{ t.code = LOG_OP_T; t.attribute.log_op = OR; return t; }
						if(i == 3 && switch_AND_OR == 1)
							{ t.code = LOG_OP_T; t.attribute.log_op = AND; return t; }
					}
				}
			}

			if (c == '!')
			{
				if ((c = b_getc(sc_buf)) == '<')
				{
					while ((c = b_getc(sc_buf)) != '\n') {}
				}
				else 
				{ 
					t.code = ERR_T; 
					errComment[1] = c;
					t.attribute.err_lex[0] = errComment[0]; 
					t.attribute.err_lex[1] = errComment[1]; 
					t.attribute.err_lex[2] = errComment[2]; 
					while ((c = b_getc(sc_buf)) != '\n') {}
					return t; 
				}
				continue;
			}
			//printf("get_c_offset:%d", b_getc_offset(sc_buf));
			//printf("char:%c\n", c);
//	IF (c == '!') TRY TO PROCESS COMMENT
//	IF THE FOLLOWING IS NOT CHAR IS NOT < REPORT AN ERROR
//	ELSE IN A LOOP SKIP CHARACTERS UNTIL \n THEN continue
			if (c == '"') // TO-DO Add first quote to buffer
			{
				//b_retract(sc_buf);
				b_setmark(sc_buf, b_getc_offset(sc_buf));
				b_setmark(str_LTBL, str_LTBL_mark);
				validString = (BOOL) copyString(sc_buf, str_LTBL, increment = getString(sc_buf, 0));
				if (!validString) { t = errorString(sc_buf); return t; }
				//b_addc(str_LTBL, '\0'); 
				t.code = STR_T; 
				t.attribute.str_offset = str_LTBL_mark; 
				str_LTBL_mark += increment + 1;
				b_getc(sc_buf);
				if (increment == 0)
					b_retract(sc_buf);
				return t;
			}
//	...
//	IF STRING (FOR EXAMPLE, "text") IS FOUND
//		SET MARK TO MARK THE BEGINNING OF THE STRING
//		IF THE STRING IS LEGAL
//			USING b_addc(..)COPY THE text FROM INPUT BUFFER INTO str_LTBL
//			ADD '\0' at the end make the string C-type string
//			SET STRING TOKEN
//			(the attribute of the string token is the offset from
//			the beginning of the str_LTBL char buffer to the beginning
//			of the string (TEXT in the example))

//			return t;
//		ELSE
//			THE STRING LITERAL IS ILLEGAL
//			SET ERROR TOKEN FOR ILLEGAL STRING (see assignment)
//			DO NOT STORE THE ILLEGAL STRING IN THE str_LTBL
		
//			return t;
   
//	IF (c == ANOTHER CHARACTER)
//		SET TOKEN
//		return t;
/* Process state transition table */  
        
//	IF (c is a digit OR c is a letter){
  
//	SET THE MARK AT THE BEGINING OF THE LEXEME
//	b_setmark(sc_buf,forward);
//	...
			if (c == 254) { scerrnum = 100; t = runError(); return t; }
	if (!isalpha(c) && !isdigit(c)) { t = errSymbol(c); return t; }
	b_setmark(sc_buf, b_getc_offset(sc_buf));
	//b_retract(sc_buf);
	while(1) 
	{
		state = get_next_state(state, c, &accept);
		if(accept == NOAS) {
			c = b_getc(sc_buf);
			continue;
		}
		break;
	} // end while(1)
	//aa_table[state];
/*	CODE YOUR FINITE STATE MACHINE HERE (FSM or DFA)
	IT IMPLEMENTS THE FOLLOWING ALGORITHM:
	
	FSM0. Begin with state = 0 and the input character c 
	FSM1. Get the next state from the transition table calling                       
		state = get_next_state(state, c, &accept);
	FSM2. Get the next character
	FSM3. If the state is not accepting (accept == NOAS), go to step FSM1
		If the step is accepting, token is found, leave the machine and
		call an accepting function as described below.*/
	//if (state == 2 || state == 5 || state == 8 || state == 10 || state == ES) 
	//	b_retract(sc_buf);7

	lexstart = b_mark(sc_buf);
	lexend = b_getc_offset(sc_buf);
	lex_buf = b_create(b_capacity(sc_buf), 0, 'f');
	b_retract_to_mark(sc_buf);
	b_retract(sc_buf);
	while (b_getc_offset(sc_buf) < (state == ES ? lexend : lexend - 1))
	{
		b_addc(lex_buf, c = b_getc(sc_buf));
	}
	b_addc(lex_buf,'\0');
	if (state == ES)
		t = aa_table[12](b_setmark(lex_buf, 0));
	else
		t = aa_table[state](b_setmark(lex_buf, 0));
	b_destroy(lex_buf);
	if (t.code == SVID_T)
		b_getc(sc_buf);
	return t;
  /*  
  RETRACT  getc_offset IF THE FINAL STATE IS A RETRACTING FINAL STATE
  GET THE BEGINNING AND THE END OF THE LEXEME
  lexstart = b_getmark(sc_buf);
  SET lexend TO getc_offset USING AN APPROPRIATE BUFFER FUNCTION
  CREATE  A TEMPORRARY LEXEME BUFFER HERE;
  lex_buf = b_create(...);
   . RETRACT getc_offset to the MARK SET PREVIOUSLY AT THE BEGINNING OF THE LEXEME AND
   . USING b_getc() COPY THE LEXEME BETWEEN lexstart AND lexend FROM THE INPUT BUFFER INTO lex_buf USING b_addc(...),
   . WHEN VID (KEYWORDS INCLUDED), FPL OR IL IS RECOGNIZED
   . YOU MUST CALL THE ACCEPTING FUNCTION USING THE ARRAY aa_table ,WHICH
   . CONTAINS POINTERS TO FUNCTIONS. THE ARRAY INDEX OF THE FUNCTION TO BE
   . CALLED IS STORED IN THE VARIABLE state.
   . YOU ARE NOT ALLOWED TO CALL ANY OF THE ACCEPTING FUNCTIONS BY NAME.
   . THE ARGUMENT TO THE FUNCTION IS THE STRING STORED IN lex_buf.
   ....
    b_destroy(lex_buf);
   return t;
      
     CHECK OTHER CHARS HERE if NEEDED, SET A TOKEN AND RETURN IT.
     FOR ILLEGAL CHARACTERS SET ERROR TOKEN. 
     THE ILLEGAL CHAR IS THE ATTRIBUTE OF THE ERROR TOKEN 
     IN A CASE OF RUNTIME ERROR, THE FUNCTION MUST STORE 
     A NON-NEGATIVE NUMBER INTO THE GLOBAL VARIABLE scerrnum
     AND RETURN AN ERROR TOKEN. THE ERROR TOKEN ATTRIBUTE MUST
     BE THE STRING "RUN TIME ERROR: "
*/
	}//end while(1)
}


/*DO NOT MODIFY THE CODE OF THIS FUNCTION
YOU CAN REMOVE THE COMMENTS*/ 

int get_next_state(int state, char c, int *accept)
{
	int col;
	int next;
	col = char_class(c);
	next = st_table[state][col];
#ifdef DEBUG
printf("Input symbol: %c Row: %d Column: %d Next: %d \n",c,state,col,next);
#endif
/*
The assert(int test) macro can be used to add run-time diagnostic to programs
and to "defend" from producing unexpected results.
assert() is a macro that expands to an if statement;
if test evaluates to false (zero), assert aborts the program
(by calling abort()) and sends the following message on stderr:

Assertion failed: test, file filename, line linenum

The filename and linenum listed in the message are the source file name
and line number where the assert macro appears.
If you place the #define NDEBUG directive ("no debugging")
in the source code before the #include <assert.h> directive,
the effect is to comment out the assert statement.
*/
		assert(next != IS);

/*
The other way to include diagnostics in a program is to use
conditional preprocessing as shown bellow. It allows the programmer
to send more details describing the run-time problem. 
Once the program is tested thoroughly #define DEBUG is commented out
or #undef DEBUF is used - see the top of the file.
*/ 
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

/*THIS FUNCTION RETURNS THE COLUMN NUMBER IN THE TRANSITION
TABLE st_table FOR THE INPUT CHARACTER c.
SOME COLUMNS MAY REPRESENT A CHARACTER CLASS .
FOR EXAMPLE IF COLUMN 1 REPRESENTS [A-Z]
THE FUNCTION RETURNS 1 EVERY TIME c IS ONE
OF THE LETTERS A,B,...,Z.*/

	return val;
}



/*HERE YOU WRITE THE DEFINITIONS FOR YOUR ACCEPTING FUNCTIONS. 
*************************************************************/

/*ACCEPTING FUNCTION FOR THE arithmentic variable identifier AND keywords (VID - AVID/KW)
REPLACE XX WITH THE CORRESPONDING ACCEPTING STATE NUMBER*/

Token aa_func02(char lexeme[]) {
	int i = 0;
	int kw_Index;
	Token t;
	//printf("lexeme: %s", lexeme);
	if((kw_Index = iskeyword(lexeme)) != -1) {
		t.code = KW_T;
		t.attribute.kwt_idx = kw_Index;
		return t;
	}
	else 
	{
		t.code = AVID_T;
		if(strlen(lexeme) >= VID_LEN) 
		{
			for(i = 0; i < VID_LEN; i++)
			{
				t.attribute.vid_lex[i] = lexeme[i];
			}
		t.attribute.vid_lex[VID_LEN] = '\0';
		}
		else 
		{
			for(i = 0; i < strlen(lexeme); i++)
			{
				t.attribute.vid_lex[i] = lexeme[i];
			}
			t.attribute.vid_lex[strlen(lexeme)] = '\0';
		}
	}

/*WHEN CALLED THE FUNCTION MUST
1. CHECK IF THE LEXEME IS A KEYWORD.
   IF YES, IT MUST RETURN A TOKEN WITH THE CORRESPONDING ATTRIBUTE
   FOR THE KEYWORD. THE ATTRIBUTE CODE FOR THE KEYWORD
   IS ITS INDEX IN THE KEYWORD LOOKUP TABLE (kw_table in table.h).
   IF THE LEXEME IS NOT A KEYWORD, GO TO STEP 2.
2. SET a AVID TOKEN.
   IF THE lexeme IS LONGER than VID_LEN (see token.h) CHARACTERS,
   ONLY FIRST VID_LEN CHARACTERS ARE STORED 
   INTO THE VARIABLE ATTRIBUTE ARRAY vid_lex[] (see token.h).
   ADD \0 AT THE END TO MAKE A C-type STRING.*/
  return t;
}


/*ACCEPTING FUNCTION FOR THE string variable identifier (VID - SVID)
REPLACE XX WITH THE CORRESPONDING ACCEPTING STATE NUMBER*/

Token aa_func03(char lexeme[]) {
	Token t;
	int i = 0;

	// set SVID token
	t.code = SVID_T;
	if(strlen(lexeme) >= VID_LEN-1) 
		{
			for(i = 0; i < VID_LEN-1; i++)
			{
				t.attribute.vid_lex[i] = lexeme[i];
			}
		t.attribute.vid_lex[VID_LEN-1] = '%';
		t.attribute.vid_lex[VID_LEN] = '\0';
		}
		else 
		{
			for(i = 0; i < strlen(lexeme); i++)
			{
				t.attribute.vid_lex[i] = lexeme[i];
			}
			t.attribute.vid_lex[strlen(lexeme)] = '%';
			t.attribute.vid_lex[strlen(lexeme)+1] = '\0';
		}

/*WHEN CALLED THE FUNCTION MUST
1. SET a SVID TOKEN.
   IF THE lexeme IS LONGER than VID_LEN characters,
   ONLY FIRST VID_LEN-1 CHARACTERS ARE STORED
   INTO THE VARIABLE ATTRIBUTE ARRAY vid_lex[],
   AND THEN THE % CHARACTER IS APPENDED TO THE NAME.
   ADD \0 AT THE END TO MAKE A C-type STRING.*/
  
  return t;
}


/*ACCEPTING FUNCTION FOR THE floating-point literal (FPL)*/

Token aa_func08(char lexeme[]) {
	Token t;
	int i = 0;
	if((float)atof(lexeme) >= FLT_MIN && (float)atof(lexeme) <= FLT_MAX || !atof(lexeme)) {
		t.code = FPL_T;
		t.attribute.flt_value = (float) atof(lexeme);
		return t;
	}
	t.code = ERR_T;
	if(strlen(lexeme) >= ERR_LEN-1) 
		{
			for(i = 0; i < ERR_LEN; i++)
			{
				t.attribute.err_lex[i] = lexeme[i];
			}
		t.attribute.err_lex[ERR_LEN] = '\0';
		}
		else 
		{
			for(i = 0; i < strlen(lexeme); i++)
			{
				t.attribute.err_lex[i] = lexeme[i];
			}
			t.attribute.err_lex[strlen(lexeme)] = '\0';
		}
	return t;

/*THE FUNCTION MUST CONVERT THE LEXEME TO A FLOATING POINT VALUE,
WHICH IS THE ATTRIBUTE FOR THE TOKEN.
THE VALUE MUST BE IN THE SAME RANGE AS the value of 4-byte float in C.
IN CASE OF ERROR (OUT OF RANGE) THE FUNCTION MUST RETURN ERROR TOKEN
THE ERROR TOKEN ATTRIBUTE IS  lexeme*/

}


/*ACCEPTING FUNCTION FOR THE integer literal(IL) - decimal constant (DIL) AND ZERO (0)*/

Token aa_func05(char lexeme[]) {
	Token t;
	int i = 0;
	if(atoi(lexeme) >= SHRT_MIN && atoi(lexeme) <= SHRT_MAX || !atoi(lexeme)) {
		t.code = INL_T;
		t.attribute.int_value = atoi(lexeme);
		return t;
	}
	t.code = ERR_T;
	if(strlen(lexeme) >= ERR_LEN-1) 
		{
			for(i = 0; i < ERR_LEN; i++)
			{
				t.attribute.err_lex[i] = lexeme[i];
			}
		t.attribute.err_lex[ERR_LEN] = '\0';
		}
		else 
		{
			for(i = 0; i < strlen(lexeme); i++)
			{
				t.attribute.err_lex[i] = lexeme[i];
			}
			t.attribute.err_lex[strlen(lexeme)] = '\0';
		}
	return t;

/*THE FUNCTION MUST CONVERT THE LEXEME REPRESENTING A DECIMAL CONSTANT AND 0
TO A DECIMAL INTEGER VALUE, WHICH IS THE ATTRIBUTE FOR THE TOKEN.
THE VALUE MUST BE IN THE SAME RANGE AS the value of 2-byte int in C.
IN CASE OF ERROR (OUT OF RANGE) THE FUNCTION MUST RETURN ERROR TOKEN
THE ERROR TOKEN ATTRIBUTE IS  lexeme*/
	
}


/*ACCEPTING FUNCTION FOR THE integer literal(IL) - octal constant (OIL)*/

Token aa_func10(char lexeme[]) {
	Token t;
	int i = 0;

	if(atool(lexeme) >= SHRT_MIN && atool(lexeme) <= SHRT_MAX) {
		t.code = INL_T;
		t.attribute.int_value = atool(lexeme);
		return t;
	}

	t.code = ERR_T;
	if(strlen(lexeme) >= ERR_LEN-1) 
		{
			for(i = 0; i < ERR_LEN; i++)
			{
				t.attribute.err_lex[i] = lexeme[i];
			}
		t.attribute.err_lex[ERR_LEN] = '\0';
		}
		else 
		{
			for(i = 0; i < strlen(lexeme); i++)
			{
				t.attribute.err_lex[i] = lexeme[i];
			}
			t.attribute.err_lex[strlen(lexeme)] = '\0';
		}

/*THE FUNCTION MUST CONVERT THE LEXEME REPRESENTING AN OCTAL CONSTANT
TO A DECIMAL INTEGER VALUE WHICH IS THE ATTRIBUTE FOR THE TOKEN.
THE VALUE MUST BE IN THE SAME RANGE AS the value of 2-byte int in C.
THIS FUNCTION IS SIMILAR TO THE FUNCTION ABOVE AND THEY CAN BE
COMBINED INTO ONE FUNCTION
THE MAIN DIFFERENCE IS THAT THIS FUNCTION CALLS
THE FUNCTION atool(char *lexeme) WHICH CONVERTS AN ASCII STRING
REPRESENTING AN OCTAL NUMBER TO INTEGER VALUE
IN CASE OF ERROR (OUT OF RANGE) THE FUNCTION MUST RETURN ERROR TOKEN
THE ERROR TOKEN ATTRIBUTE IS  lexeme*/
	
	return t;
}


/*ACCEPTING FUNCTION FOR THE ERROR TOKEN*/
/* could be either 12 OR 13; will be clear once we check against his test files */
Token aa_func12(char lexeme[]) {
	Token t;
	int i = 0;
	t.code = ERR_T;
	if(strlen(lexeme) >= ERR_LEN) 
	{
		for(i = 0; i < ERR_LEN; i++)
		{
			t.attribute.err_lex[i] = lexeme[i];
		}
	t.attribute.err_lex[ERR_LEN] = '\0';
	}
	else 
	{
		for(i = 0; i < strlen(lexeme); i++)
		{
			t.attribute.err_lex[i] = lexeme[i];
		}
		t.attribute.err_lex[strlen(lexeme)] = '\0';
	}
/*THE FUNCTION SETS THE ERROR TOKEN. lexeme[] CONTAINS THE ERROR
THE ATTRIBUTE OF THE ERROR TOKEN IS THE lexeme ITSELF
AND IT MUST BE STORED in err_lex.  IF THE ERROR LEXEME IS LONGER
than ERR_LEN caharacters, only the first ERR_LEN characters are
stored in err_lex.*/

	return t;
}


/*CONVERSION FUNCTION*/

long atool(char *lexeme) {

/*THE FUNCTION CONVERTS AN ASCII STRING
REPRESENTING AN OCTAL INTEGER CONSTANT TO INTEGER VALUE*/

	char *ptr;
	long int x;

	x = strtol(lexeme, &ptr, 8);

	return x;
}

/*HERE YOU WRITE YOUR ADDITIONAL FUNCTIONS (IF ANY).
FOR EXAMPLE*/

int iskeyword(char *kw_lexeme) {

	int i = 0;

	// checks lexeme for keyword
	for(i = 0; i < KWT_SIZE; i++) {
		// if keyword is found
		if(strstr(kw_lexeme, kw_table[i]) != NULL) {
			return i;
		}
	}

	return -1;
}

short getString(Buffer* tsc_Buf, short counter)
{
	char c = b_getc(tsc_Buf);
	if (c == '\0') { b_retract_to_mark(tsc_Buf); return -1; }
	if (c != '"') { return getString(tsc_Buf, ++counter); }
	else if (counter == 0) { return counter; }
	b_retract_to_mark(tsc_Buf);
	return counter;
}

int copyString(Buffer* s_Buf, Buffer* t_Buf, int counter)
{
	if (counter < 0)
		return 0;
	if (counter > 0 ) { b_addc(t_Buf, b_getc(s_Buf)); return copyString(s_Buf, t_Buf, --counter); }
	b_addc(t_Buf, '\0');
	return 1;
}

Token errorString(Buffer* tsc_Buf)
{
	Token errToken;
	char counter = -1;
	char c;
	errToken.code = ERR_T;
	b_retract(tsc_Buf);
	while (++counter < ERR_LEN - 3)
	{
		c = b_getc(tsc_Buf);
		if (c == '\0') { errToken.attribute.err_lex[counter] = c; return errToken; }
		errToken.attribute.err_lex[counter] = c;
	}
	for (counter; counter < ERR_LEN; ++counter)
		errToken.attribute.err_lex[counter] = '.';
	errToken.attribute.err_lex[ERR_LEN] = '\0';
	while (b_getc(tsc_Buf)) {}
	return errToken;
}

Token runError()
{
	char runError[16] = {'R', 'U', 'N', ' ', 'T', 'I', 'M', 'E', ' ', 'E', 'R', 'R', 'O', 'R', ' ', '\0'};
	Token errToken;
	char counter = -1;
	char c;
	errToken.code = ERR_T;
	while (runError[++counter])
		errToken.attribute.err_lex[counter] = runError[counter];
	errToken.attribute.err_lex[counter] = runError[counter];
	return errToken;
}

Token errSymbol(char c)
{
	Token t;
	t.code = ERR_T;
	t.attribute.err_lex[0] = c;
	t.attribute.err_lex[1] = '\0';
	return t;
}
