%{
#include <stdio.h>
#include <math.h>
#include "bison_union.h"
#include "parser.h"
#include "constants.h"

char *string = 0;
int string_length = 0;
#define dprint(...)
//#define dprint printf("%d: ", yylineno); printf
%}

%option nounput
%option noinput
%option noyywrap
%option yylineno

%x strings

%%

"PROGRAM" {
    dprint("PROGRAM\n");
    return PROGRAM;
  }

"CONST" {
  dprint("CONST\n");
  return CONST;
}

"TYPE" {
  dprint("TYPE\n");
  return TYPE;
}

"ARRAY" {
  dprint("ARRAY\n");
  return ARRAY;
}

"LIST" {
  dprint("LIST\n");
  return LIST;
}

"SET" {
  dprint("SET\n");
  return SET;
}

"OF" {
  dprint("OF\n");
  return OF;
}

"RECORD" {
  dprint("RECORD\n");
  return RECORD;
}

"VAR" {
  dprint("VAR\n");
  return VAR;
}

"FUNCTION" {
  dprint("FUNCTION\n");
  return FUNCTION;
}

"PROCEDURE" {
  dprint("PROCEDURE\n");
  return PROCEDURE;
}


"INTEGER" {
  dprint("INTEGER\n");
  return INTEGER;
}

"REAL" {
  dprint("REAL\n");
  return REAL;
}

"BOOLEAN" {
  dprint("BOOLEAN\n");
  return BOOLEAN;
}

"CHAR" {
  dprint("CHAR\n");
  return CHAR;
}

"FORWARD" {
  dprint("FORWARD\n");
  return FORWARD;
}

"LENGTH" {
  dprint("LENGTH\n");
  return LENGTH;
}

"NEW" {
  dprint("NEW\n");
  return NEW;
}

"BEGIN" {
  dprint("BEGIN\n");
  return T_BEGIN;
}

"END" {
  dprint("END\n");
  return END;
}

"IF" {
  dprint("IF\n");
  return IF;
}

"THEN" {
  dprint("THEN\n");
  return THEN;
}

"ELSE" {
  dprint("ELSE\n");
  return ELSE;
}

"WHILE" {
  dprint("WHILE\n");
  return WHILE;
}

"DO" {
  dprint("DO\n");
  return DO;
}

"CASE" {
  dprint("CASE\n");
  return CASE;
}

"OTHERWISE" {
  dprint("OTHERWISE\n");
  return OTHERWISE;
}

"FOR" {
  dprint("FOR\n");
  return FOR;
}

"TO" {
  dprint("TO\n");
  return TO;
}

"DOWNTO" {
  dprint("DOWNTO\n");
  return DOWNTO;
}

"WITH" {
  dprint("WITH\n");
  return WITH;
}

"READ" {
  dprint("READ\n");
  return READ;
}

"WRITE" {
  dprint("WRITE\n");
  return WRITE;
}

"TRUE" {
  yylval.bconst = True;
  dprint("TRUE\n");
  return BCONST;
}

"FALSE" {
  yylval.bconst = False;
  dprint("FALSE");
  return BCONST;
}

"OR" {
  dprint("OROP\n");
  yylval.op = Orop;
  return OROP;
}

[C]([A][D]*|[D]+)[R] {
  yylval.listfunc = strdup(yytext);
  dprint("LISTFUNC = %s\n", yytext);
  return LISTFUNC;
}

"+" {
  dprint("ADDOP +\n");
  yylval.op = AddopP;
  return ADDOP;
}

"-" {
  dprint("ADDOP -\n");
  yylval.op = AddopM;
  return ADDOP;
}

"*" {
  dprint("MULDIVANDOP *\n");
  yylval.op = MuldivandopM;
  return MULDIVANDOP;
}

"/" {
  dprint("MULDIVANDOP /\n");
  yylval.op = MuldivandopD;
  return MULDIVANDOP;
}

