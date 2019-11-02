#!/bin/bash
g++ -Wall -g  -c hello.cpp -o hello.o
g++ -Wall -g  -c world.cpp -o world.o
g++  -o hello hello.o world.o
