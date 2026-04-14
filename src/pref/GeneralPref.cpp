/*******************************************************
Copyright (C) 2014 Stefano Giorgio
Copyright (C) 2016, 2017, 2020 - 2022 Nikolay Akimov
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

#include "GeneralPref.h"

#include "base/_defs.h"
#include "base/_constants.h"
#include <wx/spinctrl.h>
#include <fmt/core.h>
#include <fmt/format.h>

#include "util/_util.h"
#include "model/CurrencyModel.h"
#include "dialog/CurrencyChoiceDialog.h"
#include "app/mmApp.h"

wxBEGIN_EVENT_TABLE(GeneralPref, wxPanel)
    EVT_CHOICE(ID_DIALOG_OPTIONS_WXCHOICE_DATE, GeneralPref::OnDateFormatChanged)
    EVT_COMBOBOX(ID_DIALOG_OPTIONS_LOCALE,      GeneralPref::OnLocaleChanged)
    EVT_MENU(wxID_ANY,                          GeneralPref::OnChangeGUILanguage)
    EVT_BUTTON(ID_DIALOG_OPTIONS_BUTTON_LANG,   GeneralPref::OnMouseLeftDown)
wxEND_EVENT_TABLE()

GeneralPref::GeneralPref()
{
}

GeneralPref::GeneralPref(
    wxWindow *parent,
    mmApp* app,
    wxWindowID id,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name
) :
    m_date_format(PrefModel::instance().getDateFormat()),
    w_app(app)
{
    wxPanel::Create(parent, id, pos, size, style, name);
    create();
}

GeneralPref::~GeneralPref()
{
}

void GeneralPref::create()
{
    wxBoxSizer* generalPanelSizer0 = new wxBoxSizer(wxVERTICAL);
    SetSizer(generalPanelSizer0);

    wxScrolledWindow* general_panel = new wxScrolledWindow(this, wxID_ANY);

    wxBoxSizer* generalPanelSizer = new wxBoxSizer(wxVERTICAL);

    general_panel->SetSizer(generalPanelSizer);
    generalPanelSizer0->Add(general_panel, wxSizerFlags(g_flagsExpand).Proportion(0));

    // Display Header Settings
    wxStaticBox* headerStaticBox = new wxStaticBox(general_panel, wxID_STATIC,
        _t("Display Heading")
    );

    wxStaticBoxSizer* headerStaticBoxSizer = new wxStaticBoxSizer(headerStaticBox, wxHORIZONTAL);

    headerStaticBoxSizer->Add(
        new wxStaticText(headerStaticBox, wxID_STATIC, _t("User Name")),
        g_flagsH
    );

    wxString userName = InfoModel::instance().getString("USERNAME", "");
    wxTextCtrl* userNameTextCtr = new wxTextCtrl(headerStaticBox,
        ID_DIALOG_OPTIONS_TEXTCTRL_USERNAME,
        userName
    );
    userNameTextCtr->SetMinSize(wxSize(200, -1));
    mmToolTip(userNameTextCtr, _t("The User Name is used as a title for the database."));
    headerStaticBoxSizer->Add(userNameTextCtr, g_flagsExpand);
    generalPanelSizer->Add(headerStaticBoxSizer, wxSizerFlags(g_flagsExpand).Proportion(0));

    // User Interface Language
    auto language = PrefModel::instance().getLanguageID(true);
    const auto langName = (language == wxLANGUAGE_DEFAULT)
        ? _t("System default")
        : wxLocale::GetLanguageName(language);

    wxStaticBox* langStaticBox = new wxStaticBox(general_panel, wxID_STATIC,
        _t("User Interface Language")
    );

    wxStaticBoxSizer* langFormatStaticBoxSizer = new wxStaticBoxSizer(langStaticBox, wxHORIZONTAL);
    generalPanelSizer->Add(langFormatStaticBoxSizer, wxSizerFlags(g_flagsExpand).Proportion(0));

    wxButton* langButton = new wxButton(langStaticBox,
        ID_DIALOG_OPTIONS_BUTTON_LANG,
        wxGetTranslation(langName)
    );
    langButton->SetMinSize(wxSize(200, -1));
    langFormatStaticBoxSizer->Add(langButton, g_flagsH);
    mmToolTip(langButton, _t("Change user interface language"));

    // Date Format Settings
    wxStaticBox* dateFormatStaticBox = new wxStaticBox(general_panel, wxID_STATIC,
        _t("Date Format")
    );
    wxStaticBoxSizer* dateFormatStaticBoxSizer = new wxStaticBoxSizer(dateFormatStaticBox, wxHORIZONTAL);
    generalPanelSizer->Add(dateFormatStaticBoxSizer, wxSizerFlags(g_flagsExpand).Proportion(0));

    w_date_format_choice = new wxChoice(dateFormatStaticBox, ID_DIALOG_OPTIONS_WXCHOICE_DATE);
    for (const auto& i : g_date_formats_map()) {
        w_date_format_choice->Append(i.second, new wxStringClientData(i.first));
        if (m_date_format == i.first) w_date_format_choice->SetStringSelection(i.second);
    }
    dateFormatStaticBoxSizer->Add(w_date_format_choice, g_flagsH);
    mmToolTip(w_date_format_choice, _t("Specify the date format for display"));

    w_date_label = new wxStaticText(dateFormatStaticBox, wxID_STATIC, "redefined elsewhere");
    dateFormatStaticBoxSizer->Add(
        new wxStaticText(dateFormatStaticBox, wxID_STATIC, _t("Date format sample:")),
        wxSizerFlags(g_flagsH).Border(wxLEFT, 15)
    );
    dateFormatStaticBoxSizer->Add(w_date_label, wxSizerFlags(g_flagsH).Border(wxLEFT, 5));
    w_date_label->SetLabelText(mmGetDateTimeForDisplay(wxDateTime::Now().FormatISODate()));

    // Currency Settings
    wxStaticBox* currencyStaticBox = new wxStaticBox(general_panel, wxID_STATIC,
        _t("Currency")
    );

    w_currency_sizer = new wxStaticBoxSizer(currencyStaticBox, wxVERTICAL);
    generalPanelSizer->Add(w_currency_sizer, wxSizerFlags(g_flagsExpand).Proportion(0));

    wxBoxSizer* currencyBaseSizer = new wxBoxSizer(wxHORIZONTAL);
    w_currency_sizer->Add(currencyBaseSizer, wxSizerFlags(g_flagsV).Border(wxLEFT, 0));
    currencyBaseSizer->Add(
        new wxStaticText(currencyStaticBox, wxID_STATIC, _t("Base Currency")),
        g_flagsH
    );

    const CurrencyData* currency = CurrencyModel::instance().get_id_data_n(
        PrefModel::instance().getBaseCurrencyID()
    );
    wxString currName = currency ? currency->m_name : _t("Set Currency");
    m_currency_id = currency ? currency->m_id : -1;
    w_base_currency_text = new mmComboBoxCurrency(currencyStaticBox,
        ID_DIALOG_OPTIONS_BUTTON_CURRENCY
    );
    w_base_currency_text->SetMinSize(wxSize(200, -1));
    w_base_currency_text->ChangeValue(currName);
    mmToolTip(w_base_currency_text,
        _t("Set default database currency using 'Currency Manager'")
    );
    currencyBaseSizer->Add(w_base_currency_text, g_flagsH);

    w_currency_sizer->AddSpacer(10);

    { // Locale
        const wxString locale = InfoModel::instance().getString("LOCALE", "");

        wxBoxSizer* localeBaseSizer = new wxBoxSizer(wxHORIZONTAL);
        w_currency_sizer->Add(localeBaseSizer, wxSizerFlags(g_flagsV).Border(wxLEFT, 0));

        w_locale_cb = new wxComboBox(currencyStaticBox, ID_DIALOG_OPTIONS_LOCALE,
            "",
            wxDefaultPosition, wxDefaultSize,
            g_locales()
        );
        w_locale_cb->SetValue(locale);
        w_locale_cb->AutoComplete(g_locales());
        w_locale_cb->SetMinSize(wxSize(100, -1));
        localeBaseSizer->Add(w_locale_cb, g_flagsH);

        w_amount_label = new wxStaticText(currencyStaticBox, wxID_STATIC,
            "redefined elsewhere"
        );
        localeBaseSizer->Add(w_amount_label, wxSizerFlags(g_flagsH).Border(wxLEFT, 15));
        wxString result;
        doFormatDoubleValue(locale, result);
        w_amount_label->SetLabelText(wxGetTranslation(result));

        w_currency_sizer->Add(new wxStaticText(currencyStaticBox, wxID_STATIC,
            _t("Format derived from locale.\n"
                "Leave blank to manually set format via 'Currency Manager | Edit'")),
            wxSizerFlags(g_flagsV).Border(wxTOP, 0).Border(wxLEFT, 5)
        );

        w_locale_cb->Connect(ID_DIALOG_OPTIONS_LOCALE, wxEVT_COMMAND_TEXT_UPDATED,
            wxCommandEventHandler(GeneralPref::OnLocaleChanged),
            nullptr, this
        );
        w_locale_cb->SetMinSize(wxSize(100, -1));
    }

    w_currency_sizer->AddSpacer(15);
    { // Currency History Details
        wxBoxSizer* currencyBaseSizer2 = new wxBoxSizer(wxHORIZONTAL);
        m_currencyStaticBoxSizer->Add(currencyBaseSizer2, wxSizerFlags(g_flagsV).Border(wxLEFT, 0));

        w_uh_cb = new wxCheckBox(currencyStaticBox, wxID_ANY,
            _t("Use historical currency"),
            wxDefaultPosition, wxDefaultSize,
            wxCHK_2STATE
        );
        w_uh_cb->SetValue(PrefModel::instance().getUseCurrencyHistory());
        mmToolTip(w_uh_cb,
            _t("Select to use historical currency (one rate for each day), deselect to use a fixed rate")
        );
        currencyBaseSizer2->Add(w_uh_cb, g_flagsV);

        currencyBaseSizer2->Add(new wxStaticText(currencyStaticBox, wxID_STATIC, _("Days")), g_flagsH);

        int days = PrefModel::instance().getCurrencyHistoryDays();
        wxSpinCtrl* textHistDay = new wxSpinCtrl(currencyStaticBox, ID_DIALOG_OPTIONS_CURRENCY_HIST_DAYS,
            wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 90, 99999, days);
        textHistDay->SetValue(days);
        mmToolTip(textHistDay, _("Specify number of Days for historic currency data"));
        currencyBaseSizer2->Add(textHistDay, g_flagsH);
    }
    // Financial Year Settings
    wxStaticBox* financialYearStaticBox = new wxStaticBox(general_panel, wxID_ANY,
        _t("Financial Year")
    );

    wxStaticBoxSizer* financialYearStaticBoxSizer = new wxStaticBoxSizer(financialYearStaticBox, wxVERTICAL);
    wxFlexGridSizer* financialYearStaticBoxSizerGrid = new wxFlexGridSizer(0, 2, 0, 0);
    generalPanelSizer->Add(financialYearStaticBoxSizer, wxSizerFlags(g_flagsExpand).Proportion(0));
    financialYearStaticBoxSizer->Add(financialYearStaticBoxSizerGrid);

    financialYearStaticBoxSizerGrid->Add(
        new wxStaticText(financialYearStaticBox, wxID_STATIC, _t("First Day")),
        g_flagsH
    );
    int day = InfoModel::instance().getInt("FINANCIAL_YEAR_START_DAY", 1);

    wxSpinCtrl *textFPSDay = new wxSpinCtrl(financialYearStaticBox,
        ID_DIALOG_OPTIONS_FINANCIAL_YEAR_START_DAY,
        wxEmptyString, wxDefaultPosition, wxDefaultSize,
        wxSP_ARROW_KEYS,
        1, 31, day
    );
    textFPSDay->SetValue(day);
    mmToolTip(textFPSDay, _t("Specify Day for start of financial year"));

    financialYearStaticBoxSizerGrid->Add(textFPSDay, g_flagsH);

    financialYearStaticBoxSizerGrid->Add(
        new wxStaticText(financialYearStaticBox, wxID_STATIC, _t("First Month")),
        g_flagsH
    );

    wxArrayString financialMonthsSelection;
    for (wxDateTime::Month m = wxDateTime::Jan; m <= wxDateTime::Dec; m = wxDateTime::Month(m + 1))
        financialMonthsSelection.Add(wxGetTranslation(
            wxDateTime::GetEnglishMonthName(m, wxDateTime::Name_Abbr)
        ));

    w_month_choice = new wxChoice(financialYearStaticBox,
        ID_DIALOG_OPTIONS_FINANCIAL_YEAR_START_MONTH,
        wxDefaultPosition, wxSize(100, -1),
        financialMonthsSelection
    );
    financialYearStaticBoxSizerGrid->Add(w_month_choice, g_flagsH);

    int monthItem = InfoModel::instance().getInt("FINANCIAL_YEAR_START_MONTH", 7);
    w_month_choice->SetSelection(monthItem - 1);
    mmToolTip(w_month_choice, _t("Specify month for start of financial year"));

    SetBoldFontToStaticBoxHeader(dateFormatStaticBox);
    SetBoldFontToStaticBoxHeader(langStaticBox);
    SetBoldFontToStaticBoxHeader(headerStaticBox);
    SetBoldFontToStaticBoxHeader(currencyStaticBox);
    SetBoldFontToStaticBoxHeader(financialYearStaticBox);

    Fit();
    general_panel->SetMinSize(general_panel->GetBestVirtualSize());
    general_panel->SetScrollRate(6, 6);
}

void GeneralPref::OnDateFormatChanged(wxCommandEvent& /*event*/)
{
    wxStringClientData* data = static_cast<wxStringClientData*>(
        w_date_format_choice->GetClientObject(w_date_format_choice->GetSelection())
    );
    if (data) {
        m_date_format = data->GetData();
        w_date_label->SetLabelText(
            mmGetDateTimeForDisplay(wxDateTime::Now().FormatISODate(), m_date_format)
        );
    }
}

