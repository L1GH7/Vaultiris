// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <windows.h>



DECLARE_HANDLE(HSURF);
DECLARE_HANDLE(HDEV);
DECLARE_HANDLE(DHSURF);
DECLARE_HANDLE(DHPDEV);

typedef ULONG   ROP4;

struct _GDI_CELL
{
    PINT pKernelAddress;
    USHORT wProcessId;
    USHORT wCount;
    USHORT wUpper;
    USHORT wType;
    PINT pUserAddress;
};


typedef struct _BRUSHOBJ
{
    ULONG  iSolidColor;
    PVOID  pvRbrush;
    FLONG  flColorType;
} BRUSHOBJ;

typedef struct _XLATEOBJ
{
    ULONG   iUniq;
    FLONG   flXlate;
    USHORT  iSrcType;               // Obsolete
    USHORT  iDstType;               // Obsolete
    ULONG   cEntries;
    ULONG  *pulXlate;
} XLATEOBJ;

typedef struct _CLIPOBJ
{
    ULONG   iUniq;
    RECTL   rclBounds;
    BYTE    iDComplexity;
    BYTE    iFComplexity;
    BYTE    iMode;
    BYTE    fjOptions;
} CLIPOBJ;


typedef struct _SURFOBJ
{
    DHSURF  dhsurf;
    HSURF   hsurf;
    DHPDEV  dhpdev;
    HDEV    hdev;
    SIZEL   sizlBitmap;
    ULONG   cjBits;
    _Field_size_bytes_(cjBits) PVOID   pvBits;
    PVOID   pvScan0;
    LONG    lDelta;
    ULONG   iUniq;
    ULONG   iBitmapFormat;
    USHORT  iType;
    USHORT  fjBitmap;
} SURFOBJ;




typedef struct _RTL_PROCESS_MODULE_INFORMATION
{
    HANDLE Section;
    PVOID MappedBase;
    PVOID ImageBase;
    ULONG ImageSize;
    ULONG Flags;
    USHORT LoadOrderIndex;
    USHORT InitOrderIndex;
    USHORT LoadCount;
    USHORT OffsetToFileName;
    UCHAR FullPathName[256];
} RTL_PROCESS_MODULE_INFORMATION, *PRTL_PROCESS_MODULE_INFORMATION;

typedef struct _RTL_PROCESS_MODULES
{
    ULONG NumberOfModules;
    RTL_PROCESS_MODULE_INFORMATION Modules[1];
} RTL_PROCESS_MODULES, *PRTL_PROCESS_MODULES;

typedef
struct  {
    DWORD dwMajorVersion;
    DWORD dwMinorVersion;
    DWORD wServicePackMajor;
    DWORD wServicePackMinor;
    DWORD dw5;
    DWORD dw6;
    DWORD dw7;
    DWORD dw8;
    DWORD dw9;
}VERSION28h, *PVERSION28h;

typedef struct _UNICODE_STRING
{
    USHORT Length;
    USHORT MaximumLength;
    _Field_size_bytes_part_(MaximumLength, Length) PWCH Buffer;
} UNICODE_STRING, *PUNICODE_STRING;


typedef
BOOL
(APIENTRY *NTGDIENGASSOCIATESURFACE)(
    _In_ HSURF     hsurf,
    _In_ HDEV     hdev,
    _In_ ULONG64    flHooks,
    _In_ ULONG64     Unknown1
    );

typedef NTSTATUS
(NTAPI
    * NTQUERYSYSTEMINFOMATION)
    (
        _In_ ULONG64 SystemInformationClass,
        _Out_writes_bytes_to_opt_(SystemInformationLength, *ReturnLength) PVOID SystemInformation,
        _In_ ULONG Length,
        _Out_opt_ PULONG64 UnsafeResultLength);

typedef
HANDLE
(NTAPI
    *PSREFERENCEPRIMARYTOKEN)(
        _Inout_ PULONG64 Process);

typedef
PULONG64
(NTAPI *IOTHREADTOPROCESS)(
        _In_ PULONG64 Thread);

typedef
BOOL
(APIENTRY *NTGDIENGDELETESURFACE)(
    _In_ HSURF hsurf);

typedef
NTSTATUS
(NTAPI
    *NTALLOCATEVIRUALMEMORY)(
        _In_ HANDLE ProcessHandle,
        _Inout_ _At_(*BaseAddress, _Readable_bytes_(*RegionSize) _Writable_bytes_(*RegionSize) _Post_readable_byte_size_(*RegionSize)) PVOID *BaseAddress,
        _In_ ULONG_PTR ZeroBits,
        _Inout_ PSIZE_T RegionSize,
        _In_ ULONG AllocationType,
        _In_ ULONG Protect
        );


typedef
VOID
(*RTLINITUNICODESTRING)(
    _Out_ PUNICODE_STRING DestinationString,
    _In_opt_ PWSTR SourceString
    );

