
#include <windows.h>
#include <exception>
#include "gui/mainwindow.h"
#include "misc/helper.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    Gui::mainWindow mwnd(hInstance, nCmdShow, "EXEPeekApplication");
    mwnd.loopMsg();
    return 0;
}