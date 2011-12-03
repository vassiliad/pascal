all: ce432

cloc:
	make clean
	cloc --exclude-lang=Pascal,make,"Bourne Shell" ./

ce432: symbol_table.o expressions.o parser.tab.o lex.yy.o
	gcc -Wall -g -ggdb symbol_table.o expressions.o lex.yy.o parser.tab.o -lm -o ce432

parser.tab.o: parser.y constants.h bison_union.h expressions.h
	bison -v --defines=parser.h parser.y
	gcc -c -Wall parser.tab.c -o parser.tab.o

expressions.o: expressions.c expressions.h
	gcc -c -Wall expressions.c -o expressions.o

lex.yy.c: lexer.l constants.h bison_union.h
	flex -i lexer.l
	gcc -c -Wall lex.yy.c -o lex.yy.o

clean:
	rm -f *.o parser.h parser.tab.c lex.yy.c
