(*******************************************
 * File: ArithOperators.pas
 * A test program for arithmethic operators
 *******************************************)
program ArithOperators;
var x, y: Integer;

begin
    {$ifdef RANDOM}
    Randomize;

    x := Random(100) + 1;
    y := Random(100) + 1;
    {$else}
    x := 4567;
    y := 33;
    {$endif}

    writeln('X = ', x, ' Y = ', y);
		write ('X = ', x, ' Y = ', y);

    writeln('Add ', x + y);
    writeln('Sub ', x - y);
    writeln('Mul ', x * y);
    writeln('Div ', x div y);
    writeln('Mod ', x mod y);
end.
(*** Expected output (non random) ***
X = 4567 Y = 33
Add 4600
Sub 4534
Mul 150711
Div 138
Mod 13
*)
