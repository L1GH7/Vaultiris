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

// tftpserver.cpp
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <memory.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <pwd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <map>
using namespace std;
#include "opentftpd.h"

//Global Variables
bool kRunning = true;
myMap tftpCache;
myMultiMap tftpAge;
bool verbatim = false;
char iniFile[256]="opentftp.ini";
char logFile[256]="";
char fileSep = '/';
char notFileSep = '\\';
MYWORD blksize = 65464;
MYWORD timeout = 3;
data1 network;
data1 newNetwork;
data2 cfig;
struct ifconf Ifc;
struct ifreq IfcBuf[MAX_SERVERS];
char tempbuff[256];
char extbuff[256];
char logBuff[512];
MYWORD loggingDay;
char sVersion[] = "TFTP Server SinglePort Version 1.66 Unix Built 2003";
packet* datain;
int port = 0;

int main(int argc, char **argv)
{
	system("echo 'usage: ./opentftpd port'");

	signal(SIGINT, catch_int);
	signal(SIGABRT, catch_int);
	signal(SIGTERM, catch_int);
	signal(SIGQUIT, catch_int);
	signal(SIGTSTP, catch_int);
	signal(SIGHUP, catch_int);
	signal(SIGALRM, catch_int);

    alarm(60);
    port = atoi(argv[1]);

    logBuff[0] = 0;

	if (!iniFile[0])
		strcpy(iniFile,"/etc/opentftp.ini");

	if (verbatim)
	{
		if (logBuff[0])
		{
			printf("%s\n", logBuff);
			exit(EXIT_FAILURE);
		}

		pthread_t threadId;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
		int errcode = pthread_create(&threadId, &attr, init, NULL);
		pthread_attr_destroy(&attr);

		if(errcode)
		{
			sprintf(logBuff, "Error: Main Thread Creation Failed with error %s", strerror(errcode));
			logMess(logBuff, 1);
			exit(EXIT_FAILURE);
		}

		timeval tv;
		fd_set readfds;
		request req;
		datain = (packet*)calloc(1, blksize + 4);
		int fdsReady = 0;

		if (!datain)
		{
			sprintf(logBuff,"Memory Error");
			logMess(logBuff, 0);
			exit(EXIT_FAILURE);
		}

		while (kRunning && !network.ready)
			sleep(1);

		setuid(cfig.pw_uid);
		setgid(cfig.pw_gid);

		//printf("user set to %s\n", cfig.username);

		do
		{
			network.busy = false;

			if (!network.ready)
			{
				sleep(1);
				continue;
			}

			FD_ZERO(&readfds);

			tv.tv_sec = 1;
			tv.tv_usec = 0;

			for (int i = 0; i < MAX_SERVERS && network.tftpConn[i].ready; i++)
				FD_SET(network.tftpConn[i].sock, &readfds);

			fdsReady = select(network.maxFD, &readfds, NULL, NULL, &tv);

			//if (errno)
			//	printf("%s\n", strerror(errno));

			for (int i = 0; fdsReady > 0 && i < MAX_SERVERS && network.tftpConn[i].ready; i++)
			{
				if (network.ready)
				{
					network.busy = true;

					if (FD_ISSET(network.tftpConn[i].sock, &readfds))
					{
						fdsReady--;
						memset(&req, 0, sizeof(request));
						memset(datain, 0, blksize + 4);
						req.clientsize = sizeof(req.client);
						req.sockInd = i;
						errno = 0;
						req.bytesRecd = recvfrom(network.tftpConn[req.sockInd].sock, (char*)datain, blksize + 4, 0, (sockaddr*)&req.client, &req.clientsize);

						if (req.bytesRecd < 4 || errno)
							continue;

						//printf("%u=%u\n", req.bytesRecd, blksize + 4);
						sprintf(req.mapname, "%s:%u", inet_ntoa(req.client.sin_addr), ntohs(req.client.sin_port));
						request *req1 = tftpCache[req.mapname];

						if (!req1)
							tftpCache.erase(req.mapname);

						//printf("%u\n",req1);

						if (req1)
						{
							req1->bytesRecd = req.bytesRecd;

							if (ntohs(datain->opcode) == 3 && req1->opcode == 2)
							{
								if ((MYWORD)req1->bytesRecd  <= req1->blksize + 4)
								{
									if (req1->attempt <= 3)
									{
										req1->tblock = req1->block + 1;

										if (ntohs(datain->block) == req1->tblock)
										{
											req1->block = req1->tblock;
											req1->fblock++;
											req1->attempt = 0;
											req1->acout.opcode = htons(4);
											req1->acout.block = ntohs(req1->block);
											processRecv(req1);
										}
									}
								}
								else
								{
									req1->serverError.opcode = htons(5);
									req1->serverError.errorcode = htons(4);
									sendto(network.tftpConn[req1->sockInd].sock, (const char*)&req1->serverError, strlen(req1->serverError.errormessage) + 5, 0, (sockaddr*)&req1->client, req1->clientsize);
									sprintf(req1->serverError.errormessage, "Error: Incoming Packet too large");
									logMess(req1, 1);
									cleanReq(req1);
								}
							}
							else if (ntohs(datain->opcode) == 4 && req1->opcode == 1)
							{
								if (req1->bytesRecd >= 4)
								{
									if (req1->attempt <= 3)
									{
										if (ntohs(datain->block) == req1->block)
										{
											req1->block++;
											req1->fblock++;
											req1->attempt = 0;
											processSend(req1);
										}
									}
								}
							}
							else if (req1->bytesRecd > (int)sizeof(message))
							{
								req1->serverError.opcode = htons(5);
								req1->serverError.errorcode = htons(4);
								sendto(network.tftpConn[req1->sockInd].sock, (const char*)&req1->serverError, strlen(req1->serverError.errormessage) + 5, 0, (sockaddr*)&req1->client, req1->clientsize);
								sprintf(req1->serverError.errormessage, "Error: Incoming Packet too large");
								logMess(req1, 1);
								cleanReq(req1);
							}
							else if (ntohs(datain->opcode) == 1 || ntohs(datain->opcode) == 2)
							{
								cleanReq(req1);
								if (!processNew(&req))
								{
									memcpy(req1, &req, sizeof(request));
								}
							}
							else if (ntohs(datain->opcode) == 5)
							{
								sprintf(req1->serverError.errormessage, "Error %i at Client, %s", ntohs(datain->block), &datain->buffer);
								logMess(req1, 1);
								cleanReq(req1);
							}
							else
							{
								req1->serverError.opcode = htons(5);
								req1->serverError.errorcode = htons(4);
								sprintf(req1->serverError.errormessage, "Unexpected Option Code %u", ntohs(datain->opcode));
								sendto(network.tftpConn[req1->sockInd].sock, (const char*)&req1->serverError, strlen(req1->serverError.errormessage) + 5, 0, (sockaddr*)&req1->client, req1->clientsize);
								logMess(req1, 1);
								cleanReq(req1);
							}
						}
						else if (req.bytesRecd > (int)sizeof(message))
						{
							req.serverError.opcode = htons(5);
							req.serverError.errorcode = htons(4);
							sprintf(req.serverError.errormessage, "Error: Incoming Packet too large");
							sendto(network.tftpConn[i].sock, (const char*)&req.serverError, strlen(req.serverError.errormessage) + 5, 0, (sockaddr*)&req.client, req.clientsize);
							logMess(&req, 1);
						}
						else if (ntohs(datain->opcode) == 5)
						{
							sprintf(req.serverError.errormessage, "Error %i at Client, %s", ntohs(datain->block), &datain->buffer);
							logMess(&req, 1);
						}
						else if (ntohs(datain->opcode) != 1 && ntohs(datain->opcode) != 2)
						{
							req.serverError.opcode = htons(5);
							req.serverError.errorcode = htons(5);
							sprintf(req.serverError.errormessage, "Unknown transfer ID");
							sendto(network.tftpConn[i].sock, (const char*)&req.serverError, strlen(req.serverError.errormessage) + 5, 0, (sockaddr*)&req.client, req.clientsize);
							logMess(&req, 1);
						}
						else if (!processNew(&req))
						{
							request *req1 = (request*)calloc(1, sizeof(request));

							if (!req1)
							{
								sprintf(logBuff,"Memory Error");
								logMess(logBuff, 1);
								continue;
							}

							memcpy(req1, &req, sizeof(request));
							tftpCache[req1->mapname] = req1;
							tftpAge.insert(pair<long, request*>(req1->expiry, req1));
						}
					}
				}
			}

			myMultiMap::iterator p = tftpAge.begin();
			myMultiMap::iterator q;
			time_t currentTime = time(NULL);
			request *req;

			while (p != tftpAge.end())
			{
				req = p->second;

				if (p->first > currentTime)
					break;
				else if (p->first < req->expiry && req->expiry > currentTime)
				{
					q = p;
					p++;
					tftpAge.erase(q);
					tftpAge.insert(pair<long, request*>(req->expiry, req));
				}
				else if (req->expiry <= currentTime && req->attempt >= 3)
				{
					if (req->attempt < UCHAR_MAX)
					{
						req->serverError.opcode = htons(5);
						req->serverError.errorcode = htons(0);

						if (req->fblock && !req->block)
							strcpy(req->serverError.errormessage, "Large File, Block# Rollover not supported by Client");
						else
							strcpy(req->serverError.errormessage, "Timeout");

						sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->serverError, strlen(req->serverError.errormessage) + 5, 0, (sockaddr*)&req->client, req->clientsize);
						logMess(req, 1);
					}

					q = p;
					p++;
					tftpAge.erase(q);
					tftpCache.erase(req->mapname);
					cleanReq(req);
					free(req);
				}
				else if (req->expiry <= currentTime)
				{
					if (ntohs(req->acout.opcode) == 3)
					{
						if (processSend(req))
							cleanReq(req);
						else
						{
							req->attempt++;
							req->expiry = currentTime + req->timeout;
						}
					}
					else
					{
						errno = 0;
						sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->acout, req->bytesReady, 0, (sockaddr*)&req->client, req->clientsize);
						//errno = WSAGetLastError();

						if (errno)
							cleanReq(req);
						else
						{
							req->attempt++;
							req->expiry = currentTime + req->timeout;
						}
					}
					p++;
				}
				else
					p++;
			}
		}
		while (kRunning);

		sprintf(logBuff, "closing Network Connections...");
		logMess(logBuff, 1);
		closeConn();
		close(cfig.fixedSocket);
		sprintf(logBuff, "TFTP Server Stopped !");
		logMess(logBuff, 1);

		if (cfig.logfile)
			fclose(cfig.logfile);

		exit(EXIT_SUCCESS);
	}
	else
	{
		if(logBuff[0])
		{
			syslog(LOG_MAKEPRI(LOG_LOCAL1, LOG_CRIT), "%s", logBuff);
			exit(EXIT_FAILURE);
		}

		/* Close out the standard file descriptors */
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);

		/* Daemon-specific initialization goes here */
		//Initialize
		pthread_t threadId;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
		int errcode = pthread_create(&threadId, &attr, init, NULL);
		pthread_attr_destroy(&attr);

		if(errcode)
		{
			sprintf(logBuff, "Error: Main Thread Creation Failed with error %s", strerror(errcode));
			logMess(logBuff, 1);
			exit(EXIT_FAILURE);
		}

		timeval tv;
		fd_set readfds;
		request req;
		datain = (packet*)calloc(1, blksize + 4);
		int fdsReady = 0;

		if (!datain)
		{
			sprintf(logBuff,"Memory Error");
			logMess(logBuff, 0);
			exit(EXIT_FAILURE);
		}

		while (kRunning && !network.ready)
			sleep(1);

		setuid(cfig.pw_uid);
		setgid(cfig.pw_gid);

		//printf("user set to %s\n", cfig.username);

		do
		{
			network.busy = false;

			if (!network.ready)
			{
				sleep(1);
				continue;
			}

			FD_ZERO(&readfds);

			tv.tv_sec = 1;
			tv.tv_usec = 0;

			for (int i = 0; i < MAX_SERVERS && network.tftpConn[i].ready; i++)
				FD_SET(network.tftpConn[i].sock, &readfds);

			fdsReady = select(network.maxFD, &readfds, NULL, NULL, &tv);

			//if (errno)
			//	printf("%s\n", strerror(errno));

			for (int i = 0; fdsReady > 0 && i < MAX_SERVERS && network.tftpConn[i].ready; i++)
			{
				if (network.ready)
				{
					network.busy = true;

					if (FD_ISSET(network.tftpConn[i].sock, &readfds))
					{
						fdsReady--;
						memset(&req, 0, sizeof(request));
						memset(datain, 0, blksize + 4);
						req.clientsize = sizeof(req.client);
						req.sockInd = i;
						errno = 0;
						req.bytesRecd = recvfrom(network.tftpConn[req.sockInd].sock, (char*)datain, blksize + 4, 0, (sockaddr*)&req.client, &req.clientsize);

						if (req.bytesRecd < 4 || errno)
							continue;

						//printf("%u=%u\n", req.bytesRecd, blksize + 4);
						sprintf(req.mapname, "%s:%u", inet_ntoa(req.client.sin_addr), ntohs(req.client.sin_port));
						request *req1 = tftpCache[req.mapname];

						if (!req1)
							tftpCache.erase(req.mapname);

						//printf("%u\n",req1);

						if (req1)
						{
							req1->bytesRecd = req.bytesRecd;

							if (ntohs(datain->opcode) == 3 && req1->opcode == 2)
							{
								if ((MYWORD)req1->bytesRecd  <= req1->blksize + 4)
								{
									if (req1->attempt <= 3)
									{
										req1->tblock = req1->block + 1;

										if (ntohs(datain->block) == req1->tblock)
										{
											req1->block = req1->tblock;
											req1->fblock++;
											req1->attempt = 0;
											req1->acout.opcode = htons(4);
											req1->acout.block = ntohs(req1->block);
											processRecv(req1);
										}
									}
								}
								else
								{
									req1->serverError.opcode = htons(5);
									req1->serverError.errorcode = htons(4);
									sendto(network.tftpConn[req1->sockInd].sock, (const char*)&req1->serverError, strlen(req1->serverError.errormessage) + 5, 0, (sockaddr*)&req1->client, req1->clientsize);
									sprintf(req1->serverError.errormessage, "Error: Incoming Packet too large");
									logMess(req1, 1);
									cleanReq(req1);
								}
							}
							else if (ntohs(datain->opcode) == 4 && req1->opcode == 1)
							{
								if (req1->bytesRecd >= 4)
								{
									if (req1->attempt <= 3)
									{
										if (ntohs(datain->block) == req1->block)
										{
											req1->block++;
											req1->fblock++;
											req1->attempt = 0;
											processSend(req1);
										}
									}
								}
							}
							else if (req1->bytesRecd > (int)sizeof(message))
							{
								req1->serverError.opcode = htons(5);
								req1->serverError.errorcode = htons(4);
								sendto(network.tftpConn[req1->sockInd].sock, (const char*)&req1->serverError, strlen(req1->serverError.errormessage) + 5, 0, (sockaddr*)&req1->client, req1->clientsize);
								sprintf(req1->serverError.errormessage, "Error: Incoming Packet too large");
								logMess(req1, 1);
								cleanReq(req1);
							}
							else if (ntohs(datain->opcode) == 1 || ntohs(datain->opcode) == 2)
							{
								cleanReq(req1);
								if (!processNew(&req))
								{
									memcpy(req1, &req, sizeof(request));
								}
							}
							else if (ntohs(datain->opcode) == 5)
							{
								sprintf(req1->serverError.errormessage, "Error %i at Client, %s", ntohs(datain->block), &datain->buffer);
								logMess(req1, 1);
								cleanReq(req1);
							}
							else
							{
								req1->serverError.opcode = htons(5);
								req1->serverError.errorcode = htons(4);
								sprintf(req1->serverError.errormessage, "Unexpected Option Code %u", ntohs(datain->opcode));
								sendto(network.tftpConn[req1->sockInd].sock, (const char*)&req1->serverError, strlen(req1->serverError.errormessage) + 5, 0, (sockaddr*)&req1->client, req1->clientsize);
								logMess(req1, 1);
								cleanReq(req1);
							}
						}
						else if (req.bytesRecd > (int)sizeof(message))
						{
							req.serverError.opcode = htons(5);
							req.serverError.errorcode = htons(4);
							sprintf(req.serverError.errormessage, "Error: Incoming Packet too large");
							sendto(network.tftpConn[i].sock, (const char*)&req.serverError, strlen(req.serverError.errormessage) + 5, 0, (sockaddr*)&req.client, req.clientsize);
							logMess(&req, 1);
						}
						else if (ntohs(datain->opcode) == 5)
						{
							sprintf(req.serverError.errormessage, "Error %i at Client, %s", ntohs(datain->block), &datain->buffer);
							logMess(&req, 1);
						}
						else if (ntohs(datain->opcode) != 1 && ntohs(datain->opcode) != 2)
						{
							req.serverError.opcode = htons(5);
							req.serverError.errorcode = htons(5);
							sprintf(req.serverError.errormessage, "Unknown transfer ID");
							sendto(network.tftpConn[i].sock, (const char*)&req.serverError, strlen(req.serverError.errormessage) + 5, 0, (sockaddr*)&req.client, req.clientsize);
							logMess(&req, 1);
						}
						else if (!processNew(&req))
						{
							request *req1 = (request*)calloc(1, sizeof(request));

							if (!req1)
							{
								sprintf(logBuff,"Memory Error");
								logMess(logBuff, 1);
								continue;
							}

							memcpy(req1, &req, sizeof(request));
							tftpCache[req1->mapname] = req1;
							tftpAge.insert(pair<long, request*>(req1->expiry, req1));
						}
					}
				}
			}

			myMultiMap::iterator p = tftpAge.begin();
			myMultiMap::iterator q;
			time_t currentTime = time(NULL);
			request *req;

			while (p != tftpAge.end())
			{
				req = p->second;

				if (p->first > currentTime)
					break;
				else if (p->first < req->expiry && req->expiry > currentTime)
				{
					q = p;
					p++;
					tftpAge.erase(q);
					tftpAge.insert(pair<long, request*>(req->expiry, req));
				}
				else if (req->expiry <= currentTime && req->attempt >= 3)
				{
					if (req->attempt < UCHAR_MAX)
					{
						req->serverError.opcode = htons(5);
						req->serverError.errorcode = htons(0);

						if (req->fblock && !req->block)
							strcpy(req->serverError.errormessage, "Large File, Block# Rollover not supported by Client");
						else
							strcpy(req->serverError.errormessage, "Timeout");

						sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->serverError, strlen(req->serverError.errormessage) + 5, 0, (sockaddr*)&req->client, req->clientsize);
						logMess(req, 1);
					}

					q = p;
					p++;
					tftpAge.erase(q);
					tftpCache.erase(req->mapname);
					cleanReq(req);
					free(req);
				}
				else if (req->expiry <= currentTime)
				{
					if (ntohs(req->acout.opcode) == 3)
					{
						if (processSend(req))
							cleanReq(req);
						else
						{
							req->attempt++;
							req->expiry = currentTime + req->timeout;
						}
					}
					else
					{
						errno = 0;
						sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->acout, req->bytesReady, 0, (sockaddr*)&req->client, req->clientsize);
						//errno = WSAGetLastError();

						if (errno)
							cleanReq(req);
						else
						{
							req->attempt++;
							req->expiry = currentTime + req->timeout;
						}
					}
					p++;
				}
				else
					p++;
			}
		}
		while (kRunning);

		sprintf(logBuff, "closing Network Connections...");
		logMess(logBuff, 1);
		closeConn();
		close(cfig.fixedSocket);
		sprintf(logBuff, "TFTP Server Stopped !");
		logMess(logBuff, 1);

		if (cfig.logfile)
			fclose(cfig.logfile);

		exit(EXIT_SUCCESS);
	}
}

