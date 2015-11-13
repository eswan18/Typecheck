all: cminor

cminor: token.o lex.yy.o main.o parser.tab.o expr.o decl.o stmt.o type.o param_list.o
	gcc parser.tab.o token.o expr.o decl.o stmt.o type.o param_list.o lex.yy.o main.o -o cminor -Wall -g
main.o: main.c parser.tab.h token.h
	gcc -c main.c -Wall -g
token.o: token.c token.h parser.tab.h
	gcc -c token.c -Wall -g
param_list.o: param_list.c param_list.h
	gcc -c param_list.c -Wall -g
type.o: type.c type.h
	gcc -c type.c -Wall -g
stmt.o: stmt.c stmt.h
	gcc -c stmt.c -Wall -g
decl.o: decl.c decl.h
	gcc -c decl.c -Wall -g
expr.o: expr.c expr.h
	gcc -c expr.c -Wall -g
parser.tab.o: parser.tab.c parser.tab.h decl.h expr.h stmt.h param_list.h type.h
	gcc -c parser.tab.c -Wall -g
lex.yy.o: lex.yy.c parser.tab.h token.h
	gcc -c lex.yy.c -Wall -g
parser.tab.c parser.tab.h: parser.y
	yacc -d -bparser -v parser.y
lex.yy.c: scanner.l
	flex scanner.l
clean:
	rm *.o lex.yy.c parser.tab.c parser.tab.h cminor
