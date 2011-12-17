#ifndef __CONSTANTS_H
#define __CONSTANTS_H

enum PrimitiveSizes
{
  INTEGER_SIZE = 4,
  CHAR_SIZE    = 1,
  REAL_SIZE    = 4,
  BOOLEAN_SIZE = 1
};

enum 
{
  Success, 
  Failure
};

enum ForStepType 
{ 
  FT_DownTo,
  FT_To
};

enum StatementType 
{
  ST_If,
  ST_While,
  ST_For,
  ST_Call, 
  ST_Assignment,
  ST_Case, 
  ST_With 
};

enum AssignmentType
{
  AT_Expression, 
  AT_String 
};

enum VariableType {
  VT_Integer, 
  VT_Real, 
  VT_Boolean, 
  VT_Char, 
  VT_User, 
  VT_Iconst, 
  VT_Rconst, 
  VT_Bconst,
  VT_Cconst
};

enum LimitType 
{ 
  LT_Iconst,
  LT_Bconst,
  LT_Id, 
  LT_Cconst
};

enum EpressionType 
{
  ET_Binary,
  ET_Constant, 
  ET_Not, 
  ET_Variable, 
  ET_Set, 
  ET_Call 
};

enum TypedefType 
{ 
  TT_Array, 
  TT_List, 
  TT_Set, 
  TT_Record, 
  TT_Range
};

enum BConst 
{ 
  False, 
  True
};

enum Operators
{
  AddopP, AddopM,
  RelopG=10, RelopGE, RelopL, RelopLE, RelopD,
  Orop=20,
  MuldivandopM=30, MuldivandopDiv, MuldivandopD, MuldivandopAnd, MuldivandopMod,
  Notop=40,
  Inop=50,
	Equop=60
};

enum Sign { Positive, Negative };

#endif // __CONSTANTS_H
