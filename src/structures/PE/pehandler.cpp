
#include <windows.h>
#include <memory>

#include "pehandler.h"
#include "../../gui/View/listview.h"
#include "../../gui/View/treeview.h"
#include "../../misc/fileread.h"
#include "../../resources.h"

namespace Structure
{
namespace PE
{

PEHandler::PEHandler(HWND mainHwnd, std::unique_ptr<Misc::FileRead> &fileread, std::unique_ptr<Gui::View::TreeView> &treeview) 
    : mainHwnd(mainHwnd), fileread(fileread),treeview(treeview)
{

    // ----------------------------------------------
    // pre-process the PE file (minimal processing)
    // ----------------------------------------------
    
    fptr       = (char *)(fileread->getPtr()); // base address
    dos_header = (PIMAGE_DOS_HEADER)fptr;
    nt_header  = (PIMAGE_NT_HEADERS)(fptr + dos_header->e_lfanew);
    is64bit    = nt_header->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC;

    int nt_header_size = 0;

    // 1) get pointer to & DataDirectory
    // 2) get size of NT_HEADER struct based on PE architecture
    if (is64bit) {
        pdatadir       = ((PIMAGE_OPTIONAL_HEADER64)&nt_header->OptionalHeader)->DataDirectory;
        nt_header_size = sizeof(IMAGE_NT_HEADERS64);
    } else {
        pdatadir       = ((PIMAGE_OPTIONAL_HEADER32)&nt_header->OptionalHeader)->DataDirectory;
        nt_header_size = sizeof(IMAGE_NT_HEADERS32);
    }

    // populate section ptr array
    nsection       = nt_header->FileHeader.NumberOfSections;
    section_header = new PIMAGE_SECTION_HEADER[nsection];
    for(int i=0; i<nsection; i++)
        section_header[i] = (PIMAGE_SECTION_HEADER)((char*)nt_header + nt_header_size) + i;

    // ------------------------------------------
    // populate treeview with PE structure items
    // ------------------------------------------

    treeview->insertItemRoot(fileread->getBaseName());
    treeview->insertItem("IMAGE_DOS_HEADER", fileread->getBaseName());
    treeview->insertItem("MS-DOS Stub Program", fileread->getBaseName());
    treeview->insertItem("IMAGE_NT_HEADERS", fileread->getBaseName());
    treeview->insertItem("Signature", "IMAGE_NT_HEADERS");
    treeview->insertItem("IMAGE_FILE_HEADER", "IMAGE_NT_HEADERS");
    treeview->insertItem("IMAGE_OPTIONAL_HEADER", "IMAGE_NT_HEADERS");

    // set initial hex view (for whole file) for first time
    setHexView((char*) fileread->getPtr(), fileread->size());

    initSection();
    initImport();
    initResourceDirectory();
}

PEHandler::~PEHandler()
{
}

// -------------------------------------------------------------
// ALL FUNCTIONS RELATED TO SETTINGS UP VIEW WILL BE BELOW HERE
// -------------------------------------------------------------

void PEHandler::resetAllView()
{
    listview.reset();
    hexview.reset();
}

void PEHandler::resize()
{
    treeview->configureLook();
    if (listview.get()) listview->configureLook();
    if (hexview.get()) hexview->configureLook();
}

void PEHandler::setListView()
{
    resetAllView();

    // create new listview
    listview = std::make_shared<Gui::View::ListView>(mainHwnd);

    // setup listview look
    listview->configureLook();

    // setup the appropriate settings
    listview->addColumn("pFile (h)", 120);
    listview->addColumn("Data (h)", 180);
    listview->addColumn("Description", 350);
    listview->addColumn("Value", 600);
}

void PEHandler::setHexView(const char *startPTR, uint64_t sizePTR)
{
    resetAllView();

    // create new hexview
    hexview = std::make_shared<Gui::View::HexView>(mainHwnd, fptr, startPTR, sizePTR);

    // setup hexview look
    hexview->configureLook();

    // initially print the first line
    hexview->printHex(0);
}

// ---------------------------------------------------------
// ALL FUNCTIONS RELATED TO PE STRUCTURE WILL BE BELOW HERE
// ---------------------------------------------------------

void* PEHandler::rva2raw(uint64_t rva)
{
    // iterate each section
    for (int i = 0; i < nsection; i++) {

        uint64_t VA_start = section_header[i]->VirtualAddress;
        uint64_t VA_end   = VA_start + section_header[i]->Misc.VirtualSize;

        // if RVA is in the range section, then return raw offset
        if (rva >= VA_start && rva < VA_end)  {
            
            // file Offset + (VA of section - Relative VA)
            return (fptr + section_header[i]->PointerToRawData + (rva - VA_start));
        }
    }

    return NULL;
}

const char* PEHandler::getSectionNameByRVA(uint64_t rva)
{
    // iterate each section
    for (int i = 0; i < nsection; i++) {

        uint64_t VA_start = section_header[i]->VirtualAddress;
        uint64_t VA_end   = VA_start + section_header[i]->Misc.VirtualSize;

        // if RVA is in the range section, then return its name string address
        if (rva >= VA_start && rva < VA_end)  {
            return (const char *)section_header[i]->Name;
        }
    }

    return NULL;
}

PIMAGE_SECTION_HEADER PEHandler::getSectionByName(const char *name)
{
    // return pointer to IMAGE_SECTION_HEADER by searching for its name
    for(int i=0; i<nsection; i++) {
        if (strncmp((const char *)section_header[i]->Name, name, IMAGE_SIZEOF_SHORT_NAME) == 0) {
            return section_header[i];
        }
    }

    return NULL;
}

bool PEHandler::isILTavailable()
{
    // get initial address to IMAGE_IMPORT_DESCRIPTOR struct
    auto iid_arr = (PIMAGE_IMPORT_DESCRIPTOR)rva2raw(pdatadir[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

    // loop through all the DLLs
    for ( ; iid_arr != 0 && iid_arr->Name != 0 ; iid_arr++) {
        if (iid_arr->OriginalFirstThunk > 0) {
            return true;
        }
    }

    return false;
}

bool PEHandler::isIATavailable()
{
    // get initial address to IMAGE_IMPORT_DESCRIPTOR struct
    auto iid_arr = (PIMAGE_IMPORT_DESCRIPTOR)rva2raw(pdatadir[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

    // loop through all the DLLs
    for ( ; iid_arr != 0 && iid_arr->Name != 0 ; iid_arr++) {
        if (iid_arr->FirstThunk > 0) {
            return true;
        }
    }

    return false;
}

// ---------------------------------------------------------
// BELOW FUNCTIONS HANDLE THE PARSING OF PE-FILE ITSELF AND
// RESPONSIBLE FOR DISPLAYING IT TO THE USER
// ---------------------------------------------------------

void PEHandler::handleTreeViewMessage(LPNMTREEVIEW notificationMsg)
{
    // get current selected item's string value
    char itemStr[200];
    auto hItem = notificationMsg->itemNew.hItem;
    treeview->getStrValueFromItem(hItem, itemStr, 200);

    // ----------------------------------------------
    // Match what user clicked, then give user
    // appropriate information about the PE structure
    // he wants to view
    // ----------------------------------------------

    if (strstr(itemStr, fileread->getBaseName())) {
        
        setHexView((char*) fileread->getPtr(), fileread->size());

    } else if (strstr(itemStr, "IMAGE_DOS_HEADER")) {

        imageDosHeader();

    } else if (strstr(itemStr, "MS-DOS Stub Program")) {

        char *ptrToStub = (char *)fileread->getPtr() + sizeof(IMAGE_DOS_HEADER);
        int stubSize    = (int)((char*)&nt_header->Signature - ptrToStub);
        setHexView(ptrToStub, stubSize);

    } else if (strstr(itemStr, "IMAGE_NT_HEADERS")) {

        setHexView((char*)nt_header, is64bit ? sizeof(IMAGE_NT_HEADERS64) : sizeof(IMAGE_NT_HEADERS32));

    } else if (strstr(itemStr, "Signature")) {

        setListView();
        InsertList_WithValue(nt_header->Signature, "Signature", SIGNATURE_MAP[nt_header->Signature]);

    } else if (strstr(itemStr, "IMAGE_FILE_HEADER")) {

        imageFileHeader();

    } else if (strstr(itemStr, "IMAGE_OPTIONAL_HEADER")) {

        imageOptionalHeader();

    } else if (strstr(itemStr, "IMAGE_SECTION_HEADER")) {

        handleSectionHeader(itemStr);
    
    } else if (strstr(itemStr, "SECTION")) {

        handleSection(itemStr);

    } else if (strstr(itemStr, "IMPORT Directory Table")) {

        handleImportDirHeader();
    
    } else if (strstr(itemStr, "IMPORT Lookup Table") || strstr(itemStr, "IMPORT Address Table")) {

        handleImportTable(itemStr);
    
    } else if (strstr(itemStr, "IMAGE_RESOURCE_DIRECTORY_ENTRY")) {
        
        handleDirRsrcEntry(hItem);

    } else if (strstr(itemStr, "IMAGE_RESOURCE_DIRECTORY")) {

        handleDirRsrc(hItem);

    } else if (strstr(itemStr, "IMAGE_RESOURCE_DATA_ENTRY")) {

        handleDataRsrc(hItem);

    }

}

};
};