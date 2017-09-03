
#include <windows.h>
#include <windowsx.h>
#include <cstdio>
#include <cstdint>
#include "helper.h"

namespace Misc
{

void Helper::ErrMsg(LPCSTR ErrMsg)
{
    MessageBox(0, ErrMsg, "ERROR", MB_OK | MB_ICONEXCLAMATION);
}

void Helper::ChangeFontAllWindows()
{
    // TODO: add settings to change icon, for now use default one
    const int new_size = 20;
    const char *font_type = "DejaVu Sans Mono";

    LOGFONT lfont = {0};
    lfont.lfHeight  = new_size;
    lfont.lfQuality = ANTIALIASED_QUALITY;
    snprintf(lfont.lfFaceName, LF_FACESIZE, "%s", font_type);

    auto fontIndirect = CreateFontIndirect(&lfont);
    SendMessageToDescendants(FindTopWindow(), WM_SETFONT, (WPARAM)fontIndirect, TRUE, TRUE);
}

void Helper::PrintAPILastError()
{
    DWORD errCode = GetLastError();
    LPSTR err;

    auto ret = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, errCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
        (LPTSTR) & err,
        0, NULL);

    if (!ret) {
        Helper::ErrMsg("FormatMessage() error!");
    } else {
        Helper::ErrMsg(err);
    }

    LocalFree(err);
    ExitProcess(1);
}

RECT Helper::getRect(HWND hWnd)
{
    RECT rcClient = {0};
    if (!GetClientRect(hWnd, &rcClient)) {
        Helper::PrintAPILastError();
    }
    return rcClient;
}

char *Helper::getBasename(char *path)
{
    // 1) as C standard doesn't have any function to find last occurances of substring
    // 2) then we do repeated search until not found, then we can assume last matched
    //    must be the path basename.

    char *found;

    while ((found = strstr(path, "\\"))) {
        path = found+1;
    }

    return path;
}

void Helper::SendMessageToDescendants(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, BOOL bDeep)
{
    // loop through all subwindows
    for(HWND hWndChild = GetTopWindow(hWnd); hWndChild != NULL; hWndChild = GetNextWindow(hWndChild, GW_HWNDNEXT)) {

        SendMessage(hWndChild, message, wParam, lParam);

        // if bDeep true, then recursively send the same message to sub-subwindows
        if (bDeep == true) {
            SendMessageToDescendants(hWndChild, message, wParam, lParam, bDeep);
        }
    }
}

HWND Helper::FindTopWindow()
{
    //
    // Credit: https://stackoverflow.com/a/39290139/1768052
    //

    uint64_t params[2] = { 0, GetCurrentProcessId() };

    // Enumerating the windows using a lambda to process each window
    // Note: lambda can decays into old function pointer if it doesn't capture anything, hence empty []
    BOOL bResult = EnumWindows( [](HWND hwnd, LPARAM lParam) -> BOOL 
    {
        auto *pParams = (uint64_t *)lParam;
        DWORD processId;

        if (GetWindowThreadProcessId(hwnd, &processId) && processId == pParams[1] && GetWindow(hwnd, GW_OWNER) == 0) {
            // Stop enumerating
            SetLastError(-1);
            pParams[0] = (uint64_t)hwnd;
            return FALSE;
        }

        // Continue enumerating
        return TRUE;

    }, (LPARAM)params);

    if (!bResult && GetLastError() == 0 && params[0]) {
        return (HWND)params[0];
    }

    return 0;
}

};