#!/bin/bash
g++ -Wall -g  -c hello.cpp -o hello.o
#g++ -Wall -g  -c world.cpp -o world.o # commented because world.o is already compiled 
# in this way implementation (in header file's cpp file) can be hide (in compiled version of header file's cpp file)
g++  -o hello hello.o world.o
