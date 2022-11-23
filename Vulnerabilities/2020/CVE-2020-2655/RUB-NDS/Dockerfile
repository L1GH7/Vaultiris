FROM openjdk@sha256:d909bffa542eb2670f7839c73b131d39cfe611e4fc8ec097d9d2729967758ec0 as cve-2020-2655poc
ADD target/ /target/
ADD server.jks /server.jks
EXPOSE 4434/udp
EXPOSE 4433
ENTRYPOINT ["java", "-jar", "target/PoC-Server.jar"]

