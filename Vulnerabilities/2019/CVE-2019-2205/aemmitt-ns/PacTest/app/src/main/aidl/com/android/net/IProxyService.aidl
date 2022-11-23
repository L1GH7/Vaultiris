// IProxyService.aidl
package com.android.net;

// Declare any non-default types here with import statements

interface IProxyService
{
    String resolvePacFile(String host, String url);
    oneway void setPacFile(String scriptContents);
    oneway void startPacSystem();
    oneway void stopPacSystem();
}
