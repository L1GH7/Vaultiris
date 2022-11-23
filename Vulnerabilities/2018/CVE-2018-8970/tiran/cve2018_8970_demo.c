/*
 * Copyright 2013-2016 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the OpenSSL license (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

/* Modified by Christian Heimes <christian@python.org> for CVE-2018-8970
 *
 * Based on demos/bio/client-arg.c from OpenSSL 1.1.1
 */

#include <string.h>
#include <openssl/err.h>
#include <openssl/ssl.h>

#define HOST "www.python.org"

#ifndef CORRECT_HOST
#define VERIFY_HOST "www.evil.com"
#else
#define VERIFY_HOST HOST
#endif

#define CA_BUNDLE "/etc/pki/tls/certs/ca-bundle.crt"

#define CHECK_ERROR(v, msg) \
    do { \
        if (!(v)) { \
            fprintf(stderr, msg"\n"); \
            ERR_print_errors_fp(stderr); \
            goto end; \
        } \
    } while(0)


int main(int argc, char **argv)
{
    BIO *sbio = NULL, *out = NULL;
    int len;
    int result;
    int exitcode = 1;
    char tmpbuf[1024];
    SSL_CTX *ctx = NULL;
    SSL *ssl = NULL;
    const char *sni = HOST;
    const char *connect_str = HOST ":443";
    X509_VERIFY_PARAM *param = NULL;

    out = BIO_new_fp(stdout, BIO_NOCLOSE);
    CHECK_ERROR(out, "Fail to create BIO out");

    ctx = SSL_CTX_new(TLS_client_method());
    CHECK_ERROR(ctx, "Fail to create context");

    result = SSL_CTX_set_default_verify_paths(ctx);
    CHECK_ERROR(result, "Fail to set default verify paths");

    result = SSL_CTX_load_verify_locations(ctx, CA_BUNDLE, NULL);
    CHECK_ERROR(result, "Fail to load verify location");

    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);

    sbio = BIO_new_ssl_connect(ctx);
    CHECK_ERROR(sbio, "No BIO connection");

    BIO_get_ssl(sbio, &ssl);
    CHECK_ERROR(ssl, "Can't locate SSL pointer");

    /* Don't want any retries */
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

    result = SSL_set_tlsext_host_name(ssl, HOST);
    CHECK_ERROR(result, "Fail to set SNI TLS ext");

    param = SSL_get0_param(ssl);
    result = X509_VERIFY_PARAM_set1_host(param, VERIFY_HOST, 0);
    CHECK_ERROR(result, "Fail to set verify host");

    BIO_set_conn_hostname(sbio, connect_str);
    if (BIO_do_connect(sbio) <= 0) {
        long v = SSL_get_verify_result(ssl);
        fprintf(stderr, "Error connecting to server\n");
        ERR_print_errors_fp(stderr);
        fprintf(stderr, "X509 verify error: %s\n",
                X509_verify_cert_error_string(v));
        goto end;
    }

    if (BIO_do_handshake(sbio) <= 0) {
        fprintf(stderr, "Error establishing SSL connection\n");
        ERR_print_errors_fp(stderr);
        goto end;
    }

    BIO_puts(sbio, "GET /robots.txt HTTP/1.1\r\n");
    BIO_puts(sbio, "Host: " HOST "\r\n");
    BIO_puts(sbio, "\r\n");
    for (;;) {
        len = BIO_read(sbio, tmpbuf, 512);
        BIO_write(out, tmpbuf, len);
        if (len < 512)
            break;
    }
    exitcode = 0;
 end:
    SSL_CTX_free(ctx);
    BIO_free_all(sbio);
    BIO_free(out);
    return exitcode;
}
