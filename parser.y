%{
  /*
     TODO
     [0] Fix dangling else
   *should have been taken care of with the addition of the
   matched/unmatched rules.
   [1] Constants are not defined atm, expressions should be built first.
   [2] With should shadow its parent definitions
   */
#warning WITH is not currently implemented

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "bison_union.h"
#include "expressions.h"
#include "symbol_table.h"
#include "statements.h"

#define YYERROR_VERBOSE 1

int yylex(void);
void yyerror(const char *err);

extern int yylineno;
scope_t *scope;
%}

%token PROGRAM CONST TYPE ARRAY LIST SET OF RECORD VAR FUNCTION PROCEDURE
%token INTEGER REAL BOOLEAN CHAR FORWARD LENGTH NEW T_BEGIN END IF THEN ELSE
%token WHILE DO CASE OTHERWISE FOR TO DOWNTO WITH READ WRITE
%token LISTFUNC SEMI 
%token CCONST BCONST STRING RCONST ICONST ID
%token RBRACK RPAREN COMMA  ASSIGN DOTDOT COLON
%token LBRACK INOP RELOP EQU ADDOP OROP MULDIVANDOP NOTOP DOT LPAREN

%nonassoc LBRACK 
%nonassoc INOP RELOP EQU 
%left ADDOP OROP
%left MULDIVANDOP
%nonassoc NOTOP
%left DOT LPAREN 

%union {
  char *id;
  int  iconst;
  double rconst;
  int bconst;
  char cconst;
  int op;
  char *listfunc;
  char *string;
  int sign;

  data_type_t dataType;
  limit_t limit;
  limits_t limits;
  identifiers_t identifiers;
  variabledefs_t variabledefs;
  declarations_t declarations;
  fields_t fields;
  typedefs_t typedefs;
  typedefs_entry_t type_def;
  expression_t *expr;
  expressions_t expressions;
  constant_t constant;
  variable_t *variable;
  sub_header_t sub_header;
  int pass;
  parameter_list_t params;
  constdefs_t constdefs;
  statement_t *statement;
  iter_space_t iter_space;
}

%type <id> ID header
%type <iconst> ICONST
%type <rconst> RCONST
%type <cconst> CCONST
%type <bconst> BCONST
%type <string> STRING
%type <sign> sign
%type <op> ADDOP MULDIVANDOP INOP RELOP OROP EQU
%type <limit> limit
%type <dataType> typename standard_type
%type <limits> limits dims
%type <identifiers> identifiers
%type <variabledefs> variable_defs vardefs
%type <declarations> declarations
%type <fields> fields field
%type <typedefs> typedefs type_defs
%type <type_def> type_def
%type <expr> expression
%type <expressions> expressions setlistexpression
%type <constant> constant
%type <variable> variable
%type <sub_header> sub_header
%type <pass> pass
%type <params> parameter_list formal_parameters
%type <constdefs> constdefs constant_defs
%type <statement> statement matched unmatched matched_if_statement case_statement while_statement
%type <statement> for_statement with_statement subprogram_call io_statement comp_statement 
%type <statement> assignment statements
%type <iter_space> iter_space
%error-verbose
%%

program : header declarations subprograms comp_statement DOT 
;

header : PROGRAM ID SEMI
{
  $$ = $2;
  scope = st_init(NULL); //global scope
}

;
declarations : constdefs typedefs vardefs 
{
  int i =0,j;
  //enum TypedefType { TT_Array, TT_List, TT_Set, TT_Record, TT_Range };
  const char *typedef_types[] = { "Array", "List", "Set", "Record", "Range" };

  $$.vardefs = $3;
  $$.typedefs = $2;
  $$.constdefs = $1;

  printf("Declarations::constdefs %d\n", $1.size);
  for ( i = 0 ; i < $1.size; i ++ ) {
    st_const_define($1.ids[i], $1.constants+i, scope);
    printf("constant: %s (%d)\n", $1.ids[i], $1.constants[i].type);
  }

  printf("Declarations::vardefs %d\n", $3.size);
  for ( i=0; i<$3.size; i++ ) {
    printf("type: %d %s\n", $3.types[i].dataType, ($3.types[i].dataType==VT_User ? $3.types[i].userType : "standard_type" ));
    for (j=0; j<$3.ids[i].size; j++) {
      printf("\t%s\n", $3.ids[i].ids[j]);
      st_var_define($3.ids[i].ids[j], $3.types[i], scope);
    }
  }


  printf("Declarations::typedefs %d\n", $2.size);

  for (i=0; i<$2.size; i++ ) {
    printf("%s -> %s\n", $2.typedefs[i].name, typedef_types[$2.typedefs[i].type]);
    st_typedef_register($2.typedefs+i, scope);
  }
}
;

