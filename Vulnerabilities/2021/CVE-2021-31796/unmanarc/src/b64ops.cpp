#include "b64ops.h"

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/algorithm/string.hpp>

#include <openssl/sha.h>

using namespace std;

// copied from https://stackoverflow.com/questions/7053538/how-do-i-encode-a-string-to-base64-using-only-boost
string encode64(const string &val)
{
    using namespace boost::archive::iterators;
    using It = base64_from_binary<transform_width<string::const_iterator, 6, 8>>;
    auto tmp = string(It(begin(val)), It(end(val)));
    return tmp.append((3 - val.size() % 3) % 3, '=');
}

string getSHA1Base64Digest(string appType)
{
    uint8_t hash[SHA_DIGEST_LENGTH];

    SHA_CTX ctx;
    SHA1_Init(&ctx);
    SHA1_Update(&ctx, appType.c_str(), appType.size());
    SHA1_Final(hash, &ctx);

    return encode64( string((char *)hash,SHA_DIGEST_LENGTH) );
}
