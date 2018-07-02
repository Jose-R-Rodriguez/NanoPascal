program Directives;
begin
{$ifndef NANOPASCAL}
    writeln('Hello World from NanoPascal');
    {$ifdef HOLA}
        writeln('HOLA');
        {$ifdef ADIOS}
            writeln('ADIOS');
        {$endif}
    {$else}
        writeln('HOLA2');
    {$endif}
{$else}
    writeln('Hello World from Other Pascal Compiler');
{$endif}
end.

(*
Expected Result:
Defined directives: NANOPASCAL
Token: program 'program'
Token: Identifier 'Directives'
Token: Semicolon ';'
Token: begin 'begin'
Token: writeln 'writeln'
Token: Open Parenthesis '('
Token: String 'Hello World from NanoPascal'
Token: Close Parenthesis ')'
Token: Semicolon ';'
Token: writeln 'writeln'
Token: Open Parenthesis '('
Token: String 'HOLA2'
Token: Close Parenthesis ')'
Token: Semicolon ';'
Token: end 'end'
Token: . '.'
Lexic Analisys completed succesfully.
*)