constdefs : CONST constant_defs SEMI
{
  $$ = $2;
}
|
{
  $$.size = 0;
}
;
constant_defs : constant_defs SEMI ID EQU expression 
{
  constant_t temp;
  if ( expression_evaluate($5, &temp) == Failure ) {
    yyerror("constant_defs invalid expression\n");
  } else {
    $$ = $1;
    $$.constants = ( constant_t* ) realloc($$.constants, ($$.size+1) * sizeof(constant_t));
    $$.ids = ( char** ) realloc($$.ids, ($$.size+1) * sizeof(char*));
    $$.ids[ $$.size ] = $3;
    $$.constants[ $$.size ++ ] = temp;
  }
}
| ID EQU expression 
{
  constant_t temp;
  if ( expression_evaluate($3, &temp) == Failure ) {
    yyerror("constant_defs invalid expression\n");
  } else {
    $$.size = 1;
    $$.ids = ( char** ) calloc(1, sizeof(char*));
    $$.ids[0] = $1;
    $$.constants = ( constant_t* ) calloc(1, sizeof(constant_t));
    $$.constants[0] = temp;
  }
}
;

expression : expression RELOP expression 
{
  $$ = expression_binary($1, $3, $2);
}
| expression EQU expression  
{
  $$ = expression_binary($1, $3, $2);
}

| expression INOP expression  
{
  $$ = expression_binary($1, $3, $2);
}

| expression OROP expression  
{
  $$ = expression_binary($1, $3, $2);
}

| expression ADDOP expression  
{
  $$ = expression_binary($1, $3, $2);
}

| expression MULDIVANDOP expression  
{
  $$ = expression_binary($1, $3, $2);
}

| ADDOP expression 
{
  if ( $1 == AddopP ) 
    $$ = $2;
  else {
    //TODO negative $2 ( quick hack )
    expression_t *neg = calloc(1, sizeof(expression_t));
    neg->type = ET_Constant;
    neg->constant.type = VT_Iconst;
    neg->constant.iconst = -1;
    neg->dataType = VT_Integer;
    //neg->next = NULL;

    $$ = expression_binary(neg, $2, MuldivandopM); 
    //$$ = NULL;
  }
}
| NOTOP expression
{
  $$=  expression_not($2);
}
| variable 
{
  $$ = expression_variable($1, scope);
}
| ID LPAREN expressions RPAREN 
{
  printf("%d ", yylineno);
  yyerror("Oxi id lparen expressions rparen\n");
}
| LENGTH LPAREN expression RPAREN
{
  printf("%d ", yylineno);
  yyerror("Oxi length lparen expression rparen\n");
}
| NEW LPAREN expression RPAREN 
{
  printf("%d ", yylineno);
  yyerror("NEW LPAREN expression RPAREN\n");
}
| constant 
{
  $$ = expression_constant($1.type, &($1.iconst));
}
| LPAREN expression RPAREN
{
  $$ = $2;
}
| setlistexpression 
{
  expressions_t *exprs = ( expressions_t* ) calloc(1, sizeof(expressions_t));
  exprs->exprs = $1.exprs;
  exprs->size = $1.size;
  $$ = expression_set(exprs);
}
;

variable : ID 
{
  $$ = ( variable_t* ) calloc(1, sizeof(variable_t));
  $$->id= $1;
}
| variable DOT ID 
{
  variable_t *p;
  p = $$ = $1;
  while ( p->child )
    p = p->child;
  p->child = ( variable_t * ) calloc(1, sizeof(variable_t));
  p = p->child;
  p->id = $3;
}
| variable LBRACK expressions RBRACK 
{
  variable_t *p;
  p = $$ = $1;
  while ( p->child )
    p = p->child;
  p->child = ( variable_t * ) calloc(1, sizeof(variable_t));
  p = p->child;
  p->expr = $3;
}
| LISTFUNC LPAREN expression RPAREN 
{
  $$ = NULL;
  printf("%d) Listfunc is not supported\n", yylineno);
  exit(0);
#warning  hack
}
;

