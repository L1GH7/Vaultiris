# Deploying to Kubernetes

## Deploying to Kubernetes using Kubernetes Deployment and Service

You can containerize this template app and deploy it as a Deployment and Service on Kubernetes.
See the [Spring Boot Kubernetes](https://spring.io/guides/gs/spring-boot-kubernetes/) Guide for details.

We have included a `Tiltfile` file to make this easier when deploying to a cluster.

For best results use Tilt version v0.23.2 or later. You can install Tilt by following these instructions: https://docs.tilt.dev/install.html

To set up the deployment environment set the CURRENT_CONTEXT and DEFAULT_REGISTRY environment variables.

Set CURRENT_CONTEXT using:

```
export CURRENT_CONTEXT=$(kubectl config current-context)
```

Set DEFAULT_REGISTRY to the prefix to use for the image that is built and deployed. For DockerHub use `docker.io/<DOCKER-ID>`, for GCR use `gcr.io/<PROJECT-ID>`. Here is an example for DockerHub:

```
DOCKER_ID=<DOCKER-ID>
export DEFAULT_REGISTRY=docker.io/${DOCKER_ID}
```

To build and deploy the app run:

```
tilt up
```

and follow the instructions (hitting space bar brings up the Tilt interface in your browser).

To uninstall the app run:

```
tilt down
```

## Accessing the app deployed to your cluster

While Tilt is running, you can access the application at [http://localhost:8080](http://localhost:8080)

If Tilt is not running, then you need to find the `EXTERNAL-IP` for the Kubernetes service. Run this command:

```
kubectl get service spring-petclinic
```

Now, you can access the PetCLinic app at `http://<EXTERNAL-IP>`.
