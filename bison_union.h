#ifndef __BISON_UNION
#define __BISON_UNION
#include "constants.h"

typedef struct CONSTANT_T constant_t;
typedef struct VARIABLE_T variable_t;
typedef struct PARAMETERS_T parameters_t;
typedef struct EXPRESSIONS_T expressions_t;
typedef struct STATEMENT_T statement_t;
typedef struct STATEMENT_IF_T statement_if_t;
typedef struct STATEMENT_WHILE_T statement_while_t;
typedef struct STATEMENT_ASSIGNMENT_T statement_assignment_t;
typedef struct STATEMENT_FOR_T statement_for_t;
typedef struct STATEMENT_CASE_T statement_case_t;
typedef struct STATEMENT_CALL_T statement_call_t;
typedef struct STATEMENT_WITH_T statement_with_t;
typedef struct SUB_HEADER_T sub_header_t;
typedef struct EXPRESSION_CALL_T expression_call_t;


typedef struct
{
  int type;
  union {
    int iconst;
    char cconst;
    char bconst;
    struct {
      int sign;
      char *id;
    } id;
  };
} limit_t;


typedef struct 
{
	int isRange; // if @isRange==1 then use (@from, @to) else use @limit
	union {
		struct {
			limit_t from, to;
		} range;
		limit_t limit;
	};
} limits_entry_t;

typedef struct // array of structs that hold limits ( dimensions )
{
	limits_entry_t *limits;
	int size;
} limits_t;

typedef struct
{
	int dataType; 	// see constants.h
	char *userType; // only used when @variableType = VT_User;
} data_type_t;

typedef struct
{
	char **ids;
	int size;
} identifiers_t;

typedef struct
{
	identifiers_t *ids;
	data_type_t   *types;
	int size;
} variabledefs_t;

typedef struct
{
  char **ids;
  data_type_t *types;
  int size;
} fields_t;

typedef fields_t record_t;

//=====================================================//
typedef struct 
{
	int typedefType;
	data_type_t typename;
	limits_t dims;
} array_t;

typedef struct
{
	data_type_t typename;
} list_t;

typedef struct
{
	data_type_t typename;
}set_t;

typedef struct
{
	limit_t from, to;
} range_t;

typedef struct
{
	enum TypedefType type;
	char *name;
	union {
		array_t array;
		list_t list;
		set_t set;
		record_t record;
		range_t range;
	};
} typedefs_entry_t;

typedef struct
{
	typedefs_entry_t *typedefs;
	int size;
} typedefs_t;

typedef struct 
{
  constant_t *constants;
  char **ids;
  int size;
} constdefs_t;
//=====================================================//



typedef struct
{
	typedefs_t typedefs;
	variabledefs_t vardefs;
  constdefs_t constdefs;
} declarations_t;

struct CONSTANT_T
{
	enum VariableType type;
	union {
		char cconst;
		int iconst;
		float rconst;
		char bconst;
	};

};

typedef struct CONSTANT_T expression_constant_t;
typedef struct EXPRESSION_T expression_t;

typedef struct
{
	expression_t *left, *right;
	int op;
} expression_binary_t;

struct EXPRESSION_CALL_T
{
  char *id;
  int isProcedure;
  data_type_t type;
  expression_t *params;
  int size;
  statement_t *body;
};

struct EXPRESSION_T
{
	enum ExpressionType type;
	int dataType;
	
	union {
		expression_binary_t binary;
		expression_constant_t constant;
		expression_t *notExpr;
    expressions_t *exprs;
		variable_t *variable;
    expression_call_t *call;
  };
};


struct EXPRESSIONS_T {
  expression_t *exprs;
  int size;
};

struct VARIABLE_T
{
  char *id;
  data_type_t type;
  
  expressions_t expr; // used in case of limits
  variable_t *child; // in case of records
};

struct PARAMETERS_T
{
  data_type_t type;
  int pass;
  identifiers_t ids;
};

typedef struct
{
  parameters_t *params;
  int size;
}parameter_list_t;

struct SUB_HEADER_T
{
  char *id;
	int isForward;
  int isProcedure;
  data_type_t type;
  parameters_t *params;
  int size;
  statement_t *body;
};

typedef struct
{
  expression_t *from, *to;
  enum IterSpaceType type;
} iter_space_t;

struct STATEMENT_IF_T
{
  statement_t *_true;
  statement_t *_false;
  expression_t *condition;
};

struct STATEMENT_WHILE_T
{
  statement_t* loop;
  expression_t *condition;
};

struct STATEMENT_ASSIGNMENT_T
{ 
  enum AssignmentType type;
  variable_t *var;
  union
  {
    expression_t *expr;
    char *string;
  };
};

struct VAR_T
{
	data_type_t type;
	char *id;
  int pass;
  struct VAR_T* reference; // used only when pass == 1
	int readOnly;
  char *base_address;      // to generate addresses for variables
};

struct STATEMENT_FOR_T
{
  expression_t *from, *to;
  struct VAR_T *var;
  statement_t *loop;
	enum IterSpaceType type;
};

struct STATEMENT_CASE_T
{
};

struct STATEMENT_CALL_T
{
  char *id;
  expression_t *params;
  data_type_t type;
  int size;
};

struct STATEMENT_WITH_T
{
  statement_t *statement;
  variable_t *var;
};

struct STATEMENT_T
{
  enum StatementType type;
  union {
    statement_if_t _if;
    statement_while_t _while;
    statement_assignment_t assignment;
    statement_for_t _for;
    statement_case_t _case;
    statement_call_t call;
    statement_with_t with;
  };
  statement_t *next, *prev, *join;
};



#endif // __BISON_UNION
