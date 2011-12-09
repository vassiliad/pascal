all: ce432

cloc:
	make clean
	cloc --exclude-lang=Pascal,make,"Bourne Shell" ./

ce432: symbol_table.o expressions.o parser.tab.o lex.yy.o statements.o
	gcc -o ce432 -Wall -ggdb symbol_table.o expressions.o lex.yy.o parser.tab.o statements.o -lm 

statements.o: statements.h statements.c
	gcc -c -Wall -ggdb statements.c -o statements.o

parser.tab.o: parser.y constants.h bison_union.h expressions.h
	bison -v --defines=parser.h parser.y
	gcc -c -Wall -ggdb parser.tab.c -o parser.tab.o

expressions.o: expressions.c expressions.h
	gcc -c -Wall -ggdb expressions.c -o expressions.o

symbol_table.o: symbol_table.h symbol_table.c
	gcc -c -Wall -ggdb symbol_table.c -o symbol_table.o

lex.yy.o: lexer.l constants.h bison_union.h
	flex -i lexer.l
	gcc -c -Wall -ggdb lex.yy.c -o lex.yy.o

clean:
	rm -f *.o parser.h parser.tab.c lex.yy.c parser.output ce432
