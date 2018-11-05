#!/bin/sh

res=$( python lib/cpplint.py src/Game.cpp )

echo "$res"
