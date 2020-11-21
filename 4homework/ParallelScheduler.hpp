#pragma once // use this file only once when linking

#include <iostream>
#include <queue>    // for saving functions
#include <pthread.h>

class ParallelScheduler {
    private:
        pthread_mutex_t* queueLock;     // mutex variable for lock the queue
        pthread_cond_t* condvar;        // conditional variable for waiting to push into the queue
        pthread_cond_t* cancelCondvar;  // conditional variable for waiting to finish all functions
        size_t threadsCount;
        pthread_t* threads; // an array of threads
        std::queue< std::pair< void(*)(void*), void*> > functions; // will contain functions with their arguments
    public:
        /*
         * will create $(threadsCount) threads,
         * and by default (threadsCount = 4)
         */
        ParallelScheduler(size_t threadsCount = 4);
        /*
         * cancel all threads
         * destroy mutex and conditional variables
         * delete alocated memory
         */
        ~ParallelScheduler();
        /*
         * pushs the function and his argument into the queue
         */
        void run(void(*start_rouTine)(void*), void* arg);
        /*
         * wait for all threads to finish their work
         * and cancel them
         */
        void cancel();
    private:
        /*
         * execute all functions from the queue
         */
        void* executeFunctions(void* arg);
        /*
         * threads arre created with this function,
         * and this will pass them "executeFucntions"
         */
        static void* executeFunctionsWrapper(void* arg);
};
