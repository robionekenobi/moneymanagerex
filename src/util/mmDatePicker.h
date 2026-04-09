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

class mmDatePicker : public wxPanel
{
// -- state

private:
    wxDateTime m_dateTime;

    wxWindow*         w_parent       = nullptr;
    wxStaticText*     w_weekday_text = nullptr;
    wxSpinButton*     w_spin_btn     = nullptr;
    wxDatePickerCtrl* w_date_picker  = nullptr;
    wxTimePickerCtrl* w_time_picker  = nullptr;

public:
    wxDateTime GetValue() { return m_dateTime; }

// -- constructor

public:
    mmDatePicker(
        wxWindow* parent_win,
        wxWindowID win_id,
        wxDateTime dateTime = wxDateTime::Today(),
        wxPoint pos = wxDefaultPosition,
        wxSize size = wxDefaultSize,
        long style = wxDP_DROPDOWN | wxDP_SHOWCENTURY
    );
    ~mmDatePicker();

// -- override

public:
    virtual bool Enable(bool state = true) override;
    virtual bool Show(bool state = true) override;

// -- methods

public:
    void setValue(const wxDateTime &dateTime);
    bool isItMyDateControl(wxObject* obj);
    auto mmGetLayout(bool showTimeCtrl = true) -> wxBoxSizer*;
    auto mmGetLayoutWithTime() -> wxBoxSizer*;

private:
    auto getTextWeek() -> wxStaticText*;
    auto getSpinButton() -> wxSpinButton*;

// -- event handlers

private:
    void onDateChanged(wxDateEvent& event);
    void onDateSpin(wxSpinEvent&);
};
