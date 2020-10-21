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
    /* make buffer for copying bytes, there's void type becouse I have no info about file */
    void* buffer = (void*) malloc(BUF_SIZE);
    off_t cursor = lseek(srcD, 0, SEEK_SET);   // set cursor at the begin of file
    /* test, if can't move the cursor, and exit eith an error */
    if(cursor == -1) {
        std::cout << "can't move the cursor\n";
        return errno;
    }
    int x = 4;   // while loop breaking
    while(x--) {
        /*move the cursor into the next data*/
        off_t srcCursor = lseek(srcD, SEEK_CUR - 1, SEEK_DATA);
        
        std::cout << "\n42 :: " << srcCursor << '\n';

        /*make the hole in destination from current, in the same count, that was in the source*/
        off_t destCursor = lseek(destD, srcCursor, SEEK_END);

        std::cout << "\n47 :: " << destCursor << '\n';

        cursor = lseek(srcD, SEEK_CUR - 1, SEEK_SET); //save the value of current

        std::cout << "\n52::" << cursor << '\n';

        /*move the cursor into the next hole, and count of date will be written in srcCursor*/
        srcCursor = lseek(srcD, SEEK_CUR - 1, SEEK_HOLE);
        
        std::cout << "\n53 :: " << srcCursor << '\n';

        /*move the cursor into the last readen bytes*/
        cursor = lseek(srcD, cursor, SEEK_SET);

        std::cout << "\n58:: " << cursor << '\n';

        /*
         * if srcCursor > BUF_SIZE, I need read and write
         * just BUF_SIZE bytes, but else
         * I need read and write anly $srcCount bytes
         * and then continue
         */
        if(srcCursor > BUF_SIZE) {
            ssize_t readCount = read(srcD, buffer, BUF_SIZE);
            if(readCount == -1) {
                std::cout << "can't read from source\n";
                return errno;
            }
            ssize_t writeCount = write(destD, buffer, readCount);
            if(writeCount == -1) {
                std::cout << "can't write in the destination\n";
                return errno;
            }
        } else {
            ssize_t readCount = read(srcD, buffer, srcCursor);

            std::cout << "readCount == " << readCount << '\n';

            if(readCount == -1) {
                std::cout << "can't read from source\n";
                return errno;
            }

            cursor = lseek(srcD, 0, SEEK_CUR);
            std::cout << "\n85 :: " << cursor << '\n';

            /*it means, that's end of file*/
            if(readCount == 0) {
                break;
            }
            ssize_t writeCount = write(destD, buffer, readCount);
            if(writeCount == -1) {
                std::cout << "can't write in the destination\n";
                return errno;
            }
        }
    }
    free(buffer);
    close(srcD);
    close(destD);
    return 0;
}


int printFileSize(const char* path, std::ostream& out) {
    out << "size of the file\n\n";
/*
    int fD = open(path, O_RDONLY);//open file in "read only" mode

    // test and exit with error code,  if can't open the file
    if(fD == -1) {
        out << "Something were wrong, can't open file\n";
        return errno;
    }
    off_t size = lseek(fD, 0, SEEK_END);
    out << "The phisical size of the \"" << path << "\" is: " << size << '\n';
    size = lseek(fD, 0, SEEK_HOLE);
    out << "The logical size of the \"" << path << "\" is: " << size << '\n';
    close(fD);
    return 0;
*/

}


