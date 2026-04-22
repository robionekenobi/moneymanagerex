/*******************************************************
 Copyright (C) 2014 Gabriele-V
 Copyright (C) 2021, 2022 Mark Whalley (mark@ipx.co.uk)

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
#include <wx/dataview.h>
#include <map>

#include "util/_primitive.h"
#include "data/_DataEnum.h"

class AttachmentDialog : public wxDialog
{
    wxDECLARE_DYNAMIC_CLASS(AttachmentDialog);
    wxDECLARE_EVENT_TABLE();

private:
    enum cols
    {
        ATTACHMENT_ID = 0,
        ATTACHMENT_DESCRIPTION,
        ATTACHMENT_FILENAME
    };

    enum menu_items
    {
        MENU_NEW_ATTACHMENT = 1,
        MENU_OPEN_ATTACHMENT,
        MENU_EDIT_ATTACHMENT,
        MENU_DELETE_ATTACHMENT,
    };

// -- state

private:
    std::map<int, wxString> m_col_name_m;
    RefTypeN m_ref_type;
    int64 m_ref_id = -1;
    int64 m_attachment_id = -1;

    wxDataViewListCtrl* w_list = nullptr;

    #ifdef _DEBUG
        bool m_debug = true;
    #else
        bool m_debug = false;
    #endif

// -- constructor

public:
    AttachmentDialog() {}
    AttachmentDialog(
        wxWindow* parent_win,
        RefTypeN ref_type,
        int64 ref_id,
        const wxString& name = "AttachmentDialog"
    );

private:
    void create(wxWindow* parent_win, const wxString& name);
    void createControls();
    void fillControls();

// -- methods

private:
    void addAttachment(wxString Path = "");
    void openAttachment();
    void editAttachment();
    void deleteAttachment();

// -- event handlers

private:
    void onOk(                wxCommandEvent&   event);
    void onCancel(            wxCommandEvent&   event);
    void onDropFiles(         wxDropFilesEvent& event);
    void onListItemSelected(  wxDataViewEvent&  event);
    void onMenuSelected(      wxCommandEvent&   event);
    void onMagicButton(       wxCommandEvent&   event);
    void onItemRightClick(    wxDataViewEvent&  event);
    void onListItemActivated( wxDataViewEvent&  event);
};
