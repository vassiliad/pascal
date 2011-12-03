#ifndef __CONSTANTS_H
#define __CONSTANTS_H
enum {Success, Failure};
enum VariableType { VT_Integer, VT_Real, VT_Boolean, VT_Char, VT_User };
enum ConstantType { CT_Iconst, CT_Rconst, CT_Bconst , CT_Cconst};
enum LimitType { LT_Iconst, LT_Bconst, LT_Id, LT_Cconst };
enum EpressionType { ET_Binary, ET_Constant, ET_Not, ET_Variable };
enum TypedefType { TT_Array, TT_List, TT_Set, TT_Record, TT_Range };
enum BConst { False, True };
enum Operators
{
  AddopP, AddopM,
  RelopG, RelopGE, RelopL, RelopLE, RelopD,
  Orop,
  MuldivandopM, MuldivandopDiv, MuldivandopD, 
  MuldivandopAnd, MuldivandopMod,
  Notop,
  Inop,
	Equop
};

enum Sign { Positive, Negative };

#endif // __CONSTANTS_H
