#include <windows.h>
#include <stdio.h>
#include <tchar.h>

void _tmain(int argc, TCHAR* argv[])
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    char a[255]="calc.exe & cmd.exe";
    // Start the child process. 
    if (!CreateProcess(
            NULL,           // lpApplicationName;       No module name (use command line)
            a,              // lpCommandLine
            NULL,           // lpProcessAttributes;     Process handle not inheritable
            NULL,           // lpThreadAttributes;      Thread handle not inheritable
            FALSE,          // bInheritHandles;         Set handle inheritance to FALSE
            0,              // dwCreationFlags;         No creation flags
            NULL,           // lpEnvironment;           Use parent's environment block
            NULL,           // lpCurrentDirectory;      Use parent's starting directory 
            &si,            // lpStartupInfo;           Pointer to STARTUPINFO structure
            &pi             // lpProcessInformation;    Pointer to PROCESS_INFORMATION structure
        )
       )
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return;
    }

    // Wait until child process exits.
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}