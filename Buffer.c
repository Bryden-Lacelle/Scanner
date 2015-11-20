/*****************************************************************************************************
File Name:			Buffer.c
Compiler:			MS Visual Studio 2012	
Author:				Bryden Lacelle 040754048
Course:				CST8152 - Compilers Lab Section 011
Assignment:			Assignment 1: The Buffer
Date:				2015/09/29
Professor:			Svillen Ranev
Purpose:			Create a fully operational buffer with a maximum capacity of SHRT_MAX
Function List:		b_create ();
					b_addc ();
					b_reset ();
					b_destroy ();
					b_isfull ();
					b_size ();
					b_capacity();
					b_setmark ();
					b_mark ();
					b_mode ();
					b_inc_factor ();
					b_load ();
					b_isempty ();
					b_eob ();
					b_getc ();
					b_print ();
					b_pack ();
					b_rflag ();
					b_retract ();
					b_retract_to_mark ();
					b_getc_offset ();
*****************************************************************************************************/
#include "buffer.h"
#include <stdlib.h>

/*****************************************************************************************************
Purpose:			Create and initialize a BufferDescriptor struct for use
Author:				Bryden Lacelle
History/Versions:	Version 1.0 2015/09/29
Called Versions:	calloc(), malloc(), free()
Parameters:			short, char, char ((-1) - 1)
Return Value:		BufferDescriptor*, NULL
Algorithm:			Checks parameters for invalid parameters and returns NULL if invalid parameters are detected
					Initializes a new BufferDescriptor* using calloc and initializes cb_head based on the 
					init_capacity using malloc.
					mode and inc_factor values are determined by o_mode and set appropriatly.
					Buffer capacity is set to init_capacity and addc_offset and 
					getc_offset are set to 0
*****************************************************************************************************/
Buffer* b_create (short init_capacity, char inc_factor, char o_mode)
{
	Buffer* newBuffer; /* Initialize a new Buffer* */
	if (init_capacity < 0) /* Returns NULL if invalid parameters are found */
		return NULL;
	if (o_mode != 'f' && o_mode != 'a' && o_mode != 'm') 
		return NULL;
	if ((unsigned char) inc_factor > MULT_MODE_MAX && o_mode == 'm')
		return NULL;
	if (!init_capacity && o_mode == 'f')
		return NULL;
    if (!(newBuffer = (Buffer*) calloc(1, sizeof(Buffer)))) /* TRIGGERS WARNING: Uses conditional statment to verify memory has been allocated with calloc*/
		return NULL;
	if (!(newBuffer->cb_head = (char*) malloc(init_capacity * sizeof(char)))) /*TRIGGERS WARNING: If the malloc of cb_head is unsuccessful free the buffer and return NULL */
	{
		free(newBuffer);
		return NULL;
	}
	if (o_mode == 'f' || inc_factor == 0) /* Set buffer to fixed mode */
	{
		newBuffer->mode = FIXED_MODE;
		newBuffer->inc_factor = FIXED_MODE;
	}
	else if (o_mode == 'a') /* Set buffer to additive mode */
	{
		newBuffer->mode = ADD_MODE;
		newBuffer->inc_factor = inc_factor;
	}
	else if (o_mode == 'm') /* Set buffer to multiplitive mode */
	{
		newBuffer->mode = MULT_MODE;
		newBuffer->inc_factor = inc_factor;
	}
	newBuffer->capacity = init_capacity;
	return newBuffer;
}

