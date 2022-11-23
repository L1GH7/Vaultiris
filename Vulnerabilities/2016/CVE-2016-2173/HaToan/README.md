# CVE-2016-2173 - Remote Code Execution in Spring AMQP - App Test

### Description
The class org.springframework.core.serializer.DefaultDeserializer does not validate the deserialized object against a whitelist. By supplying a crafted serialized object like Chris Frohoff's Commons Collection gadget, remote code execution can be achieved.
### Versions Affected
1.0.0 to 1.5.4
### Vendor
Spring by Pivotal
### Install
	- Maven 3.x+
	- Java 1.7+
	- [RabbitMQ](https://www.rabbitmq.com/download.htm)
### Run App Vulnerbility
	- mvn eclipse:eclipse
	- import project
	- run project (App)
	
