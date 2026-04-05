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

#include "_simple.h"

#include "base/_defs.h"
#include <wx/dcbuffer.h>
#include <wx/popupwin.h>
#include <wx/spinctrl.h>
#include <wx/graphics.h>
#include <wx/renderer.h>
#include <wx/richtooltip.h>

#include "base/_constants.h"
#include "base/mmPlatform.h"
#include "base/mmUserColor.h"
#include "mmPath.h"
#include "mmImage.h"
#include "mmDatePicker.h"
#include "mmCalcValidator.h"
#include "_util.h"

#include "model/AccountModel.h"
#include "model/CategoryModel.h"
#include "model/PayeeModel.h"
#include "model/SettingModel.h"
#include "model/TagModel.h"

#include "manager/TagManager.h"
#include "app/mmApp.h"

//------- Pop-up calendar, currently only used for MacOS only
// See: https://github.com/moneymanagerex/moneymanagerex/issues/3139

//----------------------------------------------------------------------------
// mmCalendarPopup
//----------------------------------------------------------------------------
class mmCalendarPopup: public wxPopupTransientWindow
{
public:
    mmCalendarPopup(wxWindow *parent, mmDatePicker* datePicker);
    virtual ~mmCalendarPopup();

private:
    mmDatePicker* m_datePicker;
    void OnDateSelected(wxCalendarEvent& event);
    void OnEndSelection(wxCalendarEvent& event);
    wxDECLARE_ABSTRACT_CLASS(mmCalendarPopup);
};

wxIMPLEMENT_CLASS(mmCalendarPopup, wxPopupTransientWindow);

mmCalendarPopup::mmCalendarPopup( wxWindow *parent, mmDatePicker* datePicker)
    : wxPopupTransientWindow(parent,
        wxBORDER_NONE)
    , m_datePicker(datePicker)
{
    wxWindow* panel = new wxWindow(this, wxID_ANY);

    wxCalendarCtrl* m_calendarCtrl = new wxCalendarCtrl(panel, wxID_ANY, datePicker->GetValue()
        , wxDefaultPosition, wxDefaultSize
        , wxCAL_SEQUENTIAL_MONTH_SELECTION | wxCAL_SHOW_HOLIDAYS | wxCAL_SHOW_SURROUNDING_WEEKS);
    m_calendarCtrl->Bind(wxEVT_CALENDAR_SEL_CHANGED, &mmCalendarPopup::OnDateSelected, this);
    m_calendarCtrl->Bind(wxEVT_CALENDAR_DOUBLECLICKED, &mmCalendarPopup::OnEndSelection, this);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_calendarCtrl, 0, wxALL, 5);
    panel->SetSizer(sizer);

    sizer->Fit(panel);
    SetClientSize(panel->GetSize());
}

mmCalendarPopup::~mmCalendarPopup()
{

}

void mmCalendarPopup::OnDateSelected(wxCalendarEvent& event)
{
    m_datePicker->setValue(event.GetDate());
    wxDateEvent evt(m_datePicker, m_datePicker->GetValue(), wxEVT_DATE_CHANGED);
    m_datePicker->GetEventHandler()->AddPendingEvent(evt);
}

void mmCalendarPopup::OnEndSelection(wxCalendarEvent& event)
{
    m_datePicker->setValue(event.GetDate());
    this->Dismiss();
    wxDateEvent evt(m_datePicker, m_datePicker->GetValue(), wxEVT_DATE_CHANGED);
    m_datePicker->GetEventHandler()->AddPendingEvent(evt);
}

// ------------------------------------

mmChoiceAmountMask::mmChoiceAmountMask(wxWindow* parent, wxWindowID id)
    : wxChoice(parent, id)
{
    static const std::vector <std::pair<wxString, wxString> > DATA = {
        { ".", "." },
        { ",", "," },
        { _n("None"), "" }
    };

    for (const auto& entry : DATA) {
        this->Append(wxGetTranslation(entry.first), new wxStringClientData(entry.second));
    }

    const CurrencyData* base_currency = CurrencyModel::instance().get_base_data_n();
    const auto decimal_point = base_currency->m_decimal_point;

    SetDecimalChar(decimal_point);
}

void mmChoiceAmountMask::SetDecimalChar(const wxString& str)
{
    if (str == ".")
        SetSelection(0);
    else if (str == ",")
        SetSelection(1);
    else
        SetSelection(2);
}

/*/////////////////////////////////////////////////////////////*/

mmDialogComboBoxAutocomplete::mmDialogComboBoxAutocomplete()
{
}

const wxString mmDialogComboBoxAutocomplete::getText() const
{
    return cbText_->GetValue();
}

mmDialogComboBoxAutocomplete::mmDialogComboBoxAutocomplete(wxWindow *parent, const wxString& message, const wxString& caption,
    const wxString& defaultText, const wxArrayString& choices)
    : m_default_str(defaultText),
    m_choices(choices),
    m_message(message)
{
    if (m_choices.Index(m_default_str) == wxNOT_FOUND)
    {
        m_choices.Add(m_default_str);
        m_choices.Sort(CaseInsensitiveLocaleCmp);
    }
    this->SetFont(parent->GetFont());
    Create(parent, wxID_ANY, caption);
    SetMinSize(wxSize(300, 100));
}

