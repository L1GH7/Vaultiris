package com.rgigroup.quarkuscve

import org.eclipse.microprofile.rest.client.annotation.RegisterProvider
import org.eclipse.microprofile.rest.client.inject.RegisterRestClient
import javax.ws.rs.GET
import javax.ws.rs.Path

@RegisterRestClient(baseUri = "http://localhost:8080")
@RegisterProvider(ClientInterceptor::class)
interface RestClient {

    @GET
    @Path("/example")
    fun getExample(): String
}
