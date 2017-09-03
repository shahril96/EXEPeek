
#ifndef LISTVIEW_H
#define LISTVIEW_H

#include "../basewindow.h"
#include "../../misc/fileread.h"

namespace Gui
{
namespace View
{

class ListView : public baseWindow
{

public:
    
    ListView(HWND hwndParent);
    ~ListView();

    // virtual function
    void configureLook();
    
    // control's functions
    void addColumn(const char *col_name, int width);
    void addItems(int count, ...);
    void clearColumns();
    void clearItems();
};

};
};

#endif /* LISTVIEW_H */