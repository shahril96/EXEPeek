
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <windows.h>
#include <memory>

#include "basewindow.h"
#include "../gui/View/treeview.h"
#include "../gui/View/listview.h"
#include "../misc/fileread.h"
#include "../structures/PE/pehandler.h"

namespace Gui
{

class mainWindow : public baseWindow
{

private:

    std::unique_ptr<View::TreeView> treeview;
    std::unique_ptr<View::ListView> listview;
    std::unique_ptr<Misc::FileRead> fileread;
    std::unique_ptr<Structure::PE::PEHandler> pehandler;
    int nCmdShow = 0;

public:

    mainWindow(HINSTANCE hInstance, int nCmdShow, LPCTSTR ClassName);
    ~mainWindow();

    // virtual function
    void configureLook();

    void registerClass(HINSTANCE hInstance, LPCTSTR ClassName);
    void loopMsg();

    static LRESULT CALLBACK wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    // handle messages for wndProc
    bool OnNCCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
    bool onCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
    void onCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
    void OnDropFiles(HWND hwnd, HDROP hdrop);
    void onNotify(HWND hWnd, int idCtrl, LPNMHDR lpNMHdr);
    void onSize(HWND hwnd, UINT state, int cx, int cy);
    void onGetMinMaxInfo(HWND hwnd, LPMINMAXINFO lpMinMaxInfo);
    void onDestroy(HWND hwnd);

    void prepareStructure(HWND hwnd, const char *filePath);

};

};

#endif /* MAINWINDOW_H */