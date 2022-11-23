import sys
from flask import Flask, request

app = Flask(__name__)

@app.route('/test-url')
def view_func():
    print request.headers
    return 'Request received!'


if __name__ == '__main__':
    print 'Python version is %d.%d.%d' % (sys.version_info[0], sys.version_info[1], sys.version_info[2])
    app.run(port=8000)
