#include <stdint.h>

// dameware message types
#define MSG_TYPE_VERSION				0x00001130
#define MSG_CLIENT_INFORMATION_V7		0x00011171
#define MSG_TYPE_RSA_CRYPTO_C_INIT		0x000105b8
#define MSG_000105b9					0x000105b9
#define MSG_REGISTRATION_INFORMATION	0x0000b004
#define MSG_SOCKET_ADD					0x00010626
#define MSG_D6E2						0x0000D6E2
#define MSG_SMARTCARD_COMMAND			0x0000D6F6

struct smart_card_request {
	uint32_t type;
	uint32_t unk;
	double one;
	double two;
	uint32_t three;
	uint32_t four;
	uint32_t five;
	uint32_t six;
};

struct dameware_hdr {
	uint32_t type;
	uint32_t unk;
	uint32_t size;
};