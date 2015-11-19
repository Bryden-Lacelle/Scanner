#include <string.h>
#include "stable.h"
STD sym_table;
int main(void) 
{
	int i = 0;
	char c[20];
	printf("Creating sym_table\n");
	sym_table = st_create(500);
	sym_table.st_offset = st_install(sym_table, "Hello1aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello2bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello3cccccccccccccccccccccccccccccccccccccccccccccccccccccccc", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello4ddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello5dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddc", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello6ddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddds", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello7dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello8ddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello9dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddv", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello10ddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddsf", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello11ddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddsfs", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello12ddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd..", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello13ddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd.", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello14dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddr", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello15dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddaaa", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello16ddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd5555", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello17dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddaaaaa", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello18dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddhhhh", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello19ddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd6", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello20ddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd4", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello21ddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd4", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello22ddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd3", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello23ddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd2", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello24ddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd1", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello25ddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd0", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello26ddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd7", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello27ddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd8", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello28ddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd9", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello29ddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd10", 'S', i++);
	sym_table.st_offset = st_install(sym_table, "Hello30ddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd11", 'S', i++);
	/*printf("sym_table created\n");
	printf("Installing sym_table\n");
	sym_table.st_offset = st_install(sym_table, "Test", 'S', 1);
	printf("sym_table installed\n");
	printf("Installing sym_table\n");
	sym_table.st_offset = st_install(sym_table, "11", 'I', 10);
	printf("sym_table installed\n");
	printf("Installing sym_table\n");
	sym_table.st_offset = st_install(sym_table, "10.5", 'F', 100);*/
	printf("Printing sym_table\n\n");
	st_print(sym_table);
	printf("\nsym_table Printed\n");
	printf("Destroying sym_table\n");
	st_destroy(sym_table);
	printf("sym_table deleted\n");
	return 1;
}
