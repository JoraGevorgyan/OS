//use this file only once when linking
#pragma once

#include <iostream>
#include <queue> // for saving functions
#include <vector>// a container for threads
#include <pthread.h>

class ParallelScheduler {
    private:
        //to synchronize the function queue
        pthread_mutex_t queueLock;
        pthread_cond_t condvar;
        //a container for threads
        std::vector< pthread_t > threads;
        //will contain functions with their arguments
        std::queue< std::pair< void(*)(void*), void*> > functions;
    public:
        //will create $(threadsCount) threads, and by default (threadsCount = 4)
        ParallelScheduler(size_t threadsCount);
        //cancel all threads
        ~ParallelScheduler();
        //push the function and his argument into the queue
        void run(void(*start_rouTine)(void*), void* arg);
    private:
        //execute all functions from the queue
        void* executeFunctions(void* arg);
        //threads arre created with this function, and this will pass them "executeFucntions"
        static void* executeFunctionsWrapper(void* arg);
};