bool mmDialogComboBoxAutocomplete::Create(wxWindow* parent, wxWindowID id,
    const wxString& caption, const wxPoint& pos, const wxSize& size, long style)
{
    wxDialog::Create(parent, id, caption, pos, size, style);
    const wxSizerFlags flags = wxSizerFlags().Align(wxALIGN_CENTER).Border(wxLEFT | wxRIGHT, 15);

    wxBoxSizer* Sizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(Sizer);

    Sizer->AddSpacer(10);
    wxStaticText* headerText = new wxStaticText(this, wxID_STATIC, m_message);
    Sizer->Add(headerText, flags);
    Sizer->AddSpacer(15);
    cbText_ = new mmComboBoxCustom(this, m_choices);
    cbText_->SetMinSize(wxSize(150, -1));
    Sizer->Add(cbText_, wxSizerFlags().Border(wxLEFT | wxRIGHT, 15).Expand());
    Sizer->AddSpacer(20);
    wxSizer* Button = CreateButtonSizer(wxOK | wxCANCEL);
    Sizer->Add(Button, flags);
    Sizer->AddSpacer(10);

    cbText_->SetFocus();
    cbText_->ChangeValue(m_default_str);
    cbText_->SelectAll();
    Centre();
    Fit();
    return true;
}

/*////////////////// Error Messages //////////////////////////////////////*/

void mmErrorDialogs::MessageError(wxWindow *parent
    , const wxString &message, const wxString &title)
{
    wxMessageDialog msgDlg(parent, message, title, wxOK | wxICON_ERROR);
    msgDlg.ShowModal();
}

void mmErrorDialogs::MessageWarning(wxWindow *parent
    , const wxString &message, const wxString &title)
{
    wxMessageDialog msgDlg(parent, message, title, wxOK | wxICON_WARNING);
    msgDlg.ShowModal();
}

void mmErrorDialogs::MessageInvalid(wxWindow *parent, const wxString &message)
{
    const wxString& msg = wxString::Format(_t("Entry %s is invalid"), message, wxICON_ERROR);
    MessageError(parent, msg, _t("Invalid Entry"));
}

void mmErrorDialogs::InvalidCategory(wxWindow *win)
{
    ToolTip4Object(win, _t("Please select an existing category"), _t("Invalid Category"), wxICON_ERROR);
}

void mmErrorDialogs::InvalidFile(wxWindow *object, bool open)
{
    const wxString errorHeader = open ? _t("Unable to open file.") : _t("File name is empty.");
    const wxString errorMessage = _t("Please select the file for this operation.");

    ToolTip4Object(object, errorMessage, errorHeader, wxICON_ERROR);
}

void mmErrorDialogs::InvalidAccount(wxWindow *object, bool transfer, TOOL_TIP tm)
{
    const wxString errorHeader = _t("Invalid Account");
    wxString errorMessage;
    if (!transfer)
        errorMessage = _t("Please select the account for this transaction.");
    else
        errorMessage = _t("Please specify which account the transfer is going to.");

    wxString errorTips = _t("Selection can be made by using the dropdown button.");
    if (tm == TOOL_TIP::MESSAGE_POPUP_BOX)
    {
        errorTips = _t("Activating the button will provide a selection box where the account can be selected.");
    }
    errorMessage = errorMessage + "\n\n" + errorTips + "\n";

    ToolTip4Object(object, errorMessage, errorHeader, wxICON_ERROR);
}

void mmErrorDialogs::InvalidPayee(wxWindow *object)
{
    const wxString errorHeader = _t("Invalid Payee");
    const wxString errorMessage = _t("Please type in a new payee,\n"
        "or make a selection using the dropdown button.")
        + "\n";
    ToolTip4Object(object, errorMessage, errorHeader, wxICON_ERROR);
}

void mmErrorDialogs::InvalidName(wxTextCtrl *textBox, bool alreadyexist)
{
    const wxString errorHeader = _t("Invalid Name");
    wxString errorMessage;
    if (alreadyexist)
        errorMessage = _t("Already exist!");
    else
        errorMessage = _t("Please type in a non empty name.");

    ToolTip4Object(textBox, errorMessage, errorHeader, wxICON_ERROR);
}

void mmErrorDialogs::InvalidSymbol(wxTextCtrl *textBox, bool alreadyexist)
{
    const wxString errorHeader = _t("Invalid Name");
    wxString errorMessage;
    if (alreadyexist)
        errorMessage = _t("Already exist!");
    else
        errorMessage = _t("Please type in a non empty symbol.");

    ToolTip4Object(textBox, errorMessage, errorHeader, wxICON_ERROR);
}

void mmErrorDialogs::ToolTip4Object(wxWindow *object, const wxString &message, const wxString &title, int ico)
{
    wxRichToolTip tip(title, message);
    tip.SetIcon(ico);
    tip.SetBackgroundColour(object->GetParent()->GetBackgroundColour());
    tip.ShowFor(object);
}

// --

