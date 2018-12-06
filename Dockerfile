FROM ubuntu
# FROM alpine:3.8
RUN apt-get update && apt-get install -y \
    build-essential \
    g++-7 \
    git \
    libncurses5-dev \
    libncursesw5-dev
# RUN apk add --no-cache git make ncurses g++
RUN export CC=gcc-7 && export CXX=g++-7
RUN mkdir /app && mkdir /app/hangman
COPY . /app/hangman/
CMD cd /app/hangman && ./run.zsh
