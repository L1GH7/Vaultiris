FROM golang:1.13.1

WORKDIR /xsrc
COPY . .

RUN go mod vendor
RUN go test -trimpath -mod=vendor -v -run TestDSAVerify
RUN go test -trimpath -mod=vendor -v -run TestSSHClientHostKey