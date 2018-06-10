program Directives;
begin
{$ifdef NANOPASCAL}
{$ifdef mykeyword}
{$endif}
{$else}
    writeln('Hello World from NanoPascal');
{$else}
    writeln('Hello World from Other Pascal Compiler');
{$endif}
end.
