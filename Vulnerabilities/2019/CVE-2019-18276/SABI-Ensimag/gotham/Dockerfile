FROM debian:10
RUN apt-get update && apt-get -y install gcc

USER root

#Saisie des paramètres de l'utilisateur batman:
RUN echo "batman" >> param_batman  #mdp
RUN echo "batman" >> param_batman  #confirmation du mdp
RUN echo '' >> param_batman && \
    echo '' >> param_batman && \
    echo '' >> param_batman && \
    echo '' >> param_batman && \
    echo '' >> param_batman && \
    echo 'Y' >> param_batman

#Création de l'utilisateur batman (victime) :
RUN adduser batman < param_batman
RUN rm param_batman

RUN chown batman:batman /bin/bash #batman devient owner /bin/bash
RUN chmod u+s /bin/bash

#Saisie des paramètres de l'utilisateur joker:
RUN echo "joker" >> param_joker  #mdp
RUN echo "joker" >> param_joker  #confirmation du mdp
RUN echo '' >> param_joker && \
    echo '' >> param_joker && \
    echo '' >> param_joker && \
    echo '' >> param_joker && \
    echo '' >> param_joker && \
    echo 'Y' >> param_joker
    
#Création de l'utilisateur joker (attaquant) :
RUN adduser joker < param_joker
RUN rm param_joker

#Copie de l'exploit dont les droits reviendront à joker seul :
COPY exploit.sh .
RUN chown joker:joker exploit.sh 
RUN chmod u+x exploit.sh
RUN chmod og-rwx exploit.sh
RUN mv ./exploit.sh ./home/joker/


#batman crée un fichier caché et restreint l'accès à celui-ci:
USER batman

WORKDIR /home/batman
RUN echo "Je suis Bruce Wayne !" > .secret.txt && \
    chmod og-r .secret.txt


#Au démarrage du conteneur, la session de joker est ouverte:
USER joker

WORKDIR /home

