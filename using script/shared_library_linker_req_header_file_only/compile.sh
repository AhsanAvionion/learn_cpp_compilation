#!/bin/bash
g++ -Wall -g  -c hello.cpp -o hello.o # this command requires world.h only not also its implementation in world.cpp
# libworld.so is a shared library generated with following commands
# Generation of libworld.so requires world.cpp and world.h both 
# but we wanted to hide implementation of library functions
# libworld.so is generated by world.cpp available at hidden/world.cpp
# g++ -fPIC -c world.cpp
# g++ -shared -o libworld.so world.o 
# linker will find libworld.so automatically
g++ hello.o -L . -l world -o hello

############# with only a single command #########3
#g++ hello.cpp -L . -l world -o hello
