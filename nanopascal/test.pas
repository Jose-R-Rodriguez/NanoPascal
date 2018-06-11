program Directives;
begin
{$ifdef NANOpASCAL}
{$ifdef tesing}
writeln('Hello World2');
{$else}
{$endif}
writeln('Hello World1');
{$else}
{$endif}

end.
