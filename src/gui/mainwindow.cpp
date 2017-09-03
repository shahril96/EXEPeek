
#include <windows.h>
#include <windowsx.h>
#include <memory>

#include "mainwindow.h"
#include "../gui/View/treeview.h"
#include "../gui/View/listview.h"
#include "../misc/helper.h"
#include "../resources.h"
#include "../misc/fileread.h"
#include "../structures/PE/pehandler.h"

namespace Gui
{

mainWindow::mainWindow(HINSTANCE hInstance, int nCmdShow, LPCTSTR ClassName) 
    : baseWindow(NULL), nCmdShow(nCmdShow)
{
    // initially register our main window class
    registerClass(hInstance, ClassName);

    // create main window
    hwndWindow = CreateWindow(ClassName, "EXEPeek", WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            NULL, NULL, hInstance,
            this); // store *this pointer so we can use it later in wndProc
    
    configureLook();
}

mainWindow::~mainWindow()
{
}

void mainWindow::configureLook()
{
    ShowWindow(getWindow(), nCmdShow);
}

void mainWindow::registerClass(HINSTANCE hInstance, LPCTSTR ClassName)
{
    WNDCLASSEX wndClass = {0};
    
    // setup the appropriate options to our main Window
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.cbClsExtra = 0; // extra memory allocated - set = 0
    wndClass.hInstance = hInstance; // instance of our window
    wndClass.cbWndExtra = 0; // extra mem for windows - set = 0
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = wndProc; // callback for managing messages
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // set icon
    wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // set small icon
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); // set cursor
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // set background color
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = ClassName; // set our window name

    RegisterClassEx(&wndClass);
}

void mainWindow::loopMsg()
{
    MSG msg;
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

// ---------------------------------------------------- //
// ALL FUNCTIONS RELATED TO WNDPROC WILL BE BELOW HERE
// ---------------------------------------------------- //

LRESULT CALLBACK mainWindow::wndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    // capture this pointer stored inside GWL_USERDATA
    mainWindow *mainwnd = (mainWindow *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

    switch (uMsg) {

        // -----------------------------------------------------------
        // HANDLE_MSG : macro that simplifies task by forwarding 
        //              the appropriate message into their specific
        //              functions (it's defined inside windowsx.h)
        // -----------------------------------------------------------

        HANDLE_MSG(hwnd, WM_NCCREATE,       mainwnd->OnNCCreate);
        HANDLE_MSG(hwnd, WM_CREATE,         mainwnd->onCreate);
        HANDLE_MSG(hwnd, WM_COMMAND,        mainwnd->onCommand);
        HANDLE_MSG(hwnd, WM_DROPFILES,      mainwnd->OnDropFiles);
        HANDLE_MSG(hwnd, WM_SIZE,           mainwnd->onSize);
        HANDLE_MSG(hwnd, WM_GETMINMAXINFO,  mainwnd->onGetMinMaxInfo);
        HANDLE_MSG(hwnd, WM_DESTROY,        mainwnd->onDestroy);

        case WM_NOTIFY:
            HANDLE_WM_NOTIFY(hwnd, wParam, lParam, mainwnd->onNotify);
            break;

        default:
            return DefWindowProcA(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

bool mainWindow::OnNCCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    // store *this pointer into GWLP_USERDATA
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR) lpCreateStruct->lpCreateParams);

    // set initial window pos
    SetWindowPos(hwnd, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);

    return true;
}

bool mainWindow::onCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    // registering the application to accept dropped files
    DragAcceptFiles(hwnd, TRUE);

    // create menu item
    HMENU hMenu = CreateMenu();
    HMENU hMenuBar = CreateMenu();
    AppendMenu(hMenu, MF_STRING, IDM_FILE_OPEN, "Open");
    AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hMenu, MF_STRING, IDM_FILE_QUIT, "Quit");
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hMenu, "File");
    SetMenu(hwnd, hMenuBar);

    // create new (empty) instance of treeview & listview
    treeview = std::make_unique<View::TreeView>(hwnd);
    listview = std::make_unique<View::ListView>(hwnd);

    return true;
}

void mainWindow::onCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (LOWORD(id)) {

        case IDM_FILE_OPEN:
        {
            char filePath[MAX_PATH];
            OPENFILENAME ofilename  = {0};

            ofilename.lStructSize = sizeof(OPENFILENAME);
            ofilename.lpstrTitle  = "Open your binary file...";
            ofilename.Flags       = OFN_FILEMUSTEXIST;
            ofilename.lpstrFile   = filePath;
            ofilename.nMaxFile    = MAX_PATH;

            if (GetOpenFileName(&ofilename)) {
                prepareStructure(hwnd, filePath);
            }

            break;
        }

        case IDM_FILE_QUIT:
            PostQuitMessage(WM_QUIT);
            break;
    }
}

void mainWindow::OnDropFiles(HWND hwnd, HDROP hdrop)
{
    char filePath[MAX_PATH];

    // 0xffffffff = set this to get count of dropped files
    int fileCount = DragQueryFile(hdrop, ~0, NULL, 0);

    if (fileCount > 0 && DragQueryFile(hdrop, 0, filePath, MAX_PATH)) {
        prepareStructure(hwnd, filePath);
    }
}

void mainWindow::onNotify(HWND hWnd, int idCtrl, LPNMHDR lpNMHdr)
{
    // if there is any click on TreeView's cell, then forward those message
    // into appropriate structure handler
    if (LOWORD(idCtrl) == IDC_TREE_VIEW && lpNMHdr->code == TVN_SELCHANGED) {

        // only handle PE structure for now
        pehandler->handleTreeViewMessage((LPNMTREEVIEW) lpNMHdr);
    }
}

void mainWindow::onSize(HWND hwnd, UINT state, int cx, int cy) 
{
    // if somehow user resize the window, then we need to resize both child windows again
    if (treeview.get()) treeview->configureLook();
    if (listview.get()) listview->configureLook();

    // after structure has been opened, then it is its task to resize the window
    if (pehandler.get()) pehandler->resize();
}

void mainWindow::onGetMinMaxInfo(HWND hwnd, LPMINMAXINFO lpMinMaxInfo) 
{
    // set window minimum size on initialization
    MINMAXINFO *mmInfo = lpMinMaxInfo;
    mmInfo->ptMinTrackSize.x = 800;
    mmInfo->ptMinTrackSize.y = 400;
}

void mainWindow::onDestroy(HWND hwnd)
{
    // send WM_QUIT message to PeekMessage() (to exit the application)
    PostQuitMessage(WM_QUIT);
}

void mainWindow::prepareStructure(HWND hwnd, const char *filePath)
{
    // as Struct Handler must create their own listview (or anything)
    // when displaying the data, so we must delete the current one
    listview.reset();

    // clear all treeview items
    treeview->clearItems();

    // change window title
    char title[500];
    snprintf(title, 500, "EXEPeek - %s", filePath);
    SetWindowText(hwnd, title);

    // mmap targeted file into the memory
    fileread = std::make_unique<Misc::FileRead>(filePath);

    // PEHandler - default handler for now
    pehandler = std::make_unique<Structure::PE::PEHandler>(hwnd, fileread, treeview);
}

};