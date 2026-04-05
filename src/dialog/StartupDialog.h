/*******************************************************
 Copyright (C) 2006 Madhan Kanagavel

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
#include "app/mmApp.h"

class wxCheckBox;

class StartupDialog: public wxDialog
{
    wxDECLARE_DYNAMIC_CLASS(StartupDialog);
    wxDECLARE_EVENT_TABLE();

// -- state

private:
    mmApp*      w_app       = nullptr;
    wxCheckBox* w_show_cb   = nullptr;
    wxButton*   w_close_btn = nullptr;
    wxButton*   w_exit_btn  = nullptr;

// -- constructor

public:
    StartupDialog(wxWindow* parent_win, mmApp* app, const wxString& name = "StartupDialog");
    ~StartupDialog();

private:
    bool create(
        wxWindow* parent_win,
        wxWindowID win_id,
        const wxString& caption,
        const wxPoint& pos,
        const wxSize& size,
        long style,
        const wxString& name = "StartupDialog"
    );
    void createControls();

// -- methods

public:
    void SetCloseButtonToExit();

private:
    StartupDialog(){};

// -- event handlers

private:
    void OnButtonAppstartOpenDatabaseClick( wxCommandEvent& event );
    void OnButtonAppstartNewDatabaseClick(  wxCommandEvent& event );
    void OnButtonAppstartChangeLanguage(    wxCommandEvent& /*event*/ );
    void OnButtonAppstartHelpClick(         wxCommandEvent& event );
    void OnButtonAppstartWebsiteClick(      wxCommandEvent& event );
    void OnButtonAppstartForumsClick(       wxCommandEvent& /*event*/ );
    void OnButtonAppstartLastDatabaseClick( wxCommandEvent& event );
    void OnQuit(                            wxCommandEvent& event);
    void OnClose(                           wxCloseEvent&   event);
};

