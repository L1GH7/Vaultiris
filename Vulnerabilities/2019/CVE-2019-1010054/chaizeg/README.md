# Security breach project - Cross Site Request Forgery

Dolibarr 7.0.0 is affected by: Cross Site Request Forgery (CSRF). The impact is: allow malitious html to change user password, disable users and disable password encryptation. The component is: Function User password change, user disable and password encryptation. The attack vector is: admin access malicious urls.


## Demonstration

## Demonstration steps

-launch: 'vagrant up' to download the environment (naturally takes some time : up to a couple of hours -it's vagrant whatchu gonna do  ¯\_(ツ)_/¯).

-Once the environment is ready, launch the following commands from dev directory :

```bash
rm index.html
mv backup_index.html index.html
```

-launch 

```bash
vagrant ssh
```
-follow the rest of steps in the report


## Good to know


-The report is in French, it describes measures which could limit this type of breach, it also gives a detailed analysis of the breach and a detailed walk-through of what you should do in order to reproduce the breach. 
