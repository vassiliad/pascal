program test1;
var a,b,c,d,e,f,g: integer;
begin
  a := 5;
  b := 10;
  f := 3;
  g := 7;
  d := a-b+f*g;
  c := a*f;
  a := a+b+c+d+e+f+g;
  if a < 5  then
  begin
   a := 9*b;
  end;
  b := a+c
end.