//"../lib/ParallelScheduler.hpp"
//"../lib/ParallelScheduler.cpp"
//
#include "ParallelScheduler.hpp"
#include <iostream>

void foo(void* arg) {
    int* a = (int*)arg;
    std::cout << *arg << ' ';
}

int main() {
    ParallelScheduler scheduler(14);
    for(int i = 1; i < 21; ++i) {
        scheduler.run(&foo, &i);
    }
    std::cout << std::endl;
}
