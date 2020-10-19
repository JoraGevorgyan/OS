#include <iostream>
#include "fileWorkLibrary.hpp"


int main(int argc, char** argv) {
    /*  test, if count of the given arguments is correct  */
    if(argc != 3) {
        std::cout << "There are more or less arguments.\n";
        return -1;
    }
    int printRet = printFileSize(argv[1], std::cout);
    /*test, if can't print size of file, and exit with an error*/
    if(printRet != 0) {
        std::cout << "can't print size of \"" << argv[1] << "\"\n";
        return printRet;
    }

    int copyFileRet = fileCopy(argv[1], argv[2]);
    /* test, if fileCopy returned no zero, it means, that, there's an error */
    if(copyFileRet != 0) {
        std::cout << "fileCopy  returned an error\n";
        return copyFileRet;
    }
    std::cout << "File successfully copied from \"" << argv[1] << "\", to \"" << argv[2] << "\"\n";
    
    printRet = printFileSize(argv[2], std::cout);
    /* test, if can't print size and exit with an error */
    if(printRet != 0) {
        std::cout << "can't print size of \"" << argv[2] << "\"\n";
        return printRet;
    }
    return 0;
}