expressions : expressions COMMA expression 
{
  $$ = $1;
  $$.exprs = ( expression_t * ) realloc( $$.exprs, sizeof(expression_t) * ( $$.size+1));
  //TODO auto prepei na fugei
  if ( $3 )
    $$.exprs[ $$.size++ ] = *$3;
  else {
    printf("sapio expressions comma expression\n");
    //hack
    memset( $$.exprs + $$.size++, 0, sizeof(expression_t));
  }
}
| expression
{
  if ( $1 == NULL ) {
    printf("sapio expression\n");
    // hack;
    $$.exprs = ( expression_t* ) calloc(1, sizeof(expression_t));
  } else
    $$.exprs = $1;

  $$.size = 1;
}
;

constant : ICONST 
{
  $$.type = VT_Iconst;
  $$.iconst = $1;
}
| RCONST

{
  $$.type = VT_Rconst;
  $$.rconst = $1;
}
| BCONST 
{
  $$.type = VT_Bconst;
  $$.bconst = $1;
}
| CCONST 
{
  $$.type = VT_Cconst;
  $$.cconst = $1;
}
;

setlistexpression : LBRACK expressions RBRACK
{
  $$ = $2;
}
| LBRACK RBRACK
{
  $$.size = 0;
  $$.exprs = 0;
}
;

typedefs : TYPE type_defs SEMI 
{
  $$ = $2;
}
| 
{
  $$.size = 0;
  $$.typedefs = NULL;
}
;
type_defs : type_defs SEMI ID EQU type_def 
{
  $1.size ++;
  $1.typedefs = (typedefs_entry_t*) realloc( $1.typedefs, $1.size * sizeof(typedefs_entry_t));
  $1.typedefs[ $1.size - 1 ] = $5;
  $1.typedefs[ $1.size - 1 ].name = $3;

  $$ = $1;
}
| ID EQU type_def 
{
  $$.size = 1;
  $$.typedefs = (typedefs_entry_t*) calloc(1, sizeof(typedefs_entry_t));
  $$.typedefs[0] = $3;
  $$.typedefs[0].name = $1; // this has to be done AFTER $$.typedefs[0] = $3;
}
;

type_def : ARRAY LBRACK dims RBRACK OF typename 
{
  $$.type = TT_Array;
  $$.array.dims = $3;
  $$.array.typename = $6;
}
| LIST OF typename 
{
  $$.type = TT_List;
  $$.list.typename = $3;
}	 
| SET OF typename 
{
  $$.type = TT_Set;
  $$.set.typename = $3;
}
| RECORD fields END 
{
  $$.type = TT_Record;
  $$.record = $2;
}
| limit DOTDOT limit 
{
  $$.type = TT_Range;
  $$.range.from = $1;
  $$.range.to = $3;
}
;

dims : dims COMMA limits 
{
  $1.size++;
  $1.limits = (limits_entry_t*) realloc( $1.limits, $1.size * sizeof(limits_entry_t));
  $1.limits[ $1.size-1 ] = $3.limits[0];
  $$ = $1;
}
| limits 
{
  $$ = $1;
}
;

limits : limit DOTDOT limit
{
  $$.limits = (limits_entry_t*) calloc(1, sizeof(limits_entry_t));
  $$.limits[0].isRange = 1;
  $$.limits[0].range.from = $1;
  $$.limits[0].range.to = $3;

  $$.size = 1;
}
| ID
{
  $$.limits = (limits_entry_t*) calloc(1, sizeof(limits_entry_t));
  $$.limits[0].isRange = 0;
  $$.limits[0].limit.type = LT_Id;
  $$.limits[0].limit.id.sign = Positive;
  $$.limits[0].limit.id.id = $1;

  $$.size = 1;
}
;

limit : sign ICONST {
          $$.type =LT_Iconst;

          if ( $1 == Negative )
            $$.iconst = - $2;
          else
            $$.iconst = $2;
        }
| CCONST 
{
  $$.type = LT_Cconst;
  $$.cconst = $1;
}
| BCONST 
{
  $$.type = LT_Bconst;
  $$.bconst = $1;
}
| ADDOP ID 
{
  $$.type = LT_Id;
  $$.id.id = $2;
  $$.id.sign = ( $1==AddopP ? Positive : Negative );
}
|
ID
{
  $$.type = LT_Id;
  $$.id.id = $1;
  $$.id.sign = Positive;
}
;

