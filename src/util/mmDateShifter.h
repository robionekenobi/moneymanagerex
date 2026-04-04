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

class mmDateSfifter : public wxPanel
{
    wxDECLARE_EVENT_TABLE();

private:
    int m_shift = 0;

    wxWindow* w_parent = nullptr;

public:
    mmDateSfifter();
    mmDateSfifter(wxWindow* parent_win);

private:
    bool create(wxWindow* parent_win, wxWindowID win_id);

public:
    void onButtonPress(wxCommandEvent& event);
};

