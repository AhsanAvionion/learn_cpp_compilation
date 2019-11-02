#!/bin/bash
g++ -Wall -g  -c hello.cpp -o hello.o # this command requires world.h only not also its implementation in world.cpp
# libworld.so is a shared library generated with following commands
# Generation of /usr/local/lib/libworld.so requires world.cpp and world.h both 
# but we wanted to hide implementation of library functions
# libworld.so is generated by world.cpp available at hidden/world.cpp
# libworld.so should be placed at fixed place i.e /usr/local/lib/libworld.so
# location of shared library should be same while compiling hello.cpp and executing hello
# g++ -fPIC -c world.cpp
# g++ -shared -o libworld.so world.o 

g++  -o hello hello.o /usr/local/lib/libworld.so
