/*******************************************************
 Copyright (C) 2014 Gabriele-V
 Copyright (C) 2015, 2016, 2020, 2022 Nikolay Akimov
 Copyright (C) 2022, 2023 Mark Whalley (mark@ipx.co.uk)
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

#include "mmSplitterWindow.h"

mmSplitterWindow::mmSplitterWindow(
    wxWindow* parent_win,
    wxWindowID win_id,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxColour& colour
) :
    wxSplitterWindow(parent_win, win_id, pos, size, style),
    m_colour(colour)
{
}

void mmSplitterWindow::DrawSash(wxDC& dc)
{
    dc.SetBrush(m_colour);
    dc.SetPen(*wxTRANSPARENT_PEN);

    if (GetSplitMode() == wxSPLIT_HORIZONTAL)
        dc.DrawRectangle(0, GetSashPosition(), GetSize().GetWidth(), GetSashSize());
    else
        dc.DrawRectangle(GetSashPosition(), 0, GetSashSize(), GetSize().GetHeight());
}
