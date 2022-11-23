from flask import Flask, request, jsonify
import jwt

app = Flask(__name__)

@app.route('/init', methods = ["GET"])
def init():
    priv = open('private.pem', 'r').read()
    auth_token = jwt.encode({"user": "user", "authenticated": False}, priv, algorithm="RS256")
    return jsonify({"token": auth_token.decode()})

@app.route('/verify', methods = ['GET'])
def verify():
    if 'Authorization' in request.headers:
        pub = open('public.pem', 'r').read()
        try:
            # 취약한 코드
            # verified = jwt.decode(request.headers.get('Authorization'), pub)

            # PyJWT 라이브러리 업데이트 후, 허용할 알고리즘을 명시적으로 지정할 수 있음
            verified = jwt.decode(request.headers.get('Authorization'), pub, algorithms=["RS256", "RS512"])
            
            print(verified)
            return jsonify({"success": True, "error": False, "data": verified})
        except Exception as e:
            print(e)
            return jsonify({"error": "Unable to authorize"}, 403)
    else:
        return jsonify({"error": "No Authorization code"},400)


if __name__ == "__main__":
    app.run(debug=True)