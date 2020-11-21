all: main

main: main.o ParallelScheduler.o
	@g++ -pthread main.o ParallelScheduler.o -o main

main.o: main.cpp ParallelScheduler.hpp
	@g++ -pthread -msse4 -c main.cpp -o main.o

ParallelScheduler.o: ParallelScheduler.cpp
	@g++ -pthread -msse4 -c ParallelScheduler.cpp -o ParallelScheduler.o

clean:
	@rm ./*.o
	@rm ./main