sign : ADDOP
{
  if ( $1 == AddopP )
    $$ = Positive;
  else
    $$ = Negative;
}
| 
{
  $$ = Positive;
}
;
typename : standard_type
{
  $$ = $1;
}
| ID 
{
  $$.dataType = VT_User;
  $$.userType = $1;
}
;

standard_type : INTEGER 
{
  $$.dataType = VT_Integer;
}
| REAL
{
  $$.dataType = VT_Real;
}
| BOOLEAN
{
  $$.dataType = VT_Boolean;
}
| CHAR 
{
  $$.dataType = VT_Char;
}
;

fields : fields SEMI field 
{
  $1.size ++;
  $1.ids = (identifiers_t*) realloc( $1.ids, $1.size * sizeof(identifiers_t));
  $1.types = (data_type_t*) realloc( $1.types, $1.size * sizeof(data_type_t));
  $1.ids[ $1.size - 1 ] = $3.ids[0];
  $1.types[ $1.size - 1 ] = $3.types[0];

  if ($1.types[ $1.size-1 ].dataType== VT_User )
    printf("new field: %s\n", $1.types [$1.size-1].userType);


  $$ = $1;
}
| field 
{
  $$ = $1;
}
;

field : identifiers COLON typename 
{
  $$.size = 1;
  $$.ids = (identifiers_t*) calloc(1, sizeof(identifiers_t));
  $$.types = (data_type_t*) calloc(1, sizeof(data_type_t));
  $$.ids[0] = $1;
  $$.types[0] = $3;
  if ($3.dataType == VT_User )
    printf("new field: %s\n", $3.userType);
}
;

identifiers : identifiers COMMA ID 
{
  $1.size++;
  $1.ids = (char**) realloc($1.ids, sizeof(char*) * $1.size);
  $1.ids[ $1.size - 1 ] = $3;

  $$ = $1;
}
| ID 
{
  $$.size = 1;
  $$.ids = (char**) calloc(1, sizeof(char*));
  $$.ids[0] = $1;
}
;

vardefs : VAR variable_defs SEMI 
{
  $$ = $2;
}
| 
{
  $$.size = 0;
  $$.ids = 0;
  $$.types = 0;
}
;
variable_defs : variable_defs SEMI identifiers COLON typename
{
  $1.size++;
  $1.ids = (identifiers_t*) realloc($1.ids, $1.size * sizeof(identifiers_t));
  $1.types = (data_type_t*) realloc($1.types, $1.size * sizeof(data_type_t));
  $1.ids[ $1.size - 1 ] = $3;
  $1.types[ $1.size - 1 ] = $5;

  $$ = $1;
}
| identifiers COLON typename
{
  $$.size = 1;
  $$.ids = (identifiers_t*) calloc(1, sizeof(identifiers_t));
  $$.types = (data_type_t*) calloc(1, sizeof(data_type_t));
  $$.ids[0] = $1;
  $$.types[0] = $3;
}
;

subprograms : subprograms subprogram SEMI 
| 
;

subprogram : sub_header  SEMI FORWARD
| sub_header SEMI  { scope = st_init(scope); } declarations
{
  int i,j;
  var_t *var;
  // first register the function ID as a local variable
  st_var_define($1.id, $1.type, scope);

  // then register all parameters as local variables so that they are visible
  for ( i = 0; i < $1.size; i ++ ) {
    for ( j = 0; j < $1.params[i].ids.size; j++ ) {
      var = st_var_define($1.params[i].ids.ids[j], $1.params[i].type, scope);
      if ( var )
        var->pass = $1.params[i].pass;
    }
  }

  // register the function to the global scope
  st_func_define($1.id, $1.type, $1.params, $1.size, scope->parent);
}
subprograms comp_statement
{
  scope = st_destroy( scope );
}

;

sub_header : FUNCTION ID formal_parameters COLON standard_type 
{
  $$.id = $2;
  $$.type = $5;
  $$.params = $3.params;
  $$.size = $3.size;
}
| FUNCTION ID formal_parameters COLON LIST 
{
#warning unfinished
  $$.id = $2;
  $$.type.dataType = 0;
  $$.params = $3.params;
  $$.size= $3.size;
}
| PROCEDURE ID formal_parameters 
{
#warning unfinished
  $$.id = $2;
  $$.type.dataType = 0;
  $$.params = $3.params;
  $$.size= $3.size;
}
| FUNCTION ID  
{
#warning unfinished
  $$.id = $2;
  $$.type.dataType = 0;
  $$.size= 0;
  $$.params = NULL;
}
;

