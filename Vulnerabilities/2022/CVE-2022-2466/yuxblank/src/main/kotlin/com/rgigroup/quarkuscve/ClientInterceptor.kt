package com.rgigroup.quarkuscve

import org.slf4j.Logger
import org.slf4j.LoggerFactory
import javax.enterprise.context.ApplicationScoped
import javax.inject.Inject
import javax.ws.rs.client.ClientRequestContext
import javax.ws.rs.client.ClientRequestFilter
import javax.ws.rs.core.UriBuilder

@ApplicationScoped
class ClientInterceptor : ClientRequestFilter {

    private val logger: Logger = LoggerFactory.getLogger(ClientInterceptor::class.java);

    @Inject lateinit var context: Context;

    override fun filter(requestContext: ClientRequestContext?) {
        val tenant = context.getTenant()
        requestContext!!.uri = UriBuilder
            .fromUri(requestContext.uri)
            .replacePath(tenant.plus(requestContext.uri.path))
            .build()
        requestContext.headers.putSingle("USER", context.getUser())

        logger.info("Current tenant {}, current user: {}", context.getTenant(), context.getUser())

    }
}
