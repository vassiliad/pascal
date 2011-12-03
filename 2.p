program testing;
type record_1= record
    int1,int2:integer;
    char1,char2:char
  end;
  record_2= record
    rec1 : record_1;
    _size : integer
  end;
var x: integer;
    y: char;
    test: record_2;

begin
  x:= 0;
  x:= test.rec1.int2;
end.
