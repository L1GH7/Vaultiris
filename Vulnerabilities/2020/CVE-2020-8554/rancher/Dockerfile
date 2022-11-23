# Use distroless as minimal base image to package the webhook binary
# Refer to https://github.com/GoogleContainerTools/distroless for more details
FROM gcr.io/distroless/static:nonroot
ARG ARCH
WORKDIR /
COPY build/bin/webhook .
USER nonroot:nonroot

ENTRYPOINT ["/webhook"]
