#include <stdlib.h>
#include <string.h>
#include "decl.h"
#include "symbol.h"
#include "scope.h"

struct decl *decl_create(char *name, struct type *t, struct expr *v, struct stmt *c, struct decl *next) {
	struct decl *decl = malloc(sizeof(struct decl));
	char *n = malloc(sizeof(char) * 256);
	strcpy(n,name);
	decl -> name = n;
	decl -> type = t;
	decl -> value = v;
	decl -> code = c;
	decl -> next = next;
	return decl;
}

void decl_print(struct decl *d, int indent) {
	if(!d)
		return;
	int i;
	for(i=0;i<indent;i++)
		printf("\t");
	printf("%s: ",d->name);
	type_print(d->type);
	if(d->value) {
		printf("=");
		expr_print(d->value);
	}
	if(d->code) {
		printf("= {");
		stmt_print(d->code,indent+1);
		printf("\n");
		for(i=0;i<indent;i++)
			printf("\t");
		printf("}");
	} else {
		printf(";");
	}
	printf("\n");
	decl_print(d->next,indent);
}

void decl_resolve(struct decl *d) {
	if (!d)
		return;
	//Check if the name is already defined in the local scope
	if (scope_lookup_local(d->name)) {
		fprintf(stderr,"Error: variable %s already defined in current scope\n",d->name);
		exit(1);
	}
	//Create the symbol
	struct symbol *symbol = 0;
	if (scope_level() == 0)
		symbol = symbol_create(SYMBOL_GLOBAL,d->type,d->name);
	else {
		symbol = symbol_create(SYMBOL_LOCAL,d->type,d->name);
		symbol->which = scope_symbol_count(SYMBOL_LOCAL)+1;
	}
	//Bind it and resolve internal expressions
	scope_bind(d->name, symbol);
	expr_resolve(d->value);
	//If it's a function, enter a new scope and resolve the internal statement
	if (d->code) {
		scope_enter();
		param_list_resolve(d->type->params);
		stmt_resolve(d->code);
		scope_exit();
	}
	decl_resolve(d->next);
}