void closeConn()
{
	for (int i = 0; i < MAX_SERVERS && network.tftpConn[i].loaded; i++)
	{
		if (network.tftpConn[i].ready)
		{
			close(network.tftpConn[i].sock);
			//printf("%d, %s clsoed\n", i, IP2String(tempbuff, network.tftpConn[i].server));
		}
	}
}

void catch_int(int sig_num)
{
	if (cfig.pppid == getpid())
	{
		kRunning = false;
	}
}

int processNew(request *req)
{
	if (cfig.hostRanges[0].rangeStart)
	{
		MYDWORD iip = ntohl(req->client.sin_addr.s_addr);
		MYBYTE allowed = false;

		for (MYWORD j = 0; j <= sizeof(cfig.hostRanges) && cfig.hostRanges[j].rangeStart; j++)
		{
			if (iip >= cfig.hostRanges[j].rangeStart && iip <= cfig.hostRanges[j].rangeEnd)
			{
				allowed = true;
				break;
			}
		}

		if (!allowed)
		{
			req->serverError.opcode = htons(5);
			req->serverError.errorcode = htons(2);
			strcpy(req->serverError.errormessage, "Access Denied");
			sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->serverError, strlen(req->serverError.errormessage) + 5, 0, (sockaddr*)&req->client, req->clientsize);
			logMess(req, 1);
			cleanReq(req);
			return 2;
		}
	}

	req->block = 0;
	req->blksize = 512;
	req->timeout = timeout;
	req->expiry = time(NULL) + req->timeout;
	req->opcode = ntohs(datain->opcode);
	char *inPtr = (char*)datain;
	*(inPtr + (req->bytesRecd - 1)) = 0;
	inPtr += 2;
	req->filename = inPtr;

	if (!strlen(req->filename) || strlen(req->filename) > UCHAR_MAX)
	{
		req->serverError.opcode = htons(5);
		req->serverError.errorcode = htons(0);
		strcpy(req->serverError.errormessage, "Malformed Request, Invalid/Missing Filename");
		sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->serverError, strlen(req->serverError.errormessage) + 5, 0, (sockaddr*)&req->client, req->clientsize);
		logMess(req, 1);
		cleanReq(req);
		return 1;
	}

	inPtr += strlen(inPtr) + 1;
	req->mode = inPtr;

	if (!strlen(req->mode) || strlen(req->mode) > 25)
	{
		req->serverError.opcode = htons(5);
		req->serverError.errorcode = htons(0);
		strcpy(req->serverError.errormessage, "Malformed Request, Invalid/Missing Mode");
		sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->serverError, strlen(req->serverError.errormessage) + 5, 0, (sockaddr*)&req->client, req->clientsize);
		logMess(req, 1);
		cleanReq(req);
		return 1;
	}

	inPtr += strlen(inPtr) + 1;

	for (MYWORD i = 0; i < strlen(req->filename); i++)
		if (req->filename[i] == notFileSep)
			req->filename[i] = fileSep;

	tempbuff[0] = '.';
	tempbuff[1] = '.';
	tempbuff[2] = fileSep;
	tempbuff[3] = 0;

	if (strstr(req->filename, tempbuff))
	{
		req->serverError.opcode = htons(5);
		req->serverError.errorcode = htons(2);
		strcpy(req->serverError.errormessage, "Access violation");
		sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->serverError, strlen(req->serverError.errormessage) + 5, 0, (sockaddr*)&req->client, req->clientsize);
		logMess(req, 1);
		cleanReq(req);
		return 1;
	}

	if (req->filename[0] == fileSep)
		req->filename++;

	if (!cfig.homes[0].alias[0])
	{
		if (strlen(cfig.homes[0].target) + strlen(req->filename) >= sizeof(req->path))
		{
			req->serverError.opcode = htons(5);
			req->serverError.errorcode = htons(2);
			strcpy(req->serverError.errormessage, "Filename too large");
			sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->serverError, strlen(req->serverError.errormessage) + 5, 0, (sockaddr*)&req->client, req->clientsize);
			logMess(req, 1);
			cleanReq(req);
			return 1;
		}
		strcpy(req->path, cfig.homes[0].target);
		strcat(req->path, req->filename);
	}
	else
	{
		char *bname = strchr(req->filename, fileSep);

		if (bname)
		{
			*bname = 0;
			bname++;
		}
		else
		{
			req->serverError.opcode = htons(5);
			req->serverError.errorcode = htons(2);
			sprintf(req->serverError.errormessage, "Missing directory/alias");
			sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->serverError, strlen(req->serverError.errormessage) + 5, 0, (sockaddr*)&req->client, req->clientsize);
			logMess(req, 1);
			cleanReq(req);
			return 1;
		}

		for (int i = 0; i < 8; i++)
		{
			if (cfig.homes[i].alias[0] && !strcasecmp(req->filename, cfig.homes[i].alias))
			{
				if (strlen(cfig.homes[i].target) + strlen(bname) >= sizeof(req->path))
				{
					req->serverError.opcode = htons(5);
					req->serverError.errorcode = htons(2);
					strcpy(req->serverError.errormessage, "Filename too large");
					sendto(network.tftpConn[req->sockInd].sock, (const char*) &req->serverError, strlen(req->serverError.errormessage) + 5, 0, (sockaddr*)&req->client, req->clientsize);
					logMess(req, 1);
					cleanReq(req);
					return 1;
				}

				strcpy(req->path, cfig.homes[i].target);
				strcat(req->path, bname);
				break;
			}
			else if (i == 7 || !cfig.homes[i].alias[0])
			{
				req->serverError.opcode = htons(5);
				req->serverError.errorcode = htons(2);
				sprintf(req->serverError.errormessage, "No such directory/alias %s", req->filename);
				sendto(network.tftpConn[req->sockInd].sock, (const char*) &req->serverError, strlen(req->serverError.errormessage) + 5, 0, (sockaddr*)&req->client, req->clientsize);
				logMess(req, 1);
				cleanReq(req);
				return 1;
			}
		}
	}

    if (ntohs(datain->opcode) == 1)
    {
		if (!cfig.fileRead)
		{
            req->serverError.opcode = htons(5);
            req->serverError.errorcode = htons(2);
            strcpy(req->serverError.errormessage, "GET Access Denied");
            sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->serverError, strlen(req->serverError.errormessage) + 5, 0, (sockaddr*)&req->client, req->clientsize);
            logMess(req, 1);
            cleanReq(req);
            return 1;
        }

        errno = 0;

		if (*inPtr)
		{
			char *tmp = inPtr;

			while (*tmp)
			{
				if (!strcasecmp(tmp, "blksize"))
				{
					tmp += strlen(tmp) + 1;
					MYDWORD val = atol(tmp);

					if (val < 512)
						val = 512;
					else if (val > blksize)
						val = blksize;

					req->blksize = val;
					break;
				}

				tmp += strlen(tmp) + 1;
			}
		}

		if (!strcasecmp(req->mode, "netascii") || !strcasecmp(req->mode, "ascii"))
			req->file = fopen(req->path, "rt");
		else
			req->file = fopen(req->path, "rb");

		if (errno || !req->file)
		{
            req->serverError.opcode = htons(5);
            req->serverError.errorcode = htons(1);
            strcpy(req->serverError.errormessage, "No Such File/No Access");
            sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->serverError, strlen(req->serverError.errormessage) + 5, 0, (sockaddr*)&req->client, req->clientsize);
            logMess(req, 1);
            cleanReq(req);
            return 1;
        }
    }
    else
    {
		if (!cfig.fileWrite && !cfig.fileOverwrite)
		{
            req->serverError.opcode = htons(5);
            req->serverError.errorcode = htons(2);
            strcpy(req->serverError.errormessage, "PUT Access Denied");
            sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->serverError, strlen(req->serverError.errormessage) + 5, 0, (sockaddr*)&req->client, req->clientsize);
            logMess(req, 1);
            cleanReq(req);
            return 1;
        }

		req->file = fopen(req->path, "rb");

		if (req->file)
		{
			fclose(req->file);
			req->file = NULL;

			if (!cfig.fileOverwrite)
			{
                req->serverError.opcode = htons(5);
                req->serverError.errorcode = htons(6);
                strcpy(req->serverError.errormessage, "File already exists");
                sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->serverError, strlen(req->serverError.errormessage) + 5, 0, (sockaddr*)&req->client, req->clientsize);
                logMess(req, 1);
                cleanReq(req);
                return 1;
            }
        }
		else if (!cfig.fileWrite)
		{
			req->serverError.opcode = htons(5);
			req->serverError.errorcode = htons(2);
			strcpy(req->serverError.errormessage, "Create File Access Denied");
			sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->serverError, strlen(req->serverError.errormessage) + 5, 0, (sockaddr*)&req->client, req->clientsize);
			logMess(req, 1);
			cleanReq(req);
			return 1;
		}

		errno = 0;

		if (strcasecmp(req->mode, "netascii") && strcasecmp(req->mode, "ascii"))
			req->file = fopen(req->path, "wb");
		else
			req->file = fopen(req->path, "wt");

		if (errno || !req->file)
		{
			req->serverError.opcode = htons(5);
			req->serverError.errorcode = htons(1);
			strcpy(req->serverError.errormessage, "Invalid Path or No Access");
			sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->serverError, strlen(req->serverError.errormessage) + 5, 0, (sockaddr*)&req->client, req->clientsize);
			logMess(req, 1);
			cleanReq(req);
			return 1;
		}
	}

	setvbuf(req->file, NULL, _IOFBF, 5 * req->blksize);

	if (*inPtr)
	{
		char *outPtr = req->mesout.buffer;
		req->mesout.opcode = htons(6);
		MYDWORD val;
		while (*inPtr)
		{
			//printf("%s", inPtr);
			if (!strcasecmp(inPtr, "blksize"))
			{
				strcpy(outPtr, inPtr);
				outPtr += strlen(outPtr) + 1;
				inPtr += strlen(inPtr) + 1;
				val = atol(inPtr);

				if (val < 512)
					val = 512;
				else if (val > blksize)
					val = blksize;

				req->blksize = val;
				sprintf(outPtr, "%u", val);
				outPtr += strlen(outPtr) + 1;
			}
			else if (!strcasecmp(inPtr, "tsize"))
			{
				strcpy(outPtr, inPtr);
				outPtr += strlen(outPtr) + 1;
				inPtr += strlen(inPtr) + 1;

				if (ntohs(datain->opcode) == 1)
				{
					if (!fseek(req->file, 0, SEEK_END))
					{
						if (ftell(req->file) >= 0)
						{
							req->tsize = ftell(req->file);
							sprintf(outPtr, "%u", req->tsize);
							outPtr += strlen(outPtr) + 1;
						}
						else
						{
							req->serverError.opcode = htons(5);
							req->serverError.errorcode = htons(2);
							strcpy(req->serverError.errormessage, "Invalid Path or No Access");
							sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->serverError, strlen(req->serverError.errormessage) + 5, 0, (sockaddr*)&req->client, req->clientsize);
							logMess(req, 1);
							req->attempt = UCHAR_MAX;
							break;
						}
					}
					else
					{
						req->serverError.opcode = htons(5);
						req->serverError.errorcode = htons(2);
						strcpy(req->serverError.errormessage, "Invalid Path or No Access");
						sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->serverError, strlen(req->serverError.errormessage) + 5, 0, (sockaddr*)&req->client, req->clientsize);
						logMess(req, 1);
						req->attempt = UCHAR_MAX;
						break;
					}
				}
				else
				{
					req->tsize = 0;
					sprintf(outPtr, "%u", req->tsize);
					outPtr += strlen(outPtr) + 1;
				}
			}
			else if (!strcasecmp(inPtr, "timeout"))
			{
				strcpy(outPtr, inPtr);
				outPtr += strlen(outPtr) + 1;
				inPtr += strlen(inPtr) + 1;
				val = atoi(inPtr);

				if (val < 1)
					val = 1;
				else if (val > UCHAR_MAX)
					val = UCHAR_MAX;

				req->timeout = val;
				req->expiry = time(NULL) + req->timeout;
				sprintf(outPtr, "%u", val);
				outPtr += strlen(outPtr) + 1;
			}

			inPtr += strlen(inPtr) + 1;

			//printf("=%u\n", val);
		}

		errno = 0;
		req->bytesReady = (MYLWORD)outPtr - (MYLWORD)&(req->acout);
		sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->acout, req->bytesReady, 0, (sockaddr*)&req->client, req->clientsize);
		//errno = WSAGetLastError();
	}
	else if (htons(datain->opcode) == 2)
	{
		req->acout.opcode = htons(4);
		req->acout.block = htons(0);
		errno = 0;
		req->bytesReady = 4;
		sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->acout, req->bytesReady, 0, (sockaddr*)&req->client, req->clientsize);
		//errno = WSAGetLastError();
	}

	if (errno)
	{
		sprintf(req->serverError.errormessage, "Communication Error");
		logMess(req, 1);
		cleanReq(req);
		return errno;
	}

	if (ntohs(datain->opcode) == 1)
	{
		errno = 0;
		req->pkt[0] = (packet*)calloc(1, req->blksize + 4);
		req->pkt[1] = (packet*)calloc(1, req->blksize + 4);

		if (errno || !req->pkt[0] || !req->pkt[1])
		{
			strcpy(req->serverError.errormessage, "Memory Error");
			logMess(req, 1);
			cleanReq(req);
			return 1;
		}

		long ftellLoc = ftell(req->file);

		if (ftellLoc > 0)
		{
			if (fseek(req->file, 0, SEEK_SET))
			{
				req->serverError.opcode = htons(5);
				req->serverError.errorcode = htons(4);
				strcpy(req->serverError.errormessage, "Invalid Path or No Access");
				sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->serverError, strlen(req->serverError.errormessage) + 5, 0, (sockaddr*)&req->client, req->clientsize);
				logMess(req, 1);
				cleanReq(req);
				return errno;
			}
		}
		else if (ftellLoc < 0)
		{
			req->serverError.opcode = htons(5);
			req->serverError.errorcode = htons(4);
			strcpy(req->serverError.errormessage, "Invalid Path or No Access");
			sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->serverError, strlen(req->serverError.errormessage) + 5, 0, (sockaddr*)&req->client, req->clientsize);
			logMess(req, 1);
			cleanReq(req);
			return errno;
		}

		req->pkt[0]->opcode = htons(3);
		req->pkt[0]->block = htons(1);
		req->bytesRead[0] = fread(&req->pkt[0]->buffer, 1, req->blksize, req->file);

		if (errno)
		{
			req->serverError.opcode = htons(5);
			req->serverError.errorcode = htons(4);
			strcpy(req->serverError.errormessage, "Invalid Path or No Access");
			sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->serverError, strlen(req->serverError.errormessage) + 5, 0, (sockaddr*)&req->client, req->clientsize);
			logMess(req, 1);
			cleanReq(req);
			return errno;
		}

		if (req->bytesRead[0] == req->blksize)
		{
			req->pkt[1]->opcode = htons(3);
			req->pkt[1]->block = htons(2);
			req->bytesRead[1] = fread(&req->pkt[1]->buffer, 1, req->blksize, req->file);

			if (errno)
			{
				req->serverError.opcode = htons(5);
				req->serverError.errorcode = htons(4);
				strcpy(req->serverError.errormessage, "Invalid Path or No Access");
				sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->serverError, strlen(req->serverError.errormessage) + 5, 0, (sockaddr*)&req->client, req->clientsize);
				logMess(req, 1);
				cleanReq(req);
				return errno;
			}

			if (req->bytesRead[1] < req->blksize)
			{
				fclose(req->file);
				req->file = 0;
			}
		}
		else
		{
			fclose(req->file);
			req->file = 0;
		}

		if (!req->bytesReady)
		{
			req->block = 1;
			return processSend(req);
		}
	}
	return 0;
}

