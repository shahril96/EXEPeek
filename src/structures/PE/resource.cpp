
#include "pehandler.h"

namespace Structure
{
namespace PE 
{

void PEHandler::initResourceDirectory()
{
    int importTableRVA = pdatadir[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress;
    const char *sectionName  = getSectionNameByRVA(importTableRVA);
    
    if (sectionName != NULL) {
        char buf[100];
        snprintf(buf, 100, "SECTION %s", sectionName);
        void *resourceBase = rva2raw(pdatadir[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress);
        initResourceDirectory((char *)resourceBase, (PIMAGE_RESOURCE_DIRECTORY)resourceBase, treeview->getItemHandle(buf), 0);
    }
}

void PEHandler::initResourceDirectory(char *resourceBase, PIMAGE_RESOURCE_DIRECTORY pdir, HTREEITEM parentItem, int level)
{
    HTREEITEM handle = treeview->insertItem("IMAGE_RESOURCE_DIRECTORY", parentItem);

    storeHandle[handle] = (void*) pdir;

    // get initial address to IMAGE_DIRECTORY_ENTRY_RESOURCE struct
    int count = pdir->NumberOfNamedEntries+pdir->NumberOfIdEntries; // number of resource_entry
    auto pdirEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pdir+1);

    for (int i=1; i <= count; i++, pdirEntry++) {

        char buf[500] = {0}, tmp[500];

        // if Name, then higher bit must be set
        if (pdirEntry->NameIsString) {
            // by default the Name is in unicode, we're gonna convert it into ascii
            auto prdsu = (PIMAGE_RESOURCE_DIR_STRING_U)(resourceBase + pdirEntry->NameOffset);
            WideCharToMultiByte(CP_UTF8, 0, prdsu->NameString, prdsu->Length, tmp, 500-1, 0, 0);
        } else if (level == 0) { // only first level use defined resource type
            snprintf(tmp, 500, "ID:%04x (%s)", pdirEntry->Id, RESOURCE_TYPE_MAP[pdirEntry->Id]);
        } else {
            snprintf(tmp, 500, "ID:%04x", pdirEntry->Id);
        }

        snprintf(buf, 500, "IMAGE_RESOURCE_DIRECTORY_ENTRY %s", tmp);
        HTREEITEM child_handle = treeview->insertItem(buf, handle);

        storeHandle[child_handle] = (void*) pdirEntry;
        
        if (pdirEntry->DataIsDirectory) {
            auto newpdir = (PIMAGE_RESOURCE_DIRECTORY)(resourceBase + pdirEntry->OffsetToDirectory);
            initResourceDirectory(resourceBase, newpdir, child_handle, level+1); // recursively traverse again
        } else {
            
            auto dataEntry = (PIMAGE_RESOURCE_DATA_ENTRY)(resourceBase + pdirEntry->OffsetToDirectory);
            auto dataPtr   = rva2raw(dataEntry->OffsetToData);

            HTREEITEM datahandle = treeview->insertItem("IMAGE_RESOURCE_DATA_ENTRY", child_handle);
            HTREEITEM hexdumphandle = treeview->insertItem("Hex Dump", datahandle);
            storeHandle[datahandle] = (void*)dataEntry;
            storeHandle[hexdumphandle] = (void*)dataPtr;
        }

    }
}

void PEHandler::handleDirRsrc(HTREEITEM hItem)
{
    setListView();
    auto pDir = (PIMAGE_RESOURCE_DIRECTORY) storeHandle[hItem];
    InsertList_General(pDir->Characteristics, "Characteristics");
    InsertList_TimeStamp(pDir->TimeDateStamp, "TimeDateStamp");
    InsertList_General(pDir->MajorVersion, "MajorVersion");
    InsertList_General(pDir->MinorVersion, "MinorVersion");
    InsertList_General(pDir->NumberOfNamedEntries, "NumberOfNamedEntries");
    InsertList_General(pDir->NumberOfIdEntries, "NumberOfIdEntries");
}

void PEHandler::handleDirRsrcEntry(HTREEITEM hItem)
{
    setListView();
    auto pDirEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY) storeHandle[hItem];
    InsertList_General(pDirEntry->Name, "Name");
    InsertList_General(pDirEntry->OffsetToData, "OffsetToData");
}

void PEHandler::handleDataRsrc(HTREEITEM hItem)
{
    setListView();
    auto pDataEntry = (PIMAGE_RESOURCE_DATA_ENTRY) storeHandle[hItem];
    InsertList_General(pDataEntry->OffsetToData, "OffsetToData");
    InsertList_General(pDataEntry->Size, "Size");
    InsertList_General(pDataEntry->CodePage, "CodePage");
    InsertList_General(pDataEntry->Reserved, "Reserved");
}

};
};