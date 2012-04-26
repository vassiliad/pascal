program tet_scopes;
const dim1 = 10;
var unused : integer;
		i : integer;
function try_me(i,j:integer): integer;
  var k:integer;
begin
  k:= 0;
  try_me := 5;
end;

begin
	while ( i < 10 ) do
	begin
	end
	;
	{i := try_me(1,1);
	i := try_me(1,1);
	i := try_me(1,1);}


if i < 10 then
		begin
		end
	;	

	{i := try_me(1,1); }

	if i < 10 then
		begin
		end
{	;
	
	i := try_me(1,1);
	i := try_me(1,1); }
end.