int processSend(request *req)
{
	errno = 0;
	req->expiry = time(NULL) + req->timeout;

	if (ntohs(req->pkt[0]->block) == req->block)
	{
		errno = 0;
		sendto(network.tftpConn[req->sockInd].sock, (const char*)req->pkt[0], req->bytesRead[0] + 4, 0, (sockaddr*)&req->client, req->clientsize);
		memcpy(&req->acout, req->pkt[0], 4);
		//errno = WSAGetLastError();

		if (errno)
		{
			sprintf(req->serverError.errormessage, "Communication Error");
			logMess(req, 1);
			cleanReq(req);
			return errno;
		}

		if (req->file)
		{
			req->tblock = ntohs(req->pkt[1]->block) + 1;
			if (req->tblock == req->block)
			{
				req->pkt[1]->block = htons(++req->tblock);
				req->bytesRead[1] = fread(&req->pkt[1]->buffer, 1, req->blksize, req->file);

				if (errno)
				{
					req->serverError.opcode = htons(5);
					req->serverError.errorcode = htons(4);
					sprintf(req->serverError.errormessage, "%s", strerror(errno));
					sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->serverError, strlen(req->serverError.errormessage) + 5, 0, (sockaddr*)&req->client, req->clientsize);
					logMess(req, 1);
					cleanReq(req);
					return errno;
				}
				else if (req->bytesRead[1] < req->blksize)
				{
					fclose(req->file);
					req->file = 0;
				}
			}
		}
	}
	else if (ntohs(req->pkt[1]->block) == req->block)
	{
		errno = 0;
		sendto(network.tftpConn[req->sockInd].sock, (const char*)req->pkt[1], req->bytesRead[1] + 4, 0, (sockaddr*)&req->client, req->clientsize);
		memcpy(&req->acout, req->pkt[1], 4);
		//errno = WSAGetLastError();

		if (errno)
		{
			sprintf(req->serverError.errormessage, "Communication Error");
			logMess(req, 1);
			cleanReq(req);
			return errno;
		}

		if (req->file)
		{
			req->tblock = ntohs(req->pkt[0]->block) + 1;
			if (req->tblock == req->block)
			{
				req->pkt[0]->block = htons(++req->tblock);
				req->bytesRead[0] = fread(&req->pkt[0]->buffer, 1, req->blksize, req->file);

				if (errno)
				{
					req->serverError.opcode = htons(5);
					req->serverError.errorcode = htons(4);
					sprintf(req->serverError.errormessage, "%s", strerror(errno));
					sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->serverError, strlen(req->serverError.errormessage) + 5, 0, (sockaddr*)&req->client, req->clientsize);
					logMess(req, 1);
					cleanReq(req);
					return errno;
				}
				else if (req->bytesRead[0] < req->blksize)
				{
					fclose(req->file);
					req->file = 0;
				}
			}
		}
	}
	else //if (ntohs(req->pkt[0]->block) < req->block && ntohs(req->pkt[1]->block) < req->block)
	{
		sprintf(logBuff, "Client %s %s, %i Blocks Served", req->mapname, req->path, req->fblock);
		logMess(logBuff, 2);
		cleanReq(req);
	}

	return 0;
}

