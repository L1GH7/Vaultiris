package com.rgigroup.quarkuscve

import javax.ws.rs.GET
import javax.ws.rs.Path

@Path("{tenantId}/example")
class JaxrsResouce {


    @GET
    fun example(): String {
        return "hello!"
    }
}
