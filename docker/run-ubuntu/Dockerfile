FROM ubuntu
RUN apt-get update && apt-get install -y \
    libncurses5-dev \
    libncursesw5-dev
RUN mkdir /app && mkdir /app/hangman
COPY out/Hangman-ubuntu /app/hangman/Hangman
RUN chmod +x /app/hangman/Hangman
CMD /app/hangman/Hangman