int processRecv(request *req)
{
	req->expiry = time(NULL) + req->timeout;
	errno = 0;
	req->bytesReady = 4;
	sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->acout, req->bytesReady, 0, (sockaddr*)&req->client, req->clientsize);
	//errno = WSAGetLastError();
	if (errno)
	{
		sprintf(req->serverError.errormessage, "Communication Error");
		logMess(req, 1);
		cleanReq(req);
		return errno;
	}

	if (req->bytesRecd > 4 && (fwrite(&datain->buffer, req->bytesRecd - 4, 1, req->file) != 1 || errno))
	{
		req->serverError.opcode = htons(5);
		req->serverError.errorcode = htons(3);
		strcpy(req->serverError.errormessage, "Disk full or allocation exceeded");
		sendto(network.tftpConn[req->sockInd].sock, (const char*)&req->serverError, strlen(req->serverError.errormessage) + 5, 0, (sockaddr*)&req->client, req->clientsize);
		logMess(req, 1);
		cleanReq(req);
		return 1;
	}

	//printf("%u\n", req->bytesRecd);

	if ((MYWORD)req->bytesRecd < req->blksize + 4)
	{
		sprintf(logBuff, "Client %s %s, %u Blocks Received", req->mapname, req->path, req->fblock);
		logMess(logBuff, 2);
		cleanReq(req);
	}

	return 0;
}

