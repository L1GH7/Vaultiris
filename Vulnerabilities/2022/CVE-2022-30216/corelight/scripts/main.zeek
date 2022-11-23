module CVE_2022_30216_Detection;

@load base/protocols/dce-rpc
@load base/frameworks/notice

export {
	redef enum Notice::Type += {
		ExploitAttempt,
		ExploitSuccess
	};
}

event dce_rpc_request_stub(c: connection, fid: count, ctx_id: count, opnum: count, stub: string)
	{
	if ( opnum != 74 || ! c?$dce_rpc || ! c$dce_rpc?$endpoint )
		return;

	if ( c$dce_rpc$endpoint == "srvsvc" )
		{
		local resp_h = c$id$resp_h;
		local orig_h = c$id$orig_h;
		local clean_stub = find_last(gsub(stub, /\x00/, ""), /\\((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\/);
		if ( clean_stub != "" )
			NOTICE([$note=ExploitAttempt, $conn=c, 
				$msg=fmt("Attempted CVE-2022-30216 exploit: %s attempted exploit against %s relaying to %s", 
					orig_h, resp_h, clean_stub[1:-1]),
				$identifier=cat(orig_h, resp_h)]);
		else
			NOTICE([$note=ExploitAttempt, $conn=c, 
				$msg=fmt("Attempted CVE-2022-30216 exploit: %s attempted exploit against %s", 
					orig_h, resp_h),
				$identifier=cat(orig_h, resp_h)]);
		}

	}

event dce_rpc_response_stub(c: connection, fid: count, ctx_id: count, opnum: count, stub: string)
	{
	if ( opnum != 74 || ! c?$dce_rpc_backing )
		return;

	local contains_srvsvc = F;
	for ( backing in c$dce_rpc_backing )
		{
		if ( c$dce_rpc_backing[backing]$info$endpoint == "srvsvc" )
			{
			contains_srvsvc = T;
			break;
			}
		}
	
	if ( ! contains_srvsvc )
		return;

	local resp_h = c$id$resp_h;
	local orig_h = c$id$orig_h;
	local clean_stub = find_last(gsub(stub, /\x00/, ""), /\\((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\/);
	
	if ( clean_stub != "" )
		NOTICE([$note=ExploitSuccess, $conn=c, 
			$msg=fmt("Successful CVE-2022-30216 exploit: %s exploited %s relaying to %s", 
					orig_h, resp_h, clean_stub[1:-1]),
			$identifier=cat(orig_h, resp_h)]);
	else
		NOTICE([$note=ExploitSuccess, $conn=c, 
			$msg=fmt("Successful CVE-2022-30216 exploit: %s exploited %s", 
					orig_h, resp_h),
			$identifier=cat(orig_h, resp_h)]);

	}
