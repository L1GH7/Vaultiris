module CVE202224491;

export {
	redef enum Notice::Type += {
        POTENTIAL_CVE_2022_24491,
    };
}

global SetEndPoints: set[addr] &write_expire=3mins;

event to_workers_set(resp_h: addr)
    {
@if ( Cluster::local_node_type() == Cluster::MANAGER ||
      Cluster::local_node_type() == Cluster::PROXY )
        Broker::publish(Cluster::worker_topic, to_workers_set, resp_h);
@else
	add SetEndPoints[resp_h];
@endif
    }

function CVE202224491::match_set(state: signature_state, data: string): bool
	{
@if (Cluster::is_enabled())
	local pt = Cluster::rr_topic(Cluster::proxy_pool, "CVE-2022-24491");
	Broker::publish(pt, to_workers_set, state$conn$id$resp_h);
@else
	add SetEndPoints[state$conn$id$resp_h];
@endif
	return T;
	}

function CVE202224491::match_dump(state: signature_state, data: string): bool
	{
	if (state$conn$id$resp_h in SetEndPoints)
		NOTICE([$note=POTENTIAL_CVE_2022_24491, $conn=state$conn,
		$identifier=cat(state$conn$id$orig_h,state$conn$id$resp_h),
		$msg=fmt("Possible CVE-2022-24491 exploit attempt.  An RPC portmap set with a RPC portmap dump was observed.")]);

	return T;
	}
