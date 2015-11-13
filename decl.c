#include <stdlib.h>
#include "decl.h"
#include <string.h>

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
	if(!d) {
		return;
	}
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
