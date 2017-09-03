
#ifndef HEXVIEW_H
#define HEXVIEW_H

#include <cstdint>
#include "listview.h"

namespace Gui
{
namespace View
{

class HexView : public ListView {

    HWND  hwndScrollBar;
    
    // state for hex view
    const char *basePTR;
    const char *startPTR;
    uint64_t sizePTR;

public:
    
    HexView(HWND hwndParent, const char *basePTR, const char *startPTR, uint64_t sizePTR);
    ~HexView();
    void configureLook();

    // custom window message handler
    static LRESULT CALLBACK wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR id, DWORD_PTR dwData);

    // custom handler for virtual scroll message
    void onVSCroll(HWND hwnd, HWND hwndCtl, UINT code, int pos);
    void onMouseWheel(int mousePosDelta);

    // print hex view into the ListView
    void printHex(int startline);
};

};
};

#endif /* HEXVIEW_H */