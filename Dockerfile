FROM frolvlad/alpine-glibc

RUN apk add --no-cache nodejs bash npm curl g++ make m4 patch gmp-dev perl git jq perl-utils libressl-dev

USER root

RUN npm -g config set user root
RUN npm i -g esy@latest

WORKDIR /app

VOLUME /app

