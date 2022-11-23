import python

from Call call
where call.getFunc().(Attribute).getName() = "log_opt_values"
	and call.getLocation().getFile().getBaseName().matches("keystone-all") // just restricting the file to be queried
	// and call.getLocation().getStartLine() = 89
select call

