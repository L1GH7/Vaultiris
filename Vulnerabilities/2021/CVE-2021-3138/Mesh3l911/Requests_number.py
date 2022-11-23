import requests

for _ in range(200):    header = {
              "X-CSRF-Token": "ur X-CSRF-Token",
              "Cookie": "ur Cookie",
              "X-Requested-With": "XMLHttpRequest"
              }    body = {"login": "Mesh3L", "password": "Mesh3L"+"_", "second_factor_method": "2"}    status_code = requests.post("ur target_url", headers=header, data=body).status_code    print(" \n Request num > {} Status Code > {} \n".format(_+1, status_code))    if status_code != 200:
        exit()
