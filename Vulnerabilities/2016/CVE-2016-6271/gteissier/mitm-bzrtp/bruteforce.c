#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sched.h>
#include <assert.h>

#include <nettle/sha.h>
#include <nettle/hmac.h>

#include <gmp.h>

static uint8_t nibble(const char c) {
  switch (c) {
  case '0'...'9':
    return c-'0';
  case 'a'...'f':
    return c-'a'+0xa;
  case 'A'...'F':
    return c-'A'+0xa;
  default:
    abort();
  }
}

static void unhexlify(const char *s, uint8_t **pb, size_t *ps) {
  uint8_t *b;
  size_t hexlen = strlen(s);
  int i;

  assert(hexlen % 2 == 0);

  b = malloc(hexlen/2);
  assert(b != NULL);

  for (i = 0; i < hexlen; i += 2) {
    b[i>>1] = nibble(s[i])<<4;
    b[i>>1] += nibble(s[i+1]);
  }

  *pb = b;
  *ps = hexlen/2;
}

static void kdf(const uint8_t *s0, const char *label,
  const uint8_t *KDF_Context, uint8_t *result) {
  struct hmac_sha256_ctx ctx;

  hmac_sha256_set_key(&ctx, SHA256_DIGEST_SIZE, s0);
  hmac_sha256_update(&ctx, 4, (const uint8_t *) "\x00\x00\x00\x01");
  hmac_sha256_update(&ctx, strlen(label), (const uint8_t *) label);
  hmac_sha256_update(&ctx, 1, (const uint8_t *) "\x00");
  hmac_sha256_update(&ctx, 12+12+32, KDF_Context);
  hmac_sha256_update(&ctx, 4, (const uint8_t *) "\x00\x00\x01\x00");
  hmac_sha256_digest(&ctx, SHA256_DIGEST_SIZE, result);
}

static void get_sas(const uint8_t *s0, const uint8_t *KDF_Context, uint8_t *sas) {
  uint8_t result[SHA256_DIGEST_SIZE];

  kdf(s0, "SAS", KDF_Context, result);

  memcpy(sas, result, 4);
}

/* svi */
unsigned long svi = 0;

unsigned int finished_workers = 0;
pthread_mutex_t finishing_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t finishing_cond = PTHREAD_COND_INITIALIZER;

struct worker_ctx {
  int index;

  uint8_t KDF_Context[12+12+32];

  uint8_t *initiator_chain;
  size_t initiator_size;

  mpz_t pvr;
  mpz_t p;
  mpz_t shared_secret;
  mpz_t pvr4;

  mpz_t pvi;
  mpz_t pvi4;

  uint8_t *sasval;
  size_t sasval_size;

  uint8_t *zidi, *zidr;
  size_t zid_size;

  unsigned long count;
  uint8_t *mmaped;

  unsigned long start;

  pthread_t tid;
  volatile int quit;
};

static unsigned int n_workers = 4;

