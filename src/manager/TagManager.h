/*******************************************************
 Copyright (C) 2006 Madhan Kanagavel
 Copyright (C) 2015 Gabriele-V
 Copyright (C) 2022 Mark WHalley (mark@ipx.co.uk)

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

#pragma once

#include "base/_defs.h"
#include <wx/dialog.h>
#include <wx/srchctrl.h>
#include <wx/listbox.h>
#include <wx/checklst.h>
#include <wx/listbase.h>

class TagManager : public wxDialog
{
    wxDECLARE_DYNAMIC_CLASS(TagManager);
    wxDECLARE_EVENT_TABLE();

// -- state

private:
    bool m_is_selection;
    wxArrayString m_selected_tag_a;
    wxArrayString m_tag_a;
    wxString m_mask_s = wxEmptyString;
    bool m_refresh_requested = false;
    //wxString searchText_;

    wxSearchCtrl*  w_search_ctrl = nullptr;
    wxListBox*     w_tag_list    = nullptr;
    wxButton*      w_edit_btn    = nullptr;
    wxButton*      w_add_btn     = nullptr;
    wxButton*      w_delete_btn  = nullptr;

public:
    auto getSelectedTags() const -> wxArrayString { return m_selected_tag_a; }
    bool getRefreshRequested() const { return m_refresh_requested; }

// -- constructor

public:
    TagManager();
    TagManager(
        wxWindow* parent_win,
        bool is_selection = false,
        const wxArrayString& selected_tag_a = wxArrayString()
    );
    ~TagManager();

private:
    bool create(
        wxWindow* parent_win,
        wxWindowID win_id = wxID_ANY,
        const wxString& caption = _t("Tag Manager"),
        const wxString& name = "Organize Tags",
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX | wxRESIZE_BORDER
    );
    void createControls();

// -- methods

private:
    int  getTagIndex(const wxString& tag);
    bool validateName(const wxString& name);
    void fillControls();
    void setSelectedItem(int index);
    void setSelectedString(const wxString& name);

// -- event handlers

private:
    void OnAdd(                wxCommandEvent& event);
    void OnEdit(               wxCommandEvent& event);
    void OnDelete(             wxCommandEvent& event);
    void OnTextChanged(        wxCommandEvent& event);
    void OnListSelChanged(     wxCommandEvent& event);
    void OnCheckboxSelChanged( wxCommandEvent& event);
    void OnOk(                 wxCommandEvent& event);
    void OnCancel(             wxCommandEvent& event);
};
