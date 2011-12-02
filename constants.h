#ifndef __CONSTANTS_H
#define __CONSTANTS_H
enum {Success, Failure};
enum VariableType { VT_Integer, VT_Real, VT_Boolean, VT_Char, VT_User };
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
