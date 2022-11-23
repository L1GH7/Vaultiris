#include "aeskey_decoder.h"

#include <openssl/sha.h>
#include <openssl/bio.h>

#include <string>

#include <string.h>

using namespace std;


/*
 * Obtained from https://docs.cyberark.com/Product-Doc/OnlineHelp/PAS/Latest/en/Content/PASIMP/CreateCredFile-Utility.htm
        Central Policy Manager	CPM
        Password Vault Web Access	PVWA
        Password Vault Web Access application user	PVWAApp
        OPM and Credential Provider	AppPrv
        Privileged Session Manager application user	PSMApp
        CyberArk Replicator/Restore/Prebackup	CABACKUP
        Disaster Recovery Vault	DR
        Event Notification Engine	ENE
        PrivateArk Client	WINCLIENT, GUI
        CyberArk CLI	PACLI
        CyberArk ActiveX API	XAPI
        CyberArk .Net API	NAPI
        Export Vault Data	EVD
        CyberArk Encryption Utility	CACrypt
*/

void decodeAESKey(sCrackingOptions *crackingOptions)
{

    list<string> clientApps;

    if (crackingOptions->usingClientApp)
    {
        if (!crackingOptions->sClientApp.empty())
            clientApps.push_back(crackingOptions->sClientApp);
        else
        {
            fprintf(stderr,"WARNING: value not provided \"ClientApp\" field, cracking it\n");
            clientApps = {"CABACKUP", "DR", "ENE", "GUI", "WINCLIENT", "WINCLIENT, GUI", "PACLI", "XAPI", "NAPI", "EVD", "CACrypt","CPM","PVWA","PVWAApp", "AppPrv", "PSMApp",  ""};
        }
    }
    else
    {
        clientApps.push_back("");
    }

    for (const std::string & sClientApp : clientApps)
    {
        string keyPayload = crackingOptions->composeKeyPayload(sClientApp);
        sAesKey aesKey;

        if (keyPayload.size()<40)
        {
            fprintf(stderr, "WARNING: AdditionalInformation field may be corrupted (bytes=%lu, required=40), continuing...\n", keyPayload.size());
        }
        memset(aesKey.aeskey,0,AES_KEY_SIZE);
        uint8_t hash[SHA_DIGEST_LENGTH], hash2[SHA_DIGEST_LENGTH];
        SHA_CTX ctx, ctx2;
        SHA1_Init(&ctx);
        SHA1_Init(&ctx2);

        // Hash each piece of data as it comes in:
        SHA1_Update(&ctx, keyPayload.c_str(), strlen(keyPayload.c_str()));
        SHA1_Update(&ctx, "\0\0\0\0", 4);
        SHA1_Final(hash, &ctx);

        SHA1_Update(&ctx2, keyPayload.c_str(), strlen(keyPayload.c_str()));
        SHA1_Update(&ctx2, "\0\0\0\1", 4);
        SHA1_Final(hash2, &ctx2);

        // Composing key:
        memcpy(aesKey.aeskey,hash, SHA_DIGEST_LENGTH);
        memcpy(aesKey.aeskey+SHA_DIGEST_LENGTH,hash2, AES_KEY_SIZE-SHA_DIGEST_LENGTH);

        aesKey.sClientApp = sClientApp;

        crackingOptions->aesKeys.push_back(aesKey);
    }
}

