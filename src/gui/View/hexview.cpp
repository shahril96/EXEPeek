
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <cstdio>

#include "hexview.h"
#include "../../misc/helper.h"
#include "../../resources.h"

namespace Gui
{
namespace View
{

HexView::HexView(HWND hwndParent, const char *basePTR, const char *startPTR, uint64_t sizePTR)
    : ListView(hwndParent), basePTR(basePTR), startPTR(startPTR), sizePTR(sizePTR)
{
    hwndWindow = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, "ListView",
            WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_NOSCROLL | WS_CLIPSIBLINGS,
            0, 0, 0, 0, // don't set size yet, do it using setSize()
            hwndParent, (HMENU) IDC_LIST_VIEW, GetModuleHandle(NULL), NULL);

    // create custom scrollBar
    hwndScrollBar = CreateWindow(WC_SCROLLBAR, NULL,
                    WS_CHILD | WS_VISIBLE | SBS_VERT | WS_CLIPSIBLINGS,
                    0, 0, 0, 0, getWindow(), NULL,
                    GetModuleHandle(NULL), NULL);

    // set extended style such as full row highligting, etc
    ListView_SetExtendedListViewStyle(getWindow(),
                                      LVS_EX_FULLROWSELECT    |  // item and all its subitems are highlighted when selected
                                      LVS_EX_ONECLICKACTIVATE    // enable 'hot-tracking'
                                      );
    
    // sub-class so we can handle window message send by ListView childs
    SetWindowSubclass(getWindow(), wndProc, 0, (DWORD_PTR) this);

    // we need to setup size first before inserting columns
    configureLook();

    RECT rect = Misc::Helper::getRect(getWindow());

    // as column for hexview will never changes despite different structure handler
    // so we can safely assume & create similar column for any instance
    int len = rect.right - rect.left; // get horizontal window length
    addColumn("pFile (h)", len / 5);
    addColumn("00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F", 3 * len / 5);
    addColumn("0123456789ABCDEF", len / 5);

    // set range of scrollbar
    SCROLLINFO si = {0};
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask  = SIF_RANGE | SIF_PAGE;
    si.nMax   = sizePTR / 16;
    si.nPage  = 1;
    SetScrollInfo(hwndScrollBar, SB_CTL, &si, TRUE);

}

HexView::~HexView()
{
    RemoveWindowSubclass(getWindow(), wndProc, 0);
}

void HexView::configureLook()
{
    // adjust window height & width
    RECT rect = Misc::Helper::getRect(hwndParent);
    int treewidth = (rect.right - rect.left) / 4;
    setPosAndSize(treewidth, 0, rect.right-treewidth, rect.bottom-rect.top);

    // make scrollBar appear on top of WS_LISTVIEW (using HWND_TOP to adjust the Z-order)
    rect = Misc::Helper::getRect(getWindow());
    SetWindowPos(hwndScrollBar, HWND_TOP, rect.right-rect.left-16, rect.top, 16, rect.bottom-rect.top, 0);

    // adjust window's font
    Misc::Helper::ChangeFontAllWindows();
}

LRESULT CALLBACK HexView::wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR id, DWORD_PTR dwData)
{
    auto obj = (HexView*) dwData;

    switch(uMsg) {

        HANDLE_MSG(hwnd, WM_VSCROLL, obj->onVSCroll);

        // windowsx.h doesnt have message cracker for WM_MOUSEWHEEL :'(
        case WM_MOUSEWHEEL:
            obj->onMouseWheel(HIWORD(wParam));
            break;
    }

    return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}

void HexView::onMouseWheel(int mousePosDelta)
{
    SCROLLINFO si;
    si.cbSize = sizeof(si);
    si.fMask  = SIF_ALL;
    GetScrollInfo(hwndScrollBar, SB_CTL, &si);

    // if wheel_delta THEN rotated_down ELSE rotated UP
    si.nPos += (mousePosDelta == WHEEL_DELTA) ? -2 : +2;

    // only update nPos if on the range
    if (si.nPos >= si.nMin && si.nPos <= si.nMax) {
        si.fMask = SIF_POS;
        SetScrollInfo(hwndScrollBar, SB_CTL, &si, TRUE);
        clearItems();
        printHex(si.nPos);
    }
}

void HexView::onVSCroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
    SCROLLINFO si;
    si.cbSize = sizeof(si);
    si.fMask  = SIF_ALL;
    GetScrollInfo(hwndScrollBar, SB_CTL, &si);

    // need to manually handle the ScrollBar each time we captured WM_VSCROLL
    switch (code) {
        case SB_TOP:        si.nPos = si.nMin      ; break ;
        case SB_BOTTOM:     si.nPos = si.nMax      ; break ;
        case SB_LINEUP:     si.nPos -= 1           ; break ;
        case SB_LINEDOWN:   si.nPos += 1           ; break ;
        case SB_PAGEUP:     si.nPos -= si.nPage    ; break ;
        case SB_PAGEDOWN:   si.nPos += si.nPage    ; break ;
        case SB_THUMBTRACK: si.nPos = si.nTrackPos ; break ;
    }

    // only update nPos if on the range
    if (si.nPos >= si.nMin && si.nPos <= si.nMax) {
        si.fMask = SIF_POS;
        SetScrollInfo(hwndScrollBar, SB_CTL, &si, TRUE);
        clearItems();
        printHex(si.nPos);
    }
}

/**
 * startline  - Starting line to read from ptr
 */
void HexView::printHex(int startline) {

    // -------------------------------
    // poor man Hex viewer version ;)
    // -------------------------------

    uint64_t twoGB = (uint64_t)2*1024*1024*1024;

    const int LIMIT_HEX_LINE = 50;

    for (uint64_t i=startline*16, k=0; i < sizePTR && k < LIMIT_HEX_LINE; i += 16, k++) {

        const int MAXBUF = 100;
        char offsetbuf[MAXBUF]  = {0};
        char hexabuf[MAXBUF]    = {0};
        char asciibuf[MAXBUF]   = {0};

        // if filesize exceeds 2GB, then treat offset as having 64-bit size
        const char *offsetBufzStr = i > twoGB ? "%016llX" : "%08llX";

        snprintf(offsetbuf, MAXBUF, offsetBufzStr, &startPTR[i]-basePTR);
        for (uint64_t j=i; j < (i+16) && j < sizePTR; j++) {
            snprintf(hexabuf+(3*(j-i)), MAXBUF, "%02X ", (unsigned char)startPTR[j]);
            snprintf(asciibuf+(j-i), MAXBUF, "%c", isprint(startPTR[j]) ? startPTR[j] : '.');
        }

        // add each line into the row
        addItems(3, offsetbuf, hexabuf, asciibuf);
    }

}

};
};