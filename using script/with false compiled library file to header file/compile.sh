#!/bin/bash
g++ -Wall -g  -c hello.cpp -o hello.o
#g++ -Wall -g  -c world.cpp -o world.o # use already compiled world.cpp (world.o)
# but this world.o is compiled version of backup/world.cpp which dooes not have display1 function in it
g++  -o hello hello.o world.o
#above command will throw an error 
###################### ERROR STARTED #######################
#hello.o: In function `main':
#/home/----/hello.cpp:9: undefined reference to `world::display1()'
#collect2: error: ld returned 1 exit status
###################### ERROR ENDDED #######################
