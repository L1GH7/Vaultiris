const { transporter } = require('.')
const htmlToText = require('html-to-text')
const path = require('path')
const compile = require('./compile')

const pathToFlintLogo = path.join(__dirname, 'flintlogo.png')

/**
 * Send an email
 * @param {String} to - To whom will the email be sent
 * @param {String} template - Email template
 * @param {Object} data - Data object
 */
async function sendEmail (to, template, data) {
  /*
    Skip sendEmail function to avoid error.
  */
  return
}

module.exports = sendEmail