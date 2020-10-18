SOURCES := $(wildcard source/*.cpp)  #defina all ./cpp files as "SOURCES"
OBJECTS := $(patsubst source/%.cpp, object/%.o, $(SOURCES))	#make name for all object files
															#like *.o, and
															#define as "OBJECTS"

all: copy   #project names "copy"

copy: $(OBJECTS)
	g++ $(OBJECTS) -o copy  #link object files and open in "copy"

object/copyFiles.o: source/copyFiles.cpp source/fileWorkLibrary.hpp
	mkdir -p object
	g++ -c source/copyFiles.cpp -o object/copyFiles.o   #compile until linker and open in*.o file

object/fileWorkLibrary.o: source/fileWorkLibrary.cpp
	mkdir -p object
	g++ -c source/fileWorkLibrary.cpp -o object/fileWorkLibrary.o   #compile until linker
																	# and open in*.o file


clean: #remove "object" directory, that contains object files
	rm -rf ./object 

clean_all: #remove "object" directory, that contains object files, and also "copy" project
	rm -rf ./object   
	rm copy

