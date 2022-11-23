const { Resolver } = require('dns');
const resolver = new Resolver();
resolver.setServers(['127.0.0.1']);
resolver.resolve4('evil.masahiro331.com', (err, addresses) => {
  console.log(addresses);
  console.log(err);
});
