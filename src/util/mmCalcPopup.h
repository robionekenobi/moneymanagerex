/*******************************************************
 Copyright (C) 2014 Nikolay Akimov
 Copyright (C) 2014 Gabriele-V
 Copyright (C) 2022 Mark Whalley (mark@ipx.co.uk)
 Copyright (C) 2025 Klaus Wich

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
#include "mmTextCtrl.h"

class mmCalcPopup : public wxPopupTransientWindow
{
// -- static

private:
    enum Buttons
    {
        mmID_MULTIPLY = wxID_HIGHEST,
        mmID_DIVIDE,
        mmID_DELETE
    };

// -- state

private:
    bool m_dismissed = false;
    bool m_trigger;
    wxSize m_btn_size;
    wxFont m_font;

    wxWindow*    w_panel;
    mmTextCtrl*  w_target     = nullptr;
    mmTextCtrl*  w_value_text = nullptr;
    wxButton*    w_dec_btn    = nullptr;
    wxGridSizer* w_sizer      = nullptr;

// -- constructor

public:
    mmCalcPopup(
        wxWindow* parent_win,
        mmTextCtrl* target = nullptr,
        bool trigger = false
    );
    virtual ~mmCalcPopup();

private:
    wxButton* createButton(wxString symbol, int event);

// -- override

public:
    // override wxPopupTransientWindow
    virtual void Popup(wxWindow* focus = NULL) override;

    // override wxPopupTransientWindow -> wxWindow
    virtual void SetFocus() override { w_value_text->SetFocus(); }

protected:
    // override wxPopupTransientWindow
    virtual void OnDismiss() override;

// -- methods

public:
    void setValue(wxString& value);
    void setTarget(mmTextCtrl* target);

// -- event handlers

private:
    void onButtonPressed(wxCommandEvent& event);
};
