# cve_2022_21882
If you read the below closely you will see that I note that the patch does not prevent NtUserConsoleControl from being called when the calling process is not csrss.exe since a telemetry assert isn't "really" an assert. Previously, this was sufficient to fix the issue since xxxClientAllocWindowClassExtraBytes was *not* called outside of xxxCreateWindowEx. However, this changed in late 2021, and now it's also called in the system wndprocs as well (xxxSwitchWndProc, xxxSBWndProc, xxxMenuWindowProc, xxxDesktopWndProc), ironically as a fix for another bug (I believe it's tracked as cve 2021 34516). However before January the xe8 flag was not properly checked, so we can just tweak a few things and reuse the old exploit. To fix it definitively, I think calling NtUserConsoleControl outside of CSRSS should generate an exception rather than just a telemetry assert(and I'm pretty sure it was this way at some point in Windows' long history, but maybe there's some legacy compat reason they can't). You can see here where the vulnerable code was added in July 2021

![ScreenShot](https://github.com/jessica0f0116/cve_2022_21882-cve_2021_1732/raw/main/patch0.png)

# cve_2021_1732

In Feb. 2021, Microsoft released a patch for a vulnerability in win32kfull.sys, designated CVE-2021-1732, which concerns the handling of window class extra bytes. To briefly recall the history of win32 exploits, there were a long string of publicly-disclosed vulns in the 2000's that followed a similar pattern, described in Tarjei Mandt's paper on win32 callbacks[1]: an attacker overwrites some function pointer in the KernelCallbackTable with a pointer to an attacker-designed callback, triggers the callback (ie by window creation, destruction, message-handling, etc), then leverages its behavior to corrupt the class attributes of a window or other desktop object. Since then, MS has mitigated some of these scenarios and introduced some separation of concern by changing the cbWndExtra/cbWndExtraSize members of the window class struct; now there are separate memory handles (and associated size member) for client and server window extra bytes. A bit in the window state flags, bServerSideWindowProc, indicates which one should be used. This change, however, introduced a lot of new code complexity (ie more vulns); and allocating the client window extra bytes requires a call to user-mode RtlAllocateHeap, which introduces the opportunity for more callback shenanigans. The "Smash the Ref" paper is a really good overview of how this can be used to mess with reference counting in a lot of fun ways[2]. But there are plenty of other possibilities and this vuln presents one of them. This patch made changes to several functions in win32kfull (xxxEventWndProc, xxxSetWindowLongPtr, xxxSetWindowLong, NtUserSetWindowFNID) to add new telemetry or make the extra bytes handling more, robust. But there are two particular functions which are immediately relevant to this exploit. First, there was a change to NtUserConsoleControl, where this code is inserted:

```
  lVar1 = PsGetCurrentProcess();
  if (lVar1 != *(longlong *)gpepCSRSS_exref) {
    MicrosoftTelemetryAssertTriggeredNoArgsKM();
  }
```
  
This simply calls MicrosoftTelemetryAssertTriggeredNoArgsKM if the NtUserConsoleControl is not being called from CSRSS.exe ("gpepCSRSS_exref" is the EPROCESS of CSRSS), since it's really intended to be for system use. However this isn't "actually" an assert; it just notifies Windows telemetry that a telemetry assertion failed so it can collect some info. Let's look at why this is significant, though.

NtUserConsoleControl calls xxxConsoleControl which will change the pointer to the allocated client extra bytes, at \*(pWnd + 0x28) + 0x128, to an offset:

```
    if (*(longlong *)(*tagWND + 0x128) != 0) {
      peprocess = PsGetCurrentProcess();
      cbWndClientExtra = *(void **)(*tagWND + 0x128);
      memcpy(heaphandle,cbWndClientExtra,(longlong)*(int *)(*tagWND + 0xc8));
      if ((*(uint *)(peprocess + 0x464) & 0x40000008) == 0) {
        xxxClientFreeWindowClassExtraBytes
                  (lVar4,*(undefined8 *)(*(longlong *)(pWnd + 0x28) + 0x128));
      }
    }
    *cbWndClientExtra = heaphandle - desktopheapbase
```

then a flag is set to indicate that it is now an offset:

```
*(uint *)(*tagWND + 0xe8) = *(uint *)(*tagWND + 0xe8) | 0x800;
```

To explain further, when a window is created, xxxCreateWindowEx issues a user-mode callback via KeUserModeCallback mechanism to \_\_\_xxxClientAllocWindowClassExtraBytes in user32.dll. This function allocates an object in the win32 user-mode desktop heap for the extra data using RtlAllocateHeap and returns a pointer to the object back to the caller via NtCallbackReturn. xxxCreateWindowEx writes this pointer to pWnd->WW+0x128.

There is this section in xxxCreateWindowEx, where cbWndServerExtra and cbWndClientExtra are filled based on the CLS structure attributes:

```
*(uint *)(pWnd->tagWND + 0x1c) = Style & 0xefffffff;
*(uint *)(pWnd->tagWND + 0x18) = ExStyle & 0xfdf7ffff;
//cbWndClientExtraSize
*(long *)(pWnd->tagWND + 0xc8) = *(long *)(*(longlong *)(*pCLS + 8) + 0x50);
//cbWndServerExtraSize
*(long *)(pWnd->tagWND + 0xfc) = *(long *)(*(longlong *)(*pCLS + 8) + 0x54);
```

Then later, in this loop, some space is allocated in user-mode for the class extra bytes, but only if the value which was previously copied to pWnd->tagWND+0xfc is equal to 0; then, the return value of xxxClientAllocWindowClassExtraBytes(), is placed at \*(pWnd + 0x28) + 0x128:

```
//if tagCLS->cbWndServerExtraSize == 0
if (*(int *)(*(longlong *)(*local_418[0] + 8) + 0x54) == 0) {
  ....
  ....
  ....
  if (*(int *)(tagWND + 0xc8) != 0) {
    pcbWndClientExtra = xxxClientAllocWindowClassExtraBytes();
    *(undefined8 *)(tagWND + 0x128) = pcbWndClientExtra;
```

Right after the end of this loop, if the value of (\*pWnd->tagWND)+0xfc is not 0, then space is allocated in the kernel heap and a pointer to this space is placed at pWnd+0x118:

```
pcbWndServerExtra = Win32AllocPoolZInit((ulonglong)*(uint *)(*(longlong *)(*local_418[0] + 8) +0x54),0x73777355);
*pWnd+0x118 = pcbWndServerExtra;
```

Further, xxxConsoleControl will change the heap handle at \*(pWnd + 0x28) + 0x128 into an offset from the desktop heap base. If it's successful, it sets a flag that determines how xxxSetWindowLong, xxxSetWindowLongPtr, xxxSetWindowData handle the extra bytes

```
*(uint *)(*tagWND + 0xe8) = *(uint *)(*tagWND + 0xe8) | 0x800;
```

An attacker can patch the KernelCallbackTable entry for \_\_\_xxxClientAllocWindowClassExtraBytes with a function that calls NtUserConsoleControl to set the pointer to cbWndClientExtra at \*(pWnd + 0x28)+0x128 to an offset, and then NtCallbackReturn with an arbitrary value. xxxCreateWindowEx will overwrite the previous offset at \*(pWnd+0x28)+0x128 by NtUserConsoleControl with the value returned from NtCallbackReturn. However, the flag at \*(pWnd+0x28)+0xe8 remains set and \*(pWnd+0x28)+0x128 is later presumed to still be a valid offset after the window is finished creating; if you change the offset to the offset of another window from the start of the desktop heap (the address of the wnd handle can be obtained by calling HMValidateHandle), you can overwrite that window's system-reserved bytes, leading to a nice arbitrary read/write primative. Here's how this was fixed in the February patch update, in xxxCreateWindowEx:

```
pExtraBytes = xxxClientAllocWindowClassExtraBytes();
if ((*(uint *)(tagWND + 0xe8) & 0x800) != 0) {
  MicrosoftTelemetryAssertTriggeredNoArgsKM();
  //tagWND+0xe8 now zeroed out
  tagWND = savedtagWND;
}
*(longlong *)(tagWND + 0x128) = pExtraBytes;
```

If xxxClientAllocWindowClassExtraBytes doesn't return a valid user-mode heap handle (ie because the callback was tampered with), and the flag indicating that it's an offset is unset, window creation will fail further down the line, so this fixes the issue. But anyway, there were already two good writeups on this with lots of nice visualizations[3][4] so I won't bore any further :p

[1]https://dl.packetstormsecurity.net/papers/win/mandt-win32k-paper.pdf
[2]https://www.ragestorm.net/Win32k%20Smash%20the%20Ref.pdf
[3]https://ti.dbappsecurity.com.cn/blog/index.php/2021/02/10/windows-kernel-zero-day-exploit-is-used-by-bitter-apt-in-targeted-attack/
[4]https://iamelli0t.github.io/2021/03/25/CVE-2021-1732.html

![Capture0](https://user-images.githubusercontent.com/72535217/113786008-49483900-9706-11eb-9bb6-c7f1b6cbadb4.PNG)