mmTagTextCtrl::mmTagTextCtrl(
    wxWindow* parent,
    wxWindowID id,
    bool operatorAllowed,
    const wxPoint& pos,
    const wxSize& size,
    long style
) :
    wxPanel(),
    operatorAllowed_(operatorAllowed)
{
    wxColour fgColor;
#ifdef __WXMAC__
    if (mmPlatform::isDarkMode()) {
        style |= wxBORDER_SIMPLE;
        bgColorEnabled_ = wxColour(58, 58, 58);
        bgColorDisabled_ = wxColour(45, 45, 45);
    }
    else {
        style |= wxBORDER_THEME;
        bgColorDisabled_ = wxColour(251, 251, 251);
    }
    fgColor = *mmUserColor::bestFontColor(bgColorEnabled_);
#else
    style |= wxBORDER_NONE;
    bgColorEnabled_ = mmImage::themeMetaColour(COLOR_TEXTCONTROL);
    fgColor = mmImage::themeMetaColour(COLOR_TEXTCONTROL_FONT);
#endif
    Create(parent, id, pos, size, style);
    SetFont(parent->GetFont());
    SetBackgroundColour(bgColorEnabled_);
    wxBoxSizer* panel_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* v_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* h_sizer = new wxBoxSizer(wxHORIZONTAL);
    textCtrl_ = new wxStyledTextCtrl(this, wxID_DEFAULT, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
    if (operatorAllowed_)
        mmToolTip(textCtrl_,
            _t("Enter tags to search, optionally separated by the operators '&' and '|'.") + "\n" +
            _t("The default operator between consecutive tags is AND, use '|' to change the operator to OR.")
        );
    // Set options for the text ctrl
    textCtrl_->SetLexer(wxSTC_LEX_NULL);
    textCtrl_->SetWrapMode(wxSTC_WRAP_NONE);
    textCtrl_->SetMarginWidth(1, 0);
    textCtrl_->SetMarginSensitive(1, false);
    textCtrl_->SetUseVerticalScrollBar(false);
    textCtrl_->SetUseHorizontalScrollBar(false);
    textCtrl_->AutoCompSetIgnoreCase(true);
    textCtrl_->AutoCompSetCancelAtStart(true);
    textCtrl_->AutoCompSetAutoHide(false);
    textCtrl_->StyleSetFont(1, parent->GetFont());
    textCtrl_->StyleSetBackground(1, wxColour(186, 226, 185));
    textCtrl_->StyleSetForeground(1, *wxBLACK);
    textCtrl_->StyleSetFont(0, parent->GetFont());
    textCtrl_->StyleSetBackground(0, bgColorEnabled_);
    textCtrl_->StyleSetForeground(0, fgColor);
    textCtrl_->StyleSetBackground(wxSTC_STYLE_DEFAULT, bgColorEnabled_);
    textCtrl_->StyleSetForeground(wxSTC_STYLE_DEFAULT, fgColor);
    textCtrl_->SetCaretForeground(fgColor);
    textCtrl_->SetMaxClientSize(wxSize(-1, textCtrl_->TextHeight(0)+2));
    textCtrl_->SetWordChars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_.,;:!?'\"()[]{}<>/\\|-_=+*&^%@#$~");

    // Text ctrl event handlers
    textCtrl_->Bind(wxEVT_CHAR, &mmTagTextCtrl::OnTextChanged, this);
    textCtrl_->Bind(wxEVT_STC_CLIPBOARD_PASTE, &mmTagTextCtrl::OnPaste, this);
    Bind(wxEVT_PAINT, &mmTagTextCtrl::OnPaint, this);
    textCtrl_->Bind(wxEVT_KILL_FOCUS, &mmTagTextCtrl::OnKillFocus, this);
    textCtrl_->Bind(wxEVT_CHAR_HOOK, &mmTagTextCtrl::OnKeyPressed, this);
    textCtrl_->Bind(wxEVT_STC_ZOOM, [this](wxStyledTextEvent& ) {
        // Disable zoom
        textCtrl_->SetEvtHandlerEnabled(false);
        textCtrl_->SetZoom(0);
        textCtrl_->SetEvtHandlerEnabled(true);
    });

    wxSize btnSize;
#ifdef __WXMSW__
    panelHeight_ = textCtrl_->TextHeight(0) + 8;
    textOffset_ = 3;
    h_sizer->Add(textCtrl_, 1, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, textOffset_);
    wxWindowDC dc(this);
    btnSize = wxRendererNative::Get().GetCollapseButtonSize(this, dc);
    textCtrl_->Bind(wxEVT_STC_PAINTED, [this](wxStyledTextEvent&)
    {
        if (!repaint_allowed_)
        {
            repaint_allowed_ = true;
            return;
        }

        repaint_allowed_ = false;
        Refresh();
    });
#elif __WXGTK__
    panelHeight_ = std::max(textCtrl_->TextHeight(0) + 2, 34);
    textOffset_ = 8;
    h_sizer->Add(textCtrl_, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, textOffset_);
    btnSize = wxSize(panelHeight_ + 3, panelHeight_);
#else
    panelHeight_ = textCtrl_->TextHeight(0) + 4;
    textOffset_ = 1;
    h_sizer->Add(textCtrl_, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, textOffset_);
    btnSize = wxSize(textCtrl_->TextHeight(0), textCtrl_->TextHeight(0));
#endif

    createDropButton(btnSize);

    btn_dropdown_->Bind(wxEVT_BUTTON, &mmTagTextCtrl::OnDropDown, this);
    btn_dropdown_->Bind(wxEVT_NAVIGATION_KEY, [this](wxNavigationKeyEvent& ) { textCtrl_->SetFocus(); });

#ifndef __WXMAC__
    // Event handlers for custom control painting in Windows & Linux
    Bind(wxEVT_ENTER_WINDOW, &mmTagTextCtrl::OnMouseCaptureChange, this);
    Bind(wxEVT_LEAVE_WINDOW, &mmTagTextCtrl::OnMouseCaptureChange, this);
    textCtrl_->Bind(wxEVT_ENTER_WINDOW, &mmTagTextCtrl::OnMouseCaptureChange, this);
    textCtrl_->Bind(wxEVT_LEAVE_WINDOW, &mmTagTextCtrl::OnMouseCaptureChange, this);
    textCtrl_->Bind(wxEVT_SIZE, [this](wxSizeEvent& ) {textCtrl_->Refresh(); });
    btn_dropdown_->Bind(wxEVT_LEAVE_WINDOW, &mmTagTextCtrl::OnMouseCaptureChange, this);
    btn_dropdown_->Bind(wxEVT_SET_FOCUS, &mmTagTextCtrl::OnFocusChange, this);
    btn_dropdown_->Bind(wxEVT_ENTER_WINDOW, &mmTagTextCtrl::OnMouseCaptureChange, this);
    btn_dropdown_->Bind(wxEVT_KILL_FOCUS, &mmTagTextCtrl::OnFocusChange, this);
#endif

    v_sizer->Add(h_sizer, 1, wxEXPAND, 0);
    panel_sizer->Add(v_sizer, 1, wxEXPAND, 0);
    panel_sizer->Add(btn_dropdown_, 0, wxEXPAND | wxALL, 0);

    // Popup window
    popupWindow_ = new mmTagCtrlPopupWindow(this, btn_dropdown_);
    wxScrolledWindow* scrolledWindow = new wxScrolledWindow(popupWindow_);
    tagCheckListBox_ = new wxCheckListBox(scrolledWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, {}, wxLB_SORT);
    mmThemeAutoColour(scrolledWindow, true);
    tagCheckListBox_->SetFont(GetFont());
    tagCheckListBox_->Bind(wxEVT_CHECKLISTBOX, &mmTagTextCtrl::OnPopupCheckboxSelected, this);

    wxBoxSizer* sw_sizer = new wxBoxSizer(wxVERTICAL);
    sw_sizer->Add(tagCheckListBox_, 1, wxEXPAND);
    scrolledWindow->SetSizer(sw_sizer);
    init();
    scrolledWindow->Fit();
    sw_sizer->Fit(popupWindow_);

    SetSizer(panel_sizer);
    SetSizeHints(-1, panelHeight_, -1, panelHeight_);
    Layout();
    btn_dropdown_->Refresh();
    btn_dropdown_->Update();
    mmThemeAutoColour(this);
}

void mmTagTextCtrl::createDropButton(wxSize btnSize)
{
#ifdef __WXMSW__
    // On Windows draw the drop arrow and store the bitmap to be used in the paint override
    wxWindowDC dc(this);
    btnSize.SetWidth(btnSize.GetWidth() - 2);
    btnSize.SetHeight(panelHeight_);
    btn_dropdown_ = new wxBitmapButton(this, wxID_ANY, wxNullBitmap, wxPoint(-1, 0), btnSize, wxBORDER_NONE, wxDefaultValidator, "btn_dropdown_");
    btn_dropdown_->Bind(wxEVT_PAINT, &mmTagTextCtrl::OnPaintButton, this);

    // Recreate the Native windows-style drop arrow
    wxImage img(btnSize);
    img.InitAlpha();
    wxBitmap bg = wxBitmap(img);

    // Normal wxDC doesn't play nice with an alpha channel in Windows
    // so use a wxGraphicsContext to draw the image in the bitmap
    wxGraphicsContext* gc = wxGraphicsContext::Create(bg);

    wxRect2DDouble rect(0, 0, btnSize.GetWidth(), btnSize.GetHeight());
    gc->SetBrush(wxBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_LISTBOX)));
    gc->DrawRectangle(rect);
    delete gc;

    int arrowX = (btnSize.GetWidth() - 8) / 2;
    int arrowY = (btnSize.GetHeight() - 2) / 2;

    wxPoint2DDouble ptstart[2] = {
        wxPoint(arrowX, arrowY),     // top L
        wxPoint(arrowX + 7, arrowY), // top R
    };

    wxPoint2DDouble ptend[2] = {
        wxPoint(arrowX + 3, arrowY + 3), // bottom center L
        wxPoint(arrowX + 4, arrowY + 3)  // bottom center R
    };

    wxPoint2DDouble sptstart[2] = {
        wxPoint(arrowX, arrowY - 1),     // shadow top L
        wxPoint(arrowX + 7, arrowY - 1), // shadow top R
    };

    wxPoint2DDouble sptend[2] = {
        wxPoint(arrowX + 3, arrowY + 2), // shadow bottom center L
        wxPoint(arrowX + 4, arrowY + 2)  // shadow bottom center R
    };

    wxPoint2DDouble lsptstart[2] = {
        wxPoint(arrowX - 1, arrowY), // light shadow top L
        wxPoint(arrowX + 8, arrowY), // light shadow top R
    };

    wxPoint2DDouble lsptend[2] = {
        wxPoint(arrowX + 3, arrowY + 4), // light shadow bottom center L
        wxPoint(arrowX + 4, arrowY + 4)  // light shadow bottom center R
    };

    dropArrow_ = bg;
    dropArrowInactive_ = bg;
    gc = wxGraphicsContext::Create(dropArrow_);
    wxGraphicsContext* gc2 = wxGraphicsContext::Create(dropArrowInactive_);
    gc->SetAntialiasMode(wxANTIALIAS_NONE);
    gc2->SetAntialiasMode(wxANTIALIAS_NONE);

    // draw shadow
    wxColour color = wxSystemSettings::GetColour(wxSYS_COLOUR_3DSHADOW);
    color.SetRGBA(0x90000000 | color.GetRGB());
    gc->SetPen(color);
    gc->StrokeLines(2, sptstart, sptend);
    color.SetRGBA(0x46000000 | color.GetRGB());
    gc2->SetPen(color);
    gc2->StrokeLines(2, sptstart, sptend);

    // draw triangle
    color = wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT);
    color.SetRGBA(0x90000000 | color.GetRGB());
    gc->SetPen(color);
    gc->StrokeLines(2, ptstart, ptend);
    color.SetRGBA(0x46000000 | color.GetRGB());
    gc2->SetPen(color);
    gc2->StrokeLines(2, ptstart, ptend);

    // draw light shadow
    color = wxSystemSettings::GetColour(wxSYS_COLOUR_3DLIGHT);
    color.SetRGBA(0x90000000 | color.GetRGB());
    gc->SetPen(color);
    gc->StrokeLines(2, lsptstart, lsptend);
    color.SetRGBA(0x46000000 | color.GetRGB());
    gc2->SetPen(color);
    gc2->StrokeLines(2, lsptstart, lsptend);

    delete gc;
    delete gc2;

