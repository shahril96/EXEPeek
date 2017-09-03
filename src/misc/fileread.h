
#ifndef FILEREAD_H
#define FILEREAD_H

#include <windows.h>

namespace Misc
{

class FileRead {
    
    HANDLE hFile, hFileMap;

    char filePath[MAX_PATH];
    char *basename;
    void *fptr;
    
public:

    FileRead(const char *file);
    ~FileRead ();
    
    void *getPtr();
    const char *getBaseName();
    int size();
    
};

};

#endif /* FILEREAD_H */

