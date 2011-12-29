program test1;
var a,b,c,d,e,x,y: integer;
		testUserType : userType1;
function try_me(i,j:integer): integer;
begin
  i := i+j;
  try_me := i
end;
begin
  a := 5;
  b := 10;
  
  a :=  try_me(x,b); 
  c := a*x;
  
	for y:=0 to 15 do
  begin
    if ( x > a ) then
      x := a + 10
  end;

    if ( x > a ) then
      begin
        if ( 1 in [ 1, 2, 3 ] ) then
          begin
           d := x + 5 ;
           e := 6;
           a := 9
          end
      end
    else
        x := a*3;

  if ( x > a ) then
    begin
     d := x + 5 ;
     e := 6;
     a := 9
    end
  else
    begin
      x := a*3
		end
end.
