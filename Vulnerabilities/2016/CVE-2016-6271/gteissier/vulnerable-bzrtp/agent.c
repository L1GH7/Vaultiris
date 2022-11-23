#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

#include "bzrtp/bzrtp.h"


static void hexdump(const void *ptr, size_t size) {
  const uint8_t *cptr = ptr;
  size_t i;
  for (i = 0; i < size; i++) {
    printf("%02x", cptr[i]);
  }
  printf("\n");
}


static int sd;

static int bzrtp_sendData(void *clientData, const uint8_t *packetString, uint16_t packetLength) {
  return write(sd, packetString, packetLength);
}

static int bzrtp_srtpSecretsAvailable(void *clientData, bzrtpSrtpSecrets_t *secrets,
  uint8_t part) {
  switch (part) {
  case ZRTP_SRTP_SECRETS_FOR_RECEIVER:
    printf("bzrtp SAS %s\n", secrets->sas);
    break;
  }

  return 0;
}

static int bzrtp_startSrtpSession(void *clientData, const char* sas, int32_t verified) {
  return 0;
}

static bzrtpCallbacks_t bzrtp_callbacks = {
  NULL,
  NULL,
  bzrtp_sendData,
  bzrtp_srtpSecretsAvailable,
  bzrtp_startSrtpSession,
};


int main(int argc, char **argv) {
  bzrtpContext_t *ctx;
  int ret;
  uint32_t self_ssrc;
  const char *remote;
  struct sockaddr_in addr;
  char hostname[1024];
  uint8_t buffer[1024];
  uint16_t received;
  uint64_t now;

  remote = argv[1];

  gethostname(hostname, sizeof(hostname));
  self_ssrc = (hostname[0]<<8) + hostname[1];

  sd = socket(AF_INET, SOCK_DGRAM, 0);
  assert(sd != -1);

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr("0.0.0.0");
  addr.sin_port = htons(0x1337);

  ret = bind(sd, (struct sockaddr *) &addr, sizeof(addr));
  assert(ret == 0);

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(remote);
  addr.sin_port = htons(0x1337);

  ret = connect(sd, (struct sockaddr *) &addr, sizeof(addr));
  assert(ret == 0);

  ctx = bzrtp_createBzrtpContext(self_ssrc);
  assert(ctx != NULL);

  ret = bzrtp_setCallbacks(ctx, &bzrtp_callbacks);
  assert(ret == 0);

  bzrtp_initBzrtpContext(ctx);

  bzrtp_setClientData(ctx, self_ssrc, ctx);

  ret = bzrtp_startChannelEngine(ctx, self_ssrc);
  assert(ret == 0);

  for (now = 0; now += 50;) {
    usleep(500000);

    ret = recv(sd, buffer, sizeof(buffer), MSG_DONTWAIT);
    if (ret > 0) {
      received = ret;
      bzrtp_processMessage(ctx, self_ssrc, buffer, received);
    }

    bzrtp_iterate(ctx, self_ssrc, now);
  }

  bzrtp_destroyBzrtpContext(ctx, self_ssrc);

  return 0;
}
