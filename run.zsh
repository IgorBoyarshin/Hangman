#!/bin/sh

cd src

make

./main

# rm -f *.o
rm -f *.gch
rm -f .*.gch
rm -f main
