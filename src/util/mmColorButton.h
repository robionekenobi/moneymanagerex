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

class mmColorButton : public wxButton
{
    wxDECLARE_EVENT_TABLE();

private:
    int m_color_id;
    bool m_noColorAllowed;

public:
    int  GetColorId() const { return m_color_id; }

public:
    mmColorButton(
        wxWindow* parent_win,
        wxWindowID win_id,
        wxSize size = wxDefaultSize,
        bool noColorAllowed = false
    );

public:
    void SetColor(int color_id);

private:
    void onMenuSelected(wxCommandEvent& event);
    void onColourButton(wxCommandEvent& event);
};

