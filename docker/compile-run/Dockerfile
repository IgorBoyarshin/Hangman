FROM ubuntu
RUN apt-get update && apt-get install -y \
    vim \
    build-essential \
    g++-7 \
    git \
    libncurses5-dev \
    libncursesw5-dev
RUN export CC=gcc-7 && export CXX=g++-7
RUN mkdir /app && mkdir /app/hangman
RUN mkdir /mnt/host
COPY src /app/hangman/src
COPY .git /app/hangman/.git
CMD cd /app/hangman/src && make && ./Hangman
