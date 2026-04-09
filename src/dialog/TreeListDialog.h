/*******************************************************
 Copyright (C) 2025, 2026 Klaus Wich

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 ********************************************************/

// Contains generic base classes, which are used for common definitions,
// but not directly instantiated.

#pragma once

#include "base/_defs.h"

class TreeListDialog: public wxDialog
{
private:
    wxDECLARE_DYNAMIC_CLASS(TreeListDialog);

protected:
    enum {
        BTN_BASE = 0,
        BTN_UP_TOP,
        BTN_UP,
        BTN_DOWN,
        BTN_DOWN_BOTTOM,
        BTN_DEFAULT,
        BTN_size
    };

// -- state

private:
    wxButton*       m_up_top      = nullptr;
    wxBitmapButton* m_up          = nullptr;
    wxBitmapButton* m_down        = nullptr;
    wxButton*       m_down_bottom = nullptr;

protected:
    wxTreeListCtrl* m_treeList    = nullptr;

// -- constructor

public:
    TreeListDialog();
    TreeListDialog(wxWindow* parent, wxString title);
    virtual ~TreeListDialog();

// -- virtual

public:
    virtual void createColumns() {}
    virtual void closeAction() {}
    virtual void createMiddleElements([[maybe_unused]] wxBoxSizer* itemBox) {}
    virtual void createBottomElements([[maybe_unused]] wxBoxSizer* itemBox) {}
    virtual void fillControls([[maybe_unused]] wxTreeListItem root) {}
    virtual void updateControlState([[maybe_unused]] int selIdx, [[maybe_unused]] wxClientData* selData) {}
    virtual void setDefault() {}
    virtual void copyTreeItemData([[maybe_unused]] wxTreeListItem src, [[maybe_unused]] wxTreeListItem dst) {}

// -- methods

public:
    void init(long liststyle = wxTL_3STATE | wxTL_SINGLE);
    void updateButtonState();
    void reloadTree();

private:
    void moveSelectedItem(int direction);
    void moveItemData(wxTreeListItem sel, wxTreeListItem newItem);
    wxTreeListItems getChildrenList(wxTreeListItem parent);
    void cloneSubtree(wxTreeListItem src, wxTreeListItem dstParent);
    int findItemIndex(const wxTreeListItems& items, const wxTreeListItem& target);

// -- event handlers

    void OnTop(                 wxCommandEvent&);
    void OnUp(                  wxCommandEvent&);
    void OnDown(                wxCommandEvent&);
    void OnBottom(              wxCommandEvent&);
    void OnDefault(             wxCommandEvent&);
    void OnTreeSelectionChange( wxTreeListEvent&);
    void OnClose(               wxCommandEvent&);
    void OnCloseWin(            wxCloseEvent&);
};