void cleanReq(request *req)
{
	if (req->file)
		fclose(req->file);

	if (req->pkt[0])
		free(req->pkt[0]);

	if (req->pkt[1])
		free(req->pkt[1]);

	req->file = 0;
	req->pkt[0] = 0;
	req->pkt[1] = 0;
	req->attempt = UCHAR_MAX;

}

bool getSection(const char *sectionName, char *buffer, MYBYTE serial, char *fileName)
{
	//printf("%s=%s\n",fileName,sectionName);
	char section[128];
	sprintf(section, "[%s]", sectionName);
	myUpper(section);
	FILE *f = fopen(fileName, "rt");
	char buff[512];
	MYBYTE found = 0;

	if (f)
	{
		while (fgets(buff, 511, f))
		{
			myUpper(buff);
			myTrim(buff, buff);

			if (strstr(buff, section) == buff)
			{
				found++;
				if (found == serial)
				{
					//printf("%s=%s\n",fileName,sectionName);
					while (fgets(buff, 511, f))
					{
						myTrim(buff, buff);

						if (strstr(buff, "[") == buff)
							break;

						if ((*buff) >= '0' && (*buff) <= '9' || (*buff) >= 'A' && (*buff) <= 'Z' || (*buff) >= 'a' && (*buff) <= 'z' || ((*buff) && strchr("/\\?*", (*buff))))
						{
							buffer += sprintf(buffer, "%s", buff);
							buffer++;
						}
					}
					break;
				}
			}
		}
		fclose(f);
	}

	*buffer = 0;
	*(buffer + 1) = 0;
	return (found == serial);
}

FILE *openSection(const char *sectionName, MYBYTE serial, char *fileName)
{
	//printf("%s=%s\n",fileName,sectionName);
	char section[128];
	sprintf(section, "[%s]", sectionName);
	myUpper(section);
	FILE *f = fopen(fileName, "rt");
	char buff[512];
	MYBYTE found = 0;

	if (f)
	{
		while (fgets(buff, 511, f))
		{
			myUpper(buff);
			myTrim(buff, buff);

			if (strstr(buff, section) == buff)
			{
				found++;

				if (found == serial)
					return f;
			}
		}
		fclose(f);
	}
	return NULL;
}

char *readSection(char* buff, FILE *f)
{
	while (fgets(buff, 511, f))
	{
		myTrim(buff, buff);

		if (*buff == '[')
			break;

		if ((*buff) >= '0' && (*buff) <= '9' || (*buff) >= 'A' && (*buff) <= 'Z' || (*buff) >= 'a' && (*buff) <= 'z' || ((*buff) && strchr("/\\?*", (*buff))))
			return buff;
	}

	fclose(f);
	return NULL;
}

char* myGetToken(char* buff, MYBYTE index)
{
	while (*buff)
	{
		if (index)
			index--;
		else
			break;

		buff += strlen(buff) + 1;
	}

	return buff;
}

MYWORD myTokenize(char *target, char *source, char *sep, bool whiteSep)
{
	bool found = true;
	char *dp = target;
	MYWORD kount = 0;

	while (*source)
	{
		if (sep && sep[0] && strchr(sep, (*source)))
		{
			found = true;
			source++;
			continue;
		}
		else if (whiteSep && (*source) <= 32)
		{
			found = true;
			source++;
			continue;
		}

		if (found)
		{
			if (target != dp)
			{
				*dp = 0;
				dp++;
			}
			kount++;
		}

		found = false;
		*dp = *source;
		dp++;
		source++;
	}

	*dp = 0;
	dp++;
	*dp = 0;

	//printf("%s\n", target);

	return kount;
}