/*****************************************************************************************************
Purpose:			Add a single symbol into the buffer
Author:				Bryden Lacelle
History/Versions:	Version 1.0 2015/09/29
Called Versions:	b_isfull(), b_capacity(), realloc()
Parameters:			pBuffer const, char
Return Value:		pBuffer
Algorithm:			Uses b_isfull() to determine if the buffer is full.
					If it is it will attempt to expand the size of the Buffer
					In fixed mode the buffer will not be expanded, the function will return NULL
					In ADD_MODE the inc_factor will be added to the current capacity if the result is 
					positive the new capacity will be set to that value if the result is negative the 
					function returns NULL
					In MULT_MODE if the capicity is already set to SHRT_MAX no furthur expansion can be done
					so NULL is returned. If the buffer can still be expanded it will increase by init_factor 
					as a percent of SHRT_MAX - b_capacity(). If the result is 0 capacity is set to SHRT_MAX
					If the buffer is expanded cb_head is reallocated to the new capacity of the buffer.
					If the new memory address of cb_head does not match it's previous memory location r_flag
					is set.
					A symbol that is not an EOF character is then added to the tail of the buffer
*****************************************************************************************************/
pBuffer b_addc (pBuffer const pBD, char symbol)
{
	char* old_mem_address = pBD->cb_head; /* Variable to store the old memory address of cb_head */
	if (!pBD) /* Return NULL if invalid parameters are found */
		return NULL;
	pBD->r_flag = RESET_R_FLAG; /* Reset r_flag */
	if (b_isfull(pBD)) /* Call b_isfull to determine if the buffer is full before adding a symbol */
	{
		if (pBD->mode == FIXED_MODE) /* Buffer can not be expanded in FIXED_MODE */
			return NULL;
		if (b_capacity(pBD) == SHRT_MAX || b_capacity(pBD) == R_FAIL_1) /* Buffer has reached it's max capacity no furthur expansion can be done */
				return NULL;
		else if (pBD->mode == ADD_MODE) /* Expand buffer using additive mode */
		{
			if ((short) ((short) pBD->capacity + (unsigned char) pBD->inc_factor * sizeof(char)) > 0) /* If the result does not overflow SHRT_MAX increase buffer capacity */
				pBD->capacity += (unsigned char) pBD->inc_factor * sizeof(char);
			else 
				return NULL;
		}
		else if (pBD->mode == MULT_MODE) /* Expand buffer using multiplitive mode */
		{
			short b_space = 0, incriment = 0; /* Initialize the availible space in the buffer and incriment for the capcity */
			b_space = SHRT_MAX - b_capacity(pBD);
			incriment = (short) (((long) b_space * pBD->inc_factor / 100.f) * sizeof(char));
			if ((short) (incriment + b_capacity(pBD)) == b_capacity(pBD)) /* Set capacity to SHRT_MAX if the result does not increase capacity */
				pBD->capacity = SHRT_MAX;
			else 
				pBD->capacity += incriment;
		}
		else 
			return NULL;
		if ((pBD->cb_head = (char*) realloc(pBD->cb_head, pBD->capacity)) == NULL) /* reallocate memory for cb_head using the new capacity */
			return NULL;
		if (old_mem_address != pBD->cb_head) /* Set r_flag if the new memory address of cb_head has changed */
		{
			pBD->r_flag = SET_R_FLAG;
			old_mem_address = NULL;
		}
	}
	pBD->cb_head[pBD->addc_offset++] = symbol;
	return pBD;
}

/*****************************************************************************************************
Purpose:			Resets the buffer to appear empty without deleting the contents of the original buffer
Author:				Bryden Lacelle
History/Versions:	Version 1.0 2015/09/29
Called Versions:	N/A
Parameters:			Buffer* const
Return Value:		int
Algorithm:			Resets appropriate struct members of the buffer to make the buffer appear empty 
					so that it can start overwriting the old buffer with new symbols when b_addc() 
					is called
*****************************************************************************************************/
int b_reset (Buffer* const pBD)
{
	if (pBD)
	{
		pBD->addc_offset = 0;
		pBD->getc_offset = 0;
		pBD->mark_offset = 0;
		pBD->r_flag = 0;
		pBD->eob = 0;
		return 1;
	}
	return R_FAIL_1;

}

/*****************************************************************************************************
Purpose:			Free memory allocated to the buffer
Author:				Bryden Lacelle
History/Versions:	Version 1.0 2015/09/29
Called Versions:	free()
Parameters:			Buffer* const
Return Value:		void
Algorithm:			free dynamically allocated memory for cb_head and the BufferDescriptor struct
*****************************************************************************************************/
void b_destroy (Buffer* const pBD)
{
	if (pBD) 
	{
		free(pBD->cb_head);
		free(pBD);
	}
}

