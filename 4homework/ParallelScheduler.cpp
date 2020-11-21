#include "ParallelScheduler.hpp"

ParallelScheduler::ParallelScheduler(size_t threadsCount = 4)
    :queueLock(PTHREAD_MUTEX_INITIALIZER)
     ,condvar(PTHREAD_COND_INITIALIZER)
     ,threads(threadsCount)
{
    //create threads with executeFunctionsWrapper(this) and it will call executeFunctions()
    for(pthread_t& thread : threads) {
        int err = pthread_create(&thread, NULL,  &executeFunctionsWrapper, this);
        if(err != 0) {
            std::cout << "pthread_create returns an error "
                << err
                << " number!" << std::endl;
            exit(err);
        }
    }
}

//cancel all threads
ParallelScheduler::~ParallelScheduler() {
    for(pthread_t& thread : threads) {
        pthread_cancel(thread);
    }
}

//push functinons and their arguments into the queue
void ParallelScheduler::run(void(*start_rouTine)(void*), void* arg) {
    //lock queue for correct synchronization
    pthread_mutex_lock(&queueLock);
    //push the pair of function pointer and its argument into the queue
    functions.push(std::pair< void(*)(void*), void* >(start_rouTine, arg));
    //unlock queue
    pthread_mutex_unlock(&queueLock);
    //send signal to all threads, that queue is unlocked
    pthread_cond_broadcast(&condvar);
}

void* ParallelScheduler::executeFunctions(void* arg) {
    while(true) {
        pthread_mutex_lock(&queueLock);
        while(functions.empty()) {
            pthread_cond_wait(&condvar, &queueLock);
        }
        void(*start_rouTine)(void*) = functions.front().first;
        void* arg = functions.front().second;
        functions.pop();
        start_rouTine(arg);
        pthread_mutex_unlock(&queueLock);
        
    }
    return NULL;
}

//cast from void* to ParallelScheduler*, and call executeFunctions
void* ParallelScheduler::executeFunctionsWrapper(void* arg) {
    reinterpret_cast<ParallelScheduler*>(arg)->executeFunctions(NULL);
    return NULL;
}






