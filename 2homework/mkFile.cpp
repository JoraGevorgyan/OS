#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char** argv) {
    if(argc != 2) {
        std::cout << "ther's no file path\n";
        return 0;
    }
    int fD = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
    if(fD == -1) {
        std::cout << "can't open the file\n";
        return errno;
    }
    ssize_t strSize;
    std::cout << "type the size of string: ";
    std::cin >> strSize;
    char* str = new char[strSize];
    for(size_t i = 0; i < strSize; ++i) {
        str[i] = '#';
    }
    ssize_t writeCount = write(fD, str, strSize);
    if(writeCount == -1) {
        std::cout << "can't write in the file\n";
        return errno;
    }
    off_t tmp = lseek(fD, strSize, SEEK_END);
    if(tmp == -1) {
        std::cout << "can't move the cursor\n";
        return errno;
    }
    writeCount = write(fD, str, strSize);
    if(writeCount == -1) {
        std::cout << "can't write\n";
        return errno;
    }
    std::cout << "done!\n";
    close(fD);
    delete[] str;
}
