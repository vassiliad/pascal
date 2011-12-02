program test1;
var a,b,c,d,e,x: integer;
		testUserType : userType1;
function try_me(i,j:integer): integer;
begin
  i := i+j;
end;
begin
  a := 5;
  b := 10;
  
  a :=  try_me(x,b);
  c := a*x;
  
  if ( x > a ) then
    begin
     d := x + 5 ;
     e := 6;
     a := 9;
    end
  else
      x := a*3;
  

  if ( x > a ) then
    begin
     d := x + 5 ;
     e := 6;
     a := 9;
    end
  else
    begin
      x := a*3;
  end

end.
