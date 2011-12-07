program try_it;
const g= 0B1011;
type ar= array[1..g] of integer;
     ar1= array[1..g] of real;
     re= record
           x1,x2:integer { no comment
at all! }
         end;
     re1= record b:boolean end;
     gar= -g..g;
     arre1 = array[gar] of re1;
     ilt= list of integer;
var x,y: ar; xx: ar1; z,w: real; r: re;
    i,j,k: integer; b: arre1; il: ilt;
function try_me(i,j:integer): boolean; forward;
procedure _all(n:integer; var x: ilt);
   function try_me(i,j:integer): real;
   const omega='W'; int=g div 2;
   procedure nested(var i:integer);
      var k:integer;
      begin
      if xx[i] >= 0.0 then k := lengTH(x) else k := 2;
      x := x + k;
      i := k
      end;
   begin
   nested(k);
   if (i+j > int) then try_me :=0.0
   else
      if (i > int) then try_me :=-1.0
      else try_me := 1.0
   end;
begin
if (n <= 0) then
   x := new(1) {{ or x := new(-1)}
else
   begin
   _all(try_me(n-1,y[n+1]),x);
   cdr(x) := new(-car(x))
   end
end;
function try_me;
begin
try_me := i = j;
end;
procedure kkk;
begin
write("Reached procedure kkk");
end;
begin
b[r.x1].b := not (x[r.x1] > x[r.x2]) and not try_me(0XA10,x[j]) or (z = k*3);
il := [-1,9,0,3];
with r do
   begin
   _all(-y[x[x1]],il);
   with b[x[x2]] do
      if (b and (x1 > y[x1 mod k])) then kkk;
   x2 := x1 div k;
   end;
for i := 1 to x[y[g]] do
   read(x[i],j,y[j]);
y := x;
with b[i] do
   b := not b or (r.x2 = 0);
write("endofprogram")
end.
