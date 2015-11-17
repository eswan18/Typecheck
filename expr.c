#include <stdlib.h>
#include <string.h>
#include "expr.h"
#include "scope.h"

extern int error_count;

struct expr *expr_create( expr_t kind, struct expr *left, struct expr *right ) {
	struct expr *expr = malloc(sizeof(struct expr));
	expr -> kind = kind;
	expr -> left = left;
	expr -> right = right;
	return expr;
}

struct expr *expr_create_name( const char *n ) {
	struct expr *expr = malloc(sizeof(struct expr));
	expr -> kind = EXPR_NAME;
	char *name = malloc(sizeof(char) * 256);
	strcpy(name, n);
	expr -> name = name;
	return expr;
}

struct expr *expr_create_boolean_literal( int c ) {
	struct expr *expr = malloc(sizeof(struct expr));
	expr -> kind = EXPR_BOOLEAN_LITERAL;
	expr -> literal_value = c;
	return expr;
}

struct expr * expr_create_integer_literal( int c ) {
	struct expr *expr = malloc(sizeof(struct expr));
	expr -> kind = EXPR_INTEGER_LITERAL;
	expr -> literal_value = c;
	return expr;
}

struct expr * expr_create_character_literal(int c) {
	struct expr *expr = malloc(sizeof(struct expr));
	expr -> kind = EXPR_CHARACTER_LITERAL;
	expr -> literal_value = c;
	return expr;
}

struct expr * expr_create_string_literal( const char *str ) {
	struct expr *expr = malloc(sizeof(struct expr));
	expr -> kind = EXPR_STRING_LITERAL;
	char *s = malloc(sizeof(char) * 256);
	strcpy(s,str);
	expr -> string_literal = s;
	return expr;
}

void expr_print(struct expr *e) {
	if(!e)
		return;
	expr_print(e -> left);
	expr_pretty_print(e);
	expr_print(e->right);
	if (e->kind == EXPR_FUNC)
		printf(")");
	if (e->kind == EXPR_ARRAY_DEREF)
		printf("]");
}

void expr_pretty_print(struct expr *e) {
	if (!e)
		return;
	switch(e->kind) {
		case EXPR_ASSIGN:
			printf("=");
			break;
		case EXPR_ADD:
			printf("+");
			break;
		case EXPR_SUB:
			printf("-");
			break;
		case EXPR_MUL:
			printf("*");
			break;
		case EXPR_DIV:
			printf("/");
			break;
		case EXPR_MOD:
			printf("%%");
			break;
		case EXPR_AND:
			printf("&&");
			break;
		case EXPR_OR:
			printf("||");
			break;
		case EXPR_NOT:
			printf("!");
			break;
		case EXPR_LT:
			printf("<");
			break;
		case EXPR_LE:
			printf("<=");
			break;
		case EXPR_GT:
			printf(">");
			break;
		case EXPR_GE:
			printf(">=");
			break;
		case EXPR_EQ:
			printf("==");
			break;
		case EXPR_NE:
			printf("!=");
			break;
		case EXPR_EXPON:
			printf("^");
			break;
		case EXPR_INCR:
			printf("++");
			break;
		case EXPR_DECR:
			printf("--");
			break;
		case EXPR_NEG:
			printf("-");
			break;
		case EXPR_FUNC:
			printf("(");
			break;
		case EXPR_LIST:
			if(e->right)
				printf(",");
			break;
		case EXPR_ARRAY_DEREF:
			printf("[");
			break;
		case EXPR_NAME:
			printf("%s",e->name);
			break;
		case EXPR_BOOLEAN_LITERAL:
			if(e->literal_value == 1)
				printf("true");
			else
				printf("false");
			break;
		case EXPR_INTEGER_LITERAL:
			printf("%i",e->literal_value);
			break;
		case EXPR_CHARACTER_LITERAL:
			printf("'%c'",e->literal_value);
			break;
		case EXPR_STRING_LITERAL:
			printf("\"%s\"",e->string_literal);
			break;
	}
}

void expr_resolve(struct expr *e) {
	if(!e)
		return;
	expr_resolve(e->left);
	expr_resolve(e->right);
	if(e->kind == EXPR_NAME) {
		struct symbol *s = scope_lookup(e->name);
		if(s) {
			e->symbol = s;
			printf("%s resolves to ",e->name);
			switch(s->kind) {
				case SYMBOL_LOCAL:
					printf("local %d\n",s->which);
					break;
				case SYMBOL_GLOBAL:
					printf("global %s\n",s->name);
					break;
				case SYMBOL_PARAM:
					printf("parameter %d\n",s->which);
					break;
				default:
					printf("SHOULDN'T DEFAULT\n");
			}
		} else {
			printf("Variable %s is undefined\n",e->name);
			error_count++;
		}
	}
}
