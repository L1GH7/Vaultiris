# d-os-descriptor

## Summary

The USB Gadget Subsystem includes security issues in the OS descriptor handling
section of composite_setup function (composite.c). Processing of properly
crafted control transfer request messages result in device crash due to null
pointer dereference or memory corruption.

## Description

The OS descriptor handling section of composite_setup for interface recipient
is implemented as follows.

```
case USB_RECIP_INTERFACE:
    if (w_index != 0x5 || (w_value >> 8))
        break;
    interface = w_value & 0xFF;
    buf[6] = w_index;
    count = count_ext_prop(os_desc_cfg,
        interface);
    put_unaligned_le16(count, buf + 8);
    count = len_ext_prop(os_desc_cfg,
        interface);
    put_unaligned_le32(count, buf);
    value = w_length;
    if (w_length > 0x0A) {
        value = fill_ext_prop(os_desc_cfg,
                        interface, buf);
        if (value >= 0)
            value = min_t(u16, w_length, value);
}
break;
```

The interface variable is derived from w_value and later utilized to index usb_configuration->interface array
in count_ext_prop, len_ext_prop and fill_ext_prop functions. Since c->interface array has the size of
MAX_CONFIG_INTERFACES (16) elements and interface variable is not validated in neither composite_setup's
OS descriptor handling section nor the called functions this allows an attacker to index the c->interface array
past the actual boundaries. In case interface variable has a value greater or equal to MAX_CONFIG_INTERFACES the
endpoint should be stalled.

In certain cases, depending on actual memory content indexing past c->interface array may trigger buffer overflow
of req->buf via fill_ext_prop when wLength is greater than 0x0A. If sum of ext_prop->name_len, ext_prop->data_len,
14 and 10 overflows int the count + n >= USB_COMP_EP0_OS_DESC_BUFSIZ condition would not be met allowing overflow
via memcpy in usb_ext_prop_put_binary. Yet the probability of such situation seems pretty low.

Furthermore the functions count_ext_prop, len_ext_prop and fill_ext_prop are missing validation if the *usb_function
retrieved from c->interface array is actually valid resulting in null pointer dereference. When the retrieved
usb_function pointer is null the endpoint should be stalled.

```
static int count_ext_prop(struct usb_configuration *c, int interface)
{
	struct usb_function *f;
	int j;

	f = c->interface[interface];
	for (j = 0; j < f->os_desc_n; ++j) {
		struct usb_os_desc *d;

	    if (interface != f->os_desc_table[j].if_id)
	    	continue;
	    d = f->os_desc_table[j].os_desc;
	    if (d && d->ext_compat_id)
	    	return d->ext_prop_count;
    }
return 0;
}
```

## Impact

Linux (and Android) devices exposing usb gadgets with OS descriptor support
may be arbitrarily crashed by a malicious host by means of a single control
transfer message.

## CVE

[CVE-2022-25258](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2022-25258)

## Patch

A patch addressing the described issue was accepted and is now available in
supported kernel versions. For more information consult the below link.

[USB: gadget: validate interface OS descriptor requests](https://github.com/torvalds/linux/commit/75e5b4849b81e19e9efe1654b30d7f3151c33c2c)
