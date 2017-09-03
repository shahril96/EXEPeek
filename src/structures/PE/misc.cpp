
#include "pehandler.h"

namespace Structure
{
namespace PE 
{

void PEHandler::imageDosHeader()
{
    setListView();

    InsertList_WithValue(dos_header->e_magic, "MZ Header signature", SIGNATURE_MAP[dos_header->e_magic]);
    InsertList_General(dos_header->e_cblp, "Bytes on last page of file");
    InsertList_General(dos_header->e_cp, "Pages in file");
    InsertList_General(dos_header->e_crlc, "Relocations");
    InsertList_General(dos_header->e_cparhdr, "Size of header in paragraphs");
    InsertList_General(dos_header->e_minalloc, "Minimum extra paragraphs needed");
    InsertList_General(dos_header->e_maxalloc, "Maximum extra paragraphs needed");
    InsertList_General(dos_header->e_ss, "Initial (relative) SS value");
    InsertList_General(dos_header->e_sp, "Initial SP value");
    InsertList_General(dos_header->e_csum, "Checksum");
    InsertList_General(dos_header->e_ip, "Initial IP value");
    InsertList_General(dos_header->e_cs, "Initial (relative) CS value");
    InsertList_General(dos_header->e_lfarlc, "File address of relocation table");
    InsertList_General(dos_header->e_ovno, "Overlay number");
    InsertList_General(dos_header->e_res[0], "e_res[0] Reserved word");
    InsertList_General(dos_header->e_res[1], "e_res[1] Reserved word");
    InsertList_General(dos_header->e_res[2], "e_res[2] Reserved word");
    InsertList_General(dos_header->e_res[3], "e_res[3] Reserved word");
    InsertList_General(dos_header->e_oemid, "OEM identifier (for e_oeminfo)");
    InsertList_General(dos_header->e_oeminfo, "OEM information; e_oemid specific");
    InsertList_General(dos_header->e_res2[0], "e_res2[0] Reserved word");
    InsertList_General(dos_header->e_res2[1], "e_res2[1] Reserved word");
    InsertList_General(dos_header->e_res2[2], "e_res2[2] Reserved word");
    InsertList_General(dos_header->e_res2[3], "e_res2[3] Reserved word");
    InsertList_General(dos_header->e_res2[4], "e_res2[4] Reserved word");
    InsertList_General(dos_header->e_res2[5], "e_res2[5] Reserved word");
    InsertList_General(dos_header->e_res2[6], "e_res2[6] Reserved word");
    InsertList_General(dos_header->e_res2[7], "e_res2[7] Reserved word");
    InsertList_General(dos_header->e_res2[8], "e_res2[8] Reserved word");
    InsertList_General(dos_header->e_res2[9], "e_res2[9] Reserved word");
    InsertList_General(dos_header->e_lfanew, "Offset to extended header");
}

void PEHandler::imageFileHeader()
{
    setListView();

    InsertList_WithValue(nt_header->FileHeader.Machine, "Machine", MACHINE_MAP[nt_header->FileHeader.Machine]);
    InsertList_General(nt_header->FileHeader.NumberOfSections, "NumberOfSections");
    InsertList_TimeStamp(nt_header->FileHeader.TimeDateStamp, "TimeDateStamp");
    InsertList_General(nt_header->FileHeader.PointerToSymbolTable, "PointerToSymbolTable");
    InsertList_General(nt_header->FileHeader.NumberOfSymbols, "NumberOfSymbols");
    InsertList_General(nt_header->FileHeader.SizeOfOptionalHeader, "SizeOfOptionalHeader");
    InsertList_General(nt_header->FileHeader.Characteristics, "Characteristics");

    // iterate and check PE characteristics
    char buf[100];
    for (auto it = PE_CHARACTERISTIC_MAP.begin(); it != NULL; it = PE_CHARACTERISTIC_MAP.next()) {
        if (nt_header->FileHeader.Characteristics & it->first) {
            snprintf(buf, 100, "%04X", it->first);
            InsertList("", "", (char*)buf, (char*)it->second);
        }
    }
}

void PEHandler::imageOptionalHeader()
{
    setListView();
    
    PIMAGE_OPTIONAL_HEADER32 popt_32 = (PIMAGE_OPTIONAL_HEADER32)&nt_header->OptionalHeader;
    PIMAGE_OPTIONAL_HEADER64 popt_64 = (PIMAGE_OPTIONAL_HEADER64)&nt_header->OptionalHeader;

    InsertList_WithValue(popt_32->Magic, "Magic", MAGIC_MAP[popt_32->Magic]);
    InsertList_General(popt_32->MajorLinkerVersion, "MajorLinkerVersion");
    InsertList_General(popt_32->MinorLinkerVersion, "MinorLinkerVersion");
    InsertList_General(popt_32->SizeOfCode, "SizeOfCode");
    InsertList_General(popt_32->SizeOfInitializedData, "SizeOfInitializedData");
    InsertList_General(popt_32->SizeOfUninitializedData, "SizeOfUninitializedData");
    InsertList_General(popt_32->SizeOfUninitializedData, "SizeOfUninitializedData");
    InsertList_General(popt_32->AddressOfEntryPoint, "AddressOfEntryPoint");
    InsertList_General(popt_32->BaseOfCode, "BaseOfCode");
    if (!is64bit) { InsertList_General(popt_32->BaseOfData, "BaseOfData"); }
    if (is64bit)  { InsertList_General(popt_64->ImageBase, "ImageBase"); }
    if (!is64bit) { InsertList_General(popt_32->ImageBase, "ImageBase"); }
    InsertList_General(popt_32->SectionAlignment, "SectionAlignment");
    InsertList_General(popt_32->FileAlignment, "FileAlignment");
    InsertList_General(popt_32->MajorOperatingSystemVersion, "MajorOperatingSystemVersion");
    InsertList_General(popt_32->MinorOperatingSystemVersion, "MinorOperatingSystemVersion");
    InsertList_General(popt_32->MajorImageVersion, "MajorImageVersion");
    InsertList_General(popt_32->MinorImageVersion, "MinorImageVersion");
    InsertList_General(popt_32->MajorSubsystemVersion, "MajorSubsystemVersion");
    InsertList_General(popt_32->MinorSubsystemVersion, "MinorSubsystemVersion");
    InsertList_General(popt_32->Win32VersionValue, "Win32VersionValue");
    InsertList_General(popt_32->SizeOfImage, "SizeOfImage");
    InsertList_General(popt_32->SizeOfHeaders, "SizeOfHeaders");
    InsertList_General(popt_32->CheckSum, "CheckSum");
    InsertList_WithValue(popt_32->Subsystem, "Subsystem", SUBSYSTEM_MAP[popt_32->Subsystem]);
    InsertList_General(popt_32->DllCharacteristics, "DllCharacteristics");

    // iterate and check DLL characteristics
    char buf[100];
    for (auto it = DLL_CHARACTERISTIC_MAP.begin(); it != NULL; it = DLL_CHARACTERISTIC_MAP.next()) {
        if (popt_32->DllCharacteristics & it->first) {
            snprintf(buf, 100, "%04X", it->first);
            InsertList("", "", (char*)buf, (char*)it->second);
        }
    }

    if (is64bit) {
        InsertList_General(popt_64->SizeOfStackReserve, "SizeOfStackReserve");
        InsertList_General(popt_64->SizeOfStackCommit, "SizeOfStackCommit");
        InsertList_General(popt_64->SizeOfHeapReserve, "SizeOfHeapReserve");
        InsertList_General(popt_64->SizeOfHeapCommit, "SizeOfHeapCommit");
        InsertList_General(popt_64->LoaderFlags, "LoaderFlags");
        InsertList_General(popt_64->NumberOfRvaAndSizes, "NumberOfRvaAndSizes");
    } else {
        InsertList_General(popt_32->SizeOfStackReserve, "SizeOfStackReserve");
        InsertList_General(popt_32->SizeOfStackCommit, "SizeOfStackCommit");
        InsertList_General(popt_32->SizeOfHeapReserve, "SizeOfHeapReserve");
        InsertList_General(popt_32->SizeOfHeapCommit, "SizeOfHeapCommit");
        InsertList_General(popt_32->LoaderFlags, "LoaderFlags");
        InsertList_General(popt_32->NumberOfRvaAndSizes, "Number Of Data Directories");
    }
    
    InsertList_DataDirectory(pdatadir[IMAGE_DIRECTORY_ENTRY_EXPORT], "EXPORT table");
    InsertList_DataDirectory(pdatadir[IMAGE_DIRECTORY_ENTRY_IMPORT], "IMPORT table");
    InsertList_DataDirectory(pdatadir[IMAGE_DIRECTORY_ENTRY_RESOURCE], "RESOURCE table");
    InsertList_DataDirectory(pdatadir[IMAGE_DIRECTORY_ENTRY_EXCEPTION], "EXCEPTION table");
    InsertList_DataDirectory(pdatadir[IMAGE_DIRECTORY_ENTRY_SECURITY], "CERTIFICATE table");
    InsertList_DataDirectory(pdatadir[IMAGE_DIRECTORY_ENTRY_BASERELOC], "BASE RELOCATION table");
    InsertList_DataDirectory(pdatadir[IMAGE_DIRECTORY_ENTRY_DEBUG], "DEBUG Directory");
    InsertList_DataDirectory(pdatadir[7], "Architecture specific data");
    InsertList_DataDirectory(pdatadir[IMAGE_DIRECTORY_ENTRY_GLOBALPTR], "GLOBAL POINTER register");
    InsertList_DataDirectory(pdatadir[IMAGE_DIRECTORY_ENTRY_TLS], "TLS table");
    InsertList_DataDirectory(pdatadir[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG], "LOAD CONFIGURATION table");
    InsertList_DataDirectory(pdatadir[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT], "BOUND IMPORT table");
    InsertList_DataDirectory(pdatadir[IMAGE_DIRECTORY_ENTRY_IAT], "IMPORT ADDRESS table");
    InsertList_DataDirectory(pdatadir[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT], "DELAY IMPORT Descriptors");
    InsertList_DataDirectory(pdatadir[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR], "CLI Header");
    InsertList_DataDirectory(pdatadir[15], "Reserved");
}

};
};