/*****************************************************************************************************
Purpose:			Determines if the buffer has reached capacity
Author:				Bryden Lacelle
History/Versions:	Version 1.0 2015/09/29
Called Versions:	N/A
Parameters:			Buffer* const
Return Value:		int
Algorithm:			If addc_offset is equal to capacity the buffer is full
*****************************************************************************************************/

#ifndef B_ISFULL /* Compile function if B_ISFULL is undefined */
int b_isfull (Buffer* const pBD)
{
	if (!pBD)
		return R_FAIL_1;
	if (pBD->addc_offset == pBD->capacity)
		return 1;
	else
		return 0;
}
#endif

/*****************************************************************************************************
Purpose:			Determine how many symbols are stored in the buffer
Author:				Bryden Lacelle
History/Versions:	Version 1.0 2015/09/29
Called Versions:	b_eob()
Parameters:			Buffer* const
Return Value:		short
Algorithm:			Returns the value of addc_offset
*****************************************************************************************************/
short b_size (Buffer* const pBD)
{
	return pBD->addc_offset;
}

/*****************************************************************************************************
Purpose:			Returns the capacity of the buffer
Author:				Bryden Lacelle
History/Versions:	Version 1.0 2015/09/29
Called Versions:	N/A
Parameters:			Buffer* const
Return Value:		short
Algorithm:			If the buffer has a valid capacity return the size of the buffer as a short
*****************************************************************************************************/
short b_capacity(Buffer* const pBD)
{
	if (!pBD)
		return R_FAIL_1;
	return pBD->capacity;

}

/*****************************************************************************************************
Purpose:			Set a mark at a specific location within the buffer
Author:				Bryden Lacelle
History/Versions:	Version 1.0 2015/09/29
Called Versions:	N/A
Parameters:			Buffer* const, short
Return Value:		char*
Algorithm:			Ensures that the mark does not underflow or overflow the size of the buffer before 
					setting the mark.
*****************************************************************************************************/
char* b_setmark (Buffer* const pBD, short mark)
{
	if (!pBD)
		return NULL;
	if (mark >= 0 && mark <= pBD->addc_offset) /* Set mark between 0 and addc_offset inclusive */
	{
		pBD->mark_offset = mark;
		return pBD->cb_head + pBD->mark_offset;
	}
	return NULL;
}

/*****************************************************************************************************
Purpose:			Return the mark_offset
Author:				Bryden Lacelle
History/Versions:	Version 1.0 2015/09/29
Called Versions:	N/A
Parameters:			Buffer* const
Return Value:		short
Algorithm:			returns mark_offset
*****************************************************************************************************/
short b_mark (Buffer* const pBD)
{
	if (!pBD)
		return R_FAIL_1;
	return pBD->mark_offset;
}

/*****************************************************************************************************
Purpose:			Returns the operational mode of the buffer
Author:				Bryden Lacelle
History/Versions:	Version 1.0 2015/09/29
Called Versions:	N/A
Parameters:			Buffer* const
Return Value:		int
Algorithm:			Ensures that the mode is valid (between -1 and 1) before returning the operational
					mode as an int. If it fails R_FAIL_2 (-2) is returned
*****************************************************************************************************/
int b_mode (Buffer* const pBD)
{
	if (!pBD)
		return R_FAIL_2;
	return pBD->mode;
}

/*****************************************************************************************************
Purpose:			Return the value of inc_factor
Author:				Bryden Lacelle
History/Versions:	Version 1.0 2015/09/29
Called Versions:	N/A
Parameters:			Buffer* const
Return Value:		size_t
Algorithm:			If an error occurs 256 is returned otherwise the value inc_factor is returned as a value 
					between 0-255
*****************************************************************************************************/
size_t b_inc_factor (Buffer* const pBD)
{
	if (!pBD)
		return INC_FACTOR_ERROR;
	return (unsigned char) pBD->inc_factor;
}

