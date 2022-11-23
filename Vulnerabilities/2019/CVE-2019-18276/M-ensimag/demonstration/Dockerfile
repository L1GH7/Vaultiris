FROM debian:10
# Installe de quoi compiler le programme vulnérable et le serveur ssh
RUN apt-get update && \
    apt-get -y install gcc openssh-server && \
    rm -rf /var/lib/apt/lists/*


USER root

COPY init_utilisateurs.sh .
COPY creation_secret.sh .
COPY exploit.sh .

RUN chmod +x init_utilisateurs.sh
RUN chmod +x creation_secret.sh
RUN chmod +x exploit.sh

#Initiliastion de l'envirenement du travail:
CMD ["./init_utilisateurs.sh"]
