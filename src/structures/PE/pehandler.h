
#ifndef PEHANDLER_H
#define PEHANDLER_H

#include <windows.h>
#include <memory>
#include <string>
#include <ctime>

#include "win_constant.h"
#include "../../gui/View/listview.h"
#include "../../gui/View/treeview.h"
#include "../../gui/View/hexview.h"
#include "../../misc/fileread.h"
#include "../../misc/data_structures/uthash.hpp"

namespace Structure
{
namespace PE
{

class PEHandler
{

public:

    // view related members
    std::shared_ptr<Gui::View::ListView> listview;
    std::shared_ptr<Gui::View::HexView> hexview;

    // class related members
    HWND mainHwnd;
    std::unique_ptr<Misc::FileRead> &fileread;
    std::unique_ptr<Gui::View::TreeView> &treeview;

    // PE-related
    char                           *fptr;             // store base address
    PIMAGE_DOS_HEADER              dos_header;
    PIMAGE_NT_HEADERS              nt_header;
    int                            nsection;
    PIMAGE_SECTION_HEADER          *section_header;
    PIMAGE_DATA_DIRECTORY          pdatadir;
    bool                           is64bit;
    Misc::UTHash<HTREEITEM, void*> storeHandle;   // points tree node handle to raw pointer

    PEHandler(HWND mainHwnd, std::unique_ptr<Misc::FileRead> &fileread, std::unique_ptr<Gui::View::TreeView> &treeview);
    ~PEHandler();

    // function related to handling PE's structure
    void *rva2raw(uint64_t rva);
    const char *getSectionNameByRVA(uint64_t rva);
    PIMAGE_SECTION_HEADER getSectionByName(const char *name);

    // calculate current variable address from mmaped's base address
    template <typename T>
    uint64_t distFromBase(T &var)
    {
        return (uint64_t)&var - (uint64_t)fptr;
    }

    // -----------------------------------------------
    // functions related to view below here
    // -----------------------------------------------

    // handle treeview's item if clicked
    void handleTreeViewMessage(LPNMTREEVIEW notificationMsg);

    // functions related to handling view
    void resetAllView();
    void resize();
    void setListView();
    void setHexView(const char *startPTR, uint64_t sizePTR);

    // -----------------------------------------------------------------------------
    // LIST OF FUNCTIONS THAT WRAP THE PROCESS OF INSERTING DATA INTO THE LISTVIEW
    //
    // NOTE (for column naming convention):
    //      PFILE | DATA | LABEL | VALUE
    //
    // Note: The reason I put the implementation into the header file is because
    //       there are some functions that're using template, and template AFAIC
    //       is only suitable to be implemented into the header file.
    // -----------------------------------------------------------------------------

    void InsertList(const char *pfile, const char *data, const char *label, const char *value)
    {
        listview->addItems(4, pfile, data, label, value);
    }

    template <typename T> void InsertList_General(T &value, const char *label)
    {
        char pfile[255];
        char data[255];
        char psize[20];
        snprintf(pfile, 255, "%08llX"  , distFromBase(value));
        snprintf(psize, 20 , "%%0%dX"  , (int)(2*sizeof(T)));
        snprintf(data , 255, psize     , value);
        InsertList(pfile, data, label, "");
    }

    template <typename T> void InsertList_WithValue(T &value, const char *label, const char *strValue)
    {
        char pfile[255];
        char data[255];
        char psize[20];
        snprintf(pfile, 255, "%08llX"  , distFromBase(value));
        snprintf(psize, 20 , "%%0%dX"  , (int)(2*sizeof(T)));
        snprintf(data , 255, psize     , value);
        InsertList(pfile, data, label, strValue);
    }

    template <typename T> void InsertList_TimeStamp(T &timestamp, const char *label)
    {
        char pfile[255];
        char data[255];
        char value[255];
        char psize[20];
        snprintf(pfile, 255, "%08llX"              , distFromBase(timestamp));
        snprintf(psize, 20 , "%%0%dX"              , (int)(2*sizeof(T)));
        snprintf(data , 255, psize                 , timestamp);
        strftime(value, 255, "%a %Y-%m-%d %H:%M:%S", localtime((time_t*) &timestamp));
        InsertList(pfile, data, label, value);
    }

    void InsertList_DataDirectory(IMAGE_DATA_DIRECTORY &datadir, const char *strValue)
    {
        char label[100];
        snprintf(label, 100, "RVA   [%08llx]", (uint64_t)datadir.VirtualAddress);
        InsertList("", "", label, strValue);
        snprintf(label, 100, "Size  [%08llx]", (uint64_t)datadir.Size);
        InsertList("", "", label, "");
    }

    // ------------------------------------------
    // functions related to misc PE structure (nt header, nt optional, ...)
    // definition: misc.cpp
    // ------------------------------------------

    void imageDosHeader();
    void imageFileHeader();
    void imageOptionalHeader();

    // ------------------------------------------
    // functions related to section below here
    // definition: section.cpp
    // ------------------------------------------

    void initSection();
    void handleSectionHeader(const char *treeitem_str);
    void handleSection(const char *treeitem_str);

    // ------------------------------------------
    // function related to resource below here
    // definition: resource.cpp
    // ------------------------------------------

    void initResourceDirectory();
    void initResourceDirectory(char *resourceBase, PIMAGE_RESOURCE_DIRECTORY pdir, HTREEITEM parentItem, int level);
    void handleDirRsrc(HTREEITEM hItem);
    void handleDirRsrcEntry(HTREEITEM hItem);
    void handleDataRsrc(HTREEITEM hItem);

    // ----------------------------------------------
    // functions related to import table below here
    // definition: import.cpp
    // ----------------------------------------------
    
    void initImport();
    bool isILTavailable(); // import lookup table
    bool isIATavailable(); // import address table
    void handleImportDirHeader();
    void handleImportTable(const char *treeitem_str);

};

};
};

#endif /* PEHANDLER_H */