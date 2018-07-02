(*******************************************
 * File: BitOperators.pas
 * A test program for boolean operators
 *
 * Compile with the following command:
 * # fpc -MOBJFPC BitOperators.pas
 *******************************************)
program BitOperators;

{$ifndef NANOPASCAL}
//NanoPascal won't support libraries
uses SysUtils;
{$endif}

var x, y: Integer;

begin
    {$ifdef RANDOM}
    Randomize;

    x := Random(100) + 1;
    y := Random(100) + 1;
    {$else}
    x := $00ff83ff;
    y := $ff00edff;
    {$endif}

    writeln('X = ', x, 8, ' Y = ', y);

    // And
    writeln('x and y: ', x and y);

    // or
    writeln('x or y: ', x or y);

    // not
    writeln('not x: ', not x);

    // Shift left
    writeln('x shl 4: ', x shl 4);

    // Shift right
    writeln('x shr 4: ', x shr 4);
end.
(*** Expected output (non random) ***
X = 00FF83FF Y = FF00EDFF
x and y: 000081FF
x or y: FFFFEFFF
not x: FF007C00
x shl 4: 0FF83FF0
x shr 4: 000FF83F
*)
