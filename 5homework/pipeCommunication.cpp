#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>

#include <iostream>

#define MAX_RAND_VALUE 10

// check if error isn't 0 and exit with error message
void testPipeCreation(int error) {
    if(error != 0) {
        std::cout << "Can't create a pipe: errno = " << errno << std::endl;
        exit(errno);
    }
}
    
// check if can't write and exit with error message
void testWritten(int written) { 
    if(written == -1) {
        std::cout << "Can't write in file: errno = " << errno << std::endl;
        exit(errno);
    }
}

// chack if can't read and exit with error message
void testDelivered(int delivered) {
    if(delivered == -1) {
        std::cout << "Can't read from file: errno = " << errno << std::endl;
        exit(errno);
    }
}

// check if pid is -1 and exit with error message
void testProcessCreation(int pid) {
    if(pid == -1) {
        std::cout << "Can't create a child process: errno = " << errno << std::endl;
        exit(errno);
    }
}

void printArr(int* arr, size_t arrSize) {
    for(size_t  i = 0; i < arrSize; ++i) {
        std::cout << arr[i] << ' ';
    }
    std::cout << std::endl;
}

int main() {
    size_t arrSize;
    std::cout << "Enter count of elements, that you want to sum: ";
    std::cin >> arrSize;
    int* arr = new int[arrSize];

    // fill an array with random integer numbers less than MAX_RAND_VALUE
    for(size_t i = 0; i < arrSize; ++i) {
        arr[i] = rand() % MAX_RAND_VALUE;
    }

    printArr(arr, arrSize);

    // array for pipe descriptors
    int elementsPipefd[2];

    // create a pipe for elements
    int err = pipe(elementsPipefd);
    testPipeCreation(err);

    // write all numbers into pipe
    int written = write(elementsPipefd[1], arr, sizeof(int) * arrSize);
    testWritten(written);

    // array for pipe descriptors
    int partSumsPipefd[2];

    // create a pipe for part sums
    err = pipe(partSumsPipefd);
    testPipeCreation(err);

    size_t workersCount;
    std::cout << "Enter count of workers for counting: ";
    std::cin >> workersCount;

    size_t leftIndex = 0;
    size_t loopCount = workersCount;

    pid_t parentPID = getpid();

    while(loopCount--) {
        size_t partSize = arrSize / workersCount;
        if(loopCount == 1) {
            partSize += arrSize % workersCount;
        }

        int child = fork();
        testProcessCreation(child);

        if(child == 0) {
            close(elementsPipefd[1]);
            close(partSumsPipefd[0]);

            int* partArr = new int[partSize];

            // read current part from pipe
            int delivered = read(elementsPipefd[0], partArr, sizeof(int) * partSize);
            testDelivered(delivered);

            // count and print sum of current part
            size_t partSum = 0;
            for(size_t i = 0; i < partSize; ++i) {
                partSum += partArr[i];
            }
            std::cout << "process ID: " << getpid() << std::endl
                << "sum of segment [" << leftIndex + 1 << ',' << leftIndex + partSize 
                << "] is: " << partSum << std::endl;


            // write the value of part sum into pipe
            written = write(partSumsPipefd[1], &partSum, sizeof(size_t));
            testWritten(written);

            close(elementsPipefd[0]);
            close(partSumsPipefd[1]);
            delete[] partArr;
            
            break;
        }

        leftIndex += partSize;
    }

    if(getpid() == parentPID) {
        close(elementsPipefd[0]);
        close(partSumsPipefd[1]);

        size_t sum = 0;
        while(workersCount--) {
            size_t curSum;
            int delivered = read(partSumsPipefd[0], &curSum, sizeof(size_t));
            testDelivered(delivered);

            sum += curSum;
        }
        std::cout << "Sum of all elements: " << sum << std::endl;

        close(elementsPipefd[1]);
        close(partSumsPipefd[0]);
    }
    
    delete[] arr;
    return 0;
}
