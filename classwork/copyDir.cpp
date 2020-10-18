#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

char* mkPath(const char*, dirent*);
int freeDir(const char*);
int fileCopy(const char*, const char*);
int dirCopy(const char*, const char*);
//char* inputPath(std::istream&, bool);

#define PATH_SIZE 44

char* inputPath(std::istream& in = std::cin, bool source = false) {
    char* path = new char[PATH_SIZE];
    bool inputTest = false;
    while(!inputTest) {
        in >> path;
        if(source) {
            DIR* dir = opendir(path);
            if(dir) {
                inputTest = true;
                closedir(dir);
            } else {
                std::cout << "Somethings were wrong in your input :(\nPleas try again\n";
            }
        }
    }
    return path;
}


int main() {
    /*
    char* source = new char(PATH_SIZE);
    char* dest = new char(PATH_SIZE);
    std::cout << "enter source and destination paths to copy directory\n";
    std::cout << "Type source directory's path:\n";
    source = inputPath(std::cin,true);
    std::cout << "Type destination directory's path:\n";
    dest = inputPath();
    int dirCopyValue = dirCopy(source, dest);
    if(dest == 0) {
        std::cout << "CONGRATULATIONS\nCopying successfully ended.\n";
    } else {
        std::cout << "Sorry, something was wrong.\n";
        return -1;
    }
    delete[] source;
    delete[] dest;
    */
    char* path = inputPath(std::cin, true);
    freeDir(path);
}



int dirCopy(const char* source, const char* dest) {
    DIR* sourceD = opendir(source);
    if(!sourceD) {
        return -1;
    }
    int freeDirValue = freeDir(dest);
    if(freeDirValue != 0) {
        return -1;
    }
    int destD = mkdir(dest, 0755);
    if(destD == -1) {
        return errno;
    }

    while(true) {
        dirent* current = readdir(sourceD);
        if(!current) {
            break;
        }
        char* newDest = mkPath(dest, current);
        char* newSource = mkPath(source, current);
        if(current->d_type == DT_DIR) {
            int curDir = mkdir(newDest, 0755);
            if(curDir == -1) {
                return errno;
            }
            int dirCopyValue = dirCopy(newSource, newDest);
            if(dirCopyValue != 0) {
                return -1;
            }
         } else {
             int destFile = creat(newDest, 0755);
             if(destFile == -1) {
                 return errno;
             }
             int fileCopyValue = fileCopy(newSource, newDest);
             if(fileCopyValue != 0) {
                 return -1;
             }
         }
        delete[] newDest;
        delete[] newSource;
    }
    closedir(sourceD);
    return 0;
}

#define BUF_SIZE 400

int fileCopy(const char* source, const char* destination) {
    int sourceFd = open(source, O_RDONLY);
    if(sourceFd == -1) {
        return errno;
    }
    int destinationFd = open(destination, O_WRONLY | O_CREAT | O_TRUNC);
    if(destinationFd == -1) {
        return errno;
    }
    void* buffer = (void*) malloc(BUF_SIZE);
    while(true) {
        ssize_t readCount = read(sourceFd, buffer, BUF_SIZE);
        if(readCount == -1) {
            return errno;
        }
        if(readCount == 0) {
            break;
        }
        ssize_t writeCount = write(destinationFd, buffer, readCount);
        if(writeCount == -1) {
            return errno;
        }
    }
    free(buffer);
    close(sourceFd);
    close(destinationFd);
    return 0;
}


char* mkPath(const char* path, dirent* name) {
    std::cout << "mkPath called\n";
    size_t pathLength = strlen(path);
    size_t nameLength = strlen(name->d_name);
    char* res = new char [pathLength + nameLength + 4];
    size_t resIndex = 0;
    for(size_t i = 0; i < pathLength; ++i, ++resIndex) {
        res[resIndex] = path[i];
    }
    size_t nameTest = 0;
    while(name->d_name[nameTest++] != '.'){}
    if(nameTest + 1 < nameLength) {
        return res;
    }
    res[resIndex++] = '/';
    for(size_t i = 0; i < nameLength; ++i, ++resIndex) {
        res[resIndex] = name->d_name[i];
    }
    res[resIndex] = '\0';
    std::cout << "mkPath:\n" << res << '\n';
    return res;
}
    

int freeDir(const char* dir) {
    DIR* dirO = opendir(dir);
    std::cout << "dir opened\n";
    if(!dirO && errno != ENOENT) {
        return errno;
    }
    if(errno == ENOENT) {
        return 0;
    }
    std::cout << "while loop started\n";
    while(true) {
        dirent* current = readdir(dirO);
        std::cout << "readed dir\n";
        if(!current) {
            break;
        }
        if(current->d_type == DT_DIR) {
            freeDir(mkPath(dir, current));
         } else {
             int rmFile = remove(mkPath(dir, current));
             if(rmFile != 0) {
                 return errno;
             }
         }
         rewinddir(dirO);
    }
    int rmDir = remove(dir);
    if(rmDir != 0) {
        return errno;
    }
    return 0;
}
