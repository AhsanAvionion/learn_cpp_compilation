# Makefile for the test file
Flags = -Wall -g
output_file = hello

executable: hello.o /usr/local/lib/libworld.so
	g++ hello.o /usr/local/lib/libworld.so -o $(output_file) # if not defined -o hello (if like this g++ hello.o world.o) this command will generate a.out output file instead of hello

hello.o: hello.cpp world.h
	g++ $(Flags) -c hello.cpp

clean:
	rm hello.o $(output_file)  # a.out

