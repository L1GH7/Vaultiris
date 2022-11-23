package com.rgigroup.quarkuscve

import org.eclipse.microprofile.graphql.GraphQLApi
import org.eclipse.microprofile.graphql.Query


@GraphQLApi
class GraphQLEndpoint {

    @org.eclipse.microprofile.rest.client.inject.RestClient lateinit var client: RestClient


    @Query
    fun exampleQuery(): String  {
        return this.client.getExample()
    }
}