"DIV" {
  dprint("MULDIVANDOP DIV\n");
  yylval.op = MuldivandopDiv;
  return MULDIVANDOP;
}

"AND" {
  dprint("MULDIVANDOP AND\n");
  yylval.op = MuldivandopAnd;
  return MULDIVANDOP;
}

"MOD" {
  dprint("MULDIVANDOP MOD\n");
  yylval.op = MuldivandopMod;
  return MULDIVANDOP;
}

"NOT" {
  dprint("NOTOP\n");
  yylval.op = Notop;
  return NOTOP;
}

"IN" {
  dprint("INOP\n");
  yylval.op = Inop;
  return INOP;
}

[_]?[[:alpha:]]([[:alnum:]]|[_]+[[:alnum:]])* {
  dprint("ID: %s\n", yytext);

  yylval.id = strdup(yytext);
  return ID;
}

([0]|[1-9][[:digit:]]*) {
  int num = strtoul(yytext, NULL, 10);
  dprint("ICONST(dec) = %d\n", num);

  yylval.iconst = num;
  return ICONST;
}

"0X"[1-9A-F][[:xdigit:]]* {
  int num = strtoul(yytext, NULL, 16);
  dprint("ICONST(hex) = %d (0x%x)\n", num, num);

  yylval.iconst = num;
  return ICONST;
}

"0B"[1][0-1]* {
  int num = strtoul(yytext+2, NULL, 2);
  dprint("ICONST(bin) = %d (%s)\n", num, yytext);

  yylval.iconst = num;
  return ICONST;
}

([0]|[1-9][[:digit:]]*)?\.([0]|[0]*[1-9][[:digit:]]*)("E"[-+]?(0|[1-9][[:digit:]]*))? {
  double num = strtold(yytext, 0);
  dprint("RCONST(dec) = %g (%s)\n", num, yytext);

  yylval.rconst = num;
  return RCONST;
}

([0]|[1-9][[:digit:]]*)("E"[-+]?(0|[1-9][[:digit:]]*)) {
  double num = strtold(yytext, 0);
  dprint("RCONST(dec) = %g (%s)\n", num, yytext);

  yylval.rconst = num;
  return RCONST;
}

"0X"([0]|[1-9A-F][[:xdigit:]]*)?\.([0]|[0]*[1-9A-F][[:xdigit:]]*) {
  double num = strtold(yytext, 0);
  dprint("RCONST(hex) = %g (%s)\n", num, yytext);

  yylval.rconst = num;
  return RCONST;
}

"0B"([0]|[1][01]*)?\.([0]|[0]*[1][01]*) {
  char *dekadiko;
  double num = strtoul(yytext+2, &dekadiko, 2);
  double power = 0.5;

  int i =0;
  for (i=1; i< strlen(dekadiko); i++, power*=0.5 )
    num+= (dekadiko[i]-'0') * power;

  dprint("RCONST(bin) = %g (%s)\n", num, yytext);

  yylval.rconst = num;
  return RCONST;
}

['][[:print:]]['] {
  dprint("CCONST = %c\n", yytext[1]);
  yylval.cconst = yytext[1];
  return CCONST;
}

[']\\n['] {
  dprint("CCONST = \\n\n");
  yylval.cconst = yytext[1];
  return CCONST;
}

[']\\r['] {

  dprint("CCONST = \\r\n");
  yylval.cconst = yytext[1];
  return CCONST;
}


[']\\f['] {
  dprint("CCONST = \\f\n");
  yylval.cconst = yytext[1];
  return CCONST;
}

[']\\t['] {
  dprint("CCONST = \\t\n");
  yylval.cconst = yytext[1];
  return CCONST;
}

[']\\b['] {
  dprint("CCONST = \\b\n");
  yylval.cconst = yytext[1];
  return CCONST;
}

[']\\v['] {
  dprint("CCONST = \\v\n");
  yylval.cconst = yytext[1];
  return CCONST;
}

