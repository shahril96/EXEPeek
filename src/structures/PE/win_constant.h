
#ifndef WIN_CONSTANT_H
#define WIN_CONSTANT_H

#include "../../misc/data_structures/uthash.hpp"

#define DEFINE_WIN_CONST(msg) { (unsigned int)(uint64_t)msg, #msg }

namespace Structure
{
namespace PE
{
namespace
{

Misc::UTHash<unsigned int, const char *> SIGNATURE_MAP = 
{
    DEFINE_WIN_CONST(IMAGE_DOS_SIGNATURE),
    DEFINE_WIN_CONST(IMAGE_OS2_SIGNATURE),
    DEFINE_WIN_CONST(IMAGE_OS2_SIGNATURE_LE),
    DEFINE_WIN_CONST(IMAGE_VXD_SIGNATURE),
    DEFINE_WIN_CONST(IMAGE_NT_SIGNATURE)
};

Misc::UTHash<unsigned int, const char *> MACHINE_MAP = 
{
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_UNKNOWN),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_I386),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_R3000),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_R4000),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_R10000),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_WCEMIPSV2),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_ALPHA),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_SH3),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_SH3DSP),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_SH3E),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_SH4),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_SH5),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_ARM),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_THUMB),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_ARMNT),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_AM33),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_POWERPC),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_POWERPCFP),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_IA64),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_MIPS16),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_ALPHA64),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_MIPSFPU),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_MIPSFPU16),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_AXP64),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_TRICORE),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_CEF),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_EBC),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_AMD64),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_M32R),
    DEFINE_WIN_CONST(IMAGE_FILE_MACHINE_CEE)
};

Misc::UTHash<unsigned int, const char *> PE_CHARACTERISTIC_MAP = 
{
    DEFINE_WIN_CONST(IMAGE_FILE_RELOCS_STRIPPED),
    DEFINE_WIN_CONST(IMAGE_FILE_EXECUTABLE_IMAGE),
    DEFINE_WIN_CONST(IMAGE_FILE_LINE_NUMS_STRIPPED),
    DEFINE_WIN_CONST(IMAGE_FILE_LOCAL_SYMS_STRIPPED),
    DEFINE_WIN_CONST(IMAGE_FILE_AGGRESIVE_WS_TRIM),
    DEFINE_WIN_CONST(IMAGE_FILE_LARGE_ADDRESS_AWARE),
    DEFINE_WIN_CONST(IMAGE_FILE_BYTES_REVERSED_LO),
    DEFINE_WIN_CONST(IMAGE_FILE_32BIT_MACHINE),
    DEFINE_WIN_CONST(IMAGE_FILE_DEBUG_STRIPPED),
    DEFINE_WIN_CONST(IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP),
    DEFINE_WIN_CONST(IMAGE_FILE_NET_RUN_FROM_SWAP),
    DEFINE_WIN_CONST(IMAGE_FILE_SYSTEM),
    DEFINE_WIN_CONST(IMAGE_FILE_DLL),
    DEFINE_WIN_CONST(IMAGE_FILE_UP_SYSTEM_ONLY),
    DEFINE_WIN_CONST(IMAGE_FILE_BYTES_REVERSED_HI)
};

Misc::UTHash<unsigned int, const char *> MAGIC_MAP = 
{
    DEFINE_WIN_CONST(IMAGE_NT_OPTIONAL_HDR32_MAGIC),
    DEFINE_WIN_CONST(IMAGE_NT_OPTIONAL_HDR64_MAGIC),
    DEFINE_WIN_CONST(IMAGE_ROM_OPTIONAL_HDR_MAGIC)
};

Misc::UTHash<unsigned int, const char *> SUBSYSTEM_MAP = 
{
    DEFINE_WIN_CONST(IMAGE_SUBSYSTEM_UNKNOWN),
    DEFINE_WIN_CONST(IMAGE_SUBSYSTEM_NATIVE),
    DEFINE_WIN_CONST(IMAGE_SUBSYSTEM_WINDOWS_GUI),
    DEFINE_WIN_CONST(IMAGE_SUBSYSTEM_WINDOWS_CUI),
    DEFINE_WIN_CONST(IMAGE_SUBSYSTEM_OS2_CUI),
    DEFINE_WIN_CONST(IMAGE_SUBSYSTEM_POSIX_CUI),
    DEFINE_WIN_CONST(IMAGE_SUBSYSTEM_NATIVE_WINDOWS),
    DEFINE_WIN_CONST(IMAGE_SUBSYSTEM_WINDOWS_CE_GUI),
    DEFINE_WIN_CONST(IMAGE_SUBSYSTEM_EFI_APPLICATION),
    DEFINE_WIN_CONST(IMAGE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER),
    DEFINE_WIN_CONST(IMAGE_SUBSYSTEM_EFI_RUNTIME_DRIVER),
    DEFINE_WIN_CONST(IMAGE_SUBSYSTEM_EFI_ROM),
    DEFINE_WIN_CONST(IMAGE_SUBSYSTEM_XBOX),
    DEFINE_WIN_CONST(IMAGE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION)
};

