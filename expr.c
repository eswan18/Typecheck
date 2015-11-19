#include <stdlib.h>
#include <string.h>
#include "expr.h"
#include "scope.h"

extern int resolve_error_count;
extern int type_error_count;

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
			resolve_error_count++;
		}
	}
}

void expr_delete(struct expr *e) {
	if(!e)
		return;
	expr_delete(e->left);
	expr_delete(e->right);
	free((char *)e->name);
	symbol_delete(e->symbol);
	free((char *)e->string_literal);
	free(e);
}

int expr_is_constant(struct expr *e) {
	if(e->kind == EXPR_BOOLEAN_LITERAL || e->kind == EXPR_INTEGER_LITERAL || e->kind == EXPR_CHARACTER_LITERAL || e->kind == EXPR_STRING_LITERAL)
		return 1;
	return 0;
}

struct type *expr_typecheck(struct expr *e) {
	if(!e)
		return type_create(TYPE_VOID,0,0,0);
	struct type *left = 0;
	struct type *right = 0;
	left = expr_typecheck(e->left);
	right = expr_typecheck(e->right);
	switch(e->kind) {
		case EXPR_ASSIGN:
			if(!type_compare(left,right)) {
				printf("Type Error: cannot assign ");
				type_print(right);
				printf(" to variable of type ");
				type_print(left);
				printf("\n");
				type_error_count++;
			}
			return left;
		case EXPR_ADD:
		case EXPR_SUB:
		case EXPR_MUL:
		case EXPR_DIV:
		case EXPR_MOD:
		case EXPR_EXPON:
			if(left->kind != TYPE_INTEGER || right->kind != TYPE_INTEGER) {
				printf("Type Error: cannot perform integer math on ");
				type_print(left);
				printf(" and ");
				type_print(right);
				printf("\n");
				type_error_count++;
			}
			return type_create(TYPE_INTEGER,0,0,0);
		case EXPR_LT:
		case EXPR_LE:
		case EXPR_GT:
		case EXPR_GE:
			if(left->kind != TYPE_INTEGER || right->kind != TYPE_INTEGER) {
				printf("Type Error: cannot perform integer comparison between");
				type_print(left);
				printf(" and ");
				type_print(right);
				printf("\n");
				type_error_count++;
			}
			return type_create(TYPE_BOOLEAN,0,0,0);
		case EXPR_INCR:
		case EXPR_DECR:
			if(left->kind != TYPE_INTEGER) {
				printf("Type Error: cannot increment or decrement ");
				type_print(left);
				printf("\n");
				type_error_count++;
			}
			return type_create(TYPE_INTEGER,0,0,0);
		case EXPR_NEG:
			if(left->kind != TYPE_INTEGER) {
				printf("Type Error: cannot perform unary negation on ");
				type_print(left);
				printf("\n");
				type_error_count++;
			}
			return type_create(TYPE_INTEGER,0,0,0);
		case EXPR_AND:
		case EXPR_OR:
			if(left->kind != TYPE_BOOLEAN || right->kind != TYPE_BOOLEAN) {
				printf("Type Error: cannot perform boolean logic on ");
				type_print(left);
				printf(" and ");
				type_print(right);
				printf("\n");
				type_error_count++;
			}
			return type_create(TYPE_BOOLEAN,0,0,0);
		case EXPR_NOT:
			if(left->kind != TYPE_BOOLEAN) {
				printf("Type Error: cannot perform boolean NOT on ");
				type_print(left);
				printf("\n");
				type_error_count++;
			}
			return type_create(TYPE_BOOLEAN,0,0,0);
		case EXPR_EQ:
		case EXPR_NE:
			if(!type_compare(left,right) || left->kind == TYPE_ARRAY || right->kind == TYPE_ARRAY) {
				printf("Type Error: cannot evaluate equality between ");
				type_print(left);
				printf(" and ");
				type_print(right);
				printf("\n");
				type_error_count++;
			}
			return type_create(TYPE_BOOLEAN,0,0,0);
		case EXPR_FUNC:
			return left;
		case EXPR_LIST:
			//THINK A LOT MORE
		case EXPR_ARRAY_DEREF:
			if(left->kind != TYPE_ARRAY || right->kind != TYPE_INTEGER) {
				printf("Type Error: cannot dereference an array using ");
				type_print(left);
				printf("[");
				type_print(right);
				printf("]\n");
				type_error_count++;
			}
			return left->subtype;
		case EXPR_NAME:
			return e->symbol->type;
		case EXPR_BOOLEAN_LITERAL:
			return type_create(TYPE_BOOLEAN,0,0,0);
		case EXPR_INTEGER_LITERAL:
			return type_create(TYPE_INTEGER,0,0,0);
		case EXPR_CHARACTER_LITERAL:
			return type_create(TYPE_CHARACTER,0,0,0);
		case EXPR_STRING_LITERAL:
			return type_create(TYPE_STRING,0,0,0);
		default:
			fprintf(stderr,"Invalid type\n");
			exit(1);
	}
	return 0;
}
