# CVE-2022-2466 Request-Context-not-terminated-with-GraphQL

## How to run

1) run mvn:quarkus:dev
2) run the following http requests

RUN THIS TWICE
```http request
POST http://localhost:8080/graphql
Content-Type: application/graphql
Authorization: Bearer eyJhbGciOiJSUzI1NiIsInR5cCIgOiAiSldUIiwia2lkIiA6ICJfN3gxcGhILXpKcFpoMVFfaXAtVHRVR3dNTU52OWIyOE9Wcnczc0prU2lrIn0.eyJleHAiOjE2NTg3NTA5NjUsImlhdCI6MTY1ODc1MDY2NSwiYXV0aF90aW1lIjoxNjU4NzUwNjY0LCJqdGkiOiJkNGVhNzA5ZC01YmZkLTRjYTQtYWMzMS05MWVkNTk3YmIwOGUiLCJpc3MiOiJodHRwczovL3BwbmV4dC10ZXN0LmF3cy5yZ2lncm91cC5jb20vYXV0aC9yZWFsbXMvYWNtZXRlc3QiLCJhdWQiOiJhY2NvdW50Iiwic3ViIjoiNWE5MjdjOTctZGViMi00M2Q5LWI3OTgtODFmYjRlMTdkOTgwIiwidHlwIjoiQmVhcmVyIiwiYXpwIjoiY2xpZW50LWxvY2FsIiwibm9uY2UiOiIyZTNjYzljOC02MWI3LTQ1NDYtYmI0YS1iNTY2MTYxZmIxMWEiLCJzZXNzaW9uX3N0YXRlIjoiMWZkMmQ3ZGQtZDJlYS00NzVjLTg4ZDQtYWExZDljYTk5OTcwIiwiYWNyIjoiMSIsImFsbG93ZWQtb3JpZ2lucyI6WyIqIl0sInJlYWxtX2FjY2VzcyI6eyJyb2xlcyI6WyJvZmZsaW5lX2FjY2VzcyIsInVtYV9hdXRob3JpemF0aW9uIiwiZGVmYXVsdC1yb2xlcy1hY21ldGVzdCJdfSwicmVzb3VyY2VfYWNjZXNzIjp7ImFjY291bnQiOnsicm9sZXMiOlsibWFuYWdlLWFjY291bnQiLCJtYW5hZ2UtYWNjb3VudC1saW5rcyIsInZpZXctcHJvZmlsZSJdfX0sInNjb3BlIjoib3BlbmlkIGVtYWlsIHByb2ZpbGUiLCJlbWFpbF92ZXJpZmllZCI6ZmFsc2UsInJlYWxtIjoiYWNtZXRlc3QiLCJwcmVmZXJyZWRfdXNlcm5hbWUiOiJ1c2VyMSJ9.FSGwdLrlx7Y453mefK-wwDPe1LdkaaqNUSxKcsbWoQaZQOjmQcP5Ng8LJyQ7GIlnrziGjeSgA4EZt22Kkyz1s4hJGTkHLTz2i4k3IxPkek0NAhT7xeNVw2qBTxHIofEGJL4ZmAocXE8KzI5VeyWAXbdcYMlC5HIigPoufmFpuMq_7GfaJ5eC2W5L9tFbFYe3GT-xu8lGhuSuo4JN9cweMWRcnV2Wd6aAQepXX6M4x7_QERbWcdedmdmDGgZfMB9uX9ZUBsWXlNvTXhOA5B8oH38fwtde8fbLNPommxC3WakF8F7Z9BjI0pyCeBwk-XVnhZBtdGEd_dqjMvnzaJ7uCw

query {
  exampleQuery
}
```
RUN THIS
```http request
POST http://localhost:8080/graphql
Content-Type: application/graphql

query {
  exampleQuery
}
```

The second query MUST fail, since no Authorization header has been set.
JWTAwareContext throws an exception when there are no authorization headers.

If it doesn't happen, just try again run http request 1 and then run http request 2.

Once you get the second request to respond:
```http
HTTP/1.1 200 OK
content-type: application/graphql+json; charset=UTF-8
content-length: 34

{
  "data": {
    "exampleQuery": "hello!"
  }
}
```

Enter SmallRyeGraphQLAbstractHandler in debug and terminate the context in the handle method.

```java
    @Override
    public void handle(final RoutingContext ctx) {

        if (currentManagedContext.isActive()) { // terminate the context here evaluating via debug inspector
            handleWithIdentity(ctx);
        } else {

            currentManagedContext.activate();
            ctx.response()
                    .endHandler(currentManagedContextTerminationHandler)
                    .exceptionHandler(currentManagedContextTerminationHandler)
                    .closeHandler(currentManagedContextTerminationHandler);

            try {
                handleWithIdentity(ctx);
            } catch (Throwable t) {
                currentManagedContext.terminate();
                throw t;
            }
        }
    }

```

After termination, the response will return the correct answer (system error)

```http
{
  "errors": [
    {
      "message": "System error",
      "locations": [
        {
          "line": 1,
          "column": 11
        }
      ],
      "path": [
        "exampleQuery"
      ],
      "extensions": {
        "classification": "DataFetchingException"
      }
    }
  ],
  "data": {
    "exampleQuery": null
  }
}
```

## Reversing the requests

If you restart and reverse the request order, so TWICE request 2 and one time request 1 you get the Error instead of the success for the OK request.
