#include "ParallelScheduler.hpp"

//a simple function for testing ParallelScheduler
void foo(void* arg) {
    int* a = (int*)arg;
    std::cout << *a << ' ';
}

int main() {
    ParallelScheduler* scheduler = new ParallelScheduler(14);
    for(int i = 1; i < 21; ++i) {
        scheduler->run(foo, &i);
    }
    std::cout << std::endl;
    delete scheduler;
}
