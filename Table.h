/* Filename: table.h
* Transition Table and function declarations necessary for the scanner implementation  
* as required for CST8152 - Assignment #2.
* Version: 1.15.02
* Date: 29 September 2015
* Provided by: Svillen Ranev
* The file is incomplete. You are to complete it.
***************************************************
* REPLACE THIS HEADER WITH YOUR HEADER
***************************************************
*/

#ifndef  TABLE_H_
#define  TABLE_H_ 

#ifndef BUFFER_H_
#include "buffer.h"
#endif

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*   Source end-of-file (SEOF) sentinel symbol
*    '\0' or only one of the folowing constants: 255, 0xFF , EOF
*/

/*  Single-lexeme tokens processed separately one by one
*  in the token-driven part of the scanner
*  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' ,
*       space
*  !<comment , ',' , '"' , ';' , '-' , '+' , '*' , '/', # ,
*  .AND., .OR. , SEOF, 'wrong symbol',
*/


/* REPLACE *ESN* WITH YOUR ERROR STATE NUMBER */
#define ES -2	/* Error state */
#define IS -1	/* Invalid state */

	/* State transition table definition */

/*	REPLACE *CN* WITH YOUR COLUMN NUMBER  */

#define TABLE_COLUMNS 7
	/*transition table - type of states defined in separate table */
	int st_table[ ][TABLE_COLUMNS] = {
		/* State 0 */	{1, 6, 4, 4, IS, IS, IS}, 
		/* State 1 */	{1, 1, 1, 1, 2, 3, 2}, 
		/* State 2 */	{IS, IS, IS, IS, IS, IS, IS}, 
		/* State 3 */	{IS, IS, IS, IS, IS, IS, IS}, 
		/* State 4 */	{ES, 4, 4, 4, 7, 5, 5}, 
		/* State 5 */	{IS, IS, IS, IS, IS, IS, IS}, 
		/* State 6 */	{ES, ES, 9, ES, 7, ES, 5}, 
		/* State 7 */	{8, 7, 7, 7, 8, 8, 8}, 
		/* State 8 */	{IS, IS, IS, IS, IS, IS, IS}, 
		/* State 9 */	{ES, 9, 9, ES, ES, 10, 10}, 
		/* State 10 */	{IS, IS, IS, IS, IS, IS, IS}, 
		/* State 11 */	{IS, IS, IS, IS, IS, IS, IS}, 
		/* State 12 */	{IS, IS, IS, IS, IS, IS, IS}, 
		/* State 13 */	{IS, IS, IS, IS, IS, IS, IS}
	};
		/* Accepting state table definition */
/*		REPLACE *N1*, *N2*, and *N3* WITH YOUR NUMBERS */
#define ASWR 97		/* accepting state with retract */
#define ASNR 98		/* accepting state with no retract */
#define NOAS 99		/* not accepting state */

		int as_table[ ] = {NOAS, NOAS, ASWR, ASNR, NOAS, ASWR, NOAS, NOAS, ASWR, NOAS, ASWR, ASNR, ASWR};

		/* Accepting action function declarations */

/*		FOR EACH OF YOUR ACCEPTING STATES YOU MUST PROVIDE
			ONE FUNCTION PROTOTYPE. THEY ALL RETURN Token AND TAKE
			ONE ARGUMENT: A string REPRESENTING A TOKEN LEXEME. */

			Token aa_func02(char *lexeme);
			Token aa_func03(char *lexeme);
			Token aa_func05(char *lexeme);
			Token aa_func08(char *lexeme);
			Token aa_func10(char *lexeme);
			Token aa_func12(char *lexeme);
			Token aa_func13(char *lexeme);

/*		Replace XX with the number of the accepting state: 02, 03 and so on. */

			/* defining a new type: pointer to function (of one char * argument) 
			returning Token
			*/  

			typedef Token (*PTR_AAF)(char *lexeme);


		/* Accepting function (action) callback table (array) definition */
		/* If you do not want to use the typedef, the equivalent declaration is:
		* Token (*aa_table[])(char lexeme[]) = {
		*/

		PTR_AAF aa_table[ ] = {

			NULL, NULL, *aa_func02, *aa_func03, NULL, *aa_func05, NULL, NULL, *aa_func08, NULL, *aa_func10, NULL, *aa_func12, *aa_func12 /* change these last 2 to either 12 OR 13 */

			/*HERE YOU MUST PROVIDE AN INITIALIZATION FOR AN ARRAY OF POINTERS
			TO ACCEPTING FUNCTIONS. THE ARRAY HAS THE SAME SIZE AS as_table[ ].
			YOU MUST INITIALIZE THE ARRAY ELEMENTS WITH THE CORRESPONDING
			ACCEPTING FUNCTIONS (FOR THE STATES MARKED AS ACCEPTING IN as_table[]).
			THE REST OF THE ELEMENTS MUST BE SET TO NULL.*/

		};

		/* Keyword lookup table (.AND. and .OR. are not keywords) */

#define KWT_SIZE 8

		char *kw_table[] = {
			"ELSE",
			"IF",
			"INPUT",
			"OUTPUT",
			"PLATYPUS",
			"REPEAT",
			"THEN",
			"USING"   
		};

#endif
