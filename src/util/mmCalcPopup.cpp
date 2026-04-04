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

#include "mmCalcPopup.h"

#include <wx/display.h>
#include "base/_constants.h"
#include "mmCalcValidator.h"

mmCalcPopup::mmCalcPopup(
    wxWindow* parent,
    mmTextCtrl* target,
    bool trigger
) :
    wxPopupTransientWindow(parent, wxBORDER_THEME | wxPU_CONTAINS_CONTROLS),
    target_(target),
    trigger_(trigger)
{
    panel = new wxWindow(this, wxID_ANY);
    font = parent->GetFont();
    SetFont(font);
    int fontSize = font.GetPointSize();
    btnSize = wxSize(fontSize + 25, fontSize + 25);

    wxFlexGridSizer* sizer = new wxFlexGridSizer(2, 1, 0, 0);
    sizer->SetFlexibleDirection(wxVERTICAL);
    sizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    valueTextCtrl_ = new mmTextCtrl(panel, wxID_ANY,
        "",
        wxDefaultPosition, wxDefaultSize,
        wxALIGN_RIGHT | wxTE_PROCESS_ENTER,
        mmCalcValidator()
    );
    valueTextCtrl_->SetFont(font);
    valueTextCtrl_->SetIgnoreFocusChange(true);
    sizer->Add(valueTextCtrl_, g_flagsExpand);

    buttonSizer = new wxGridSizer(5, 4, 0, 0);

    createButton("(", wxID_ANY);
    createButton(")", wxID_ANY);
    createButton("C", wxID_ANY);
    createButton(wxString::FromUTF8Unchecked("\u232b"), mmID_DELETE);

    createButton("7", wxID_ANY);
    createButton("8", wxID_ANY);
    createButton("9", wxID_ANY);
    createButton(wxString::FromUTF8Unchecked("\u00f7"), mmID_DIVIDE);

    createButton("4", wxID_ANY);
    createButton("5", wxID_ANY);
    createButton("6", wxID_ANY);
    createButton(wxString::FromUTF8Unchecked("\u00d7"), mmID_MULTIPLY);

    createButton("1", wxID_ANY);
    createButton("2", wxID_ANY);
    createButton("3", wxID_ANY);
    createButton("-", wxID_ANY);

    createButton("0", wxID_ANY);
    button_dec_ = createButton(".", wxID_ANY);
    createButton("=", wxID_ANY);
    createButton("+", wxID_ANY);

    sizer->Add(buttonSizer, g_flagsH);

    panel->SetSizer(sizer);
    sizer->Fit(panel);
    SetClientSize(panel->GetSize());
}

wxButton* mmCalcPopup::createButton(wxString symbol, int event)
{
    wxButton* button = new wxButton(panel, event, symbol, wxDefaultPosition, btnSize);
    button->Bind(wxEVT_BUTTON, &mmCalcPopup::OnButtonPressed, this);
    button->SetFont(font);
    buttonSizer->Add(button, wxSizerFlags(g_flagsH).Border(wxALL, 1));
    return button;
}


mmCalcPopup::~mmCalcPopup()
{
}

void mmCalcPopup::Popup(wxWindow* focus)
{
    if (dismissedByButton_ == false)
    {
        wxPoint pt = GetParent()->GetScreenPosition();
        wxRect displayRect = wxDisplay(wxDisplay::GetFromPoint(pt)).GetGeometry();

        int x = std::min(pt.x, displayRect.GetRight() - GetSize().GetWidth());
        int y = std::min(pt.y + GetParent()->GetSize().GetHeight(), displayRect.GetBottom() - GetSize().GetHeight());
        SetPosition(wxPoint(x, y));
        valueTextCtrl_->ChangeValue(target_->GetValue());
        valueTextCtrl_->SelectNone();
        valueTextCtrl_->SetInsertionPointEnd();
        target_->Enable(false);
        wxPopupTransientWindow::Popup(focus);
    }
    else {
        dismissedByButton_ = false;
    }
}

void mmCalcPopup::SetValue(wxString& value)
{
    if (target_)
        valueTextCtrl_->SetCurrency(target_->GetCurrency());
    valueTextCtrl_->ChangeValue(value);
    valueTextCtrl_->SelectNone();
    valueTextCtrl_->SetInsertionPointEnd();
}

void mmCalcPopup::OnButtonPressed(wxCommandEvent& event)
{
    wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
    int id = event.GetId();
    wxString text = btn->GetLabel();
    valueTextCtrl_->SelectNone();

    if (text == "=") {
        valueTextCtrl_->Calculate();
    }
    else if (text == "C") {
        valueTextCtrl_->ChangeValue("");
    }
    else if (id == mmID_DELETE) {
        long to, from;
        valueTextCtrl_->GetSelection(&from, &to);
        if (from != to) {
            valueTextCtrl_->ChangeValue(valueTextCtrl_->GetValue().Remove(from, to - from));
        }
        else {
            int ip = valueTextCtrl_->GetInsertionPoint();
            if (ip > 0) {
                valueTextCtrl_->Remove(ip - 1, ip);
                ip -= 1;
            }
        }
    }
    else if (id == mmID_MULTIPLY) {
        valueTextCtrl_->WriteText("*");
    }
    else if (id == mmID_DIVIDE) {
        valueTextCtrl_->WriteText("/");
    }
    else {
        valueTextCtrl_->WriteText(text);
    }
    valueTextCtrl_->SetFocus();
    valueTextCtrl_->SelectNone();
    valueTextCtrl_->SetInsertionPointEnd();
}

void mmCalcPopup::OnDismiss()
{
#ifdef __WXMSW__
    // On MSW check if the button was used to dismiss to prevent the popup from reopening
    wxPoint mousePos = wxGetMousePosition();
    dismissedByButton_ = (GetParent()->GetClientRect().Contains(GetParent()->ScreenToClient(mousePos)));
#endif
    if (target_) {
        valueTextCtrl_->Calculate();
        if (trigger_) {
            target_->SetValue(valueTextCtrl_->GetValue());
        }
        else {
            target_->ChangeValue(valueTextCtrl_->GetValue());
        }
        //target_->ChangeValue(valueTextCtrl_->GetValue());
        target_->Enable(true);
        target_->SetFocus();
    }
}