typedef
NTSTATUS
(NTAPI
    *NTFREEVIRTUALMEMORY)(
        _In_ HANDLE ProcessHandle,
        _Inout_ PVOID *BaseAddress,
        _Inout_ PSIZE_T RegionSize,
        _In_ ULONG FreeType
        );

typedef
NTSTATUS
(NTAPI
*NTCALLBACKRETURN)(
    _In_reads_bytes_opt_(OutputLength) PVOID OutputBuffer,
    _In_ ULONG OutputLength,
    _In_ NTSTATUS Status
);
typedef
INT
(APIENTRY     
*NTGDISTARTDOC)(
    IN HDC hdc, 
    IN DOCINFOW *pdi, 
    OUT BOOL *pbBanding, 
    IN INT iJob);

typedef 
HDC
(APIENTRY
*NTGDIOPENDCW)(
    _In_opt_ PUNICODE_STRING pustrDevice,
    _In_ DEVMODEW *pdm,
    _In_ PUNICODE_STRING pustrLogAddr,
    _In_ ULONG iType,
    _In_ BOOL bDisplay,
    _In_opt_ HANDLE hspool,
    _At_((PUMDHPDEV*)pUMdhpdev, _Out_) PVOID pUMdhpdev,
    ULONG64 unknown1);

typedef
BOOL
(APIENTRY *NTGDIBITBLT)(
    HDC     hDCDest,
    INT     XDest,
    INT     YDest,
    INT     Width,
    INT     Height,
    HDC     hDCSrc,
    INT     XSrc,
    INT     YSrc,
    DWORD     dwRop,
    IN DWORD     crBackColor,
    IN FLONG     fl
    );

typedef
BOOL
(APIENTRY *NTGDIENDDOC)(
    HDC hDC);

typedef
SURFOBJ*
(APIENTRY *NTGDIENGLOCKSURFACE)(
    _In_ HSURF hsurf);

typedef
SURFOBJ*
(APIENTRY *NTGDIENGUNLOCKSURFACE)(
    _In_ SURFOBJ* pSurfObj);





typedef
BOOL
(APIENTRY *NTGDIENGBITBLT)(
    _In_ SURFOBJ *psoTrg,
    _In_opt_ SURFOBJ *psoSrc,
    _In_opt_ SURFOBJ *psoMask,
    _In_opt_ CLIPOBJ *pco,
    _In_opt_ XLATEOBJ *pxlo,
    _In_ RECTL *prclTrg,
    _In_opt_ POINTL *pptlSrc,
    _In_opt_ POINTL *pptlMask,
    _In_opt_ BRUSHOBJ *pbo,
    _In_opt_ POINTL *pptlBrush,
    _In_ ROP4 rop4);

typedef
HBITMAP
(APIENTRY *NTGDIENGCREATEBITMAP)(
    _In_ SIZEL sizl,
    _In_ LONG lWidth,
    _In_ ULONG iFormat,
    _In_ FLONG fl,
    _In_opt_ PVOID pvBits);

typedef
PVOID
(APIENTRY *NTGDIBRUSHOBJ_PVALLOCRBRUSH)(
    _In_ BRUSHOBJ * pbo,
    _In_ ULONG  cj
    );

typedef
HSURF
(APIENTRY*NTGDIENGCREATEDEVICESURFACE)(
        IN DHSURF dhsurf,
        IN SIZEL sizl,
        IN ULONG iFormatCompat);

typedef 
BOOL    
(APIENTRY *NTGDIDELETEOBJECTAPP)(
    HANDLE hobj);

typedef
ULONG64 
(APIENTRY*NTGDIDRAWSTREAM)(
    HDC hDC, 
    ULONG64 ulBufLen, 
    ULONG64 ulBuf);

DWORD GetAndSetSharedGdiObj(HDC hDC);



extern "C" ULONG64 GetKernelCallbackTable(DWORD dwIndex);
extern "C" ULONG64 GetPEB();

ULONG64 sub_140001A00(HUMPD hUMPD, ULONG64 *pOutputBuf, DWORD *pdwOutputBufSize);
ULONG64 sub_140001AE0(HUMPD hUMPD, ULONG64 *pOutputBuf, DWORD *pdwOutputBufSize);
ULONG64 sub_140001C70(HUMPD hUMPD, ULONG64 *pOutputBuf, DWORD *pdwOutputBufSize);
ULONG64 fake_ClientPrinterThunk(HUMPD hUMPD);
NTSTATUS fakeGdiPrinterThunk(HUMPD hUMPD, ULONG64 *pOutputBuf, DWORD *pdwOutputBufSize);
PVOID GetSysModuleBase_Name(PCHAR pszBuf);
signed __int64 GetSysVersion();
__int64 DrawStream();
ULONG64 sub_140001B90(ULONG64 buffer, ULONG64 brush);
ULONG64 selfGetProcAddr(HMODULE hMODULE, unsigned __int64 a2);
signed __int64 GetGdiFunc();
int LoadKrnlAndInitpfn();