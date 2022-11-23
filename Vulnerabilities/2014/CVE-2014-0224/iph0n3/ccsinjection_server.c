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

#define BACKLOG 5

char handshake_message[] =
    "\x16" // handshake
    "\x03\x01"
    "\x00\x31"
    "\x02" // server_hello
    "\x00\x00\x2d"
    "\x03\x01"
    "\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
    "\x00"
    "\x00\x00"
    "\x00"
    "\x00\x05"
    "\x00\x0f"
    "\x00\x01"
    "\x01"
;

void
usage(const char *name)
{
    fprintf(stderr, "Usage: %s [-dhv][-p port] [host]\n", name);
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

    while ((c = getopt(argc, argv, "dhp:v")) != -1) {
        switch (c) {
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

    if (argv[optind] == NULL)
        argv[optind] = "0.0.0.0";

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

    if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(s, BACKLOG) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if (debug || verbose)
        fprintf(stderr, "Listening on %s:%d\n", argv[optind], port);

    for (;;) {
        int tmp;
        struct sockaddr_in sin;
        socklen_t sin_len = sizeof(sin);

        if((tmp = accept(s, (struct sockaddr *)&sin, &sin_len)) == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        if (debug || verbose)
            fprintf(stderr, "Accepted connection from %s:%d\n", inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));

        if (!fork()) {
            for (;;) {
                fd_set fds;
                char buf[16384];

                FD_ZERO(&fds);
                FD_SET(tmp, &fds);

                if (select(FD_SETSIZE, &fds, NULL, NULL, NULL) == -1) {
                    if (errno == EINTR)
                        continue;
                    perror("select");
                    exit(EXIT_FAILURE);
                }

                if (FD_ISSET(tmp, &fds)) {
                    if ((count = read(tmp, buf, sizeof(buf))) < 1) {
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
                                buf[i + 5] == '\x01' && // client_hello
                                buf[i + 9] == '\x03') {

                                /* Use the protocol version sent by the
                                 * client. This should be the latest version
                                 * supported by the client, which may also
                                 * be the only acceptable.
                                 */
                                handshake_message[2] = handshake_message[10] = buf[i + 10];

                                // Copy gmt_unix_time and random_bytes.
                                memcpy(&handshake_message[11], &buf[11], 32);

                                /* Use the first cipher suite sent by the
                                 * client.
                                 */
                                handshake_message[44] = buf[i + 46];
                                handshake_message[45] = buf[i + 47];

                                if ((count = send(tmp, handshake_message, sizeof(handshake_message) - 1, 0)) == -1) {
                                    perror("send");
                                    exit(EXIT_FAILURE);
                                }

                                if (debug)
                                    hexdump(stderr, handshake_message, sizeof(handshake_message) - 1);

                                if (debug || verbose)
                                    fprintf(stderr, "%d bytes sent\n", count);

                                /* Use the protocol version sent by the
                                 * client. This should be the latest version
                                 * supported by the client, which may also
                                 * be the only acceptable.
                                 */
                                ccs_message[2] = buf[i + 10];

                                /* Send the change cipher spec message twice
                                 * to force an alert in the case the client
                                 * is not patched.
                                 */

                                if ((count = send(tmp, ccs_message, sizeof(ccs_message) - 1, 0)) == -1) {
                                    perror("send");
                                    exit(EXIT_FAILURE);
                                }

                                if (debug)
                                    hexdump(stderr, ccs_message, sizeof(ccs_message) - 1);

                                if (debug || verbose)
                                    fprintf(stderr, "%d bytes sent\n", count);

                                if ((count = send(tmp, ccs_message, sizeof(ccs_message) - 1, 0)) == -1) {
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

        close(tmp);
    }

    exit(EXIT_SUCCESS);
}
