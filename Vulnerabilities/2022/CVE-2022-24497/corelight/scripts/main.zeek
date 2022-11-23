module CVE202224497;

export {
	redef enum Notice::Type += {
        POTENTIAL_CVE_2022_24497,
    };
}

function CVE202224497::match_portmap(state: signature_state, data: string): bool
	{
	NOTICE([$note=POTENTIAL_CVE_2022_24497, $conn=state$conn,
	$identifier=cat(state$conn$uid),
	$msg=fmt("Possible CVE-2022-24497 exploit attempt.  An RPC portmap getport and portmap dump were observed.")]);

	return T;
	}