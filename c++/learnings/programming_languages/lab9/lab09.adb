with Ada.Text_IO;
use Ada.Text_IO; 
with Ada.Integer_Text_IO; use Ada.Integer_Text_IO;

procedure Lab09 is
    
    package Flt_IO is new Ada.Text_IO.Float_IO(FLOAT);
    use Flt_IO;


    n:  NATURAL := 0;
    DOZEN: constant := 12;    
    FIRST : constant := 1;
    LAST : constant := 10;  
    MAX_ROW : constant := 3;
    MAX_COL : constant := 5;

  
    subtype SUB_INT is INTEGER range 1..5000;

    outf, inf : FILE_TYPE;
    
    type ARRAY_A is array(INTEGER range 1..MAX_ROW, INTEGER range 1..MAX_COL)  of INTEGER;
    type ARRAY_B is array(INTEGER range 1..MAX_ROW, INTEGER range 1..MAX_COL)  of INTEGER; 
    type ARRAY_C is array(INTEGER range 1..MAX_ROW, INTEGER range 1..MAX_COL)  of INTEGER; 

    a: ARRAY_A;
    b: ARRAY_B;
    c: ARRAY_C;
   

    begin
    Create(outf, Out_File, "lab09.out");
    Open(inf, In_File, "lab09.in");

    for i in 1 .. 3 loop
        for j in 1 .. 5 loop
            Get(inf, n);
            a(i, j) := n;
        end loop;
    end loop;

    
    Set_line(inf,2);
    for i in 1 .. 3 loop
        for j in 1 .. 5 loop
            Get(inf, n);
            b(i, j) := n;
        end loop;
    end loop;

    for i in 1 .. 3 loop
        for j in 1 .. 5 loop
            c(i, j) := a(i, j) * b(i, j);
            Put(outf, c(i, j));
        end loop;
    end loop;

    Close(inf);
    Close(outf);
                    

end Lab09;
