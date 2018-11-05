#!/usr/bin/zsh

GT="googletest/googletest"
GM="googletest/googlemock"

g++ -c $GT/src/gtest-all.cc $GM/src/gmock-all.cc $GM/src/gmock_main.cc -I $GT -I $GT/include -I $GM -I $GM/include -lpthread -std=c++14
