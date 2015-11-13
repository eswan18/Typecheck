#include <stdlib.h>
#include "stmt.h"

struct stmt *stmt_create(stmt_kind_t kind, struct decl *d, struct expr *init_expr, struct expr *e, struct expr *next_expr, struct stmt *body, struct stmt *else_body) {
	struct stmt *stmt = malloc(sizeof(struct stmt));
	stmt -> kind = kind;
	stmt -> decl = d;
	stmt -> init_expr = init_expr;
	stmt -> expr = e;
	stmt -> next_expr = next_expr;
	stmt -> body = body;
	stmt -> else_body = else_body;
	return stmt;
}

void stmt_print(struct stmt *s, int indent) {
	if(!s)
		return;
	printf("\n");
	int i;
	for(i=0;i<indent-1;i++)
		printf("\t");
	//blocks should have their braces indented less than the indent number:
	if(s->kind != STMT_BLOCK)
		printf("\t");
	switch(s->kind) {
		case STMT_DECL:
			decl_print(s->decl,0);
			break;
		case STMT_EXPR:
			expr_print(s->expr);
			printf(";");
			break;
		case STMT_IF_ELSE:
			printf("if(");
			expr_print(s->expr);
			printf(")");
			stmt_print(s->body,indent+1);
			printf("\n");
			for(i=0;i<indent;i++)
				printf("\t");
			if(s->else_body)
				printf("else");
			stmt_print(s->else_body,indent+1);
			break;
		case STMT_FOR:
			printf("for(");
			expr_print(s->init_expr);
			printf(";");
			expr_print(s->expr);
			printf(";");
			expr_print(s->next_expr);
			printf(")");
			stmt_print(s->body,indent+1);
			break;
		case STMT_PRINT:
			printf("print ");
			expr_print(s->expr);
			printf(";");
			break;
		case STMT_RETURN:
			printf("return");
			if (s->expr) {
				printf(" ");
				expr_print(s->expr);
			}
			printf(";");
			break;
		case STMT_BLOCK:
			printf("{");
			stmt_print(s->body,indent+1);
			printf("\n");
			for(i=0;i<indent-1;i++)
				printf("\t");
			printf("}");
			break;
	}
	stmt_print(s->next,indent);
}
