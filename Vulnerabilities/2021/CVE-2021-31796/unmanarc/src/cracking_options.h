#ifndef CRACKING_OPTIONS_H
#define CRACKING_OPTIONS_H

#include <string>
#include <string.h>

#include <boost/algorithm/string.hpp>

#include "b64ops.h"

#define AES_KEY_SIZE 32

#define NOT_PROVIDED "NOT PROVIDED"
#define PROVIDED "OK"

struct sAesKey
{
    sAesKey()
    {
        memset(aeskey,0,AES_KEY_SIZE);
    }
    std::string sClientApp;
    uint8_t aeskey[AES_KEY_SIZE];
};

struct sCrackingOptions
{
    sCrackingOptions(uint uVerificationsFlag)
    {
        usingClientApp=false;
        usingAppPath=false;
        usingClientIP=false;
        usingClientHostname=false;
        usingOSUser=false;

        std::string sAdditionalInformation;

        if (uVerificationsFlag<16)
        {
            fprintf(stderr, "WARNING: \"verificationFlags\" field should start in 16 (now=%d)",uVerificationsFlag);
        }
        uVerificationsFlag-=16;

        usingClientApp      = ((uVerificationsFlag&0x1) != 0);
        usingAppPath        = ((uVerificationsFlag&0x2) != 0);
        usingClientIP       = ((uVerificationsFlag&0x4) != 0);
        usingOSUser         = ((uVerificationsFlag&0x8) != 0);
        usingClientHostname = ((uVerificationsFlag&0x20) != 0);
    }

    bool validateCrackingOptions()
    {
        bool r = true;
        if (usingClientApp)
        {
            fprintf(stderr, ">>> Verifying \"ClientApp\" Field => %s\n",  sClientApp.empty()?NOT_PROVIDED:PROVIDED );
            //r&=!sClientApp.empty();
        }
        if (usingAppPath)
        {
            fprintf(stderr, ">>> Verifying \"AppPath\" Field => %s\n",  sAppPath.empty()?NOT_PROVIDED:PROVIDED );
            r&=!sAppPath.empty();
        }
        if (usingClientIP)
        {
            fprintf(stderr, ">>> Verifying \"ClientIP\" Field => %s\n",  sClientIP.empty()?NOT_PROVIDED:PROVIDED );
            r&=!sClientIP.empty();
        }
        if (usingClientHostname)
        {
            fprintf(stderr, ">>> Verifying \"ClientHostname\" Field => %s\n",  sClientHostname.empty()?NOT_PROVIDED:PROVIDED );
            r&=!sClientHostname.empty();
        }
        if (usingOSUser)
        {
            fprintf(stderr, ">>> Verifying \"OSUser\" Field => %s\n",  sOSUser.empty()?NOT_PROVIDED:PROVIDED );
            r&=!sOSUser.empty();
        }

        return r;
    }


    std::string composeKeyPayload(const std::string & _sClientAppType)
    {
        std::string keyPayload;


        if (!_sClientAppType.empty()) keyPayload+=getSHA1Base64Digest(boost::to_lower_copy(_sClientAppType)); //

        if (usingAppPath) keyPayload+=boost::to_lower_copy(sAppPath); //
        if (usingClientIP) keyPayload+=sClientIP; //
        if (usingClientHostname) keyPayload+=boost::to_lower_copy(sClientHostname); //
        if (usingOSUser) keyPayload+=boost::to_lower_copy(sOSUser); //

        keyPayload+=sAdditionalInformation;


        return keyPayload;
    }

    std::string sAdditionalInformation;

    bool usingClientApp;
    std::string sClientApp;

    bool usingAppPath;
    std::string sAppPath;

    bool usingClientIP;
    std::string sClientIP;

    bool usingOSUser;
    std::string sOSUser;

    bool usingClientHostname;
    std::string sClientHostname;

    std::list<sAesKey> aesKeys;
};






#endif // CRACKING_OPTIONS_H
