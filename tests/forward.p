program tet_scopes;
const dim1 = 10;

var unused : integer;

function try_me_2(i:integer):integer; FORWARD;

function try_me(i,j:integer): integer;
  var k:integer;
begin
  k:= 0;
  try_me := 5;
  try_me := try_me_2(1);
end;

function try_me_2;
begin
	try_me_2 := 1;
  
end;
begin
try_me(1,2);

end.
