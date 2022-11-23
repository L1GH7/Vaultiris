program Payload;

uses Unix, Process;

var
  output: AnsiString;
begin
  RunCommand ('notify-send', ['Fake Nautilus',
      'This was sent by the target.mp3 file you downloaded.'],
      output);

  FpExecVP ('nautilus', argv);
end.
