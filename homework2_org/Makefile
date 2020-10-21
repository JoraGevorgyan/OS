#defina all ./cpp files as "SOURCES"
SOURCES := $(wildcard source/*.cpp)

#make name for all object files like *.o, and define as "OBJECTS"
OBJECTS := $(patsubst source/%.cpp, object/%.o, $(SOURCES))

all: copy

#link object files and open in "copy"
copy: $(OBJECTS)
	g++ $(OBJECTS) -o copy


#compile until linker and open in*.o file
object/copyFiles.o: source/copyFiles.cpp source/fileWorkLibrary.hpp
	mkdir -p object
	g++ -c source/copyFiles.cpp -o object/copyFiles.o


#compile until linker and open in*.o file
object/fileWorkLibrary.o: source/fileWorkLibrary.cpp
	mkdir -p object
	g++ -c source/fileWorkLibrary.cpp -o object/fileWorkLibrary.o


#remove "object" directory, that contains object files
clean:
	rm -rf ./object 

#remove "object" directory, that contains object files, and also "copy" project
clean_all:
	rm -rf ./object   
	rm copy

