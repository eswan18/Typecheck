#include <stdlib.h>
#include <stdio.h>
#include "scope.h"

struct scope_table *global_st = 0;

void scope_enter() {
	struct hash_table *h = hash_table_create(0,0);
	struct scope_table *st = malloc(sizeof(struct scope_table));
	st -> hash_table = h;
	st -> next = global_st;
	global_st = st;
}

void scope_exit() {
	struct scope_table *st = global_st;
	global_st = global_st -> next;
	hash_table_delete(st->hash_table);
	free(st);
	st = 0;
}

int scope_level() {
	int count = 0;
	struct scope_table *current_st = global_st;
	while (current_st->next) {
		current_st = current_st -> next;
		count++;
	}
	return count;
}

void scope_bind(const char *name, struct symbol *s) {
	if(!hash_table_insert(global_st->hash_table, name, s)) {
		fprintf(stderr,"Unable to add variable %s to the current scope table.\n",name);
		exit(1);
	}
}

struct symbol *scope_lookup(const char *name) {
	struct scope_table *current_st = global_st;
	struct symbol *symbol = 0;
	while (current_st) {
		symbol = hash_table_lookup(current_st->hash_table, name);
		if (symbol)
			return symbol;
		current_st = current_st->next;
	}
	return 0;
}

struct symbol *scope_lookup_local(const char *name) {
	return hash_table_lookup(global_st->hash_table, name);
}
