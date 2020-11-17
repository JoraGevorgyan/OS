//"ParallelScheduler.hpp"
//"../program/main.cpp"
//
#include "ParallelScheduler.hpp"

ParallelScheduler::ParallelScheduler()
    :threadsCount(4)
     ,functionsCount(0)
     ,queueLock(1)
     ,threads(4)
{
    createJoinThreads();
}

ParallelScheduler::ParallelScheduler(size_t threadsCount)
    :threadsCount(threadsCount)
     ,functionsCount(0)
     ,queueLock(1)
     ,threads(threadsCount)
{
    createJoinThreads();
}

ParallelScheduler::~ParallelScheduler() {
    for(pthread_t& thread : threads) {
        pthread_cancel(thread);
    }
}

void ParallelScheduler::run(void(*start_rouTine)(void*), void* arg) {
    functions.push(std::make_pair< void(*)(void*), void* >(start_rouTine, arg));
    signal(functionsCount);
}

void ParallelScheduler::executeFunctions(void* arg) {
    sem_wait(queueLock);
    sem_wait(functionsCount);
    void(*start_rouTine)(void*) = functions.front().first;
    void* arg = functions.front().second;
    functions.pop();
    signal(queueLock);
    start_rouTine(arg);
}


void ParallelScheduler::createJoinThreads() {
    for(pthread_t& thread : threads) {
        int err = pthread_create(thread, NULL,  &executeFunctions, NULL);
        if(err != 0) {
            std::cout << "pthread_create returns an error "
                << err
                << " number!" << std::endl;
            exit(err);
        }
        err = pthread_join(thread, NULL);
        if(err != 0) {
            std::cout << "pthread_join retunrs an error "
                << err
                << " number!" << std::endl;
            exit(err);
        }
    }
}
