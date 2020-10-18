#include "fileWorkLibrary.hpp"
#include <iostream>
#include <errno.h>
#include <sys/types.h>  // for using "lseek" and "open"
#include <sys/stat.h>   // for using "open"
#include <fcntl.h>      // for using "open"
#include <unistd.h>     // for using "lseek"
#include <stdlib.h>     // for using "*alloc"

#define BUF_SIZE 444

int fileCopy(const char* src, const char* dest) {
    int srcD = open(src, O_RDONLY);//open file source file in "read only" mode
    /* test and exit with error code,  if can't open the file */
    if(srcD == -1) {
        std::cout << "Something were wrong, can't open file\n";
        return errno;
    }
    /* open file from "dest" as destination
     * in write only mode, and make it empty,
     * if there is no file, create it
     * with "read and write" mode for user
     */
    int destD = open(dest, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);

    /* test, if can't open file, and exit with an error */
    if(destD == -1) {
        std::cout << "can't open or create destination file\n";
        return errno;
    }
    void* buffer = (void*) malloc(BUF_SIZE);    // make buffer for copying bytes, there's void type
                                                // 'couse I have no info about file
    off_t tmp = lseek(srcD, 0, 0);   // set cursor at the begin of file
    /* test, if can't move the cursor, and exit eith an error */
    if(tmp == -1) {
        std::cout << "can't move the cursor\n";
        return errno;
    }
    while(true) {
        ssize_t readCount = read(srcD, buffer, BUF_SIZE);   // read BUF_SIZE bytes from source file
                                                            //and write it in buffer
                                                            //count readed bytes will be in readCount
        /* test, if can't read from source file and exit with an error */
        if(readCount == -1) {
            std::cout << "can't read from source file\n";
            return errno;
        }
        /*test, if there is no readed bytes from source, it means, that, it's end of file */
        if(readCount == 0) {
            break;
        }
        ssize_t writeCount = write(destD, buffer, readCount);   // write all readed bytes in
                                                                //destination file from buffer
                                                                //and count of written bytes
                                                                //will be in writeCount
        /* test, if can't write and exit with an error */
        if(writeCount == -1) {
            std::cout << "can't write in destination file\n";
            return errno;
        }
    }
    /*
     * just for testing *
    tmp = lseek(destD, 44, SEEK_END);
    std::cout << "\nlseek ::: " << tmp << "\n";
    char arr[3] = "ab";
    ssize_t wrC = write(destD, arr, 2);
        if(wrC == -1) {
            std::cout << "can't write in destination, after lseek\n";
            return errno;
        }
    */
    free(buffer);
    close(srcD);
    close(destD);
    return 0;
}


int printFileSize(const char* path, std::ostream& out) {

    int fD = open(path, O_RDONLY);//open file in "read only" mode

    /* test and exit with error code,  if can't open the file */
    if(fD == -1) {
        out << "Something were wrong, can't open file\n";
        return errno;
    }
    off_t size = lseek(fD, 0, SEEK_END);  // this will move the cursor from the begin to
                                          // end of file and count distance
    out << "The phisical size of the \"" << path << "\" is: " << size << '\n';
    size = lseek(fD, 0, SEEK_HOLE);   // this will move the cursor from the begin to
                                        // end of "hole" (may be it not exist)
    out << "The logical size of the \"" << path << "\" is: " << size << '\n';
    close(fD);
    return 0;
}

