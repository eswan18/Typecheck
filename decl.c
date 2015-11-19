#include <stdlib.h>
#include <string.h>
#include "decl.h"
#include "symbol.h"
#include "scope.h"

extern int type_error_count;

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

void decl_resolve(struct decl *d, int should_print) {
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
	d->symbol = symbol;
	expr_resolve(d->value, should_print);
	//If it's a function, enter a new scope and resolve the internal statement
	if (d->code) {
		scope_enter();
		param_list_resolve(d->type->params, should_print);
		stmt_resolve(d->code, should_print);
		scope_exit();
	}
	decl_resolve(d->next, should_print);
}

void decl_typecheck(struct decl *d) {
	if(!d)
		return;
	if(d->value) {
		struct type *value_type = expr_typecheck(d->value);
		if(!type_compare(d->type,value_type)) {
			printf("Type Error: cannot assign ");
			type_print(value_type);
			printf(" to variable of type ");
			type_print(d->type);
			printf("\n");
			type_error_count++;
		}
	}
	if(d->symbol && d->symbol->kind == SYMBOL_GLOBAL) {
		if(d->value && !expr_is_constant(d->value)) {
			printf("Type Error: global variable %s may only be assigned a literal value\n",d->name);
			type_error_count++;
		}
	}
	if(d->code)
		stmt_typecheck(d->code);
	decl_typecheck(d->next);
}
