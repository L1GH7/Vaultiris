#ifndef AEAESBlock_Decryptor_DECRYPTOR_H
#define AEAESBlock_Decryptor_DECRYPTOR_H

#include "cracking_options.h"

#include <string>
#include <openssl/evp.h>

class AESBlock_Decryptor
{
public:
    AESBlock_Decryptor();
    bool init( const char * hexBytes, const uint16_t & hexBytesSize );
    bool decode(const EVP_CIPHER * cipher, sCrackingOptions *key);
    void print();

    std::string getId() const;
    void setId(const std::string &value);

private:
    std::string id;
    uint8_t * encoded;
    int32_t encodedLenght;
    uint8_t * decoded;
    int32_t decodedLenght;
};

#endif // AEAESBlock_Decryptor_DECRYPTOR_H