#else
    // Begin the image with a transparent background
    wxImage img(btnSize);
    img.InitAlpha();
    unsigned char* alpha = img.GetAlpha();
    memset(alpha, wxIMAGE_ALPHA_TRANSPARENT, img.GetWidth() * img.GetHeight());
    dropArrow_ = wxBitmap(img);
    // On Linux wxDC works fine with an alpha channel, so use a wxMemoryDC to draw right into the bitmap
    wxMemoryDC memDC;
    memDC.SelectObject(dropArrow_);

#ifdef __WXGTK__
    // On Linux just draw a drop arrow bitmap for the normal bitmap button
    btn_dropdown_ = new wxBitmapButton(this, wxID_ANY, wxNullBitmap, wxDefaultPosition, btnSize, wxBORDER_DEFAULT, wxDefaultValidator, "btn_dropdown_");

    memDC.SetPen(wxPen(btn_dropdown_->GetForegroundColour()));
    memDC.SetBrush(wxBrush(btn_dropdown_->GetForegroundColour()));

    wxRect rect(btnSize);
    rect.width *= 0.75;
    rect.height *= 0.75;
    rect.x += rect.width / 5;
    rect.y += rect.height / 5 - 1;

    wxRendererNative::GetGeneric().DrawDropArrow(this, memDC, rect);
    dropArrowInactive_ = dropArrow_;

