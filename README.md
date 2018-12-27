# Hangman #

[![Build Status](https://travis-ci.org/IgorBoyarshin/Hangman.svg?branch=dev)](https://travis-ci.org/IgorBoyarshin/Hangman)

Welcome to the **Hangman**, a game where you get to hang your friends!

There are a couple of ways to run the app, depending on your preferences and environment.

The easiest way to try the app is to compile-and-run it using `Docker`.

If you want to run the app natively (currently only for `Ubuntu` or `Arch linux`), you can compile it  either natively or using `Docker`, producing an executable, and then run it.

You can also download a pre-compiled executable (for `Ubuntu` or `Arch linux`) from the releases page.

For in-depth instructions on all of these options, see below.

## Controls ##
You can navigate around the screen using either `hjkl` or arrow keys.

Use `Enter` to press a button or start editing a Field (press `Enter` again in order to save changes).

## Compile from source and run (with Docker) ##
This way you don't have to install anything (apart from `Docker`). The most *clean* way to build and run the app.
* Download the project source code and navigate into the project folder:
    ```sh
    $ git clone https://github.com/IgorBoyarshin/Hangman
    $ cd Hangman
    ```
* Create a new image with the appropriate `Dockerfile` and then run it:
    ```sh
    $ docker build -t hangman-compile-run -f docker/compile-run/Dockerfile . # note: may need root to run docker
    $ docker run -it hangman-compile-run # note: may need root to run docker
    ```

## Compile from source (for `Ubuntu` or `Arch linux`) (with Docker)
You can also use `Docker` to generate the executable for your target platform (`Ubuntu` or `Arch linux`) and run the app natively.
* Download the project source code and navigate into the project folder:
    ```sh
    $ git clone https://github.com/IgorBoyarshin/Hangman
    $ cd Hangman
    ```
* Create a new image with the appropriate `Dockerfile` and then run this image. You will need to specify the **absolute** path of the folder where you want `Docker` to output the executable:
    * To compile for `Ubuntu`:
        ```sh
        $ docker build -t hangman-compile-ubuntu -f docker/compile-ubuntu/Dockerfile . # note: may need root to run docker
        $ docker run -v <ABSOLUTE PATH TO DESTINATION FOLDER>:/mnt/host -it hangman-compile-ubuntu
        ```
    * To compile for `Arch linux`:
        ```sh
        $ docker build -t hangman-compile-archlinux -f docker/compile-archlinux/Dockerfile . # note: may need root to run docker
        $ docker run -v <ABSOLUTE PATH TO DESTINATION FOLDER>:/mnt/host -it hangman-compile-archlinux

## Compile from source and run (for `Ubuntu` or `Arch linux`) (natively) ##
If you want to tweak the source code.

You could also run the compiled executable using `Docker` (for whatever reason). For this consult the section "Run a pre-compiled executable":"using `Docker`".
* Make sure all the required dependencies are installed:
    * **g++** >= 7:
        * Ubuntu:
            ```sh
            $ sudo apt-get update && sudo apt-get install g++-7
            ```
        * Arch:
            ```sh
            $ sudo pacman -S gcc
            ```
    * **make**:
        * Ubuntu:
            ```sh
            $ sudo apt-get update && sudo apt-get install build-essential
            ```
        * Arch:
            ```sh
            $ sudo pacman -S make
            ```
    * **ncurses** library:
        * Ubuntu:
            ```sh
            $ sudo apt-get update && sudo apt-get install libncurses5-dev libncursesw5-dev
            ```
        * Arch:
            ```sh
            $ sudo pacman -S ncurses
            ```
    * **git**:
        * Ubuntu:
            ```sh
            $ sudo apt-get update && sudo apt-get install git
            ```
        * Arch:
            ```sh
            $ sudo pacman -S git
            ```
* Make sure the correct versions of the language will be used:
    ```sh
    $ export CC=gcc-7 && export CXX=g++-7
    ```
* Download the project source code (will create a new folder **Hangman**):
    ```sh
    $ git clone https://github.com/IgorBoyarshin/Hangman
    ```
* Navigate into the **src** folder inside the project folder, compile the code and run the application:
    ```sh
    $ cd Hangman/src
    $ make
    $ ./Hangman
    ```

## Run a pre-compiled executable ##
In order to run a pre-compiled executable (with `Ubuntu`, `Arch linux` or `Docker`):
* If you are using `Ubuntu`:
    * Install the necessary dependencies:
        ```sh
        $ sudo apt-get update && sudo apt-get install libncurses5-dev libncursesw5-dev
        ```
    * Download the executable `Hangman-ubuntu` for the desired release from the releases page.
    * Navigate to your **Downloads** folder and make sure that `Hangman-ubuntu` is executable. Finally, run it:
        ```sh
        $ cd ~/Downloads
        $ chmod +x Hangman-ubuntu
        $ ./Hangman-ubuntu
        ```
* If you are using `Arch linux`:
    * Install the necessary dependencies:
        ```sh
        $ sudo pacman -S ncurses
        ```
    * Download the executable `Hangman-archlinux` for the desired release from the releases page.
    * Navigate to your **Downloads** folder and make sure that `Hangman-archlinux` is executable. Finally, run it:
        ```sh
        $ cd ~/Downloads
        $ chmod +x Hangman-archlinux
        $ ./Hangman-archlinux
        ```
* Using `docker`:

    You can either run it in the `Arch linux` or `Ubuntu` environment of `Docker`. This example covers the `Ubuntu` case. For the `Arch linux` case just replace all occurances of `ubuntu` with `archlinux`.
    * Download the `Dockerfile` contained inside **docker/run-ubuntu/**.
    * Create a new directory (e.g. **~/Hangman**) and place the downloaded `Dockerfile` there:
        ```sh
        $ mkdir ~/Hangman
        $ mv ~/Downloads/Dockerfile ~/Hangman/
        ```
    * Download the executable `Hangman-ubuntu` for the desired release from the releases page.
    * Inside your app directory (e.g. **~/Hangman**) create a new directory called **out** and place the downloaded `Hangman-ubuntu` executable there:
        ```sh
        $ mkdir ~/Hangman/out
        $ mv ~/Downloads/Hangman-ubuntu ~/Hangman/out/
        ```
    * You should now have the following project tree structure:
        ```sh
        |-- Hangman
              |-- Dockerfile
              |-- out
                   |-- Hangman-ubuntu
        ```
    * Navigate to your app folder (e.g. **~/Hangman**). Build docker image from inside this folder and then run the image:
        ```sh
        $ cd ~/Hangman
        $ docker build -t hangman-run-ubuntu . # note: may need root to run docker
        $ docker run -it hangman-run-ubuntu    # note: may need root to run docker
        ```

## Running tests
You can run tests either natively or using `Docker`.
* Using `Docker`:

    Head to the root of the project.

    In order to see the output of the *MockPlayer* you need to run this container in a separate console **not** in a detached state. Otherwise (to just run the tests), do the following:
    ```sh
    $ docker build -t hangman-test-run-mock-player -f test/docker/run-mock-player/Dockerfile . # note: may need root to run docker
    $ docker build -t hangman-test-run -f test/docker/run-test/Dockerfile . # note: may need root to run docker
    $ docker run --name mock-player -d -p 8080 -p 1234 -it hangman-test-run-mock-player # note: may need root to run docker
    $ docker run -p 8080 -p 1234 -it hangman-test-run # note: may need root to run docker
    # --- Tests running ---
    $ docker stop mock-player # stop the detached earlier container
    ```
* Natively:

    Head to the **test** project folder.

    You will need 2 separate console instances. The reason for that is that some test cases required a mock player to be running in the background.

    Do things in the following order:
    * In the 1st console instance:
        ```sh
        $ make run_mockplayer
        ```
    * In the 2st console instance:
        ```sh
        $ make run_tests
        # --- Tests running ---
        ```
    * In the 1st console instance:
        ```sh
        # Having executed the tests in 2nd console:
        $ ^C # stop the execution with e.g. Ctrl+C
        ```
