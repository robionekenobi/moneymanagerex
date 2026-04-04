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

// -- constructor

mmCalcPopup::mmCalcPopup(
    wxWindow* parent_win,
    mmTextCtrl* target,
    bool trigger
) :
    wxPopupTransientWindow(parent_win, wxBORDER_THEME | wxPU_CONTAINS_CONTROLS),
    w_target(target),
    m_trigger(trigger)
{
    w_panel = new wxWindow(this, wxID_ANY);
    m_font = parent_win->GetFont();
    SetFont(m_font);
    int fontSize = m_font.GetPointSize();
    m_btn_size = wxSize(fontSize + 25, fontSize + 25);

    wxFlexGridSizer* sizer = new wxFlexGridSizer(2, 1, 0, 0);
    sizer->SetFlexibleDirection(wxVERTICAL);
    sizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    w_value_text = new mmTextCtrl(w_panel, wxID_ANY,
        "",
        wxDefaultPosition, wxDefaultSize,
        wxALIGN_RIGHT | wxTE_PROCESS_ENTER,
        mmCalcValidator()
    );
    w_value_text->SetFont(m_font);
    w_value_text->SetIgnoreFocusChange(true);
    sizer->Add(w_value_text, g_flagsExpand);

    w_sizer = new wxGridSizer(5, 4, 0, 0);

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
    w_dec_btn = createButton(".", wxID_ANY);
    createButton("=", wxID_ANY);
    createButton("+", wxID_ANY);

    sizer->Add(w_sizer, g_flagsH);

    w_panel->SetSizer(sizer);
    sizer->Fit(w_panel);
    SetClientSize(w_panel->GetSize());
}

mmCalcPopup::~mmCalcPopup()
{
}

wxButton* mmCalcPopup::createButton(wxString symbol, int event)
{
    wxButton* button = new wxButton(
        w_panel, event, symbol,
        wxDefaultPosition, m_btn_size
    );
    button->Bind(wxEVT_BUTTON, &mmCalcPopup::onButtonPressed, this);
    button->SetFont(m_font);
    w_sizer->Add(button, wxSizerFlags(g_flagsH).Border(wxALL, 1));
    return button;
}

// -- override

void mmCalcPopup::Popup(wxWindow* focus)
{
    if (m_dismissed) {
        m_dismissed = false;
        return;
    }

    wxPoint pt = GetParent()->GetScreenPosition();
    wxRect displayRect = wxDisplay(wxDisplay::GetFromPoint(pt)).GetGeometry();

    int x = std::min(
        pt.x,
        displayRect.GetRight() - GetSize().GetWidth()
    );
    int y = std::min(
        pt.y + GetParent()->GetSize().GetHeight(),
        displayRect.GetBottom() - GetSize().GetHeight()
    );
    SetPosition(wxPoint(x, y));
    w_value_text->ChangeValue(w_target->GetValue());
    w_value_text->SelectNone();
    w_value_text->SetInsertionPointEnd();
    w_target->Enable(false);
    wxPopupTransientWindow::Popup(focus);
}

void mmCalcPopup::OnDismiss()
{
#ifdef __WXMSW__
    // On MSW check if the button was used to dismiss to prevent the popup from reopening
    wxPoint mousePos = wxGetMousePosition();
    m_dismissed = GetParent()->GetClientRect().Contains(
        GetParent()->ScreenToClient(mousePos)
    );
#endif
    if (w_target) {
        w_value_text->Calculate();
        if (m_trigger) {
            w_target->SetValue(w_value_text->GetValue());
        }
        else {
            w_target->ChangeValue(w_value_text->GetValue());
        }
        //w_target->ChangeValue(w_value_text->GetValue());
        w_target->Enable(true);
        w_target->SetFocus();
    }
}

// -- methods

void mmCalcPopup::setValue(wxString& value)
{
    if (w_target)
        w_value_text->SetCurrency(w_target->GetCurrency());
    w_value_text->ChangeValue(value);
    w_value_text->SelectNone();
    w_value_text->SetInsertionPointEnd();
}

void mmCalcPopup::setTarget(mmTextCtrl* target)
{
    w_target = target;
    w_dec_btn->SetLabel(target->GetDecimalPoint());
}

// -- event handlers

void mmCalcPopup::onButtonPressed(wxCommandEvent& event)
{
    wxButton* btn = dynamic_cast<wxButton*>(event.GetEventObject());
    int id = event.GetId();
    wxString text = btn->GetLabel();
    w_value_text->SelectNone();

    if (text == "=") {
        w_value_text->Calculate();
    }
    else if (text == "C") {
        w_value_text->ChangeValue("");
    }
    else if (id == mmID_DELETE) {
        long to, from;
        w_value_text->GetSelection(&from, &to);
        if (from != to) {
            w_value_text->ChangeValue(w_value_text->GetValue().Remove(from, to - from));
        }
        else {
            int ip = w_value_text->GetInsertionPoint();
            if (ip > 0) {
                w_value_text->Remove(ip - 1, ip);
                ip -= 1;
            }
        }
    }
    else if (id == mmID_MULTIPLY) {
        w_value_text->WriteText("*");
    }
    else if (id == mmID_DIVIDE) {
        w_value_text->WriteText("/");
    }
    else {
        w_value_text->WriteText(text);
    }
    w_value_text->SetFocus();
    w_value_text->SelectNone();
    w_value_text->SetInsertionPointEnd();
}
