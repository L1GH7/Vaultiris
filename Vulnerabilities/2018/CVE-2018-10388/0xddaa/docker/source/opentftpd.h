/**************************************************************************
*   Copyright (C) 2005 by Achal Dhir                                      *
*   achaldhir@gmail.com                                                   *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

// tftpserver.h

// tftpserver.h
#ifndef LOG_MAKEPRI
#define	LOG_MAKEPRI(fac, pri)	(((fac) << 3) | (pri))
#endif

#ifndef SIOCGIFCONF
#include <sys/sockio.h>
#endif

#ifndef INADDR_NONE
#define INADDR_NONE ULONG_MAX
#endif

#ifndef IFF_DYNAMIC
#define IFF_DYNAMIC 0x8000
#endif

#ifndef INADDR_NONE
#define INADDR_NONE UINT_MAX
#endif

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

//Constants
#define MYBYTE unsigned char
#define MYWORD unsigned short
#define MYDWORD unsigned int
#define MYLWORD unsigned long
#define SOCKET int
#define MAX_SERVERS 8

//Structs
struct home
{
	char alias[64];
	char target[256];
};

struct tftpConnType
{
	SOCKET sock;
	sockaddr_in addr;
	MYDWORD server;
	MYWORD port;
	bool ready;
	bool loaded;
};

struct acknowledgement
{
	MYWORD opcode;
	MYWORD block;
};

struct message
{
	MYWORD opcode;
	char buffer[514];
};

struct packet
{
	MYWORD opcode;
	MYWORD block;
	char buffer;
};

struct data12
{
	MYDWORD rangeStart;
	MYDWORD rangeEnd;
};

struct tftperror
{
	MYWORD opcode;
	MYWORD errorcode;
	char errormessage[508];
};

struct request
{
	char mapname[32];
	MYBYTE opcode;
	MYBYTE attempt;
	MYBYTE sockInd;
	time_t expiry;
	char path[256];
	FILE *file;
	char *filename;
	char *mode;
	MYDWORD tsize;
	MYDWORD blksize;
	MYDWORD timeout;
	MYDWORD fblock;
	MYWORD block;
	MYWORD tblock;
	int bytesRecd;
	MYWORD bytesRead[2];
	int bytesReady;
	sockaddr_in client;
	socklen_t clientsize;
	packet* pkt[2];
	union
	{
		acknowledgement acout;
		message mesout;
		tftperror serverError;
	};
};

struct data1
{
	tftpConnType tftpConn[MAX_SERVERS];
	MYDWORD allServers[MAX_SERVERS];
	MYDWORD staticServers[MAX_SERVERS];
	MYDWORD listenServers[MAX_SERVERS];
	MYWORD listenPorts[MAX_SERVERS];
	bool ready;
	bool busy;
	SOCKET maxFD;
};

struct data2
{
	data12 hostRanges[32];
	home homes[8];
	FILE *logfile;
	char username[128];
	uid_t pw_uid;
	uid_t pw_gid;
	char fileRead;
	char fileWrite;
	char fileOverwrite;
	pid_t pppid;
	MYBYTE logLevel;
	MYDWORD failureCount;
	MYDWORD failureCycle;
    struct ifreq IfcBuf[MAX_SERVERS];
	MYBYTE ifc_len;
	SOCKET fixedSocket;
	MYDWORD oldservers[MAX_SERVERS];
	bool ifspecified;
};

struct data15
{
	union
	{
		//MYDWORD ip;
		unsigned ip:32;
		MYBYTE octate[4];
	};
};

//types
typedef map<string, request*> myMap;
typedef multimap<long, request*> myMultiMap;

//Functions
bool getSection(char*, char*, MYBYTE, char*);
bool detectChange();
bool addServer(MYDWORD*, MYDWORD);
char* IP2String(char*, MYDWORD);
char* myGetToken(char*, MYBYTE);
char* myLower(char* string);
char* myTokenize(char*, char*);
char* myTrim(char*, char*);
char* myUpper(char* string);
char* readSection(char*, FILE*);
MYDWORD my_inet_addr(char*);
MYDWORD* findServer(MYDWORD*, MYDWORD);
extern void getServ(MYDWORD*, MYWORD*, const MYBYTE);
FILE* openSection(const char*, MYBYTE, char*);
int processNew(request*);
int processRecv(request*);
int processSend(request*);
void catch_int(int sig_num);
void cleanReq(request*);
void closeConn();
void getInterfaces(data1*);
void *init(void*);
void logMess(char*, MYBYTE);
void logMess(request*, MYBYTE);
void mySplit(char*, char*, char*, char);
void runProg();
