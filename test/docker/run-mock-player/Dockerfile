FROM ubuntu
RUN apt-get update && apt-get install -y \
    vim \
    build-essential \
    g++-7 \
    git \
    libncurses5-dev \
    libncursesw5-dev
RUN apt-get install -y \
    net-tools \
    iputils-ping
RUN export CC=gcc-7 && export CXX=g++-7
RUN mkdir /app && mkdir /app/hangman
COPY src /app/hangman/src
COPY test /app/hangman/test
COPY lib /app/hangman/lib
CMD ifconfig && cd /app/hangman/test && make mockplayer && ./mockplayer 8080
