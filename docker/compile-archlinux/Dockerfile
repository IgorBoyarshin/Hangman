FROM base/archlinux
RUN pacman -Syy
RUN echo "Server = https://mirror.pkgbuild.com/\$repo/os/\$arch" > /etc/pacman.d/mirrorlist && \
    echo "Server = https://mirrors.kernel.org/archlinux/\$repo/os/\$arch" >> /etc/pacman.d/mirrorlist
RUN pacman -S --noconfirm ncurses
RUN pacman -S --noconfirm \
    vim \
    make \
    gcc \
    git
RUN export CC=gcc-7 && export CXX=g++-7
RUN mkdir /app && mkdir /app/hangman
RUN mkdir /mnt/host
COPY src /app/hangman/src
COPY .git /app/hangman/.git
CMD cd /app/hangman/src && make && cp -a Hangman /mnt/host/Hangman-archlinux
