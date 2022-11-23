@set local enableextensions
@set "COMPLUS_ENABLE_64BIT=0" rem  always select x86 for AnyCPU
@set "COMPLUS_Version=v4.0.30319"  rem  force .NET 4.0 for PowerShell 4.0
@set "COMPLUS_LoadFromRemoteSources=1"  rem  load assemblies from network

@set "powershell=%SystemRoot%\syswow64\WindowsPowerShell\v1.0\powershell.exe"
@set "rootPath=%~dp0"
@set "arguments=-Version 4.0 -NoProfile -NonInteractive -NoLogo -ExecutionPolicy Unrestricted -Command "

@set DefaultTask=Default

@if "%1" NEQ "" (SET DefaultTask=%1) 

@%powershell% %arguments% "&{ $ErrorActionPreference='Stop'; . '%~dp0\build.ps1' -taskList '%DefaultTask%'; exit $LASTEXITCODE; }"

@exit /B %ERRORLEVEL%
