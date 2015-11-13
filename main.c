#include <string.h>
#include "token.h"
#include "decl.h"
#include "parser.tab.h"

extern FILE *yyin;
extern char *yytext;
extern int yyparse();
extern struct decl *parser_result;
extern int yylex();
extern void convertString(char *);

int scan(char *filename);
int parse(char *filename);
int resolve(char *filename);
int typecheck(char *filename);

int main(int argc, char *argv[]) {
        char *filename;
	char *option;
        if (argc >= 3) {
                option = argv[1];
		filename = argv[2];
        } else {
		fprintf(stderr,"error: provide both an option (e.g. -scan, -parse) and a filename when invoking cminor.\nfor example: './cminor -parse myfile.cminor'\n");
		return(1);
        }
	
	if (strcmp(option,"-scan") == 0) {
		scan(filename);
	} else if (strcmp(option,"-parse") == 0 || strcmp(option,"-print") == 0) {
		return parse(filename);
	} else if (strcmp(option,"-resolve") == 0) {
		return resolve(filename);
	} else if (strcmp(option,"-typecheck") == 0) {
		return typecheck(filename);
	} else {
		fprintf(stderr,"error: illegal option provided\nPlease choose from 'scan', 'parse', 'resolve', or 'typecheck'\n");
		return(1);
	}
	
	return(1);
}

int scan(char *filename) {
        yyin  = fopen(filename,"r");
	if (!yyin) {
		fprintf(stderr,"scan error: unable to open %s\n",filename);
		exit(1);
	}
        while(yyin) {
        	int t = yylex();
                if (!t)
                        break;
                printf("%s", token_string(t));
                if ( t == TOKEN_STRING_LITERAL || t == TOKEN_CHAR_LITERAL) {
                        char string[600];
                        strcpy(string,yytext);
                        convertString(string);
                        printf(" %s", string);
                }
                printf("\n");
        }
        fclose(yyin);
	return 0;
}

int parse(char *filename) {
	yyin = fopen(filename,"r");
	if (!yyin) {
		fprintf(stderr,"unable to open %s\n",filename);
		exit(1);
	}
	if (yyparse() == 0) {
		decl_print(parser_result,0);
		return 0;
	} else {
		printf("Parse Failed!\n");
		return 1;
	}
	fclose(yyin);
	return 1;
}

int resolve(char *filename) {
	yyin = fopen(filename,"r");
	if (!yyin) {
		fprintf(stderr,"unable to open %s\n",filename);
		exit(1);
	}
	if (yyparse() != 0) {
		return 1;
	}
	fclose(yyin);
	
	//use parser_result
	return 0;
}

int typecheck(char *filename) {
	return 0;
}