Misc::UTHash<unsigned int, const char *> DLL_CHARACTERISTIC_MAP = 
{
    DEFINE_WIN_CONST(IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE),
    DEFINE_WIN_CONST(IMAGE_DLLCHARACTERISTICS_FORCE_INTEGRITY),
    DEFINE_WIN_CONST(IMAGE_DLLCHARACTERISTICS_NX_COMPAT),
    DEFINE_WIN_CONST(IMAGE_DLLCHARACTERISTICS_NO_ISOLATION),
    DEFINE_WIN_CONST(IMAGE_DLLCHARACTERISTICS_NO_SEH),
    DEFINE_WIN_CONST(IMAGE_DLLCHARACTERISTICS_NO_BIND),
    DEFINE_WIN_CONST(IMAGE_DLLCHARACTERISTICS_WDM_DRIVER),
    DEFINE_WIN_CONST(IMAGE_DLLCHARACTERISTICS_TERMINAL_SERVER_AWARE)
};

Misc::UTHash<unsigned int, const char *> IMAGE_CHARACTERISTIC_MAP = 
{
    DEFINE_WIN_CONST(IMAGE_SCN_TYPE_NO_PAD),
    DEFINE_WIN_CONST(IMAGE_SCN_CNT_CODE),
    DEFINE_WIN_CONST(IMAGE_SCN_CNT_INITIALIZED_DATA),
    DEFINE_WIN_CONST(IMAGE_SCN_CNT_UNINITIALIZED_DATA),
    DEFINE_WIN_CONST(IMAGE_SCN_LNK_OTHER),
    DEFINE_WIN_CONST(IMAGE_SCN_LNK_INFO),
    DEFINE_WIN_CONST(IMAGE_SCN_LNK_REMOVE),
    DEFINE_WIN_CONST(IMAGE_SCN_LNK_COMDAT),
    DEFINE_WIN_CONST(IMAGE_SCN_MEM_FARDATA),
    DEFINE_WIN_CONST(IMAGE_SCN_MEM_PURGEABLE),
    DEFINE_WIN_CONST(IMAGE_SCN_MEM_16BIT),
    DEFINE_WIN_CONST(IMAGE_SCN_MEM_LOCKED),
    DEFINE_WIN_CONST(IMAGE_SCN_MEM_PRELOAD),
    DEFINE_WIN_CONST(IMAGE_SCN_ALIGN_1BYTES),
    DEFINE_WIN_CONST(IMAGE_SCN_ALIGN_2BYTES),
    DEFINE_WIN_CONST(IMAGE_SCN_ALIGN_4BYTES),
    DEFINE_WIN_CONST(IMAGE_SCN_ALIGN_8BYTES),
    DEFINE_WIN_CONST(IMAGE_SCN_ALIGN_16BYTES),
    DEFINE_WIN_CONST(IMAGE_SCN_ALIGN_32BYTES),
    DEFINE_WIN_CONST(IMAGE_SCN_ALIGN_64BYTES),
    DEFINE_WIN_CONST(IMAGE_SCN_ALIGN_128BYTES),
    DEFINE_WIN_CONST(IMAGE_SCN_ALIGN_256BYTES),
    DEFINE_WIN_CONST(IMAGE_SCN_ALIGN_512BYTES),
    DEFINE_WIN_CONST(IMAGE_SCN_ALIGN_1024BYTES),
    DEFINE_WIN_CONST(IMAGE_SCN_ALIGN_2048BYTES),
    DEFINE_WIN_CONST(IMAGE_SCN_ALIGN_4096BYTES),
    DEFINE_WIN_CONST(IMAGE_SCN_ALIGN_8192BYTES),
    DEFINE_WIN_CONST(IMAGE_SCN_ALIGN_MASK),
    DEFINE_WIN_CONST(IMAGE_SCN_LNK_NRELOC_OVFL),
    DEFINE_WIN_CONST(IMAGE_SCN_MEM_DISCARDABLE),
    DEFINE_WIN_CONST(IMAGE_SCN_MEM_NOT_CACHED),
    DEFINE_WIN_CONST(IMAGE_SCN_MEM_NOT_PAGED),
    DEFINE_WIN_CONST(IMAGE_SCN_MEM_SHARED),
    DEFINE_WIN_CONST(IMAGE_SCN_MEM_EXECUTE),
    DEFINE_WIN_CONST(IMAGE_SCN_MEM_READ),
    DEFINE_WIN_CONST(IMAGE_SCN_MEM_WRITE)
};

Misc::UTHash<unsigned int, const char *> RESOURCE_TYPE_MAP = 
{
    DEFINE_WIN_CONST(RT_CURSOR),
    DEFINE_WIN_CONST(RT_BITMAP),
    DEFINE_WIN_CONST(RT_ICON),
    DEFINE_WIN_CONST(RT_MENU),
    DEFINE_WIN_CONST(RT_DIALOG),
    DEFINE_WIN_CONST(RT_STRING),
    DEFINE_WIN_CONST(RT_FONTDIR),
    DEFINE_WIN_CONST(RT_FONT),
    DEFINE_WIN_CONST(RT_ACCELERATOR),
    DEFINE_WIN_CONST(RT_RCDATA),
    DEFINE_WIN_CONST(RT_MESSAGETABLE),
    DEFINE_WIN_CONST(RT_GROUP_CURSOR),
    DEFINE_WIN_CONST(RT_GROUP_ICON),
    DEFINE_WIN_CONST(RT_VERSION),
    DEFINE_WIN_CONST(RT_DLGINCLUDE),
    DEFINE_WIN_CONST(RT_PLUGPLAY),
    DEFINE_WIN_CONST(RT_VXD),
    DEFINE_WIN_CONST(RT_ANICURSOR),
    DEFINE_WIN_CONST(RT_ANIICON),
    DEFINE_WIN_CONST(RT_HTML),
    DEFINE_WIN_CONST(RT_MANIFEST)
};

};
};
};

#endif /* WINCONSTANT_H */