package com.rgigroup.quarkuscve

import io.vertx.ext.web.RoutingContext
import org.jose4j.jwt.JwtClaims
import java.nio.charset.StandardCharsets
import java.util.*
import javax.enterprise.context.RequestScoped
import javax.inject.Inject


interface Context {
    fun getTenant(): String;
    fun getUser(): String
}

@RequestScoped
class JWTAwareContext @Inject constructor(
    private val routingContext: RoutingContext
): Context {
    override fun getTenant(): String {
        return parseClaims()?.getClaimValueAsString("realm") ?: "WRONG"
    }

    override fun getUser(): String {
        return parseClaims()?.getClaimValueAsString("preferred_username") ?: "WRONG"
    }

    fun parseClaims(): JwtClaims? {
        val rawToken = this.routingContext.request().headers().get("Authorization")?.replace("Bearer ", "")
        if (rawToken != null) {
            val jsonClaims = String(Base64.getUrlDecoder().decode(rawToken.split(".")[1]), StandardCharsets.UTF_8)
            return JwtClaims.parse(jsonClaims)
        }
        throw RuntimeException("missing token");
    }
}
