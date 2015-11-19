#ifndef  STABLE_H_
#define  STABLE_H_

#ifndef BUFFER_H_
#include "buffer.h"
#endif
typedef union InitialValue {
	int int_val;						/* integer variable initial value */
	float fpl_val;						/* floating-point variable initial value */
	int str_offset;						/* string variable initial value (offset) */
} InitialValue;

typedef struct SymbolTableVidRecord {
	unsigned short status_field;		/* variable record status field*/
	char *plex;							/* pointer to lexeme (VID name) in CA */
	int o_line;							/* line of first occurrence */
	InitialValue i_value;				/* variable initial value */
	size_t reserved;					/* reserved for future use */
} STVR;

typedef struct SymbolTableDescriptor {
	STVR *pstvr;						/* pointer to array of STVR */
	int st_size;						/* size in number of STVR elements */
	int st_offset;						/* offset in number of STVR elements */
	Buffer *plsBD;						/* pointer to the lexeme storage buffer descriptor */
} STD;
extern STD g_sym_table;
STD st_create(int);
int st_install(STD, char*, char, int);
int st_lookup(STD, char*);
int st_update_type(STD, int, char);
int st_update_value(STD, int, InitialValue);
char st_get_type (STD, int);
void st_destroy(STD);
int st_print(STD);
static void st_setsize(void);
static void st_incoffset(void);
int st_store(STD);
int st_sort(STD, char);

#endif