void GeneralPref::OnLocaleChanged(wxCommandEvent& /*event*/)
{
    wxComboBox* cbln = static_cast<wxComboBox*>(FindWindow(ID_DIALOG_OPTIONS_LOCALE));
    const wxString locale = cbln->GetValue();
    wxString result;

    doFormatDoubleValue(locale, result);
    w_amount_label->SetLabelText(wxGetTranslation(result));
}

bool GeneralPref::SaveFinancialYearStart()
{
    //Save Financial Year Start Month
    wxDateTime::Month month = wxDateTime::Month(w_month_choice->GetSelection());
    PrefModel::instance().saveFinancialFirstMonth(month);
    int last_month_day = wxDateTime(1, month, 2015).GetLastMonthDay().GetDay();

    //Save Financial Year Start Day
    wxSpinCtrl* fysDay = static_cast<wxSpinCtrl*>(
        FindWindow(ID_DIALOG_OPTIONS_FINANCIAL_YEAR_START_DAY)
    );
    int day = fysDay->GetValue();
    if (last_month_day < day)
        day = last_month_day;

    PrefModel::instance().saveFinancialFirstDay(day);
    return last_month_day < day;
}

bool GeneralPref::SaveSettings()
{
    int64 baseCurrencyOLD = PrefModel::instance().getBaseCurrencyID();
    int64 currency_id = w_base_currency_text->mmGetId();
    if (currency_id != baseCurrencyOLD) {
        if (!w_base_currency_text->mmIsValid()) {
            mmErrorDialogs::ToolTip4Object(w_base_currency_text,
                _t("Invalid value"),
                _t("Currency"),
                wxICON_ERROR
            );
            return false;
        }
        m_currency_id = currency_id;

        if (PrefModel::instance().getUseCurrencyHistory()) {
            if (wxMessageBox(
                _t("Changing base currency will delete all historical rates, proceed?"),
                _t("Currency Manager"),
                wxYES_NO | wxYES_DEFAULT | wxICON_WARNING
            ) != wxYES)
                return false;
        }

        PrefModel::instance().saveBaseCurrencyID(m_currency_id);
    }

    wxTextCtrl* stun = static_cast<wxTextCtrl*>(
        FindWindow(ID_DIALOG_OPTIONS_TEXTCTRL_USERNAME)
    );
    PrefModel::instance().saveUserName(stun->GetValue());

    wxComboBox* cbln = static_cast<wxComboBox*>(FindWindow(ID_DIALOG_OPTIONS_LOCALE));
    wxString value;
    if (doFormatDoubleValue(cbln->GetValue(), value)) {
        PrefModel::instance().saveLocaleName(cbln->GetValue());
    }
    else {
        mmErrorDialogs::ToolTip4Object(w_locale_cb,
            _t("Invalid value"),
            _t("Locale"),
            wxICON_ERROR
        );
        return false;
    }

    PrefModel::instance().saveUseCurrencyHistory(w_uh_cb->GetValue());
    //Save History Days
    wxSpinCtrl* hisDay = static_cast<wxSpinCtrl*>(FindWindow(ID_DIALOG_OPTIONS_CURRENCY_HIST_DAYS));
    int day = hisDay->GetValue();
    if (day < 90)
        day = 90;
    PrefModel::instance().saveCurrencyHistoryDays(day);

    PrefModel::instance().saveDateFormat(m_date_format);
    SaveFinancialYearStart();

    return true;
}

