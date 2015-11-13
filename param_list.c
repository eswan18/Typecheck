#include <stdlib.h>
#include "param_list.h"
#include <string.h>

struct param_list *param_list_create(char *name, struct type *type, struct param_list *next) {
	struct param_list *param_list = malloc(sizeof(struct param_list));
	char *n = malloc(sizeof(char) * 256);
	strcpy(n,name);
	param_list -> name = n;
	param_list -> type = type;
	param_list -> next = next;
	return param_list;
}

void param_list_print(struct param_list *a) {
	if(!a)
		return;
	printf("%s: ",a->name);
	type_print(a->type);
	if(a->next) {
		printf(", ");
	}
	param_list_print(a->next);
}
