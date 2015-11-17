#ifndef SCOPE_H
#define SCOPE_H

#include "symbol.h"
#include "hash_table.h"

struct scope_table *global_scope_table;

struct scope_table {
	struct hash_table *hash_table;
	struct scope_table *next;
};

void scope_enter();
void scope_exit();
void scope_bind(const char *name, struct symbol *s);
struct symbol *scope_lookup(const char *name);

#endif
