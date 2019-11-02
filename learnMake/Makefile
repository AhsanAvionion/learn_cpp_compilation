# Makefile for the test file

flags = -Wall -g

executable: hello.o world.o
	g++ hello.o world.o -o hello # if not defined -o hello (if like this g++ hello.o world.o) this command will generate a.out output file instead of hello

hello.o: hello.cpp world.h
	g++ -Wall -g -c hello.cpp

world.o: world.cpp world.h
	g++ -Wall -g -c world.cpp

clean:
	rm hello.o world.o hello # a.out
