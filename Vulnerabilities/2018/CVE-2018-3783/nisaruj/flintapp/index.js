const Flint = require('flintcms')
const mongoose = require('mongoose')

const flintServer = new Flint({
  siteName: 'Let\'s Exploit!',
  scssEntryPoint: 'main.scss', // The entry point from where you'll import your other SCSS files
  scssPath: 'scss', // The folder that stores your SCSS files
  scssIncludePaths: ['node_modules', 'scss'] // List of directories to enable `@import` from
});

async function insertHomepage() {
  const Page = mongoose.model('Page')
  const homepage = {
    "handle":"itWorks!",
    "slug":"it-works",
    "route":"/",
    "title":"It works!",
    "template":"homepage.njk",
    "homepage":true,
    "fields":[],
    "fieldLayout":[],
    "dateCreated":"2019-10-07T00:00:00.000Z",
    "default": true
  }
  await Page.findOneAndUpdate({ "homepage": true }, homepage, { upsert: true, new: true, setDefaultsOnInsert: true })
  console.log('Added homepage')
}

async function startServer() {
  await flintServer.startServer()
  await insertHomepage()
}

startServer()