char* myTrim(char *target, char *source)
{
	while ((*source) && (*source) <= 32)
		source++;

	int i = 0;

	for (; i < 511 && source[i]; i++)
		target[i] = source[i];

	target[i] = source[i];
	i--;

	for (; i >= 0 && target[i] <= 32; i--)
		target[i] = 0;

	return target;
}

/*
void mySplit(char *name, char *value, char *source, char splitChar)
{
	char *dp = strchr(source, splitChar);

	if (dp)
	{
		strncpy(name, source, (dp - source));
		name[dp - source] = 0;
		strcpy(value, dp + 1);
		myTrim(name, name);
		myTrim(value, value);
	}
	else
	{
 		strcpy(name, source);
		myTrim(name, name);
 		*value = 0;
	}
}
*/

void mySplit(char *name, char *value, char *source, char splitChar)
{
	int i = 0;
	int j = 0;
	int k = 0;

	for (; source[i] && j <= 510 && source[i] != splitChar; i++, j++)
	{
		name[j] = source[i];
	}

	if (source[i])
	{
		i++;
		for (; k <= 510 && source[i]; i++, k++)
		{
			value[k] = source[i];
		}
	}

	name[j] = 0;
	value[k] = 0;

	myTrim(name, name);
	myTrim(value, value);
	//printf("%s %s\n", name, value);
}

char *IP2String(char *target, MYDWORD ip)
{
	data15 inaddr;
	inaddr.ip = ip;
	sprintf(target, "%u.%u.%u.%u", inaddr.octate[0], inaddr.octate[1], inaddr.octate[2], inaddr.octate[3]);
	return target;
}

char *myUpper(char *string)
{
	char diff = 'a' - 'A';

	MYWORD len = strlen(string);

	for (int i = 0; i < len; i++)
		if (string[i] >= 'a' && string[i] <= 'z')
			string[i] -= diff;

	return string;
}

char *myLower(char *string)
{
	char diff = 'a' - 'A';

	MYWORD len = strlen(string);

	for (int i = 0; i < len; i++)
		if (string[i] >= 'A' && string[i] <= 'Z')
			string[i] += diff;

	return string;
}

bool isIP(char *string)
{
	int j = 0;

	for (; *string; string++)
	{
		if (*string == '.' && *(string + 1) != '.')
			j++;
		else if (*string < '0' || *string > '9')
			return 0;
	}

	if (j == 3)
		return 1;
	else
		return 0;
}

