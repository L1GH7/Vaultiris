module CVE202226937;

export {
	redef enum Notice::Type += {
		CVE_2022_26937_Attempt
	};
}

# Set to save NLM endpoints
global nlm_endpoints: set[addr, addr] &write_expire=300 secs &backend=Broker::MEMORY; # This will get the info to the workers

# Called on every large getaddr signature match.
function CVE202226937::match_getaddr(state: signature_state, data: string): bool
	{
	if ([state$conn$id$resp_h, state$conn$id$orig_h] !in nlm_endpoints) # Swap resp and orig because victim creates connection.
		return F;

	NOTICE( [$note=CVE_2022_26937_Attempt, $conn=state$conn, 
		$msg=fmt("Potential NFS CVE-2022-26937 exploit attempt: %s attempted exploit against %s", 
		state$conn$id$resp_h, state$conn$id$orig_h), # The resp is the attacker machine
		$identifier=cat(state$conn$id$orig_h, state$conn$id$resp_h)]);

	return T;
	}

# Called on every NLM signature match.
function CVE202226937::match_nlm(state: signature_state, data: string): bool
	{
	add nlm_endpoints[state$conn$id$orig_h, state$conn$id$resp_h];
	return T;
	}