#ifndef __BISON_UNION
#define __BISON_UNION
typedef struct CONSTANT_T constant_t;
typedef struct VARIABLE_T variable_t;
typedef struct PARAMETERS_T parameters_t;

enum LimitType { LT_Iconst, LT_Cconst, LT_Bconst, LT_Id };
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
	int dataType; 	// see symbol_table.h
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


typedef variabledefs_t fields_t; 	// it's basically the same information
//=====================================================//
enum TypedefType { TT_Array, TT_List, TT_Set, TT_Record, TT_Range };
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
typedef fields_t record_t; 				// >>
typedef struct
{
	limit_t from, to;
} range_t;

typedef struct
{
	int type;
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
//=====================================================//



typedef struct
{
	typedefs_t typedefs;
	variabledefs_t vardefs;
} declarations_t;

enum ConstantType { CT_Iconst, CT_Cconst, CT_Bconst, CT_Rconst };
struct CONSTANT_T
{
	int type;
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


enum EpressionType { ET_Binary, ET_Constant, ET_Not, ET_Variable };
struct EXPRESSION_T
{
	int type;
	int dataType;
	
	union {
		expression_binary_t binary;
		expression_constant_t constant;
		expression_t *notExpr;
		variable_t *variable;
	};

	expression_t *next, *prev;
};


typedef struct {
	expression_t *exprs;
	int size;
} expressions_t;

struct VARIABLE_T
{
	char *id;
	int variableType;
	char *userType; // only used when variableType = VT_User;
	
	expressions_t expr; // used in case of limits
	variable_t *parent; // in case of records
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

typedef struct
{
  char *id;
  data_type_t type;
  parameters_t *params;
  int size;
} sub_header_t;

#endif // __BISON_UNION