static void worker_init(struct worker_ctx *w, int argc, char **argv) {
  unhexlify(argv[1], &w->initiator_chain, &w->initiator_size);

  mpz_init_set_str(w->p, "FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD129024E088A67CC74020BBEA63B139B22514A08798E3404DDEF9519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245E485B576625E7EC6F44C42E9A637ED6B0BFF5CB6F406B7EDEE386BFB5A899FA5AE9F24117C4B1FE649286651ECE45B3DC2007CB8A163BF0598DA48361C55D39A69163FA8FD24CF5F83655D23DCA3AD961C62F356208552BB9ED529077096966D670C354E4ABC9804F1746C08CA18217C32905E462E36CE3BE39E772C180E86039B2783A2EC07A28FB5C55DF06F4C52C9DE2BCBF6955817183995497CEA956AE515D2261898FA051015728E5A8AAAC42DAD33170D04507A33A85521ABDF1CBA64ECFB850458DBEF0A8AEA71575D060C7DB3970F85A6E1E4C7ABF5AE8CDB0933D71E8C94E04A25619DCEE3D2261AD2EE6BF12FFA06D98A0864D87602733EC86A64521F2B18177B200CBBE117577A615D6C770988C0BAD946E208E24FA074E5AB3143DB5BFCE0FD108E4B82D120A93AD2CAFFFFFFFFFFFFFFFF", 16);
  mpz_init_set_str(w->pvr, argv[2], 16);
  mpz_init(w->shared_secret);
  mpz_init(w->pvr4);

  mpz_init(w->pvi);
  mpz_init(w->pvi4);

  unhexlify(argv[3], &w->sasval, &w->sasval_size);

  unhexlify(argv[4], &w->zidi, &w->zid_size);
  assert(w->zid_size == 12);
  memcpy(&w->KDF_Context[0], w->zidi, 12);

  unhexlify(argv[5], &w->zidr, &w->zid_size);
  assert(w->zid_size == 12);
  memcpy(&w->KDF_Context[12], w->zidr, 12);

  /* shared_secret = pvr^x mod p */
  /* each worker will follow x=start+4*k */
  /* hence, shared_secret = pvr^(start+4*k) */
  /* first shared_secret = pvr^start */
  /* next shared_secret = shared_secret * pvr^4 */
  mpz_powm_ui(w->shared_secret, w->pvr, w->start, w->p);
  mpz_powm_ui(w->pvr4, w->pvr, n_workers, w->p);

  mpz_set_ui(w->pvi, 2UL);
  mpz_powm_ui(w->pvi, w->pvi, w->start, w->p);
  mpz_set_ui(w->pvi4, 2UL);
  mpz_powm_ui(w->pvi4, w->pvi4, n_workers, w->p);

  w->quit = 0;
}

static const uint8_t dhpart2_header[76] = "PZ\x00uDHPart2 \xbe\xcf\xdd)\xcd" "ba\xda\x03\x88o\xcb\xdc\x99\x1b"
  "b\xa2\x87\xaa\xaf\xf6k>\xb2S\xae\x92\xa4" "1!Z\xc3;\xf9j8-\xd8\xb3S;\xf9j8-\xd8\xb3S\xcc\x19\x0c\x03\xff\xd7\x81\x0b;\xf9j8-\xd8\xb3S";


