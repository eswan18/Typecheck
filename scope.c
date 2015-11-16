#include "scope.h"

void scope_enter() {
	//create new hash table
	//point it to old one
}

void scope_exit() {
	//remove current hash table
}

void scope_bind(const char *name, struct symbol *s) {
}

struct symbol *scope_lookup(const char *name) {
}
