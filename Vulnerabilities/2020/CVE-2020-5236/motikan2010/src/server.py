import sys
import os

sys.path.append(os.path.join(os.path.dirname(os.path.realpath(__file__)), 'libs'))
from bottle import route, run, template

@route('/hello/<name>')
def greet(name):
    return template('<b>Hello {{name}}</b>!', name=name)

run(server="waitress", host='0.0.0.0', port=8080)


