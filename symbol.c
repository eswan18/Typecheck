#include <stdlib.h>
#include "symbol.h"

struct symbol *symbol_create(symbol_t kind, struct type *type, char *name) {
	struct symbol *symbol = malloc(sizeof(struct symbol));
	symbol -> kind = kind;
	symbol -> type = type;
	symbol -> name = name;
	return symbol;
}

void symbol_delete(struct symbol *s) {
	type_delete(s->type);
	free(s->name);
	free(s);
}
