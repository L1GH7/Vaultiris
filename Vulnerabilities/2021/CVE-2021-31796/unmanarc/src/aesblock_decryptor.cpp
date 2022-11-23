#include "aesblock_decryptor.h"

#include <openssl/aes.h>
#include <string.h>

AESBlock_Decryptor::AESBlock_Decryptor()
{
    encoded=nullptr;
    encodedLenght=0;
    decoded=nullptr;
    decodedLenght=0;
}

bool AESBlock_Decryptor::init( const char * hexBytes, const uint16_t & hexBytesSize )
{
    uint16_t retSize = hexBytesSize/2;

    encodedLenght = 0;
    encoded = new uint8_t[retSize+16];
    decoded = new uint8_t[retSize+128];

    if (!encoded) return encoded;
    memset(encoded,0,retSize+16);

    for ( uint16_t pos = 0; pos<hexBytesSize; pos+=2 )
    {
        unsigned int c=0;
        if (sscanf(hexBytes+pos, "%02X", (unsigned int *)&c) != 1)
        {
            return false;
        }
        encoded[pos/2]=(uint8_t)c;
        encodedLenght++;
    }
    return true;
}

// Adapted from https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
bool AESBlock_Decryptor::decode(const EVP_CIPHER * cipher, sCrackingOptions *crackingOptions)
{
    int ok = false;
    for ( const sAesKey & aesKey : crackingOptions->aesKeys )
    {
        EVP_CIPHER_CTX *ctx;

        int dec_len;

        if(!(ctx = EVP_CIPHER_CTX_new()))
        {
            fprintf(stderr, "ERROR: creating EVP Cipher CTX Context.\n");
            continue;
        }

        if(EVP_DecryptInit_ex(ctx, cipher, nullptr, aesKey.aeskey, nullptr)!=1)
        {
            fprintf(stderr, "ERROR: initializing cipher key.\n");
            continue;
        }

        if(EVP_DecryptUpdate(ctx, decoded, &dec_len, encoded, encodedLenght)!=1)
        {
            fprintf(stderr, "ERROR: decrypting information.\n");
            continue;
        }

        decodedLenght = dec_len;
        if(EVP_DecryptFinal_ex(ctx, decoded+dec_len, &dec_len) != 1)
        {
            if (crackingOptions->aesKeys.size()==1)
            {
                fprintf(stderr, "WARNING: No working AES Encryption Key found.\n");
            }
            else
            {
                fprintf(stderr, "Attempt to crack AES Encryption Key with ClientApp=%s failed.\n", aesKey.sClientApp.c_str());
            }
        }
        else
        {
            decodedLenght += dec_len;

            if (!aesKey.sClientApp.empty())
            {
                fprintf(stderr,"Working 256-bit AES Encryption Key Cracked (with ClientApp: %s)\n", aesKey.sClientApp.c_str());
            }
            else
            {
                fprintf(stderr,"Working 256-bit AES Encryption Key:\n");
            }
            BIO_dump_fp (stderr, (char *)aesKey.aeskey, AES_KEY_SIZE);


            if (decodedLenght<36)
            {
                fprintf(stderr, "WARNING: error in decoded size.\n");
                continue;
            }
            else
            {
                ok = true;
            }
            break;
        }
        EVP_CIPHER_CTX_free(ctx);
    }
    return ok;
}

void AESBlock_Decryptor::print()
{
    fprintf(stderr,"\033[1;34m");

    fprintf(stderr,"<%s> Encoded Data (%d bytes):\n", id.c_str(), encodedLenght);
    BIO_dump_fp (stderr, (char *)encoded, encodedLenght);
    fprintf(stderr,"<%s> Decoded Data (%d bytes, payload=%d bytes):\n", id.c_str(), decodedLenght, decodedLenght<36?0:decodedLenght-36);
    BIO_dump_fp (stderr, (char *)decoded, decodedLenght);

    fflush(stderr);
    fflush(stdout);

    fprintf(stderr,"\033[0m");

    if (decodedLenght>36)
    {
        decoded[decodedLenght-20]=0;
        printf("%s\n",decoded+16);
    }
}

std::string AESBlock_Decryptor::getId() const
{
    return id;
}

void AESBlock_Decryptor::setId(const std::string &value)
{
    id = value;
}
