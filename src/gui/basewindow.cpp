
#include <windows.h>

#include "basewindow.h"
#include "../misc/helper.h"

namespace Gui
{

baseWindow::baseWindow(HWND hwndParent) : hwndParent(hwndParent)
{
    hwndWindow = NULL;
}

baseWindow::~baseWindow()
{
    DestroyWindow(getWindow());
}

void baseWindow::setPosAndSize(int X, int Y, int cx, int cy)
{
    SetWindowPos(getWindow(), NULL, X, Y, cx, cy, 0);
}

HWND baseWindow::getWindow()
{
    return hwndWindow;
}

HWND baseWindow::getParentWindow() {
    return hwndParent;
}

};