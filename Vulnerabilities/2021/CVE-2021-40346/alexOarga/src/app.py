from flask import Flask, render_template, request
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

@app.route('/index', methods=['GET', 'POST'])
def hello_world():
    return render_template('index.html')

@app.route('/admin', methods=['GET', 'POST'])
def hello_world1():
    return render_template('admin.html')

@app.route('/headers', methods=['GET', 'POST'])
def get_headers():
    return str(request.headers)


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
