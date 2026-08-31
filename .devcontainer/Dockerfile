FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    make \
    gcc-avr \
    avr-libc \
    avrdude \
    git \
    ca-certificates \
    && rm -rf /var/lib/apt-get/lists/*