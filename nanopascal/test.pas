program Directives;
begin
{$ifdef NANOpASCAL}
{$ifdef testing}
{$else}
writeln('Hello World1');
{$endif}
{$else}
writeln('Hello World2');
{$endif}
end.
