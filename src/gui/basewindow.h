
#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include "../misc/helper.h"

namespace Gui
{

class baseWindow {

protected:

    // parent of this window (NULL if top-window)
    HWND hwndParent;

    // handle to current window object
    HWND hwndWindow;

public:

    baseWindow(HWND hwndParent);
    virtual ~baseWindow();

    virtual void configureLook() = 0;

    void setPosAndSize(int X, int Y, int cx, int cy);
    HWND getWindow();
    HWND getParentWindow();

};

};

#endif /* BASEWINDOW_H */