void *init(void *lpParam)
{
	//printf("In Init\n");
	memset(&cfig, 0, sizeof(cfig));
	cfig.pppid = getpid();

	char raw[512];
	char name[512];
	char value[512];
	FILE *f = NULL;

	if (verbatim)
	{
		cfig.logLevel = 2;
		printf("%s\n\n", sVersion);
	}
	else if (f = openSection("LOGGING", 1, iniFile))
	{
		cfig.logLevel = 1;
		tempbuff[0] = 0;

		while (readSection(raw, f))
		{
			if (!strcasecmp(raw, "None"))
				cfig.logLevel = 0;
			else if (!strcasecmp(raw, "Errors"))
				cfig.logLevel = 1;
			else if (!strcasecmp(raw, "All"))
				cfig.logLevel = 2;
			else
				sprintf(tempbuff, "Section [LOGGING], Invalid LogLevel: %s", raw);
		}
	}

	if (!verbatim && cfig.logLevel && logFile[0])
	{
		time_t t = time(NULL);
		tm *ttm = localtime(&t);
		loggingDay = ttm->tm_yday;
		strftime(extbuff, sizeof(extbuff), logFile, ttm);

		cfig.logfile = fopen(extbuff, "at");

		if (cfig.logfile)
		{
			sprintf(logBuff, "%s Starting..", sVersion);
			logMess(logBuff, 1);

			if (tempbuff[0])
				logMess(tempbuff, 0);
		}
    }

	if ((f = fopen(iniFile, "rt")))
	{
		fclose(f);
	}
	else
	{
		sprintf(logBuff, "Warning: file %s not found, defaults will be used", iniFile);
		logMess(logBuff, 1);
	}

	if (f = openSection("HOME", 1, iniFile))
	{
		while (readSection(raw, f))
		{
			mySplit(name, value, raw, '=');

			if (strlen(value))
			{
				if (!cfig.homes[0].alias[0] && cfig.homes[0].target[0])
				{
					sprintf(logBuff, "Section [HOME], alias and bare path mixup, entry %s ignored", raw);
					logMess(logBuff, 1);
				}
				else if (strchr(name, notFileSep) || strchr(name, fileSep) || strchr(name, '>') || strchr(name, '<') || strchr(name, '.'))
				{
					sprintf(logBuff, "Section [HOME], invalid chars in alias %s, entry ignored", name);
					logMess(logBuff, 1);
				}
				else if (name[0] && strlen(name) < 64 && value[0])
				{
					for (int i = 0; i < 8; i++)
					{
						if (cfig.homes[i].alias[0] && !strcasecmp(name, cfig.homes[i].alias))
						{
							sprintf(logBuff, "Section [HOME], Duplicate Entry: %s ignored", raw);
							logMess(logBuff, 1);
							break;
						}
						else if (!cfig.homes[i].alias[0])
						{
							strcpy(cfig.homes[i].alias, name);
							strcpy(cfig.homes[i].target, value);

							if (cfig.homes[i].target[strlen(cfig.homes[i].target) - 1] != fileSep)
							{
								tempbuff[0] = fileSep;
								tempbuff[1] = 0;
								strcat(cfig.homes[i].target, tempbuff);
							}

							break;
						}
					}
				}
				else
				{
					sprintf(logBuff, "Section [HOME], alias name %s too large", name);
					logMess(logBuff, 1);
				}
			}
			else if (!cfig.homes[0].alias[0] && !cfig.homes[0].target[0])
			{
				strcpy(cfig.homes[0].target, name);

				if (cfig.homes[0].target[strlen(cfig.homes[0].target) - 1] != fileSep)
				{
					tempbuff[0] = fileSep;
					tempbuff[1] = 0;
					strcat(cfig.homes[0].target, tempbuff);
				}
			}
			else if (cfig.homes[0].alias[0])
			{
				sprintf(logBuff, "Section [HOME], alias and bare path mixup, entry %s ignored", raw);
				logMess(logBuff, 1);
			}
			else if (cfig.homes[0].target[0])
			{
				sprintf(logBuff, "Section [HOME], Duplicate Path: %s ignored", raw);
				logMess(logBuff, 1);
			}
			else
			{
				sprintf(logBuff, "Section [HOME], missing = sign, Invalid Entry: %s ignored", raw);
				logMess(logBuff, 1);
			}
		}
	}

	cfig.fileRead = true;

	if (f = openSection("TFTP-OPTIONS", 1, iniFile))
	{
		while (readSection(raw, f))
		{
			mySplit(name, value, raw, '=');

			if (strlen(value))
			{
				if (!strcasecmp(name, "UserName"))
				{
					if (strlen(value) < 128)
					{
						passwd *pwd = getpwnam(value);

						if (pwd)
						{
							cfig.pw_uid = pwd->pw_uid;
							cfig.pw_gid = pwd->pw_gid;
							strcpy(cfig.username, value);

							if (!cfig.homes[0].target[0])
							{
								if (cfig.pw_uid)
									sprintf(cfig.homes[0].target, "%s/", pwd->pw_dir);
								else
									strcpy(cfig.homes[0].target, "/home/");
							}
						}
						else
						{
							sprintf(logBuff, "Section [TFTP-OPTIONS], unknown username: %s, stopping", value);
							logMess(logBuff, 1);
							exit(EXIT_FAILURE);
						}
					}
					else
					{
						sprintf(logBuff, "Section [TFTP-OPTIONS], invalid username: %s, stopping", value);
						logMess(logBuff, 1);
						exit(EXIT_FAILURE);
					}
				}
				else if (!strcasecmp(name, "blksize"))
				{
					MYDWORD tblksize = atol(value);

					if (tblksize < 512)
						blksize = 512;
					else if (tblksize > 65464)
						blksize = 65464;
					else
						blksize = tblksize;
				}
				else if (!strcasecmp(name, "timeout"))
				{
					timeout = atol(value);
					if (timeout < 1)
						timeout = 1;
					else if (timeout > UCHAR_MAX)
						timeout = UCHAR_MAX;
				}
				else if (!strcasecmp(name, "Read"))
				{
					if (strchr("Yy", *value))
						cfig.fileRead = true;
					else
						cfig.fileRead = false;
				}
				else if (!strcasecmp(name, "Write"))
				{
					if (strchr("Yy", *value))
						cfig.fileWrite = true;
					else
						cfig.fileWrite = false;
				}
				else if (!strcasecmp(name, "Overwrite"))
				{
					if (strchr("Yy", *value))
						cfig.fileOverwrite = true;
					else
						cfig.fileOverwrite = false;
				}
				else
				{
					sprintf(logBuff, "Warning: unknown option %s, ignored", name);
					logMess(logBuff, 1);
				}
			}
		}
	}

	if (f = openSection("ALLOWED-CLIENTS", 1, iniFile))
	{
		int i = 0;

		while (readSection(raw, f))
		{
			if (i < 32)
			{
				MYDWORD rs = 0;
				MYDWORD re = 0;
				mySplit(name, value, raw, '-');
				rs = htonl(my_inet_addr(name));

				if (strlen(value))
					re = htonl(my_inet_addr(value));
				else
					re = rs;

				if (rs && rs != INADDR_NONE && re && re != INADDR_NONE && rs <= re)
				{
					cfig.hostRanges[i].rangeStart = rs;
					cfig.hostRanges[i].rangeEnd = re;
					i++;
				}
				else
				{
					sprintf(logBuff, "Section [ALLOWED-CLIENTS] Invalid entry %s in ini file, ignored", raw);
					logMess(logBuff, 1);
				}
			}
		}
	}

	if (!cfig.username[0])
	{
		passwd *pwd = getpwuid(getuid());
		strcpy(cfig.username, pwd->pw_name);

		if (!cfig.homes[0].target[0])
		{
			if (pwd->pw_uid)
				strcpy(cfig.homes[0].target, pwd->pw_dir);
			else
				strcpy(cfig.homes[0].target, "/home/");
		}
	}

	sprintf(logBuff, "username: %s", cfig.username);
	logMess(logBuff, 1);

	for (int i = 0; i < MAX_SERVERS; i++)
		if (cfig.homes[i].target[0])
		{
			sprintf(logBuff, "alias /%s is mapped to %s", cfig.homes[i].alias, cfig.homes[i].target);
			logMess(logBuff, 1);
		}

	if (cfig.hostRanges[0].rangeStart)
	{
		char temp[128];

		for (MYWORD i = 0; i <= sizeof(cfig.hostRanges) && cfig.hostRanges[i].rangeStart; i++)
		{
			sprintf(logBuff, "%s", "permitted clients: ");
			sprintf(temp, "%s-", IP2String(tempbuff, htonl(cfig.hostRanges[i].rangeStart)));
			strcat(logBuff, temp);
			sprintf(temp, "%s", IP2String(tempbuff, htonl(cfig.hostRanges[i].rangeEnd)));
			strcat(logBuff, temp);
			logMess(logBuff, 1);
		}
	}
	else
	{
		sprintf(logBuff, "%s", "permitted clients: all");
		logMess(logBuff, 1);
	}

	sprintf(logBuff, "max blksize: %u", blksize);
	logMess(logBuff, 1);
	sprintf(logBuff, "default blksize: %u", 512);
	logMess(logBuff, 1);
	sprintf(logBuff, "default timeout: %u", timeout);
	logMess(logBuff, 1);
	sprintf(logBuff, "file read allowed: %s", cfig.fileRead ? "Yes" : "No");
	logMess(logBuff, 1);
	sprintf(logBuff, "file create allowed: %s", cfig.fileWrite ? "Yes" : "No");
	logMess(logBuff, 1);
	sprintf(logBuff, "file overwrite allowed: %s", cfig.fileOverwrite ? "Yes" : "No");
	logMess(logBuff, 1);

	if (!verbatim)
	{
		sprintf(logBuff, "logging: %s", cfig.logLevel > 1 ? "all" : "errors");
		logMess(logBuff, 1);
	}

	cfig.fixedSocket = socket(AF_INET, SOCK_DGRAM, 0);

	if (cfig.fixedSocket < 0)
	{
		sprintf(logBuff, "Failed to Create Socket");
		logMess(logBuff, 1);
		exit(EXIT_FAILURE);
	}

	do
	{
		bool bindfailed = false;

		if (!cfig.ifspecified && (f = openSection("LISTEN-ON", 1, iniFile)))
		{
			MYBYTE i = 0;

			while (readSection(raw, f))
			{
				MYWORD port = 69;

				cfig.ifspecified = true;
				mySplit(name, value, raw, ':');

				if (value[0])
					port = atoi(value);

				if(i < MAX_SERVERS)
				{
					if (isIP(name))
					{
						MYDWORD addr = my_inet_addr(name);

						if (!addr)
						{
							newNetwork.listenServers[0] = 0;
							newNetwork.listenPorts[0] = port;
							fclose(f);
							break;
						}
						else if (!findServer(newNetwork.listenServers, addr))
						{
							newNetwork.listenServers[i] = addr;
							newNetwork.listenPorts[i] = port;
							i++;
						}
					}
					else
					{
						sprintf(logBuff, "Warning: Section [LISTEN-ON], Invalid Interface Address %s, ignored", raw);
						logMess(logBuff, 1);
					}
				}
			}
		}

		if (!cfig.ifspecified)
		{
			sprintf(logBuff, "detecting Interfaces..");
			logMess(logBuff, 1);
			getInterfaces(&newNetwork);
			memcpy(cfig.oldservers, newNetwork.staticServers, (MAX_SERVERS * sizeof(MYDWORD)));

			for (MYBYTE n = 0; n < MAX_SERVERS && newNetwork.staticServers[n]; n++)
			{
				newNetwork.listenServers[n] = newNetwork.staticServers[n];
				newNetwork.listenPorts[n] = 69;
			}
		}

		MYBYTE i = 0;

		for (int j = 0; j < MAX_SERVERS && newNetwork.listenPorts[j]; j++)
		{
			int k = 0;

			for (; k < MAX_SERVERS && network.tftpConn[k].loaded; k++)
			{
				if (network.tftpConn[k].ready && network.tftpConn[k].server == newNetwork.listenServers[j] && network.tftpConn[k].port == newNetwork.listenPorts[j])
					break;
			}

			if (network.tftpConn[k].ready && network.tftpConn[k].server == newNetwork.listenServers[j] && network.tftpConn[k].port == newNetwork.listenPorts[j])
			{
				memcpy(&(newNetwork.tftpConn[i]), &(network.tftpConn[k]), sizeof(tftpConnType));

				if (newNetwork.maxFD < newNetwork.tftpConn[i].sock)
					newNetwork.maxFD = newNetwork.tftpConn[i].sock;

				network.tftpConn[k].ready = false;
				//printf("%d, %s found\n", i, IP2String(tempbuff, newNetwork.tftpConn[i].server));
				i++;
				continue;
			}
			else
			{
				newNetwork.tftpConn[i].sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

				if (newNetwork.tftpConn[i].sock == INVALID_SOCKET)
				{
					bindfailed = true;
					sprintf(logBuff, "Failed to Create Socket");
					logMess(logBuff, 1);
					continue;
				}

				//printf("Socket %u\n", newNetwork.tftpConn[i].sock);

				errno = 0;
				newNetwork.tftpConn[i].addr.sin_family = AF_INET;
				newNetwork.tftpConn[i].addr.sin_addr.s_addr = newNetwork.listenServers[j];
				newNetwork.tftpConn[i].addr.sin_port = htons(port);
				int nRet = bind(newNetwork.tftpConn[i].sock, (sockaddr*)&newNetwork.tftpConn[i].addr, sizeof(struct sockaddr_in));

				if (nRet == SOCKET_ERROR || errno)
				{
					bindfailed = true;
					close(newNetwork.tftpConn[i].sock);
					sprintf(logBuff, "%s Port %i bind failed, %s", IP2String(tempbuff, newNetwork.listenServers[j]), newNetwork.listenPorts[j], strerror(errno));
					logMess(logBuff, 1);
					continue;
				}

				newNetwork.tftpConn[i].loaded = true;
				newNetwork.tftpConn[i].ready = true;
				newNetwork.tftpConn[i].server = newNetwork.listenServers[j];
				newNetwork.tftpConn[i].port = newNetwork.listenPorts[j];

				//printf("%d, %s created\n", i, IP2String(tempbuff, newNetwork.tftpConn[i].server));

				if (newNetwork.maxFD < newNetwork.tftpConn[i].sock)
					newNetwork.maxFD = newNetwork.tftpConn[i].sock;

				if (!newNetwork.listenServers[j])
					break;

				i++;
			}
		}

		newNetwork.maxFD++;

		if (bindfailed)
			cfig.failureCount++;
		else
			cfig.failureCount = 0;

		closeConn();
		memcpy(&network, &newNetwork, sizeof(data1));

		//printf("%i %i %i\n", network.tftpConn[0].ready, network.dnsUdpConn[0].ready, network.dnsTcpConn[0].ready);

		if (!network.tftpConn[0].ready)
		{
			sprintf(logBuff, "No Static Interface ready, Waiting...");
			logMess(logBuff, 1);
			continue;
		}

		for (int i = 0; i < MAX_SERVERS && network.tftpConn[i].ready; i++)
		{
			sprintf(logBuff, "listening on: %s:%i", IP2String(tempbuff, network.tftpConn[i].server), network.tftpConn[i].port);
			logMess(logBuff, 1);
		}

		network.ready = true;

	} while (detectChange());

	//printf("Exiting init\n");

	pthread_exit(NULL);
}

