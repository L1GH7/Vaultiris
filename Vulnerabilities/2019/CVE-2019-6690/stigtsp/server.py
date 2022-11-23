#!/usr/bin/env python3
from flask import Flask,request,abort

import gnupg, secrets, json

app = Flask(__name__)
gpg = gnupg.GPG(gnupghome="/tmp/gpg")


def encrypt_data(password, data):
    return str(gpg.encrypt(
        data,
        recipients  = None,
        symmetric   = True,
        passphrase  = password
    ))

# random root password! very secure!
rootPassword = secrets.token_hex(32);

# encrypted database, also very secure! you need to know the password
# to decrypt and use the user information
encryptedDatabase = {
    "root" : encrypt_data(rootPassword, '{ "uid" : 0 }'),
    "harmless_user" : encrypt_data("password123", '{ "uid" : 1000 }')
}

# demonstrates descrypt vulnerability
@app.route("/login", methods=['POST'])
def login():
    (username, password) = (request.form['username'], request.form['password'])

    if not username in encryptedDatabase:
        return "You are not in the database, sorry."

    decrypted = gpg.decrypt(
        encryptedDatabase[username],
        passphrase = password
    );

    if not decrypted:
        return "Oh sorry, wrong decryption password!"

    decryptedData = json.loads(str(decrypted))

    if decryptedData["uid"] == 0:
        return "**** Hello root! In case you forgot your password, it's: %s ****" % rootPassword
    else:
        return "Hi normal user!"

# demonstrates encrypt vulnerability
@app.route("/encryption_as_a_service", methods=['POST'])
def encryption_as_a_service():
    plaintext = "This cannot be changed!"
    (password) = (request.form['password'])
    encrypted = encrypt_data(password, plaintext);
    return str(encrypted)




if __name__ == "__main__":
    app.run(host='0.0.0.0')
