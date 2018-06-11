program Directives;
begin
{$ifdef NANOpASCAL}
{$ifdef tesing}
{$else}
writeln('Hello World2');
{$endif}
writeln('Hello World1');
{$else}
{$endif}

end.
