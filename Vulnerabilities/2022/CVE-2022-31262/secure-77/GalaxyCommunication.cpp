// GOG Galaxy.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//
#include <iostream>
#include <Windows.h>
#include <Wtsapi32.h>
#include <fstream>
#include <Lmcons.h>
#include <stdlib.h>
#include <WinBase.h>

using namespace std;

#pragma comment(lib, "Wtsapi32.lib")


void spawn() {
    wchar_t desktop[] = L"WinSta0\\Default";
    wchar_t cmd[] = L"C:\\windows\\system32\\cmd.exe";
  
    HANDLE hToken;
    DWORD sessionid = 0xFFFFFFFF;
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
    PWTS_SESSION_INFO sessions;
    DWORD count;

    if (WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, 0, 1,
        &sessions, &count))
    {
        for (DWORD i = 0; i < count; ++i)
        {
            if (sessions[i].State == WTSActive)
            {
                sessionid = sessions[i].SessionId;
            }
        }
    }
    if (sessionid == 0xFFFFFFFF)
    {
        sessionid = WTSGetActiveConsoleSessionId();
    }

    si.cb = sizeof(si);
    RtlZeroMemory(&si, sizeof(si));
    RtlZeroMemory(&pi, sizeof(pi));

    si.wShowWindow = SW_SHOW;
    si.lpDesktop = desktop;

    sessionid = WTSGetActiveConsoleSessionId();

    OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken);
    DuplicateTokenEx(hToken, TOKEN_ALL_ACCESS, NULL, SecurityAnonymous, TokenPrimary, &hToken);
    SetTokenInformation(hToken, TokenSessionId, &sessionid, sizeof(sessionid));
    if (CreateProcessAsUser(hToken, cmd, NULL, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

}


int main()
{
    spawn();

}