#else
    btn_dropdown_ = new wxBitmapButton(this, wxID_ANY, wxNullBitmap, wxDefaultPosition, btnSize, wxBORDER_NONE, wxDefaultValidator, "btn_dropdown_");
    dropArrowInactive_ = wxBitmap(img);

    // Use the native renderer to draw a combobox. We can't use the text control portion due to macOS dark mode bug
    // so we can isolate the button instead
    wxRect rect(wxSize(textCtrl_->GetSize().GetWidth(), panelHeight_));
    rect.x -= textCtrl_->GetSize().GetWidth() - panelHeight_ + 1;
    rect.y -= 2;
    wxRendererNative::Get().DrawComboBox(this, memDC, rect);

    // macOS button has rounded corners but wxBitmap has square, so need to cover the overhang
    wxRect borderRect(btnSize);
    borderRect.Inflate(1);
    memDC.SetPen(wxPen(bgColorEnabled_));
    memDC.SetBrush(wxBrush(*wxTRANSPARENT_BRUSH));
    memDC.DrawRoundedRectangle(borderRect, 4);

    // Inactive arrow
    memDC.SelectObject(dropArrowInactive_);
    wxRendererNative::Get().DrawComboBox(this, memDC, rect, wxCONTROL_DISABLED);
    memDC.DrawRoundedRectangle(borderRect, 4);
#endif
    memDC.SelectObject(wxNullBitmap);
    btn_dropdown_->SetBitmap(dropArrow_);
#endif
}

void mmTagTextCtrl::OnMouseCaptureChange(wxMouseEvent& event)
{
    textCtrl_->Refresh(false);
    event.Skip();
}

void mmTagTextCtrl::OnFocusChange(wxFocusEvent& event)
{
    textCtrl_->Refresh(false);
    event.Skip();
}

