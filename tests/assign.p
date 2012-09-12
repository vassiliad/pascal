program test1;
var a,b,c,d,e,f,g: integer;
begin
  a := 5;
  b := a+5;
  f := 5+b+a;
  g := 7+f;
  c := a*f;
  a := a+b+c+d+e+f+g;
  if a < 5 then
   a := 9*b
  else
		begin
		b:= a*10;
		end;
  b := a+c;
  d := a-b+f*g;
  d := a-b+f*g+3+2*2312*c;
end.
