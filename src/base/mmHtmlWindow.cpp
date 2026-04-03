/*******************************************************
 Copyright (C) 2006 Madhan Kanagavel
 Copyright (C) 2013-2022 Nikolay Akimov
 Copyright (C) 2021-2024 Mark Whalley (mark@ipx.co.uk)
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

#include "mmHtmlWindow.h"
#include <wx/clipbrd.h>

// mmHtmlWindow just adds a right click menu to save text to the system clipboard

mmHtmlWindow::mmHtmlWindow(
    wxWindow* parent,
    wxWindowID id,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name
) :
    wxHtmlWindow(parent, id, pos, size, style, name)
{
    this->Bind(wxEVT_RIGHT_DOWN, &mmHtmlWindow::onMouseRightClick, this);
    this->Bind(wxEVT_MENU,       &mmHtmlWindow::onMenuSelected, this);
}

void mmHtmlWindow::onMouseRightClick(wxMouseEvent& /*event*/)
{
    wxMenu menu;
    menu.Append(wxID_HIGHEST + 1, _t("Copy all text to clipboard"));
    PopupMenu(&menu);
}

void mmHtmlWindow::onMenuSelected(wxCommandEvent& event)
{
    int i = event.GetId() - wxID_HIGHEST;
    // There is only one anyway
    if (i == 1 && wxTheClipboard->Open()) {
        wxTheClipboard->SetData(new wxTextDataObject(this->ToText()));
        wxTheClipboard->Close();
    }
}
