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

#include "mmSingleChoice.h"

#include "base/_constants.h"
#include "mmPath.h"

mmSingleChoice::mmSingleChoice()
{
}

mmSingleChoice::mmSingleChoice(
    wxWindow* parent_win,
    const wxString& message,
    const wxString& caption,
    const wxArrayString& choice_a
) {
    if (parent_win)
        this->SetFont(parent_win->GetFont());
    wxSingleChoiceDialog::Create(parent_win, message, caption, choice_a);
    mmThemeAutoColour(this);
    SetMinSize(wxSize(220, 384));
    SetIcon(mmPath::getProgramIcon());

    wxButton* ok_btn = static_cast<wxButton*>(FindWindow(wxID_OK));
    if (ok_btn)
        ok_btn->SetLabel(wxGetTranslation(g_OkLabel));
    wxButton* cancel_btn = static_cast<wxButton*>(FindWindow(wxID_CANCEL));
    if (cancel_btn)
        cancel_btn->SetLabel(wxGetTranslation(g_CancelLabel));
    Fit();
}

mmSingleChoice::mmSingleChoice(
    wxWindow* parent_win,
    const wxString& message,
    const wxString& caption,
    const AccountModel::DataA& account_a
) {
    if (parent_win)
        this->SetFont(parent_win->GetFont());
    wxArrayString choice_a;
    for (const auto& account_d : account_a)
        choice_a.Add(account_d.m_name);
    wxSingleChoiceDialog::Create(parent_win, message, caption, choice_a);
    mmThemeAutoColour(this);
    SetMinSize(wxSize(220, 384));
    SetIcon(mmPath::getProgramIcon());

    wxButton* ok_btn = static_cast<wxButton*>(FindWindow(wxID_OK));
    if (ok_btn)
        ok_btn->SetLabel(wxGetTranslation(g_OkLabel));
    wxButton* cancel_btn = static_cast<wxButton*>(FindWindow(wxID_CANCEL));
    if (cancel_btn)
        cancel_btn->SetLabel(wxGetTranslation(g_CancelLabel));
    Fit();
}