static void *worker_work(void *arg) {
  struct worker_ctx *w = arg;

  unsigned long x;

  struct sha256_ctx sha_ctx;

  size_t dh_exportedbytes;
  uint8_t dhexport[384];
  uint8_t dhresult[384];

  uint8_t total_hash[SHA256_DIGEST_SIZE];

  struct sha256_ctx s0_ctx;
  uint8_t s0[SHA256_DIGEST_SIZE];

  uint8_t sas[4];

  mpz_t shared;

  size_t pvi_exportedbytes;
  uint8_t pviexport[384];
  uint8_t pviresult[384];

  struct hmac_sha256_ctx hmac_ctx;
  uint8_t hmac_result[SHA256_DIGEST_SIZE];



  mpz_init(shared);

  for (x = w->start; x < w->count && !w->quit; x += n_workers) {
    /* export pvi bytes, for hmac and total hash calculation */
    pvi_exportedbytes = sizeof(pviexport);
    mpz_export(pviexport, &pvi_exportedbytes, 1, 1, 1, 0, w->pvi);
    assert(pvi_exportedbytes <= sizeof(pviexport));

    size_t zero_fill = sizeof(pviresult)-pvi_exportedbytes;
    if (zero_fill > 0) {
      memset(pviresult, 0, zero_fill);
    }
    memcpy(&pviresult[zero_fill], pviexport, pvi_exportedbytes);

    hmac_sha256_set_key(&hmac_ctx, 32, (const uint8_t *) "omg wtf!omg wtf!omg wtf!omg wtf!");
    hmac_sha256_update(&hmac_ctx, sizeof(dhpart2_header), (const uint8_t *) dhpart2_header);
    hmac_sha256_update(&hmac_ctx, 384, pviresult);
    hmac_sha256_digest(&hmac_ctx, 32, hmac_result);

    /* now compute total_hash */
    sha256_init(&sha_ctx);
    sha256_update(&sha_ctx, w->initiator_size, w->initiator_chain);
    sha256_update(&sha_ctx, sizeof(dhpart2_header), (const uint8_t *) dhpart2_header);
    sha256_update(&sha_ctx, 384, pviresult);
    sha256_update(&sha_ctx, 8, hmac_result);
    sha256_digest(&sha_ctx, sizeof(total_hash), total_hash);

    /* set total_hash in KDF_Context */
    memcpy(&w->KDF_Context[24], total_hash, sizeof(total_hash));

    /* now compute dhresult */
    /* lean mpz_powm_ui(shared, w->pvr, x, w->p); */
    /* mean assert(mpz_cmp(shared, w->shared_secret) == 0); */

    dh_exportedbytes = sizeof(dhexport);
    mpz_export(dhexport, &dh_exportedbytes, 1, 1, 1, 0, w->shared_secret);
    assert(dh_exportedbytes <= sizeof(dhexport));

    zero_fill = sizeof(dhresult)-dh_exportedbytes;
    if (zero_fill > 0) {
      memset(dhresult, 0, zero_fill);
    }
    memcpy(&dhresult[zero_fill], dhexport, dh_exportedbytes);


    /* compute next public value for initiator */
    mpz_mul(w->pvi, w->pvi, w->pvi4);
    mpz_mod(w->pvi, w->pvi, w->p);

    /* compute next shared secret */
    mpz_mul(w->shared_secret, w->shared_secret, w->pvr4);
    mpz_mod(w->shared_secret, w->shared_secret, w->p);


    sha256_init(&s0_ctx);
    sha256_update(&s0_ctx, 4, (const uint8_t *) "\x00\x00\x00\x01");
    sha256_update(&s0_ctx, sizeof(dhresult), dhresult);
    sha256_update(&s0_ctx, 13, (const uint8_t *) "ZRTP-HMAC-KDF");
    sha256_update(&s0_ctx, sizeof(w->KDF_Context), w->KDF_Context);
    sha256_update(&s0_ctx, 12, (const uint8_t *) "\x00\x00\x00\x00" "\x00\x00\x00\x00" "\x00\x00\x00\x00");
    sha256_digest(&s0_ctx, sizeof(s0), s0);

    get_sas(s0, w->KDF_Context, sas);

    /* check that 16+4 leftmost bytes are equal */
    if (memcmp(sas, w->sasval, 2) == 0 && ((sas[2] & 0xf0) == (w->sasval[2] & 0xf0))) {
      pthread_mutex_lock(&finishing_mutex);
      if (svi != 0) {
        pthread_mutex_unlock(&finishing_mutex);
        return NULL;
      }

      svi = x;
      pthread_cond_signal(&finishing_cond);
      pthread_mutex_unlock(&finishing_mutex);

      return NULL;
    }
  }

  pthread_mutex_lock(&finishing_mutex);
  finished_workers += 1;
  pthread_cond_signal(&finishing_cond);
  pthread_mutex_unlock(&finishing_mutex);

  return NULL;
}

int main(int argc, char **argv) {
  int i;
  int ret;
  unsigned long count = 0xdeadbeef;
  struct worker_ctx *workers;

  if (argc != 7) {
    fprintf(stderr, "need five arguments <initiator-chain> <pvr> <sasval> <zidi> <zidr> <workers>!\n");
    exit(1);
  }

  n_workers = atoi(argv[6]);

  workers = calloc(sizeof(*workers), n_workers);
  assert(workers != NULL);

  for (i = 0; i < n_workers; i++) {
    workers[i].index = i;
    workers[i].count = count;
    workers[i].start = (unsigned long) i;

    worker_init(&workers[i], argc, argv);
  }

  for (i = 0; i < n_workers; i++) {
    ret = pthread_create(&workers[i].tid, NULL, worker_work, &workers[i]);
    assert(ret == 0);
  }

  pthread_mutex_lock(&finishing_mutex);
  while (svi == 0 && finished_workers < n_workers) {
    pthread_cond_wait(&finishing_cond, &finishing_mutex);
  }
  pthread_mutex_unlock(&finishing_mutex);

  for (i = 0; i < n_workers; i++) {
    workers[i].quit = 1;
  }

  for (i = 0; i < n_workers; i++) {
    pthread_join(workers[i].tid, NULL);
  }

  if (finished_workers == n_workers) {
    printf("fitting svi not found\n");
  }
  else {
    printf("found svi = %ld\n", svi);
  }

  return 0;
}
