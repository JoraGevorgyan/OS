#pragma once       // test, if this header file already difined in this project, do not it again
#include <ostream>  // for usinf ostream iterator in printFileSize function

int fileCopy(const char*, const char*);
int printFileSize(const char*, std::ostream&);

