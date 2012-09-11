%{
#include <getopt.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dead_code.h"
#include "constants.h"
#include "bison_union.h"
#include "expressions.h"
#include "symbol_table.h"
#include "statements.h"
#include "tree.h"
#include "graph.h"
#include "printer.h"
#include "register.h"
#include "subexpression_elim.h"
#include "schedule.h"

#define YYERROR_VERBOSE 1


extern FILE* yyin;
int enable_constant_propagation = 0;
int enable_dead_code_elimination = 0;
int enable_subexpression_elimination = 0;
int enable_scheduling = 0;

static const struct option l_opts[] = {
  	{	"help",		no_argument,		NULL,	'h' },
		{ "constant_propagation", no_argument, NULL , 'c' },
		{ "dead_code_elimination", no_argument, NULL , 'd' },
    { "subexpression_elimination", no_argument, NULL, 'e'},
    { "scheduling", no_argument, NULL , 'e'}
	};

static const char s_opts[] = "hcdes";


int yylex(void);
void yyerror(const char *fmt, ...);

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
%type <statement> statement matched matched_if_statement case_statement while_statement
%type <statement> subprogram_call io_statement comp_statement unmatched
%type <statement> for_statement with_statement  
%type <statement> assignment statements
%type <iter_space> iter_space
%error-verbose
%%

program : header declarations subprograms comp_statement DOT 
{
  graph_t g;
  node_list_t *main_tree;
	statement_t *body = $4;
	
	if ( enable_dead_code_elimination ) {
		body = dead_code_elimination(body, scope);
	}
	
	main_tree = tree_generate_tree( body, scope );
 

  if ( main_tree == NULL )
  {
		printf("[-] Failed to generate instruction tree\n");
  } else {
    if ( enable_subexpression_elimination ) 
      subexpressions_eliminate(main_tree);

		givepostnumbers_tree(main_tree);

 		
    printf("[+] Printing instruction tree\n");
    //find_use_def_stmt(main_tree);

		//print_use_def_stmt(main_tree);
		rg_init();
		init_reg_lives();
		
    if ( enable_scheduling )
		  schedule(main_tree);// CAUSES SEG FAULT AT THE MOMENT!
		
//		assign_nodes_list(main_tree);
//	print_nodes();
		give_regs();

	  g = graph_init(fopen("plot","w"));
    graph_tree(main_tree, &g);
    graph_finalize(&g);
    fclose(g.output);
		print_code();



 }
}
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
    if ( st_const_define($1.ids[i], $1.constants+i, scope) == NULL ) {
			yyerror("Could not define Constant");
			return 1;
		}
  }

  printf("Declarations::typedefs %d\n", $2.size);

  for (i=0; i<$2.size; i++ ) {
    printf("%s -> %s\n", $2.typedefs[i].name, typedef_types[$2.typedefs[i].type]);
    if ( st_typedef_register($2.typedefs+i, scope) != Success ) {
			yyerror("Could not define typedef");
			return 1;
		}
  }

  printf("Declarations::vardefs %d\n", $3.size);
  for ( i=0; i<$3.size; i++ ) {
    printf("type: %d %s\n", $3.types[i].dataType, ($3.types[i].dataType==VT_User ? $3.types[i].userType : "standard_type" ));
    for (j=0; j<$3.ids[i].size; j++) {
      printf("\t%s\n", $3.ids[i].ids[j]);
      if ( st_var_define($3.ids[i].ids[j], $3.types[i], scope) == NULL ) {
				printf("Could not define var");
				return 1;
			}
    }
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
  if ( expression_evaluate($5, &temp, scope) == Failure ) {
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
  if ( expression_evaluate($3, &temp, scope) == Failure ) {
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
	constant_t temp1, temp2, temp;
	expression_t *l, *r;

	// Attempt to evaluate expression in compile time
	if ( enable_constant_propagation) {
	  if ( expression_evaluate($1, &temp1, scope) == Success )
			l = expression_constant(temp1.type , &(temp1.bconst));
		else 
			l = $1;
		if ( expression_evaluate($3, &temp2, scope) == Success )
			r = expression_constant(temp2.type, &(temp2.bconst));
		else
			r = $3;
	} else {
		l = $1;
		r = $3;
	}
	
  $$ = expression_binary(l, r, $2);

	if ( enable_constant_propagation )
		if ( expression_evaluate($$, &temp, scope) == Success ) {
			$$ = expression_constant(temp.type, &(temp.bconst));
		}
}
| expression EQU expression  
{
	constant_t temp1, temp2, temp;
	expression_t *l, *r;

	// Attempt to evaluate expression in compile time
	if ( enable_constant_propagation) {
	  if ( expression_evaluate($1, &temp1, scope) == Success )
			l = expression_constant(temp1.type , &(temp1.bconst));
		else 
			l = $1;
		if ( expression_evaluate($3, &temp2, scope) == Success )
			r = expression_constant(temp2.type, &(temp2.bconst));
		else
			r = $3;
	} else {
		l = $1;
		r = $3;
	}

  $$ = expression_binary(l, r, $2);

	if ( enable_constant_propagation )
		if ( expression_evaluate($$, &temp, scope) == Success ) {
			$$ = expression_constant(temp.type, &(temp.bconst));
		}
}

| expression INOP expression  
{
	constant_t temp1, temp2, temp;
	expression_t *l, *r;

	// Attempt to evaluate expression in compile time
	if ( enable_constant_propagation) {
	  if ( expression_evaluate($1, &temp1, scope) == Success )
			l = expression_constant(temp1.type , &(temp1.bconst));
		else 
			l = $1;
		if ( expression_evaluate($3, &temp2, scope) == Success )
			r = expression_constant(temp2.type, &(temp2.bconst));
		else
			r = $3;
	} else {
		l = $1;
		r = $3;
	}

  $$ = expression_binary(l, r, $2);

	if ( enable_constant_propagation )
		if ( expression_evaluate($$, &temp, scope) == Success ) {
			$$ = expression_constant(temp.type, &(temp.bconst));
		}
}

| expression OROP expression  
{
	constant_t temp1, temp2, temp;
	expression_t *l, *r;

	// Attempt to evaluate expression in compile time
	if ( enable_constant_propagation) {
	  if ( expression_evaluate($1, &temp1, scope) == Success )
			l = expression_constant(temp1.type , &(temp1.bconst));
		else 
			l = $1;
		if ( expression_evaluate($3, &temp2, scope) == Success )
			r = expression_constant(temp2.type, &(temp2.bconst));
		else
			r = $3;
	} else {
		l = $1;
		r = $3;
	}

  $$ = expression_binary(l, r, $2);

	if ( enable_constant_propagation )
		if ( expression_evaluate($$, &temp, scope) == Success ) {
			$$ = expression_constant(temp.type, &(temp.bconst));
		}
}

| expression ADDOP expression  
{
 	constant_t temp1, temp2, temp;
	expression_t *l, *r;

	// Attempt to evaluate expression in compile time
	if ( enable_constant_propagation) {
	  if ( expression_evaluate($1, &temp1, scope) == Success )
			l = expression_constant(temp1.type , &(temp1.bconst));
		else 
			l = $1;
		if ( expression_evaluate($3, &temp2, scope) == Success )
			r = expression_constant(temp2.type, &(temp2.bconst));
		else
			r = $3;
	} else {
		l = $1;
		r = $3;
	}

  $$ = expression_binary(l, r, $2);

	if ( enable_constant_propagation )
		if ( expression_evaluate($$, &temp, scope) == Success ) {
			$$ = expression_constant(temp.type, &(temp.bconst));
		}
}

| expression MULDIVANDOP expression  
{
 	constant_t temp1, temp2, temp;
	expression_t *l, *r;

	// Attempt to evaluate expression in compile time
	if ( enable_constant_propagation) {
	  if ( expression_evaluate($1, &temp1, scope) == Success )
			l = expression_constant(temp1.type , &(temp1.bconst));
		else 
			l = $1;
		if ( expression_evaluate($3, &temp2, scope) == Success )
			r = expression_constant(temp2.type, &(temp2.bconst));
		else
			r = $3;
	} else {
		l = $1;
		r = $3;
	}

  $$ = expression_binary(l, r, $2);

	if ( enable_constant_propagation )
		if ( expression_evaluate($$, &temp, scope) == Success ) {
			$$ = expression_constant(temp.type, &(temp.bconst));
		}
}

| ADDOP expression 
{
	constant_t temp;
	expression_t *e;
	
	e = $2;

	if ( enable_constant_propagation ) {
		if ( expression_evaluate($2, &temp, scope) == Success ) 
			e = expression_constant(temp.type, &(temp.bconst));
	}

  if ( $1 == AddopP ) 
    $$ = e;
  else {
    //TODO negative e ( quick hack )
    expression_t *neg = calloc(1, sizeof(expression_t));
    neg->type = ET_Constant;
    neg->constant.type = VT_Iconst;
    neg->constant.iconst = -1;
    neg->dataType = VT_Integer;
    //neg->next = NULL;

    $$ = expression_binary(neg, e, MuldivandopM); 
    //$$ = NULL;
  }
}
| NOTOP expression
{
	constant_t temp;
	expression_t *e;
	
	e = $2;

	if ( enable_constant_propagation ) {
		if ( expression_evaluate($2, &temp, scope) == Success ) 
			e = expression_constant(temp.type, &(temp.bconst));
	}

	$$=  expression_not(e);

	if ( enable_constant_propagation )
		if ( expression_evaluate($$, &temp, scope) == Success ) {
			$$ = expression_constant(temp.type, &(temp.bconst));
		}
}
| variable 
{
  $$ = expression_variable($1, scope);
}
| ID LPAREN expressions RPAREN 
{
  $$ = expression_call($1, $3.exprs, $3.size, scope);
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
  p->expr = $3;
}
| LISTFUNC LPAREN expression RPAREN 
{
  $$ = NULL;
  printf("%d) Listfunc is not supported\n", yylineno);
	assert(0 && "ListFunc is not supported");
}
;

expressions : expressions COMMA expression 
{
  $$ = $1;
  $$.exprs = ( expression_t * ) realloc( $$.exprs, sizeof(expression_t) * ( $$.size+1));

	assert( $3 && "Failed to parse expression in expressions(1)");

	$$.exprs[ $$.size++ ] = *$3;

	free($3);
}
| expression
{
	assert($1 && "Failed to parse expression in expressions(2)");
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

limit : sign ICONST 
{
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
	int i,j ;
  $1.ids = (char**) realloc( $1.ids, ($1.size+$3.size) * sizeof(char*));
  $1.types = (data_type_t*) realloc( $1.types, ($1.size+$3.size) * sizeof(data_type_t));
	for ( j=0, i=$1.size; i < $1.size + $3.size; i++, j++ ) {
	  $1.ids[ i ] = $3.ids[j];
  	$1.types[ i ] = $3.types[j];
	}
	$1.size += $3.size;
  $$ = $1;
}
| field 
{
  $$ = $1;
}
;

field : identifiers COLON typename 
{
	int i;

  $$.size = $1.size;
  $$.ids = (char **) calloc(1, $1.size * sizeof(char*));
  $$.types = (data_type_t*) calloc(1, $1.size * sizeof(data_type_t));
	
	for ( i = 0; i < $1.size; i ++ ) {
		$$.ids[ i ] = $1.ids[i];
		$$.types[ i ] = $3;
	}
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
{
	int i,j;
  var_t *var;
  var_t *params=0;
  int size = 0;
  func_t *func;
	


	if ( $1.isForward == 0 ) {
		scope = st_init(scope);
		// This function is declared and specified here

		// first register the function ID as a local variable
		st_var_define($1.id, $1.type, scope);

		// then register all parameters as local variables so that they are visible
		for ( i = 0; i < $1.size; i ++ ) {
			for ( j = 0; j < $1.params[i].ids.size; j++ ) {
				var = st_var_define($1.params[i].ids.ids[j], $1.params[i].type, scope);
				if ( var ) {
					var->pass = $1.params[i].pass;
					size++;

					params = ( var_t * ) realloc(params, size * sizeof(var_t));
					params[ size-1 ] = *var;
				} else {
					
				}
			}
		}

		// register the function to the global scope
		func = st_func_define($1.id, $1.type, params, size, scope->parent);
		func->isProcedure = $1.isProcedure;
		func->scope = scope; // store the scope
		scope = scope->parent;
	}
	
}
| sub_header SEMI  
{
	if ( $1.isForward ) {
		func_t *func = st_func_find($1.id, scope);
		if ( func == NULL ) {
			yyerror("[-] Function %s is not forward-declared.", $1.id);
			return 1;
		}
		scope = func->scope;
	} else
		scope = st_init(scope);
} declarations
{
  int i,j;
  var_t *var;
  var_t *params=0;
  int size = 0;
  func_t *func;

	if ( $1.isForward == 0 ) {
		// This function is declared and specified here

		// first register the function ID as a local variable
		st_var_define($1.id, $1.type, scope);

		// then register all parameters as local variables so that they are visible
		for ( i = 0; i < $1.size; i ++ ) {
			for ( j = 0; j < $1.params[i].ids.size; j++ ) {
				var = st_var_define($1.params[i].ids.ids[j], $1.params[i].type, scope);
				if ( var ) {
					var->pass = $1.params[i].pass;
					size++;

					params = ( var_t * ) realloc(params, size * sizeof(var_t));
					params[ size-1 ] = *var;
				} else {
					yyerror("[-] Param %s is already defined.", $1.params[i].ids.ids[j]);
					return 1;
				}
			}
		}

		// register the function to the global scope
		func = st_func_define($1.id, $1.type, params, size, scope->parent);
		func->isProcedure = $1.isProcedure;
	} else {
		// This function is already forward-declared
		func = st_func_find($1.id, scope->parent);
		if ( func == NULL ) {
			yyerror("[-] Function %s is not forward-declared.");
			return 1;
		} else if ( func->body ) {
			yyerror("[-] Function %s already has a body.");
			return 1;
		}
	}
}
subprograms comp_statement
{
	func_t *func = st_func_find($1.id, scope->parent);
	func->body = $7;
  scope = scope->parent; // TODO should the scope be destroyed here? I think not
}

;

sub_header : FUNCTION ID formal_parameters COLON standard_type 
{
	$$.isForward = 0;
  $$.id = $2;
  $$.isProcedure = 0;
  $$.type = $5;
  $$.params = $3.params;
  $$.size = $3.size;
}
| FUNCTION ID formal_parameters COLON LIST 
{
	$$.isForward = 0;
  $$.id = $2;
  $$.isProcedure = 0;
  $$.type.dataType = 0;
  $$.params = $3.params;
  $$.size= $3.size;
}
| PROCEDURE ID formal_parameters 
{
	$$.isForward = 0;
  $$.id = $2;
  $$.isProcedure = 1;
  $$.type.dataType = 0;
  $$.params = $3.params;
  $$.size= $3.size;
}
| FUNCTION ID  
{
	$$.isForward = 1;
  $$.id = $2;
  $$.isProcedure = 0;
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
  if ( p  && $3 ) {
    p->next = $3;
		p->join = $3;
 
    if ( $3 ) {
      $3 -> prev = p;
		}
	
    switch ( p->type )
    {
      case ST_If:
        if ( ( t = p->_if._true ) != NULL ) {
          for ( ;  t->next; t = t->next );
          t->join = $3;
        }
        
        if ( ( t = p->_if._false ) != NULL ) {
          for ( ; t->next; t = t->next );
          t->join = $3;
        }
			p->join = $3;
      break;

      case ST_While:
        if ( ( t = p->_while.loop ) != NULL ) {
          for ( ; t && t->next; t = t->next );
          
          t->join = $3;
        }
      break;

      case ST_For:
        if ( ( t = p->_for.loop ) != NULL ) {
          for ( ; t && t->next; t = t->next );
          
          t->join = $3;
        }
			break;

      case ST_With:
        if ( ( t = p->with.statement ) != NULL ) {
          for ( ; t && t->next; t = t->next );
          t->join = $3;
        }
      break;

			default:
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
| unmatched

;

matched: assignment 
| matched_if_statement
| case_statement 
| while_statement 
| for_statement 
| with_statement 
| subprogram_call 
| io_statement 
| comp_statement
|
{
  $$ = NULL;
}
;


matched_if_statement: IF expression THEN matched ELSE matched
{
  $$ = statement_if($2, $4, $6);

	if ( $$ == NULL ) {
		yyerror("[-] Failed to generate IF statement.");
		return 1;
	}
}
;

unmatched: IF expression THEN statement
{ 
  $$ = statement_if($2, $4, NULL);

	if ( $$ == NULL ) {
		yyerror("[-] Failed to generate IF statement.");
		return 1;
	}
}
| IF expression THEN matched ELSE unmatched
{
  $$ = statement_if($2, $4, $6);

	if ( $$ == NULL ) {
		yyerror("[-] Failed to generate IF statement.");
		return 1;
	}
}
;

assignment : variable ASSIGN expression 
{
	$$ = statement_assignment($1, $3, scope);
	
	if ( $$ == NULL ) {
		yyerror("[-] Failed to generate ASSIGNMENT statement.");
		return 1;
	}
}
| variable ASSIGN STRING 
{
  $$ = statement_assignment_str($1, $3, scope);

	if ( $$ == NULL ) {
		yyerror("[-] Failed to generate ASSIGNMENT_STR statement");
		return 1;
	}
}
;

case_statement : CASE expression OF cases case_tail END 
{
	$$ = NULL;

	printf("[-] CASE statement is not supported at the moment.");

	return 0;
}
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

	
	if ( $$ == NULL ) {
		yyerror("[-] Failed to generate WHILE statement.");
		return 1;
	}

}
;


for_statement : FOR ID ASSIGN iter_space DO statement
{
  $$ = statement_for($2, &$4, $6, scope);

	if ( $$ == NULL ) {
		yyerror("[-] Failed to generate FOR statement.");
		return 1;
	}
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


with_statement : WITH variable DO 
{
  scope = st_init(scope);
  $<statement>$ = statement_with($2, NULL, scope);
}
statement
{
  $$ = $<statement>4;
  scope = st_destroy(scope);
  $$->with.statement = $5;
}
;

subprogram_call : ID
{
  scope_t *global;
  for ( global = scope; global->parent; global = global->parent);

  $$ = statement_call($1,NULL, 0, global);

	if ( $$ == NULL ) {
		yyerror("[-] Failed to generate CALL statement.");
		return 1;
	}
}
| ID LPAREN expressions RPAREN 
{
  scope_t *global;
  for ( global = scope; global->parent; global = global->parent);

  $$ = statement_call($1, $3.exprs, $3.size, global);

	if ( $$ == NULL ) {
		yyerror("[-] Failed to generate CALL statement.");
		return 1;
	}
}
;

io_statement : READ LPAREN read_list RPAREN 
{
	$$ = NULL; 

	yyerror("[-] io_statement(READ) is not supported at the moment\n");
	return 1;
}
| WRITE LPAREN write_list RPAREN
{
	$$ = NULL;
	
	yyerror("[-] io_statement(WRITE) is not supported at the moment\n");
}
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


void yyerror(const char *fmt, ...)
{
	const char *p;
	va_list argp;
	int i;
	char *s;

  printf("Error[%d]: ", yylineno);
	va_start(argp, fmt);

	for(p = fmt; *p != '\0'; p++)
		{
		if(*p != '%')
			{
			putchar(*p);
			continue;
			}

		switch(*++p)
			{
			case 'c':
				i = va_arg(argp, int);
				putchar(i);
				break;

			case 'd':
				i = va_arg(argp, int);
				printf("%d", i);
				break;

			case 's':
				s = va_arg(argp, char *);
				fputs(s, stdout);
				break;

			case 'x':
				i = va_arg(argp, int);
				printf("%x", i);
				break;

			case '%':
				putchar('%');
				break;
			}
	}

	va_end(argp);
	printf("\n");
}

void print_help(char *path)
{
	char *file;
	file = strrchr(path, '/');
	
	if ( file == NULL )
		file = path;
	else
		file ++;
	
	printf("Usage: %s [OPTIONS] SOURCE_FILE\n"
				 "Options\n"
				 "\t-c --constant_propagation      Enable constant propagation\n"
				 "\t-d --dead_code_elimination     Enable dead code elimination\n"
         "\t-e --subexpression_elimination Enable common subexpression elimination\n"
				 "\n"
				 
				 , file);
}


int main(int argc, char* argv[])
{
  int ret;
	
	if ( argc == 1 ) 
	{
		print_help(argv[0]);
		return 0;		
	}

	while ( ( ret = getopt_long(argc, argv, s_opts, l_opts, NULL) ) != -1 ) {
		switch ( ret ) {
			case 'h':
				print_help(argv[0]);
				return 0;
			break;

			case 'c':
				enable_constant_propagation = 1;
			break;

			case 'd':
				enable_dead_code_elimination = 1;
			break;

      case 'e':
        enable_subexpression_elimination = 1;
      break;

      case 's':
        enable_scheduling = 1;
      break;
		}
	}
	
	if ( optind >= argc ) {
		printf("[-] Missing File argument.\n");
		return 1;
	}

	yyin = fopen(argv[optind], "r");

	if ( yyin == NULL ) {
		printf("[-] Could not open \"%s\".\n", argv[optind]);
		return 1;
	}

  ret = yyparse();

  printf("[$] yyparse: %d\n", ret);
  return 0;
}