bool GeneralPref::doFormatDoubleValue(const wxString& locale, wxString& result)
{
    if (locale.empty()) {
        result = "";
        return true;
    }

    try {
        auto test = fmt::format(std::locale(locale.c_str()), "{:.2Lf}", 1234567.89);
        for (auto &i : test) {
            if (i < 0) i = ' ';
        }
        result = wxString::Format(_t("Currency value sample: %s"), test);
    }
    catch (std::exception & ex) {
        result = wxString(ex.what());
        if (result.Contains("locale name not valid")) {
            result = wxTRANSLATE("Bad locale name");
        }
        return false;
    }

    return true;
}

void GeneralPref::OnChangeGUILanguage(wxCommandEvent& event)
{
    wxLanguage lang = static_cast<wxLanguage>(event.GetId() - wxID_LAST - 1);
    if (lang != w_app->getGUILanguage() && w_app->setGUILanguage(lang))
        mmErrorDialogs::MessageWarning(this,
            _t("The language for this application has been changed. "
                "The change will take effect the next time the application is started."),
            _t("Language change")
        );
}

void GeneralPref::OnMouseLeftDown(wxCommandEvent& event)
{
    wxMenu menuLang;
    for (auto const& lang : g_translations()) {
        int            lang_id    = std::get<0>(lang);
        const wxString lang_label = std::get<1>(lang);
        const wxString lang_help  = std::get<2>(lang);
        menuLang.AppendRadioItem(
            wxID_LAST + 1 + lang_id, lang_label, lang_help
        )->Check(
            lang_id == w_app->getGUILanguage()
        );
    }
    PopupMenu(&menuLang);

    event.Skip();
}
