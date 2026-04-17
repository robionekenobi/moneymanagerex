/*******************************************************
Copyright (C) 2014 Stefano Giorgio
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

#include "_PrefBase.h"

class GeneralPref : public PrefBase
{
    wxDECLARE_EVENT_TABLE();

// -- static

private:
    enum
    {
        ID_DIALOG_OPTIONS_BUTTON_CURRENCY = wxID_HIGHEST + 10,
        ID_DIALOG_OPTIONS_WXCHOICE_DATE,
        ID_DIALOG_OPTIONS_BUTTON_LANG,
        ID_DIALOG_OPTIONS_TEXTCTRL_USERNAME,
        ID_DIALOG_OPTIONS_LOCALE,
        ID_DIALOG_OPTIONS_FINANCIAL_YEAR_START_DAY,
        ID_DIALOG_OPTIONS_FINANCIAL_YEAR_START_MONTH,
        ID_DIALOG_OPTIONS_CURRENCY_HIST_DAYS
    };

// -- state

private:
    int64 m_currency_id = -1;
    wxString m_date_format;

    mmApp*              w_app                = nullptr;
    wxStaticBoxSizer*   w_currency_sizer     = nullptr;
    mmComboBoxCurrency* w_base_currency_text = nullptr;
    wxChoice*           w_date_format_choice = nullptr;
    wxChoice*           w_month_choice       = nullptr;
    wxComboBox*         w_locale_cb          = nullptr;
    wxStaticText*       w_date_label         = nullptr;
    wxStaticText*       w_amount_label       = nullptr;
    wxCheckBox*         w_uh_cb              = nullptr;

// -- constructor

public:
    GeneralPref();
    GeneralPref(
        wxWindow *parent,
        mmApp* app,
        wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL,
        const wxString &name = wxPanelNameStr
    );
    ~GeneralPref();

private:
    void create();

// -- virtual

public:
    virtual bool SaveSettings();

// -- methods

private:
    bool SaveFinancialYearStart();
    bool doFormatDoubleValue(const wxString& locale, wxString& result);

// -- event handlers

private:
    void OnDateFormatChanged( wxCommandEvent& event);
    void OnLocaleChanged(     wxCommandEvent& event);
    void OnChangeGUILanguage( wxCommandEvent& event);
    void OnMouseLeftDown(     wxCommandEvent& event);
};
