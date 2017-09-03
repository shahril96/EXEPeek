
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <cstdio>
#include <cctype>
#include <cstdarg>

#include "listview.h"
#include "../../misc/helper.h"
#include "../../resources.h"
#include "../../misc/fileread.h"

namespace Gui
{
namespace View
{

//
// hwndParent  - Handle to parent window containing this child window
//
ListView::ListView(HWND hwndParent) : baseWindow(hwndParent)
{
    hwndWindow = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, "ListView",
            // set style
            WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | LVS_REPORT,
            // set positioning & size
            0, 0, 0, 0, // don't set size yet, do it using setPosAndSize()
            // GetModuleHandle(NULL) = NULL means get instance of current executable (refer MSDN)
            hwndParent, (HMENU) IDC_LIST_VIEW, GetModuleHandle(NULL), NULL);

    if(!getWindow()) {
        Misc::Helper::PrintAPILastError();
    }

    // set extended style such as full row highligting, etc
    ListView_SetExtendedListViewStyle(getWindow(),
                                      LVS_EX_FULLROWSELECT    |  /* item and all its subitems are highlighted when selected */
                                      LVS_EX_ONECLICKACTIVATE    /* enable 'hot-tracking' */
                                      );
}

ListView::~ListView()
{
}

void ListView::configureLook()
{
    // adjust window height & width
    RECT rect = Misc::Helper::getRect(hwndParent);
    int treewidth = (rect.right-rect.left) / 4;
    setPosAndSize(treewidth, 0, rect.right-treewidth, rect.bottom-rect.top);

    // adjust window's font
    Misc::Helper::ChangeFontAllWindows();
}

void ListView::addColumn(LPCSTR col_name, int width)
{
    LV_COLUMN column;
    column.mask     = LVCF_TEXT | LVCF_WIDTH; // set the option masks
    column.pszText  = (LPSTR) col_name; // enabled with LVCF_TEXT
    column.cx       = width; // enabled with LVCF_WIDTH
    int col_count   = Header_GetItemCount(ListView_GetHeader(getWindow()));
    ListView_InsertColumn(getWindow(), col_count, &column);
}

void ListView::addItems(int count, ...)
{
    LVITEM lvitem = {0};
    lvitem.mask   = LVIF_TEXT;
    lvitem.iItem  = ListView_GetItemCount(getWindow());

    va_list ap;
    va_start(ap, count);

    for (int i=0; i<count; i++) {

        lvitem.iSubItem = i;
        lvitem.pszText  = va_arg(ap, char *);

        /*
         * 0th - left most side = row item (ListView_InsertItem)
         * 1th and more nth = subitem for the row item (ListView_SetItem)
         */

        if (i==0) {
            ListView_InsertItem(getWindow(), &lvitem);
        } else if (i>0) {
            ListView_SetItem(getWindow(), &lvitem);
        }
    }

    va_end(ap);

}

void ListView::clearColumns()
{
    while( ListView_DeleteColumn((HWND) getWindow(), 0) ) {};
}

void ListView::clearItems()
{
    ListView_DeleteAllItems((HWND) getWindow());
}

};
};