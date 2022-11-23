// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include <iostream>
#include <windows.h>
#include <Wtsapi32.h>
#include <sstream>
#include <memory>

//extern "C" __declspec()
#pragma comment(lib, "Wtsapi32.lib")

//exports goes here
#pragma comment(linker, "/export:CommandLineToArgvW=dbghelper.dll.CommandLineToArgvW")
#pragma comment(linker, "/export:CreateRandomAccessStreamOnFile=dbghelper.dll.CreateRandomAccessStreamOnFile")
#pragma comment(linker, "/export:CreateRandomAccessStreamOverStream=dbghelper.dll.CreateRandomAccessStreamOverStream")
#pragma comment(linker, "/export:CreateStreamOverRandomAccessStream=dbghelper.dll.CreateStreamOverRandomAccessStream")
#pragma comment(linker, "/export:DllCanUnloadNow=dbghelper.dll.DllCanUnloadNow")
#pragma comment(linker, "/export:DllGetActivationFactory=dbghelper.dll.DllGetActivationFactory")
#pragma comment(linker, "/export:DllGetClassObject=dbghelper.dll.DllGetClassObject")
#pragma comment(linker, "/export:GetCurrentProcessExplicitAppUserModelID=dbghelper.dll.GetCurrentProcessExplicitAppUserModelID")
#pragma comment(linker, "/export:GetDpiForMonitor=dbghelper.dll.GetDpiForMonitor")
#pragma comment(linker, "/export:GetDpiForShellUIComponent=dbghelper.dll.GetDpiForShellUIComponent")
#pragma comment(linker, "/export:GetFeatureEnabledState=dbghelper.dll.GetFeatureEnabledState")
#pragma comment(linker, "/export:GetFeatureVariant=dbghelper.dll.GetFeatureVariant")
#pragma comment(linker, "/export:GetProcessDpiAwareness=dbghelper.dll.GetProcessDpiAwareness")
#pragma comment(linker, "/export:GetProcessReference=dbghelper.dll.GetProcessReference")
#pragma comment(linker, "/export:GetScaleFactorForDevice=dbghelper.dll.GetScaleFactorForDevice")
#pragma comment(linker, "/export:GetScaleFactorForMonitor=dbghelper.dll.GetScaleFactorForMonitor")
#pragma comment(linker, "/export:IStream_Copy=dbghelper.dll.IStream_Copy")
#pragma comment(linker, "/export:IStream_Read=dbghelper.dll.IStream_Read")
#pragma comment(linker, "/export:IStream_ReadStr=dbghelper.dll.IStream_ReadStr")
#pragma comment(linker, "/export:IStream_Reset=dbghelper.dll.IStream_Reset")
#pragma comment(linker, "/export:IStream_Size=dbghelper.dll.IStream_Size")
#pragma comment(linker, "/export:IStream_Write=dbghelper.dll.IStream_Write")
#pragma comment(linker, "/export:IStream_WriteStr=dbghelper.dll.IStream_WriteStr")
#pragma comment(linker, "/export:IUnknown_AtomicRelease=dbghelper.dll.IUnknown_AtomicRelease")
#pragma comment(linker, "/export:IUnknown_GetSite=dbghelper.dll.IUnknown_GetSite")
#pragma comment(linker, "/export:IUnknown_QueryService=dbghelper.dll.IUnknown_QueryService")
#pragma comment(linker, "/export:IUnknown_Set=dbghelper.dll.IUnknown_Set")
#pragma comment(linker, "/export:IUnknown_SetSite=dbghelper.dll.IUnknown_SetSite")
#pragma comment(linker, "/export:IsOS=dbghelper.dll.IsOS")
#pragma comment(linker, "/export:IsProcessInIsolatedContainer=dbghelper.dll.IsProcessInIsolatedContainer")
#pragma comment(linker, "/export:RecordFeatureError=dbghelper.dll.RecordFeatureError")
#pragma comment(linker, "/export:RecordFeatureUsage=dbghelper.dll.RecordFeatureUsage")
#pragma comment(linker, "/export:RegisterScaleChangeEvent=dbghelper.dll.RegisterScaleChangeEvent")
#pragma comment(linker, "/export:RegisterScaleChangeNotifications=dbghelper.dll.RegisterScaleChangeNotifications")
#pragma comment(linker, "/export:RevokeScaleChangeNotifications=dbghelper.dll.RevokeScaleChangeNotifications")
#pragma comment(linker, "/export:SHAnsiToAnsi=dbghelper.dll.SHAnsiToAnsi")
#pragma comment(linker, "/export:SHAnsiToUnicode=dbghelper.dll.SHAnsiToUnicode")
#pragma comment(linker, "/export:SHCopyKeyA=dbghelper.dll.SHCopyKeyA")
#pragma comment(linker, "/export:SHCopyKeyW=dbghelper.dll.SHCopyKeyW")
#pragma comment(linker, "/export:SHCreateMemStream=dbghelper.dll.SHCreateMemStream")
#pragma comment(linker, "/export:SHCreateStreamOnFileA=dbghelper.dll.SHCreateStreamOnFileA")
#pragma comment(linker, "/export:SHCreateStreamOnFileEx=dbghelper.dll.SHCreateStreamOnFileEx")
#pragma comment(linker, "/export:SHCreateStreamOnFileW=dbghelper.dll.SHCreateStreamOnFileW")
#pragma comment(linker, "/export:SHCreateThread=dbghelper.dll.SHCreateThread")
#pragma comment(linker, "/export:SHCreateThreadRef=dbghelper.dll.SHCreateThreadRef")
#pragma comment(linker, "/export:SHCreateThreadWithHandle=dbghelper.dll.SHCreateThreadWithHandle")
#pragma comment(linker, "/export:SHDeleteEmptyKeyA=dbghelper.dll.SHDeleteEmptyKeyA")
#pragma comment(linker, "/export:SHDeleteEmptyKeyW=dbghelper.dll.SHDeleteEmptyKeyW")
#pragma comment(linker, "/export:SHDeleteKeyA=dbghelper.dll.SHDeleteKeyA")
#pragma comment(linker, "/export:SHDeleteKeyW=dbghelper.dll.SHDeleteKeyW")
#pragma comment(linker, "/export:SHDeleteValueA=dbghelper.dll.SHDeleteValueA")
#pragma comment(linker, "/export:SHDeleteValueW=dbghelper.dll.SHDeleteValueW")
#pragma comment(linker, "/export:SHEnumKeyExA=dbghelper.dll.SHEnumKeyExA")
#pragma comment(linker, "/export:SHEnumKeyExW=dbghelper.dll.SHEnumKeyExW")
#pragma comment(linker, "/export:SHEnumValueA=dbghelper.dll.SHEnumValueA")
#pragma comment(linker, "/export:SHEnumValueW=dbghelper.dll.SHEnumValueW")
#pragma comment(linker, "/export:SHGetThreadRef=dbghelper.dll.SHGetThreadRef")
#pragma comment(linker, "/export:SHGetValueA=dbghelper.dll.SHGetValueA")
#pragma comment(linker, "/export:SHGetValueW=dbghelper.dll.SHGetValueW")
#pragma comment(linker, "/export:SHOpenRegStream2A=dbghelper.dll.SHOpenRegStream2A")
#pragma comment(linker, "/export:SHOpenRegStream2W=dbghelper.dll.SHOpenRegStream2W")
#pragma comment(linker, "/export:SHOpenRegStreamA=dbghelper.dll.SHOpenRegStreamA")
#pragma comment(linker, "/export:SHOpenRegStreamW=dbghelper.dll.SHOpenRegStreamW")
#pragma comment(linker, "/export:SHQueryInfoKeyA=dbghelper.dll.SHQueryInfoKeyA")
#pragma comment(linker, "/export:SHQueryInfoKeyW=dbghelper.dll.SHQueryInfoKeyW")
#pragma comment(linker, "/export:SHQueryValueExA=dbghelper.dll.SHQueryValueExA")
#pragma comment(linker, "/export:SHQueryValueExW=dbghelper.dll.SHQueryValueExW")
#pragma comment(linker, "/export:SHRegDuplicateHKey=dbghelper.dll.SHRegDuplicateHKey")
#pragma comment(linker, "/export:SHRegGetIntW=dbghelper.dll.SHRegGetIntW")
#pragma comment(linker, "/export:SHRegGetPathA=dbghelper.dll.SHRegGetPathA")
#pragma comment(linker, "/export:SHRegGetPathW=dbghelper.dll.SHRegGetPathW")
#pragma comment(linker, "/export:SHRegGetValueA=dbghelper.dll.SHRegGetValueA")
#pragma comment(linker, "/export:SHRegGetValueW=dbghelper.dll.SHRegGetValueW")
#pragma comment(linker, "/export:SHRegSetPathA=dbghelper.dll.SHRegSetPathA")
#pragma comment(linker, "/export:SHRegSetPathW=dbghelper.dll.SHRegSetPathW")
#pragma comment(linker, "/export:SHReleaseThreadRef=dbghelper.dll.SHReleaseThreadRef")
#pragma comment(linker, "/export:SHSetThreadRef=dbghelper.dll.SHSetThreadRef")
#pragma comment(linker, "/export:SHSetValueA=dbghelper.dll.SHSetValueA")
#pragma comment(linker, "/export:SHSetValueW=dbghelper.dll.SHSetValueW")
#pragma comment(linker, "/export:SHStrDupA=dbghelper.dll.SHStrDupA")
#pragma comment(linker, "/export:SHStrDupW=dbghelper.dll.SHStrDupW")
#pragma comment(linker, "/export:SHTaskPoolAllowThreadReuse=dbghelper.dll.SHTaskPoolAllowThreadReuse")
#pragma comment(linker, "/export:SHTaskPoolDoNotWaitForMoreTasks=dbghelper.dll.SHTaskPoolDoNotWaitForMoreTasks")
#pragma comment(linker, "/export:SHTaskPoolGetCurrentThreadLifetime=dbghelper.dll.SHTaskPoolGetCurrentThreadLifetime")
#pragma comment(linker, "/export:SHTaskPoolGetUniqueContext=dbghelper.dll.SHTaskPoolGetUniqueContext")
#pragma comment(linker, "/export:SHTaskPoolQueueTask=dbghelper.dll.SHTaskPoolQueueTask")
#pragma comment(linker, "/export:SHTaskPoolSetThreadReuseAllowed=dbghelper.dll.SHTaskPoolSetThreadReuseAllowed")
#pragma comment(linker, "/export:SHUnicodeToAnsi=dbghelper.dll.SHUnicodeToAnsi")
#pragma comment(linker, "/export:SHUnicodeToUnicode=dbghelper.dll.SHUnicodeToUnicode")
#pragma comment(linker, "/export:SetCurrentProcessExplicitAppUserModelID=dbghelper.dll.SetCurrentProcessExplicitAppUserModelID")
#pragma comment(linker, "/export:SetProcessDpiAwareness=dbghelper.dll.SetProcessDpiAwareness")
#pragma comment(linker, "/export:SetProcessReference=dbghelper.dll.SetProcessReference")
#pragma comment(linker, "/export:SubscribeFeatureStateChangeNotification=dbghelper.dll.SubscribeFeatureStateChangeNotification")
#pragma comment(linker, "/export:UnregisterScaleChangeEvent=dbghelper.dll.UnregisterScaleChangeEvent")
#pragma comment(linker, "/export:UnsubscribeFeatureStateChangeNotification=dbghelper.dll.UnsubscribeFeatureStateChangeNotification")
#pragma comment(linker, "/export:SHRegGetValueFromHKCUHKLM=dbghelper.dll.SHRegGetValueFromHKCUHKLM")


void RunPayload()
{
    std::wstringstream ss;

    ss << L"Hello From Dll ";
    MessageBox(nullptr, ss.str().c_str(), L"Hello", MB_OK | MB_ICONINFORMATION);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        RunPayload();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