void mmTagTextCtrl::OnDropDown(wxCommandEvent& )
{
#ifndef __WXMAC__
    if (!popupWindow_->dismissedByButton_)
    {
        ValidateTagText();
        wxPoint pos = ClientToScreen(textCtrl_->GetPosition());
        pos.y += (textCtrl_->GetSize().GetHeight() + panelHeight_) / 2;
        pos.x -= textOffset_;
        popupWindow_->Position(pos, wxSize(0, 0));
        popupWindow_->SetSize(GetSize().GetWidth(), -1);
        tagCheckListBox_->GetParent()->SetSize(popupWindow_->GetSize());
        tagCheckListBox_->SetSize(popupWindow_->GetSize());
        for (unsigned int i = 0; i < tagCheckListBox_->GetCount(); i++)
        {
            if (tags_.find(tagCheckListBox_->GetString(i)) != tags_.end())
                tagCheckListBox_->Check(i);
            else
                tagCheckListBox_->Check(i, false);
        }

        popupWindow_->Popup();
        tagCheckListBox_->SetFocus();
    }
    else
        popupWindow_->dismissedByButton_ = false;
#else
    // mac code has a bug where the tag dropdown doesn't process clicks over underlying controls.
    // just show the tag dialog instead until the issue is fixed.
    wxKeyEvent evt(wxEVT_CHAR_HOOK);
    evt.SetEventObject(textCtrl_);
    evt.m_keyCode = WXK_RETURN;
    OnKeyPressed(evt);
#endif
}

void mmTagTextCtrl::OnKeyPressed(wxKeyEvent& event)
{
    int keyCode = event.GetKeyCode();
    if (keyCode == WXK_RETURN || keyCode == WXK_NUMPAD_ENTER) {
        int ip = textCtrl_->GetInsertionPoint();
        if (textCtrl_->GetText().IsEmpty() ||
            ip == 0 || textCtrl_->GetTextRange(ip - 1, ip) == " "
        ) {
            TagManager dlg(this, true, parseTags(textCtrl_->GetText()));
            if (dlg.ShowModal() == wxID_OK) {
                wxString selection;
                for (const auto& tag : dlg.getSelectedTags())
                    selection.Append(tag + " ");
                textCtrl_->SetText(selection);
                textCtrl_->GotoPos(textCtrl_->GetLastPosition());
            }
            if (dlg.getRefreshRequested())
                init();
        }
        else if (textCtrl_->AutoCompActive()) {
            textCtrl_->AutoCompComplete();
        }

        ValidateTagText();
        return;
    }
    else if (keyCode == WXK_TAB && !event.AltDown()) {
        bool prev = event.ShiftDown();
        wxWindow* next_control = prev ? GetPrevSibling() : GetNextSibling();
        while (next_control && !next_control->IsFocusable())
            next_control = prev
                ? next_control->GetPrevSibling()
                : next_control->GetNextSibling();
        if (next_control)
            next_control->SetFocus();
        return;
    }
    else if (keyCode == WXK_SPACE) {
        textCtrl_->AutoCompCancel();
        textCtrl_->InsertText(textCtrl_->GetInsertionPoint(), " ");
        ValidateTagText();
        return;
    }
    event.Skip();
}

void mmTagTextCtrl::init()
{
    // Initialize the tag map and dropdown checkboxes
    tag_map_.clear();
    tagCheckListBox_->Clear();
    for (const auto& tag_d : TagModel::instance().find_all(
        TagCol::COL_ID_TAGNAME
    )) {
        tag_map_[tag_d.m_name] = tag_d.m_id;
        tagCheckListBox_->Append(tag_d.m_name);
    }

    tagCheckListBox_->Fit();
}

void mmTagTextCtrl::OnTextChanged(wxKeyEvent& event)
{
    int keyCode = event.GetUnicodeKey();
    if (!event.AltDown() && keyCode > 32 && keyCode < 127)
    {

        int ip = textCtrl_->GetInsertionPoint();
        int anchor = textCtrl_->GetAnchor();
        int tag_start = textCtrl_->WordStartPosition(std::min(anchor, ip), true);
        int tag_end = textCtrl_->WordEndPosition(std::max(anchor, ip), true);

        // Show autocomplete
        wxString pattern = textCtrl_->GetText().Mid(tag_start, std::min(ip, anchor) - tag_start).Append(event.GetUnicodeKey());
        autocomplete_string_.Clear();
        for (const auto& tag_d : tag_map_) {
            if (tag_d.first.Lower().Matches(pattern.Lower().Prepend("*").Append("*")))
                autocomplete_string_.Append(tag_d.first + " ");
        }
        if (!autocomplete_string_.IsEmpty()) {
            textCtrl_->AutoCompShow(tag_end - tag_start, autocomplete_string_);
        }
        else
            textCtrl_->AutoCompCancel();
    }
    event.Skip();
}

