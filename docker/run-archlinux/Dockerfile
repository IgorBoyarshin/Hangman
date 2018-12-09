FROM base/archlinux
RUN pacman -Syy
RUN echo "Server = https://mirror.pkgbuild.com/\$repo/os/\$arch" > /etc/pacman.d/mirrorlist && \
    echo "Server = https://mirrors.kernel.org/archlinux/\$repo/os/\$arch" >> /etc/pacman.d/mirrorlist
RUN pacman -S --noconfirm ncurses
RUN mkdir /app && mkdir /app/hangman
COPY out/Hangman-archlinux /app/hangman/Hangman
RUN chmod +x /app/hangman/Hangman
CMD /app/hangman/Hangman
