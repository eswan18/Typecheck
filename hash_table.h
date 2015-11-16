#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "symbol.h"

struct hash_table_entry {
	char *name;
	symbol *symbol;
	struct hash_table_entry *next;
};

struct hash_table {
	struct hash_table_entry *entry_list;
	struct hash_table *last;
};



#endif
