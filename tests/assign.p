program test1;
var a,b,c,d,e,f,g: integer;
begin
  a := 5;
  b := a+5;
  f := 5+b+a;
  g := 7+f;
  d := a-b+f*g;
  c := a*f;
  a := a+b+c+d+e+f+g;
  if a < 5  then
  begin
   a := 9*b;
  end;
  b := a+c
end.