void mmTagTextCtrl::OnPopupCheckboxSelected(wxCommandEvent& event)
{
    // When the user clicks in item in the checkbox update the textCtrl
    wxString selectedText = event.GetString();
    // If they checked a box, append to the string
    if (tagCheckListBox_->IsChecked(event.GetSelection()))
        textCtrl_->SetText(textCtrl_->GetText().Trim() + " " + selectedText +  " ");
    else {
        // If they unchecked a box, remove the tag from the string.
        int pos = 0;
        while (pos <= textCtrl_->GetLastPosition()) {
            pos = textCtrl_->FindText(pos, textCtrl_->GetLastPosition(), selectedText);

            if (pos == wxNOT_FOUND)
                break;

            pos = textCtrl_->WordStartPosition(pos, true);
            int end = textCtrl_->WordEndPosition(pos, true);
            if (textCtrl_->GetTextRange(pos, end) == selectedText) {
                textCtrl_->Remove(pos, end);
                break;
            }
            pos = end + 1;
        }
    }
    ValidateTagText();
}

void mmTagTextCtrl::OnPaste(wxStyledTextEvent& event)
{
    wxString currText = textCtrl_->GetText();
    ValidateTagText(currText.insert(textCtrl_->GetInsertionPoint(), event.GetString()));
    event.SetString("");
}

void mmTagTextCtrl::OnKillFocus(wxFocusEvent& event)
{
    textCtrl_->AutoCompCancel();
    // Remove any non-tags
    ValidateTagText();
    wxString tagString;
    wxArrayString tags = parseTags(textCtrl_->GetText());
    for (const auto& tag : tags)
        if (tags_.find(tag) != tags_.end()
            || (operatorAllowed_ && (tag == "&" || tag == "|")))
            tagString.Append(tag + " ");

    textCtrl_->SetText(tagString.Trim());
    event.Skip();
}

bool mmTagTextCtrl::Enable(bool enable)
{
    if (enable == textCtrl_->IsEnabled())
        return false;

    wxPanel::Enable(enable);
    textCtrl_->Enable(enable);
    btn_dropdown_->Enable(enable);

    if (enable) {
        textCtrl_->StyleSetBackground(wxSTC_STYLE_DEFAULT, bgColorEnabled_);
        SetBackgroundColour(bgColorEnabled_);
        btn_dropdown_->SetBitmap(dropArrow_);
    }
    else {
        textCtrl_->StyleSetBackground(wxSTC_STYLE_DEFAULT, bgColorDisabled_);
        SetBackgroundColour(bgColorDisabled_);
        btn_dropdown_->SetBitmap(dropArrowInactive_);
    }

    return true;
}

void mmTagTextCtrl::OnPaint(wxPaintEvent& event)
{
    int end = textCtrl_->GetTextLength();
    int position = 0;

    // Reset the text style
    textCtrl_->ClearDocumentStyle();

    while (position < end) {
        // Find start and end of word
        int wordStart = textCtrl_->WordStartPosition(position, true);
        int wordEnd = textCtrl_->WordEndPosition(position, true);
        wxString word = textCtrl_->GetTextRange(wordStart, wordEnd);

        // If the word is a valid tag, color it
        if (auto it = tag_map_.find(word); it != tag_map_.end()) {
            textCtrl_->StartStyling(wordStart);
            textCtrl_->SetStyling(wordEnd - wordStart, 1);
        }

        position = wordEnd + 1;
    }

#ifndef __WXMAC__
    // paint a TextCtrl over the background -- not currently used on macOS due to dark mode bug
    wxPaintDC dc(this);
    wxRegion clipRegion(GetClientRect());
    if (btn_dropdown_->GetClientRect().Contains(btn_dropdown_->ScreenToClient(wxGetMousePosition()))) {
        wxRect btnRect(btn_dropdown_->GetRect());
        btnRect.y = 1;
        btnRect.height -= 2;
        btnRect.width -= 1;
        clipRegion.Subtract(btnRect); // Exclude the button face (minus top, right, and bottom borders)
    }
    dc.SetDeviceClippingRegion(clipRegion);
    wxRendererNative::Get().DrawTextCtrl(this, dc, GetClientRect(), textCtrl_->IsEnabled() ? wxCONTROL_NONE : wxCONTROL_DISABLED);
#endif

#ifdef __WXMSW__
    dc.DestroyClippingRegion();

    borderColor_ = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWFRAME);
    if (!textCtrl_->IsEnabled())
        borderColor_ = wxSystemSettings::GetColour(wxSYS_COLOUR_SCROLLBAR);
    else if (textCtrl_->HasFocus() || btn_dropdown_->HasFocus() || popupWindow_->IsShown())
        borderColor_ = wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT);
    else if (GetClientRect().Contains(ScreenToClient(wxGetMousePosition())))
        borderColor_ = *wxBLACK;

    dc.SetPen(borderColor_);

    wxRect rect(GetClientRect());

    wxPoint pt[5]{
        wxPoint(0, 0),
        wxPoint(rect.width - 1, 0),
        wxPoint(rect.width -1, rect.height - 1),
        wxPoint(0, rect.height - 1),
        wxPoint(0, 0)
    };

    dc.DrawLines(5, pt);

    if (!initialRefreshDone_) {
        btn_dropdown_->Refresh();
        initialRefreshDone_ = true;
    }

#endif

    event.Skip();
}

