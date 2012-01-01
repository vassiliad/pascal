program tet_scopes;

function try_me(i,j:integer): integer;
  var k:integer;
begin
  k:= 0;
  try_me := 5
end;

function try_me_2:integer;
begin
  
end;
begin
try_me(1,2);

end.
