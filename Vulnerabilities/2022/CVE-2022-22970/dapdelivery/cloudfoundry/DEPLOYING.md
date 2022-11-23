# Deploying to Cloud Foundry

## Deploying to Cloud Foundry as an app with cf push

When you are done developing your app, you can simply build and deploy it using:

```
./mvnw package  
cf push spring-petclinic -p target/spring-petclinic-2.6.0-SNAPSHOT.jar -f config/manifest.yml --random-route
```

## Accessing the app deployed app

Determine the URL to use for the accessing the app by running:

```
cf app spring-petclinic
```

To access the deployed app use the URL shown.

### Deleting the app

You can delete the running app using:

```
cf delete spring-petclinic
```
