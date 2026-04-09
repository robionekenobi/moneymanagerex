/*******************************************************
 Copyright (C) 2014 Nikolay Akimov
 Copyright (C) 2022  Mark Whalley (mark@ipx.co.uk)

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
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <wx/richtooltip.h>

class UNUSED_mmComboBoxText : public wxTextCtrl
{
public:
    using wxTextCtrl::Connect;

private:
    bool m_is_payee = false;

public:
    UNUSED_mmComboBoxText() {}
    UNUSED_mmComboBoxText(
        wxWindow* parent_win,
        wxWindowID win_id,
        const wxString& value,
        bool is_payee,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxTextCtrlNameStr
    );

    wxString GetValue() const
    {
        return wxTextCtrl::GetValue();
    }

    void setSelection(int& id)
    auto getID() -> int64;
};
