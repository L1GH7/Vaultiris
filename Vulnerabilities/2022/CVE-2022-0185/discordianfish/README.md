# Container running cve-2022-0185 crash POC
![seccomp or bust](https://user-images.githubusercontent.com/275966/151207659-56d63de4-0556-4d3a-9b57-0586d3880ac4.png)

This is a docker container that runs the [crash POC](crash.c) from
https://www.willsroot.io/2022/01/cve-2022-0185.html.

The container runs unprivileged as UID 65534. You can also use `docker run -u`
or set a security context with RunAsUser additionally, all should yield the same
result.

## Run it
**To be clear, if you're vulnerable that will crash your server!**

### Docker
`docker run docker.io/fish/cve-2022-0185-crash-poc`

You can also build it yourself:
```
docker build -t crashtest .
docker run crashtest
```

### Kubernetes
`kubectl apply -f pod.yaml`
