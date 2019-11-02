#!/bin/bash
g++ -Wall -g  -c hello.cpp -o hello.o
# commented because libworld.a is already compiled 
# and archived using following 2 commands
# g++ -c world.cpp	# this requires world.h
# ar rcs libworld.a world.o
#   in this way implementation (in header file's cpp file) can be hide (in compiled version of header file's cpp file)
g++ -o hello hello.o libworld.a