/*****************************************************************************************************
Purpose:			Load a file into the buffer
Author:				Bryden Lacelle
History/Versions:	Version 1.0 2015/09/29
Called Versions:	feof(), b_addc()
Parameters:			FILE* const, Buffer* const
Return Value:		int
Algorithm:			While the end of file has not been reached call b_addc to add a single symbol
					into the buffer and incriment char_count. When the EOF character is reached
					exit of the loop and return char_count
*****************************************************************************************************/
int b_load (FILE* const fi, Buffer* const pBD)
{
	int char_count = 0; /* Counts the number of chars added to the buffer */
	char symbol = 0; /* Current symbol to add to the buffer */
	if (!pBD)
		return R_FAIL_1;
	while (1) /* TRIGGERS WARNING: Loop uses break statement to escape from loop*/
	{
		symbol = (char) fgetc(fi);
		if (feof(fi)) /* If EOF is hit exit loop */
			break;
		if (b_addc(pBD, symbol) == NULL) /* Return an error if the character is not added to the buffer */
			return LOAD_FAIL;
		++char_count;
	}
	return char_count;
}

/*****************************************************************************************************
Purpose:			Determine if the buffer is empty
Author:				Bryden Lacelle
History/Versions:	Version 1.0 2015/09/29
Called Versions:	N/A
Parameters:			Buffer* const
Return Value:		int
Algorithm:			If addc_offset is set to 0 the buffer is empty and returns 1
					If addc_offest is greater then 0 the buffer is not empty and returns 0
					If an error occurs R_FAIL_1 (-1) is returned
*****************************************************************************************************/
int b_isempty (Buffer* const pBD)
{
	if (!pBD)
		return R_FAIL_1;
	if (!pBD->addc_offset)
		return 1;
	if (pBD->addc_offset > 0)
		return 0;
	else 
		return R_FAIL_1;
}

/*****************************************************************************************************
Purpose:			Determine if the end of buffer is reached
Author:				Bryden Lacelle
History/Versions:	Version 1.0 2015/09/29
Called Versions:	N/A
Parameters:			Buffer* const
Return Value:		int
Algorithm:			If getc_offest is equal to addc_offset or capacity the buffer is full and the
					function returns 1 otherwise if getc_offset is positive and not equal to SHRT_MAX
					the buffer is not full and the function returns 0
					If an error occurs the function returns R_FAIL_1 (-1)
*****************************************************************************************************/
int b_eob (Buffer* const pBD)
{
	if (!pBD)
		return R_FAIL_1;
	return pBD->eob;
}

/*****************************************************************************************************
Purpose:			Gets a character from cb_head
Author:				Bryden Lacelle
History/Versions:	Version 1.0 2015/09/29
Called Versions:	N/A
Parameters:			Buffer* const
Return Value:		char
Algorithm:			Invalid parameters returns R_FAIL_2 (-2)
					If getc_offset equals addc_offset eob is set to 1 and R_FAIL_1 (-1) is returned
					Otherwise the char located at getc_offset in cb_head is returned and 
					getc_offset is incrimented
*****************************************************************************************************/
char b_getc (Buffer* const pBD)
{
	if (!pBD)
		return R_FAIL_2;
	if (pBD->getc_offset == pBD->addc_offset) /* getc_offset has reached the end of the buffer */
	{
		pBD->eob = 1;
		return R_FAIL_1;
	}
	pBD->eob = 0;
	return pBD->cb_head[pBD->getc_offset++];
}

/*****************************************************************************************************
Purpose:			Utility to print the contents of the buffer to the console
Author:				Bryden Lacelle
History/Versions:	Version 1.0 2015/09/29
Called Versions:	b_eob(), b_getc()
Parameters:			Buffer* const
Return Value:		int
Algorithm:			Until the end of buffer is reached the function prints the contents of the buffer 
					character by character by calling b_getc(). The function returns the amount of 
					characters stored in the buffer as an int.
*****************************************************************************************************/
int b_print (Buffer* const pBD)
{
	int char_count = 0; /* Counts the amount of chars in the buffer */
	char next_char = 0; /* Storage for the next char to print */
	if (!pBD)
		return R_FAIL_1;
	pBD->getc_offset = 0;
	if (pBD->addc_offset == 0) /* The buffer is empty return an error */
	{
		printf("The buffer is empty.\n");
		return R_FAIL_1;
	}
	while(next_char = b_getc(pBD)) /* TRIGGERS WARNING: Intended declaration in conditional expression*/
	{
		if (b_eob(pBD)) /* Break from loop when eob is reached*/
			break;
		printf("%c", next_char);
		++char_count;
	}
	pBD->getc_offset = 0;
	printf("\n");
	return char_count;
}

