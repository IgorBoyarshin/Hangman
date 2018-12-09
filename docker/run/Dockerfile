FROM ubuntu
RUN apt-get update && apt-get install -y \
    libncurses5-dev \
    libncursesw5-dev
RUN mkdir /app && mkdir /app/hangman
COPY out/ubuntu/Hangman /app/hangman/Hangman
CMD /app/hangman/Hangman
