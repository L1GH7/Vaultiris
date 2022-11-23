FROM debian as builder

RUN apt-get -qy update && \
  apt-get -qy install curl make gcc libfuse-dev
COPY crash.c /usr/src
RUN gcc -static -o /crash /usr/src/crash.c

FROM busybox
COPY --from=builder /crash /crash
USER 65534
ENTRYPOINT [ "unshare", "-Urm", "/crash" ]