bool detectChange()
{
	//printf("Entering detectchange\n");

	if (!cfig.failureCount)
	{
		if (cfig.ifspecified || cfig.pw_uid)
			return false;
	}

    while (true)
    {
		sleep(20);
		//printf("Checking Networks (failue count=%u, failue cycle=%u)..\n", cfig.failureCount, cfig.failureCycle);

		if (!cfig.ifspecified)
			getInterfaces(&newNetwork);
		else
		{
			memcpy(newNetwork.listenServers, newNetwork.listenServers, (MAX_SERVERS * sizeof(MYDWORD)));
			memcpy(newNetwork.listenPorts, newNetwork.listenPorts, (MAX_SERVERS * sizeof(MYWORD)));
		}

		if (!cfig.ifspecified && memcmp(cfig.oldservers, newNetwork.staticServers, (MAX_SERVERS * sizeof(MYDWORD))))
		{
			memcpy(cfig.oldservers, newNetwork.staticServers, (MAX_SERVERS * sizeof(MYDWORD)));
			sprintf(logBuff, "Network changed, re-detecting Listening Interfaces..");
			logMess(logBuff, 1);
			break;
		}
		else if (cfig.failureCount)
		{
			cfig.failureCycle++;

			if (cfig.failureCycle == (MYDWORD)pow(2, cfig.failureCount))
			{
				sprintf(logBuff, "Retrying failed Listening Interfaces..");
				logMess(logBuff, 1);
				break;
			}
		}
		else
			cfig.failureCycle = 0;
	}

	network.ready = false;

	while (network.busy)
		sleep(1);

	//printf("Returning from detectchange\n");

	return true;
}

void getInterfaces(data1 *network)
{
	memset(network, 0, sizeof(data1));

	Ifc.ifc_len = sizeof(IfcBuf);
	Ifc.ifc_buf = (char*)IfcBuf;

	if (ioctl(cfig.fixedSocket, SIOCGIFCONF, &Ifc) >= 0)
	{

		MYDWORD addr, mask;
		short flags;
		struct ifreq pIfr;
		MYBYTE numInterfaces = Ifc.ifc_len / sizeof(ifreq);

		for (MYBYTE i = 0 ; i < numInterfaces; i++)
		{
			memcpy(&pIfr, &(IfcBuf[i]), sizeof(ifreq));

			if (!ioctl(cfig.fixedSocket, SIOCGIFADDR, &pIfr))
				addr = ((struct sockaddr_in*)&pIfr.ifr_addr)->sin_addr.s_addr;
			else
				addr = 0;

			if (!ioctl(cfig.fixedSocket, SIOCGIFNETMASK, &pIfr))
				mask = ((struct sockaddr_in*)&pIfr.ifr_addr)->sin_addr.s_addr;
			else
				mask = 0;

			if (!ioctl(cfig.fixedSocket, SIOCGIFFLAGS, &pIfr))
				flags = pIfr.ifr_flags;
			else
				flags = 0;


			//printf("%s IFF_RUNNING %d IFF_UP %d\n", (flags & IFF_RUNNING), (flags & IFF_UP));

			if (addr)
				addServer(network->allServers, addr);

			//if (addr && mask && !(flags & IFF_POINTOPOINT) && !(flags & IFF_DYNAMIC))
			//if (addr && mask && !(flags & IFF_POINTOPOINT) && !(flags & IFF_DYNAMIC) && (flags & IFF_RUNNING) && (flags & IFF_UP))
			//if (addr && mask && !(flags & IFF_POINTOPOINT) && !(flags & IFF_LOOPBACK) && !(flags & IFF_DYNAMIC))
			//if (addr && mask && !(flags & IFF_POINTOPOINT) && !(flags & IFF_LOOPBACK))
			if (addr && mask && !(flags & IFF_POINTOPOINT) && (flags & IFF_RUNNING) && (flags & IFF_UP))
			{
				addServer(network->staticServers, addr);
			}
		}
    }
}

bool addServer(MYDWORD *array, MYDWORD ip)
{
	for (MYBYTE i = 0; i < MAX_SERVERS; i++)
	{
		if (!ip || array[i] == ip)
			return 0;
		else if (!array[i])
		{
			array[i] = ip;
			return 1;
		}
	}
	return 0;
}

MYDWORD *findServer(MYDWORD *array, MYDWORD ip)
{
	if (ip)
	{
		for (MYBYTE i = 0; i < MAX_SERVERS && array[i]; i++)
		{
			if (array[i] == ip)
				return &(array[i]);
		}
	}
	return 0;
}

MYDWORD my_inet_addr(char *str)
{
    if (str == NULL || !str[0])
        return INADDR_ANY;
    else
    	return inet_addr(str);
}

void logMess(char *logBuff, MYBYTE logLevel)
{
	//pthread_mutex_lock( &mutLog );

	if (verbatim)
		printf("%s\n", logBuff);
	else if (cfig.logfile && logLevel <= cfig.logLevel)
	{
		time_t t = time(NULL);
		tm *ttm = localtime(&t);

		if (ttm->tm_yday != loggingDay)
		{
			loggingDay = ttm->tm_yday;
			strftime(extbuff, sizeof(extbuff), logFile, ttm);
			fprintf(cfig.logfile, "Logging Continued on file %s\n", extbuff);
			fclose(cfig.logfile);
			cfig.logfile = fopen(extbuff, "at");

			if (cfig.logfile)
				fprintf(cfig.logfile, "%s\n\n", sVersion);
			else
				return;
		}

		strftime(extbuff, sizeof(extbuff), "%d-%b-%y %X", ttm);
		fprintf(cfig.logfile, "[%s] %s\n", extbuff, logBuff);
		fflush(cfig.logfile);
	}
	else if (logLevel <= cfig.logLevel)
		syslog(LOG_MAKEPRI(LOG_LOCAL1, LOG_CRIT), "%s", logBuff);

	//pthread_mutex_unlock( &mutLog );
}

void logMess(request *req, MYBYTE logLevel)
{
	//pthread_mutex_lock( &mutLog );

	if (verbatim)
	{
		if (!req->serverError.errormessage[0])
			printf(req->serverError.errormessage, strerror(errno));

		if (req->path[0])
			printf("Client %s:%u %s, %s\n", IP2String(tempbuff, req->client.sin_addr.s_addr), ntohs(req->client.sin_port), req->path, req->serverError.errormessage);
		else
			printf("Client %s:%u, %s\n", IP2String(tempbuff, req->client.sin_addr.s_addr), ntohs(req->client.sin_port), req->serverError.errormessage);

	}
	else if (cfig.logfile && logLevel <= cfig.logLevel)
	{
		time_t t = time(NULL);
		tm *ttm = localtime(&t);

		if (ttm->tm_yday != loggingDay)
		{
			loggingDay = ttm->tm_yday;
			strftime(extbuff, sizeof(extbuff), logFile, ttm);
			fprintf(cfig.logfile, "Logging Continued on file %s\n", extbuff);
			fclose(cfig.logfile);
			cfig.logfile = fopen(extbuff, "at");

			if (cfig.logfile)
				fprintf(cfig.logfile, "%s\n\n", sVersion);
			else
				return;
		}

		strftime(extbuff, sizeof(extbuff), "%d-%b-%y %X", ttm);

		if (req->path[0])
			fprintf(cfig.logfile, "[%s] Client %s:%u %s, %s\n", extbuff, IP2String(tempbuff, req->client.sin_addr.s_addr), ntohs(req->client.sin_port), req->path, req->serverError.errormessage);
		else
			fprintf(cfig.logfile, "[%s] Client %s:%u, %s\n", extbuff, IP2String(tempbuff, req->client.sin_addr.s_addr), ntohs(req->client.sin_port), req->serverError.errormessage);

		fflush(cfig.logfile);
	}
	else if (logLevel <= cfig.logLevel)
	{
		char logBuff[1024];

		if (!req->serverError.errormessage[0])
			sprintf(req->serverError.errormessage, "%s", strerror(errno));

		if (req->path[0])
			snprintf(logBuff, sizeof logBuff, "Client %s:%u %s, %s\n", IP2String(tempbuff, req->client.sin_addr.s_addr), ntohs(req->client.sin_port), req->path, req->serverError.errormessage);
		else
			snprintf(logBuff, sizeof logBuff, "Client %s:%u, %s\n", IP2String(tempbuff, req->client.sin_addr.s_addr), ntohs(req->client.sin_port), req->serverError.errormessage);

		syslog(LOG_MAKEPRI(LOG_LOCAL1, LOG_CRIT), logBuff);
	}

	//pthread_mutex_unlock( &mutLog );
}