/*****************************************************************************************************
Purpose:			Resize the buffer is the size of the buffer + 1
Author:				Bryden Lacelle
History/Versions:	Version 1.0 2015/09/29
Called Versions:	realloc(), b_size()
Parameters:			Buffer* const
Return Value:		Buffer*
Algorithm:			Attempts the shrink the buffer to the size of the characters stored in the buffer + 1
*****************************************************************************************************/
Buffer* b_pack (Buffer* const pBD)
{
	char* old_mem_address = pBD->cb_head; /* Old memory address of cb_head  */
	if (!pBD)
		return NULL;
	if (b_size(pBD) == SHRT_MAX) /* Buffer can not be resized */
		return NULL;
	pBD->capacity = (b_size(pBD) + sizeof(char)) * sizeof(char); /* Sets capacity equal to b_size + 1 char in chars */
	pBD->r_flag = RESET_R_FLAG;
	if ((pBD->cb_head = (char*) realloc(pBD->cb_head, pBD->capacity)) != NULL) /* Attempts to reallocate cb_head to capacity */
		{
			if (old_mem_address != pBD->cb_head); /* Sets r_flag if memory location of cb_head changes */
				pBD->r_flag = SET_R_FLAG;
			pBD->r_flag = RESET_R_FLAG;
			return pBD;
		}
	return NULL;
}

/*****************************************************************************************************
Purpose:			Returns the value of r_flag
Author:				Bryden Lacelle
History/Versions:	Version 1.0 2015/09/29
Called Versions:	Buffer* const
Parameters:			Buffer* const
Return Value:		char
Algorithm:			Returns a valid r_flag (0 or 1) or -1 if an invalid r_flag is stored
*****************************************************************************************************/
char b_rflag (Buffer* const pBD)
{
	if (!pBD)
		return R_FAIL_1;
	if (pBD->r_flag == 0 || pBD->r_flag == 1)
		return pBD->r_flag;
	else 
		return R_FAIL_1;
}

/*****************************************************************************************************
Purpose:			Decriments getc_offset
Author:				Bryden Lacelle
History/Versions:	Version 1.0 2015/09/29
Called Versions:	N/A
Parameters:			Buffer* const
Return Value:		short
Algorithm:			Attempts to decriment getc_offset if the decrimented value is greater than or 
					equal to 0 the decrimented value of getc_offset is returned
					If the result causes getc_offset to be less then 0 it is incrimented back to 0 and
					R_FAIL_1 is returned
					Invalid parameters returns R_FAIL_1
*****************************************************************************************************/
short b_retract (Buffer* const pBD)
{
	if (!pBD)
		return R_FAIL_1;
	if (--(pBD->getc_offset) >= 0)
		return pBD->getc_offset;
	else
		return R_FAIL_1;
	
}

/*****************************************************************************************************
Purpose:			Sets getc_offset to mark_offset
Author:				Bryden Lacelle
History/Versions:	Version 1.0 2015/09/29
Called Versions:	N/A
Parameters:			Buffer* const
Return Value:		short
Algorithm:			Set getc_offset equal to mark_offset and return getc_offset
					If an error occurs return R_FAIL_1 (-1)
*****************************************************************************************************/
short b_retract_to_mark (Buffer* const pBD)
{
	if (!pBD)
		return R_FAIL_1;
	return pBD->getc_offset = pBD->mark_offset;

}

/*****************************************************************************************************
Purpose:			Return getc_offset
Author:				Bryden Lacelle
History/Versions:	Version 1.0 2015/09/29
Called Versions:	N/A
Parameters:			Buffer* const
Return Value:		short
Algorithm:			If a valid call is made the function returns getc_offset
					If the function fails R_FAIL_1 (-1) is returned
*****************************************************************************************************/
short b_getc_offset (Buffer* const pBD)
{
	if (!pBD)
		return R_FAIL_1;
	return pBD->getc_offset;
}
