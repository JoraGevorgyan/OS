#include "ParallelScheduler.hpp"

/*
 * will create $(threadsCount) threads,
 * and by default (threadsCount = 4)
 */
ParallelScheduler::ParallelScheduler(size_t threadsCount)
    :threadsCount(threadsCount)
{
    queueLock = new pthread_mutex_t();    // mutex variable for lock the queue
    condvar = new pthread_cond_t();       // conditional variable for waiting to push into the queue
    cancelCondvar = new pthread_cond_t(); // conditional variable for waiting to finish all functions
    pthread_mutex_init(queueLock, NULL);
    pthread_cond_init(condvar, NULL);
    pthread_cond_init(cancelCondvar, NULL);
    threads = new pthread_t[threadsCount];  // array of $(threadsCount) threads
    /*
     * create threads with executeFunctionsWrapper(this)
     * and it will call executeFunctions()
     */
    for(size_t i = 0; i < threadsCount; ++i) {
        int err = pthread_create(&threads[i], NULL,  &executeFunctionsWrapper, this);
        if(err != 0) {
            std::cout << "pthread_create returns an error "
                << err
                << " number!" << std::endl;
            exit(err);
        }
    }
}

/*
 * wait for all threads to finish their work
 * and cancel all threads
 */
void ParallelScheduler::cancel() {
    pthread_mutex_lock(queueLock); // lock the queue
    while(!functions.empty()) {    // if queue isn't empty, wait for it
        pthread_cond_wait(cancelCondvar, queueLock); // unlock queue and wait for cond signal
    }
    pthread_mutex_unlock(queueLock); // unlock the queue
    for(size_t i = 0; i < threadsCount; ++i) { // cancel all threads
        pthread_cancel(threads[i]);
    }
}

/*
 * destroy mutex and conditional variables
 * delete alocated memory
 */
ParallelScheduler::~ParallelScheduler() {
    pthread_mutex_destroy(queueLock);
    pthread_cond_destroy(condvar);
    pthread_cond_destroy(cancelCondvar);
    delete cancelCondvar;
    delete queueLock;
    delete condvar;
    delete[] threads;
}

/*
 * push functinons and their arguments into the queue
 */
void ParallelScheduler::run(void(*start_rouTine)(void*), void* arg) {
    pthread_mutex_lock(queueLock); // lock queue for correct synchronization
    
    // push the pair of function pointer and its argument into the queue
    functions.push(std::pair< void(*)(void*), void* >(start_rouTine, arg));
    
    pthread_mutex_unlock(queueLock); // unlock queue
    
    // send a signal to all threads, that queue is unlocked
    pthread_cond_broadcast(condvar);
}

/*
 * execute all functions from queue
 */
void* ParallelScheduler::executeFunctions(void* arg) {
    while(true) {
        pthread_mutex_lock(queueLock); // lock the queue
        while(functions.empty()) {     // if queue is empty ...
            pthread_cond_broadcast(cancelCondvar); // send signal for cancel
            pthread_cond_wait(condvar, queueLock); // wait if from run(..) sent a signal
        }
        void(*start_rouTine)(void*) = functions.front().first;  // last function pointer in the queue
        void* arg = functions.front().second;                   // last function's argument
        functions.pop();
        pthread_mutex_unlock(queueLock); // unlock the queue
        start_rouTine(arg); // call the current function
        
    }
    return NULL;
}

/*
 * cast from void* to ParallelScheduler*, and call executeFunctions
 */
void* ParallelScheduler::executeFunctionsWrapper(void* arg) {
    reinterpret_cast<ParallelScheduler*>(arg)->executeFunctions(NULL);
    return NULL;
}


