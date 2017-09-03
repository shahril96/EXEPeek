
#include "pehandler.h"

namespace Structure
{
namespace PE
{

void PEHandler::initImport()
{
    int importTableRVA = pdatadir[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
    const char *sectionName  = getSectionNameByRVA(importTableRVA);

    char buf[100];

    if (sectionName != NULL) {

        snprintf(buf, 100, "SECTION %s", sectionName);
        treeview->insertItem("IMPORT Directory Table", buf);

        if (isILTavailable())
            treeview->insertItem("IMPORT Lookup Table", buf);

        if (isIATavailable())
            treeview->insertItem("IMPORT Address Table", buf);
    }
}

void PEHandler::handleImportDirHeader()
{
    // ------------------------------------------------------------------
    //
    // This table contains IMAGE_IMPORT_DESCRIPTOR pointer to each of
    // DLLs that PE Loader gonna import into the address space
    // of this EXE. In this structure, it contains three important 
    // members, which are:
    //
    //  -> Name                    = RVA to NULL-terminated String for DLL name.
    //
    //  ->  OriginalFirstThunk     = RVA to IMAGE_THUNK_DATA array, which further
    //     (Import Lookup Table)     contains to 'Ordinal Number' OR 'IMAGE_IMPORT_BY_NAME'.
    //
    //  ->      FirstThunk         = RVA to IMAGE_THUNK_DATA array. Initially identical 
    //     (Import Address Table)    to OriginalFirstThunk (although not pointing to the 
    //                               same IMAGE_THUNK_DATA). When PE file is loaded into 
    //                               memory, PE Loader will overwrite all table entries 
    //                               with the Virtual Address of all DLL's functions.
    //                               This process is called a binding.
    //
    // ------------------------------------------------------------------
    //
    // IMAGE_THUNK_DATA array that tells PE loader on how to import the 
    // DLL's functions. There are two ways to do so:
    //  
    //  -> Import by Ordinal = Using some kind of weird numeric ordinal
    //  -> Import by Name    = Using regular and easy-to-read function's name
    //
    // Both of these tasked as a reference on how PE loader going to load 
    // the DLL's functions. Nowadays, people doesn't uses 'import by ordinal' 
    // anymore, because it is archaic & weird. Though some older EXEs might still 
    // use this technique.
    //
    // IMAGE_THUNK_DATA contains 4 unions that refer to same the DWORD (or ULONGLONG for 64-bit) 
    // memory block, where:
    //
    //      <1-bit> <31-bit> = for 32-bit EXE
    //      <1-bit> <63-bit> = for 64-bit EXE
    //
    // 1-bit     = If set, then use Import by Ordinal
    // 31/63 bit = 'Ordinal number' OR 'RVA function's name address'
    //
    // NOTE: IMAGE_THUNK_DATA array entries are ended with NULL-terminated IMAGE_THUNK_DATA.
    //
    // ------------------------------------------------------------------

    setListView();
    
    // get initial address to IMAGE_IMPORT_DESCRIPTOR struct
    auto iid_arr = (PIMAGE_IMPORT_DESCRIPTOR)rva2raw(pdatadir[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

    // if struct is NULL, then time to stop
    while (iid_arr->Name != 0 || iid_arr->TimeDateStamp != 0) {

        InsertList("", "", "", "");
        InsertList_General(iid_arr->OriginalFirstThunk, "Import Name Table RVA");
        InsertList_TimeStamp(iid_arr->TimeDateStamp, "TimeDateStamp");
        InsertList_General(iid_arr->ForwarderChain, "ForwarderChain");
        InsertList_WithValue(iid_arr->Name, "Name RVA", (char *)rva2raw(iid_arr->Name));
        InsertList_General(iid_arr->FirstThunk, "Import Address Table RVA");

        iid_arr++;
    }
}

void PEHandler::handleImportTable(const char *treeitem_str)
{
    // set to default view
    setListView();
    
    // get initial address to IMAGE_IMPORT_DESCRIPTOR struct
    auto iid_arr = (PIMAGE_IMPORT_DESCRIPTOR)rva2raw(pdatadir[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

    // loop through all the DLLs
    for ( ; iid_arr != 0 && iid_arr->Name != 0 ; iid_arr++) {

        PIMAGE_THUNK_DATA32 pthunk32;
        PIMAGE_THUNK_DATA64 pthunk64;

        if (strstr(treeitem_str, "IMPORT Lookup Table")) {
            pthunk32 = (PIMAGE_THUNK_DATA32)rva2raw(iid_arr->OriginalFirstThunk);
            pthunk64 = (PIMAGE_THUNK_DATA64)rva2raw(iid_arr->OriginalFirstThunk);
        } else {
            pthunk32 = (PIMAGE_THUNK_DATA32)rva2raw(iid_arr->FirstThunk);
            pthunk64 = (PIMAGE_THUNK_DATA64)rva2raw(iid_arr->FirstThunk);
        }

        // macros to avoid the mess code of 32 and 64 bit PE
        #define PTHUNK_ADDR (is64bit ? (uint64_t)pthunk64 : (uint64_t)pthunk32)
        #define PTHUNK_DATA (is64bit ? pthunk64->u1.AddressOfData : pthunk32->u1.AddressOfData)
        #define PTHUNK_NOT_EMPTY (is64bit ? pthunk32 != 0 : pthunk64 != 0)
        #define PTHUNK_ORDINAL_FLAG (is64bit ? IMAGE_ORDINAL_FLAG64 : IMAGE_ORDINAL_FLAG32)

        InsertList("", "", "", "");
        InsertList("", "", "", "");
        InsertList("", "", "", (char *)rva2raw(iid_arr->Name));
        InsertList("", "", "", "-------------------");

        // loop through all IMAGE_THUNK_DATAs
        for ( ; PTHUNK_NOT_EMPTY && PTHUNK_DATA != 0 ; pthunk32++, pthunk64++) {

            auto pname = (PIMAGE_IMPORT_BY_NAME)rva2raw(PTHUNK_DATA);
            int ordinal = IMAGE_ORDINAL(PTHUNK_DATA);
            bool isOrdinal = PTHUNK_DATA & PTHUNK_ORDINAL_FLAG;

            char buf[100];

            if (isOrdinal) {

                // IF IMPORT BY ORDINAL

                snprintf(buf, 100, "%04X", ordinal);
                is64bit ? InsertList_WithValue(pthunk64->u1.AddressOfData, "(import by ordinal)", buf) 
                        : InsertList_WithValue(pthunk32->u1.AddressOfData, "(import by ordinal)", buf);

            } else if (pname != 0) {

                // IF IMPORT BY NAME

                snprintf(buf, 100, "%04X  %s", pname->Hint, pname->Name);

                is64bit ? InsertList_WithValue(pthunk64->u1.AddressOfData, "Hint/Name RVA", buf) 
                        : InsertList_WithValue(pthunk32->u1.AddressOfData, "Hint/Name RVA", buf);

            }
        }
    }
    
}

};
};