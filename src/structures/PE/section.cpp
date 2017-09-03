
#include "pehandler.h"

namespace Structure
{
namespace PE 
{

void PEHandler::initSection()
{
    char buf[100];

    for (int i=0; i<nsection; i++) {
        snprintf(buf, 100, "IMAGE_SECTION_HEADER %s", section_header[i]->Name);
        treeview->insertItem(buf, fileread->getBaseName());
    }

    for (int i=0; i<nsection; i++) {
        snprintf(buf, 100, "SECTION %s", section_header[i]->Name);
        treeview->insertItem(buf, fileread->getBaseName());
    }
}

void PEHandler::handleSectionHeader(const char *treeitem_str)
{
    setListView();

    char *sectionName = strstr(treeitem_str, " ")+1;
    auto psection = getSectionByName(sectionName);

    InsertList_WithValue(psection->Name, "Name", (char*)psection->Name);
    InsertList_General(psection->VirtualAddress, "VirtualAddress");
    InsertList_General(psection->Misc.VirtualSize, "VirtualSize");
    InsertList_General(psection->SizeOfRawData, "SizeOfRawData");
    InsertList_General(psection->PointerToRawData, "PointerToRawData");
    InsertList_General(psection->PointerToRelocations, "PointerToRelocations");
    InsertList_General(psection->PointerToLinenumbers, "PointerToLinenumbers");
    InsertList_General(psection->NumberOfRelocations, "NumberOfRelocations");
    InsertList_General(psection->NumberOfLinenumbers, "NumberOfLinenumbers");
    InsertList_General(psection->Characteristics, "Characteristics");

    // iterate and check IMAGE characteristics
    char buf[100];
    for (auto it = IMAGE_CHARACTERISTIC_MAP.begin(); it != NULL; it = IMAGE_CHARACTERISTIC_MAP.next()) {
        if (psection->Characteristics & it->first) {
            snprintf(buf, 100, "%04X", it->first);
            InsertList("", "", buf, it->second);
        }
    }
}

void PEHandler::handleSection(const char *treeitem_str)
{
    char *sectionName = strstr(treeitem_str, " ")+1;
    auto psection = getSectionByName(sectionName);

    if (psection->PointerToRawData != 0) {
        setHexView(fptr + psection->PointerToRawData, psection->SizeOfRawData);
    } else {
        // if section has no data (for eq: BSS not initialized yet on disk)
        // then we will display empty hex view
        setHexView(0, 0);
    }
}

};
};