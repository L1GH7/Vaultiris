from flask import *
import yaml, base64

app = Flask(__name__)

@app.route("/api/load", methods=['POST'])
def api_load():
	try:
		if "data" not in request.form:
			return "data not found."
		_load = yaml.load(base64.b64decode(request.form["data"]).decode("utf-8"))
	except Exception:
		return "Error."
	return str(_load)

@app.route("/")
def home():
    return render_template("index.html")

app.run(host="0.0.0.0")
