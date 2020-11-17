//"ParallelScheduler.cpp"
//"../program/main.cpp"
//
//use this file only once when linking
#pragma once

#include <iostream>
#include <queue> // will use this queue for functions
#include <vector> // will use this constainer for threads
#include <pthread.h>
#include <semaphore.h>

class ParallelScheduler {
    public:
        ParallelScheduler();
        ParallelScheduler(size_t threadsCount);
        ~ParallelScheduler();
        // pushes the function into queue
        void run(void(*start_rouTine)(void*), void* arg);
    private:
        void createJoinThreads();
        void executeFunctions(void* arg);
        sem_t functionsCount;
        sem_t queueLock;
        size_t threadsCount;
        std::queue< std::pair< void(*)(void*), void*> > functions;
        std::vector< pthread_t > threads;
};
