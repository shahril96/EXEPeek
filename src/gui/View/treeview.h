
#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <commctrl.h>

#include "../basewindow.h"
#include "../../misc/data_structures/uthash.hpp"

/**
 * This is my own implementation that wrapped the TreeView into a class.
 * Inside this class, there are two modes (imaginary mode lol) that you can choose to operate with:
 *
 *      1) totally dependent on string, which mean all insertion can be done in 
 *         a higher level way. in this mode, you can't possibly have multiple nodes that 
 *         have same name, as I implemented this feature using UTHash_StrKey (unique key only)
 *
 *      2) you can also operate in a mixed-mode, where you can combine both power of 
 *         higher level string flexibility with the old HTREEITEM handle way.
 *         - this mode is suitable if you wanna create a treeview's nodes that have same name,
 *           as in string-mode you can't possibly have that.
 *
 * Please keep ensure that when you're operating in the mixed-mode, then when you wanna
 * delete some item, if the item wasn't recorded into the UTHash_StrKey, you must manually find the
 * handle yourself and use deleteItem() to delete the item.
 *
 * An easy way to delete multiple unrecorded items is to delete the top-level item that previously
 * recorded by the UTHash_StrKey, as the delete operation works in recursive way, so all the below-level
 * items will be deleted also.
 */

namespace Gui
{
namespace View
{

class TreeView : public baseWindow {
    
    // contains info NAME = HTREEITEM
    Misc::UTHash_StrKey<HTREEITEM> nodesHandle;
    
public:

    TreeView(HWND hwndParent);
    ~TreeView();

    // virtual function
    void configureLook();
 
    // if operating in string-mode
    HTREEITEM insertItemRoot(const char *title);
    HTREEITEM insertItem(const char *title, const char *parent);
    HTREEITEM getItemHandle(const char *itemStr);
    void deleteItem(const char *itemStr);

    // if operating in handle-mode
    HTREEITEM getParent(HTREEITEM hItem);
    HTREEITEM getPrevSibling(HTREEITEM hItem);
    HTREEITEM insertItem(const char *title, HTREEITEM hParent);
    void getStrValueFromItem(HTREEITEM hItem, char *buf, int bufsize);
    void deleteItem(HTREEITEM hItem);
    
    void clearItems();
};

};
};

#endif /* TREEVIEW_H */