">" {
  dprint("RELOP = >\n");
  yylval.op = RelopG;
  return RELOP;
}

">=" {
  dprint("RELOP = >=\n");
  yylval.op = RelopGE;
  return RELOP;
}

"<" {
  dprint("RELOP = <\n");
  yylval.op = RelopL;
  return RELOP;
}

"<=" {
  dprint("RELOP = <=\n");
  yylval.op = RelopLE;
  return RELOP;
}

"<>" {
  dprint("RELOP = <>\n");
  yylval.op = RelopD;
  return RELOP;
}

"\"" {
  string_length=0;
  string = (char*) malloc(sizeof(char));
  string[0] = 0;

  BEGIN(strings);
}

<strings>{
  [\"\n] {
    BEGIN(INITIAL);

    yylval.string = strdup(string);
    free(string);

    dprint("STRING = \"%s\"\n", yylval.string);
    return STRING;
  }

  "\\n" {
    string_length++;
    string = (char*) realloc(string,sizeof(char) * ( string_length + 1 ));
    string[string_length-1] = '\n' ;
    string[string_length] = 0;
  }

  "\\r" {
    string_length++;
    string = (char*) realloc(string,sizeof(char) * ( string_length + 1 ));
    string[string_length-1] = '\r' ;
    string[string_length] = 0;

  }

  "\\f" {
    string_length++;
    string = (char*) realloc(string,sizeof(char) * ( string_length + 1 ));
    string[string_length-1] = '\f' ;
    string[string_length] = 0;

  }

  "\\t" {
    string_length++;
    string = (char*) realloc(string,sizeof(char) * ( string_length + 1 ));
    string[string_length-1] = '\t' ;
    string[string_length] = 0;
  }

  "\\v" {
    string_length++;
    string = (char*) realloc(string,sizeof(char) * ( string_length + 1 ));
    string[string_length-1] = '\v' ;
    string[string_length] = 0;

  }

  "\\b" {
    string_length++;
    string = (char*) realloc(string,sizeof(char) * ( string_length + 1 ));
    string[string_length-1] = '\b' ;
    string[string_length] = 0;
  }

  "\\\\" {
    string_length++;
    string = (char*) realloc(string,sizeof(char) * ( string_length + 1 ));
    string[string_length-1] = '\\' ;
    string[string_length] = 0;
  }

  "\\\"" {
    string_length++;
    string = (char*) realloc(string,sizeof(char) * ( string_length + 1 ));
    string[string_length-1] = '\"' ;
    string[string_length] = 0;
  }

  "\\\n" {
  }

  . {
    string_length++;
    string = (char*) realloc(string,sizeof(char) * ( string_length + 1 ));
    string[string_length-1] = yytext[0];
    string[string_length] = 0;
  }
}

"(" {
  dprint("LPAREN\n");
  return LPAREN;
}

")" {
  dprint("RPAREN\n");
  return RPAREN;
}

";" {
  dprint("SEMI\n");
  return SEMI;
}

"." {
  dprint("DOT\n");
  return DOT;
}

"," {
  dprint("COMMA\n");
  return COMMA;
}


"=" {
  dprint("EQU\n");
	yylval.op = Equop;
  return EQU;
}

":" {
  dprint("COLON\n");
  return COLON;
}

"[" {
  dprint("LBRACK\n");
  return LBRACK;
}

"]" {
  dprint("RBRACK\n");
  return RBRACK;
}

":=" {
  dprint("ASSIGN\n");
  return ASSIGN;
}

".." {
  dprint("DOTDOT\n");
  return DOTDOT;
}

"{"([^}]|[\n])*"}" { dprint("Comment: \n\"%s\"\n", yytext); } 
[\n\r \t] {}

. {
  dprint("Skipped: %s\n", yytext);
}

%%


/*int main( int argc, char *argv[]) {
  while ( yylex() );

  return 0;
}*/

