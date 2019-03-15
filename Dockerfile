FROM alpine:3.8

COPY . /src
WORKDIR /src

RUN apk add --no-cache \
    cmake \
    gcc \
    g++ \
    libc6-compat \
    python \
    make \
    musl-dev
