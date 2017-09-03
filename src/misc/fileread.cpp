
#include <windows.h>
#include <string>

#include "fileread.h"
#include "helper.h"

namespace Misc
{

FileRead::FileRead(const char *file)
{
    hFile = CreateFile(file,                   // file to open
                        GENERIC_READ,          // open for reading
                        FILE_SHARE_READ,       // share for reading
                        NULL,                  // default security
                        OPEN_EXISTING,         // existing file only
                        FILE_ATTRIBUTE_NORMAL, // normal file
                        NULL);                 // no attr. template

    if (!hFile) {
        Misc::Helper::PrintAPILastError();
    }

    // create file mapping object
    hFileMap = CreateFileMapping(hFile,             // handle to create mapping
                                    NULL,           // ptr to security attribute
                                    PAGE_READONLY,  // page protection
                                    0, 0,           // high & low DWORD specifying max size of mmap file object
                                    NULL);          // name of the file mapping object

    // map view into process address space
    fptr = MapViewOfFile(hFileMap,           // handle to a file mapping object
                            FILE_MAP_READ,   // type of access
                            0, 0,            // high & low DWORD specifying initial offset 
                            0);              // number of bytes to map (0 = till end of file mapping)

    // create a local copy
    snprintf(filePath, MAX_PATH, "%s", file);    
}

FileRead::~FileRead () {
    UnmapViewOfFile(fptr);
    CloseHandle(hFileMap);
    CloseHandle(hFile);
}

void *FileRead::getPtr()
{
    return fptr;
}

const char *FileRead::getBaseName()
{
    return Misc::Helper::getBasename(filePath);
}

int FileRead::size()
{
    return GetFileSize(hFile, NULL);
}

};