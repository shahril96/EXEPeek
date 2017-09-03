
#include <windows.h>
#include <commctrl.h>

// standard libs
#include <cstdio>
#include <cstring>

#include "treeview.h"
#include "../../misc/helper.h"
#include "../../resources.h"

namespace Gui
{
namespace View
{

TreeView::TreeView(HWND hwndParent) : baseWindow(hwndParent)
{
    hwndWindow = CreateWindowEx(WS_EX_CLIENTEDGE, WC_TREEVIEW, "TreeView",
            // set style
            WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS,
            // set positioning & size
            0, 0, 0, 0, // don't set size yet, let the resize() do the job
            // GetModuleHandle(NULL) = NULL means get instance of current executable (refer MSDN)
            hwndParent, (HMENU) IDC_TREE_VIEW, GetModuleHandle(NULL), NULL);
    
    if(!getWindow()) {
        Misc::Helper::PrintAPILastError();
    }

    nodesHandle["ROOT"] = TVI_ROOT;
}

TreeView::~TreeView()
{
}

void TreeView::configureLook()
{
    // adjust window height & width
    RECT rect = Misc::Helper::getRect(hwndParent);
    int treewidth = (rect.right - rect.left) / 4;
    setPosAndSize(0, 0, treewidth, rect.bottom - rect.top);

    // adjust window's font
    Misc::Helper::ChangeFontAllWindows();
}

HTREEITEM TreeView::getItemHandle(const char *itemStr)
{
    return nodesHandle[itemStr];
}

HTREEITEM TreeView::getParent(HTREEITEM hItem)
{
    return TreeView_GetParent(getWindow(), hItem);
}

HTREEITEM TreeView::insertItemRoot(const char *title)
{
    return insertItem(title, "ROOT");
}

HTREEITEM TreeView::insertItem(const char *title, const char *parent)
{
    // search if parent not exists
    if (!nodesHandle.exists(parent)) {
        char buf[100];
        snprintf(buf, 100, "Parent not found : %s", parent);
        Misc::Helper::ErrMsg(buf); // if not found
        return NULL;
    }

    // insert to the tree
    auto itemHandle = insertItem(title, getItemHandle(parent));

    // store handle into map
    nodesHandle[title] = itemHandle;

    return itemHandle;
}

HTREEITEM TreeView::insertItem(const char *title, HTREEITEM hParent)
{
    TVITEM tvi;
    TVINSERTSTRUCT tvistruct;
    tvi.pszText             = (char *)title;
    tvi.cchTextMax          = strlen(title);
    tvi.mask                = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
    tvi.iImage              = 0;
    tvi.iSelectedImage      = 0;
    tvistruct.item          = tvi;
    tvistruct.hInsertAfter  = TVI_LAST;
    tvistruct.hParent       = hParent;

    // send message to insert the new item
    return (HTREEITEM) TreeView_InsertItem(getWindow(), &tvistruct);
}

HTREEITEM TreeView::getPrevSibling(HTREEITEM hItem)
{
    return TreeView_GetPrevSibling(getWindow(), hItem);
}

void TreeView::getStrValueFromItem(HTREEITEM hItem, char *buf, int bufsize)
{
    TVITEM item = {0};
    item.hItem = hItem;
    item.mask = TVIF_TEXT;
    item.cchTextMax = bufsize;
    item.pszText = buf;
    TreeView_GetItem(getWindow(), &item);
}

void TreeView::deleteItem(const char *item)
{
    // delete if only the item exist on map
    if (!nodesHandle.exists(item)) {
        
        deleteItem(getItemHandle(item));

        // delete from map
        nodesHandle.erase(item);
    }
}

void TreeView::deleteItem(HTREEITEM hItem)
{
    TreeView_DeleteItem(getWindow(), hItem);
}

void TreeView::clearItems()
{
    TreeView_DeleteAllItems(getWindow());
    nodesHandle.clear();
    nodesHandle["ROOT"] = TVI_ROOT;
}

};
};