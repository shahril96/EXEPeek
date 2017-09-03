
#ifndef HELPER_H
#define HELPER_H

namespace Misc
{

namespace Helper
{
    void ChangeFontAllWindows();
    RECT getRect(HWND hWnd);
    void ErrMsg(LPCSTR ErrMsg);
    void PrintAPILastError();
    char *getBasename(char *path);
    void SendMessageToDescendants(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, BOOL bDeep);
    HWND FindTopWindow();
}

};


#endif /* HELPER_H */

