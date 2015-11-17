#include "scope.h"
#include <stdlib.h>
#include <stdio.h>

void scope_enter() {
	struct hash_table *h = hash_table_create(0,0);
	struct scope_table *st = malloc(sizeof(struct scope_table));
	st -> hash_table = h;
	st -> next = global_scope_table;
	global_scope_table = st;
}

void scope_exit() {
	struct scope_table *st = global_scope_table;
	global_scope_table = global_scope_table -> next;
	hash_table_delete(st->hash_table);
	free(st);
}

void scope_bind(const char *name, struct symbol *s) {
	if(!hash_table_insert(global_scope_table->hash_table, name, s)) {
		fprintf(stderr,"Unable to add variable %s to the current hash table.\n",name);
		exit(1);
	}
}

struct symbol *scope_lookup(const char *name) {
	return hash_table_lookup(global_scope_table->hash_table, name);
}
