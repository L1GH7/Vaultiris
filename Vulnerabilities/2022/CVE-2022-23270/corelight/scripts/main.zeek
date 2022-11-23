module CVE202223270;

export {
	redef enum Notice::Type += {
		CVE_2022_23270_Attempt,
		CVE_2022_23270_Success
	};
}

redef record connection += {
	cve_2022_23270_match: bool &default=F;
};

# Called on every signature match.
function CVE202223270::match(state: signature_state, data: string): bool
	{
	NOTICE( [$note=CVE_2022_23270_Attempt, $conn=state$conn, 
		$msg=fmt("Potential PPTP CVE-2022-23270 exploit attempt: %s attempted exploit against %s", state$conn$id$orig_h, state$conn$id$resp_h),
		$identifier=cat(state$conn$id$orig_h, state$conn$id$resp_h)]);
	state$conn$cve_2022_23270_match = T;
	return T;
	}

event connection_state_remove(c: connection)
	{
	if (c$cve_2022_23270_match == F)
		return;

	if (c?$conn && c$conn?$conn_state && "S1" == c$conn$conn_state)
		NOTICE( [$note=CVE_2022_23270_Success, $conn=c, 
			$msg=fmt("PPTP CVE-2022-23270 exploit success: %s exploited %s", c$id$orig_h, c$id$resp_h),
			$identifier=cat(c$id$orig_h, c$id$resp_h)]);
	
	}