void mmTagTextCtrl::OnPaintButton(wxPaintEvent& )
{
    wxPaintDC dc(btn_dropdown_);

    wxRect rect = btn_dropdown_->GetClientRect();

    // Figure out what style the button needs
    long style = wxCONTROL_NONE;
    if (popupWindow_->IsShown())
        style = wxCONTROL_PRESSED;
    else if (btn_dropdown_->IsEnabled() && rect.Contains(btn_dropdown_->ScreenToClient(wxGetMousePosition())))
        style = wxCONTROL_CURRENT;

    if(style != wxCONTROL_NONE)
        wxRendererNative::Get().DrawComboBoxDropButton(this, dc, rect, style);
    else {
        // If we aren't interacting with the button, draw the drop arrow
        // directly on the texctrl like the native combobox
        dc.DrawBitmap(btn_dropdown_->IsEnabled() ? dropArrow_ : dropArrowInactive_, wxPoint(0, 0));
    }

    // Redraw the top, right, and bottom borders to match the window border
    if (style != wxCONTROL_CURRENT && style != wxCONTROL_PRESSED) {
        dc.SetPen(borderColor_);
        dc.DrawLine(rect.x, 0, rect.x + rect.width, 0);
        dc.DrawLine(rect.x + rect.width - 1, 0, rect.x + rect.width - 1, rect.height);
        dc.DrawLine(rect.x + rect.width - 1, rect.height - 1, rect.x, rect.height - 1);
    }
}

/* Validates all tags passed in tagText, or the contents of the text control if tagText is blank */
bool mmTagTextCtrl::ValidateTagText(const wxString& tagText)
{
    // Clear stored tags
    tags_.clear();

    int ip = textCtrl_->GetInsertionPoint();

    // If we are passed a string validate it, otherwise validate the text control contents
    wxString tags_in = tagText;
    if (tags_in.IsEmpty())
        tags_in = textCtrl_->GetText();

    if (tags_in.IsEmpty()) return true;

    textCtrl_->SetEvtHandlerEnabled(false);
    wxString tags_out;
    bool is_valid = true;
    // parse the tags and prompt to create any which don't exist
    for (const auto& tag : parseTags(tags_in)) {
        // ignore search operators
        if (tag == "&" || tag == "|") {
            tags_out.Append(tag + " ");
            continue;
        }

        if (tag_map_.find(tag) == tag_map_.end()) {
            // Prompt user to create a new tag
            if (wxMessageDialog(nullptr,
                wxString::Format(_t("Create new tag '%s'?"), tag),
                _t("New tag entered"),
                wxYES_NO
            ).ShowModal() == wxID_YES) {
                TagData new_tag_d = TagData();
                new_tag_d.m_name = tag;
                TagModel::instance().add_data_n(new_tag_d);
                // Save the new tag to reference
                tag_map_[tag] = new_tag_d.m_id;
                tagCheckListBox_->Append(tag);

                // Generate an event to tell the parent that a new tag has been added
                // This is necessary for dialogs that contain multiple tag controls (e.g. split transaction)
                // since the parent must reinitialize other tag controls to include the new tag
                wxListEvent evt(wxEVT_COMMAND_LIST_INSERT_ITEM);
                evt.SetId(GetId());
                GetEventHandler()->AddPendingEvent(evt);
            }
            else {
                is_valid = false;
                continue;
            }
        }

        tags_[tag] = tag_map_[tag];
        tags_out.Append(tag + " ");
    }

    // Replace tags with case-corrected versions and remove duplicates
    textCtrl_->SetText(tags_out);
    textCtrl_->GotoPos(textCtrl_->WordEndPosition(ip, true) + 1);
    textCtrl_->SetEvtHandlerEnabled(true);
    return is_valid;
}

/* Return a list of tag IDs contained in the control */
const wxArrayInt64 mmTagTextCtrl::GetTagIDs() const
{
    wxArrayInt64 tags_out;
    for (const auto& tag : tags_)
        tags_out.push_back(tag.second);

    return tags_out;
}

wxArrayString mmTagTextCtrl::parseTags(const wxString& tagString)
{
    wxStringTokenizer tokenizer = wxStringTokenizer(tagString, " ");
    wxArrayString tags;
    while (tokenizer.HasMoreTokens())
    {
        wxString token = tokenizer.GetNextToken();


        // ignore search operators
        if (token == "&" || token == "|")
        {
            if (operatorAllowed_) tags.push_back(token);
            continue;
        }

        bool tagUsed = false;
        // if the tag has already been entered, skip it to avoid duplicates
        for (const auto& tag : tags)
            if (tag.IsSameAs(token, false))
            {
                tagUsed = true;
                break;
            }

        if (tagUsed) continue;

        if (auto it = tag_map_.find(token); it != tag_map_.end())
            // case correction for existing tag
            tags.push_back((*it).first);
        else
            tags.push_back(token);
    }

    return tags;
}

void mmTagTextCtrl::SetTags(const wxArrayInt64& tagIds)
{
    // Save the tag IDs and tag names
    tags_.clear();
    for (const auto& tagId : tagIds)
        for (const auto& tag : tag_map_)
            if (tag.second == tagId)
            {
                tags_[tag.first] = tagId;
                break;
            }

    // Set the text of the control (sorted)
    wxString tagString;
    for (const auto& tag : tags_)
        tagString.Append(tag.first + " ");

    textCtrl_->SetText(tagString);
}
