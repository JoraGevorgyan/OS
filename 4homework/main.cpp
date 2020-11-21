#include "ParallelScheduler.hpp"

/*
 * a simple function for testing ParallelScheduler
 */
void foo(void* arg) {
    int* a = (int*)arg; // cast from (void*) to (int*)
    std::cout << *a << ' ';
}

#define ARR_SIZE 21

int main() {
    int* arr = new int[ARR_SIZE];
    ParallelScheduler* scheduler = new ParallelScheduler(14);
    for(int i = 1; i < ARR_SIZE; ++i) {
        arr[i] = i;
        scheduler->run(foo, &arr[i]);
    }
    scheduler->cancel();
    std::cout << std::endl;
    delete[] arr;
    delete scheduler;
}