formal_parameters : LPAREN parameter_list RPAREN
{
  $$ = $2;
}
| 
{
  $$.size = 0;
}
;

parameter_list : parameter_list SEMI pass identifiers COLON typename 
{
  $1.params = ( parameters_t* ) realloc($1.params, ($1.size+1) * sizeof(parameters_t));
  $1.params[ $1.size ].pass = $3;
  $1.params[ $1.size ].ids  = $4;
  $1.params[ $1.size++ ].type = $6;
  $$ = $1;
}
| pass identifiers COLON typename 
{
  $$.size = 1;
  $$.params = ( parameters_t * ) calloc(1, sizeof(parameters_t));
  $$.params->pass = $1;
  $$.params->ids  = $2;
  $$.params->type = $4;
}
;

pass : VAR 
{ 
  $$ = 1;
} 
| 
{
  $$ = 0;
}
;
comp_statement : T_BEGIN statements END 
{
  $$ = $2;
}
;

statements : statements SEMI statement 
{
  statement_t *p, *t;
  $$ = $1;

  for ( p = $$; p && p->next; p = p->next );

  if ( p ) {
    p->next = $3;
    p->join = $3;
#warning edw na valw join gia ta statements
    switch ( p->type )
    {
      case ST_If:
        for (t = p->_if._true; t && t->next; t = t->next );

        if ( t )
          t->join = $3;

        for (t = p->_if._false; t && t->next; t = t->next );

        if ( t )
          t->join = $3;
        break;
    }
  }
}
| statement 
{
  $$ = $1;
}
;

statement : matched
{
  $$ = $1;
}
| unmatched
{
  $$ = $1;
}
;

matched: assignment 
{
  $$ = $1;
}
| matched_if_statement
{
  $$ = $1;
}
| case_statement 
{
  $$ = $1;
}
| while_statement 
{
  $$ = $1;
}
| for_statement 
{
  $$ = $1;
}
| with_statement 
{
  $$ = $1;
}
| subprogram_call 
{
  $$ = $1;
}
| io_statement 
{
  $$ = $1;
}
| comp_statement
{
  $$ = $1;
}
|
{
  $$ = NULL;
}
;


matched_if_statement: IF expression THEN matched ELSE matched
{
  $$ = statement_if($2, $4, $6);
}
;

unmatched: IF expression THEN statement
{ 
  printf("cond: %p, _true; %p\n", $2, $4);
  $$ = statement_if($2, $4, NULL);
}
| IF expression THEN matched ELSE unmatched
{
  $$ = statement_if($2, $4, $6);
}
;

assignment : variable ASSIGN expression 
{
  $$ = statement_assignment($1, $3, scope);
}
| variable ASSIGN STRING 
{
  $$ = statement_assignment_str($1, $3, scope);
}
;

case_statement : CASE expression OF cases case_tail END 
;

cases : cases SEMI single_case 
| single_case 
;

single_case : label_list COLON statement 
| 
;

label_list : label_list COMMA label 
| label 
;

label : sign constant 
| sign ID 
;


case_tail : SEMI OTHERWISE COLON statement 
| 
;

while_statement : WHILE expression DO statement 
{
  $$ = statement_while($2, $4);
}
;

for_statement : FOR ID ASSIGN iter_space DO statement 
{
  $$ = statement_for($2, &$4, $6, scope);
}
;

iter_space : expression TO expression
{
  $$.from = $1;
  $$.to = $3;
  $$.type = FT_To;
}
| expression DOWNTO expression 
{
  $$.from = $1;
  $$.to = $3;
  $$.type = FT_DownTo;
}
;

with_statement : WITH variable DO statement {
                   printf("%d) With is not currently implemented\n", yylineno);
                   exit(0);
                 }
;

subprogram_call : ID 
| ID LPAREN expressions RPAREN 
;

io_statement : READ LPAREN read_list RPAREN 
| WRITE LPAREN write_list RPAREN
;

read_list : read_list COMMA read_item 
| read_item 
;

read_item : variable 
;

write_list : write_list COMMA write_item 
| write_item 
;

write_item : expression 
| STRING
;

%%


void yyerror(const char *err)
{
  printf("Error[%d]: %s\n", yylineno, err);
}

int main(int argc, char* argv[])
{
  int ret;

  ret = yyparse();

  printf("[$] yyparse: %d\n", ret);
  return 0;
}

