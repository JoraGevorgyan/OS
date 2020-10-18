objects = main.o compFunctions.o #this variable is for *.o files

all: main

main: $(objects)
	g++ $(objects) -o main

main.o: main.cpp complex.h
	g++ -c main.cpp -o main.o

compFunctions.o: compFunctions.cpp complex.h
	g++ -c compFunctions.cpp -o compFunctions.o

clean:
	rm *o main
