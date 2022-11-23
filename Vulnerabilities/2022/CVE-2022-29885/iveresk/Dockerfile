# syntax=docker/dockerfile:1

# Alpine is chosen for its small footprint
# compared to Ubuntu
from golang:1.17-alpine

# Adding Variables
ARG INPUT_FILE="<Your target file name>.txt"
ENV INPUT_FILE=${INPUT_FILE}

# Download necessary Go modules
WORKDIR app/
COPY go.mod ./
RUN go mod download

# COPY necessary files
ADD . ./
COPY *.go ./src/
COPY *.txt ./src/
RUN go build -o /cve-2022-29885
CMD [ "/cve-2022-29885", "-t", ${INPUT_FILE} ]
