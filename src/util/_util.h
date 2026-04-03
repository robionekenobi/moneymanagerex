/*******************************************************
 Copyright (C) 2006 Madhan Kanagavel
 Copyright (C) 2013-2022 Nikolay Akimov
 Copyright (C) 2021,2024 Mark Whalley (mark@ipx.co.uk)
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
#include <algorithm>
#include <map>
#include <curl/curl.h>
#include <wx/clipbrd.h>
#include <wx/valnum.h>
#include <rapidjson/document.h>

#include "_primitive.h"
#include "model/PrefModel.h"

class mmGUIApp;

bool isDark(wxColour c);
void mmThemeAutoColour(wxWindow* object, bool recursive = true);
void enableMSWDarkMode(wxWindow* object, bool darkMode);
wxString JSON_PrettyFormated(rapidjson::Document& j_doc);
wxString JSON_Formated(rapidjson::Document& j_doc);
rapidjson::Value* JSON_GetValue(rapidjson::Document& j_doc, const rapidjson::MemoryStream::Ch* name);
bool JSON_GetBoolValue(rapidjson::Document& j_doc, const rapidjson::MemoryStream::Ch* name, bool& value);
bool JSON_GetIntValue(rapidjson::Document& j_doc, const rapidjson::MemoryStream::Ch* name, int& value);
bool JSON_GetStringValue(rapidjson::Document& j_doc, const rapidjson::MemoryStream::Ch* name, wxString& value);

struct ValuePair
{
    wxString label;
    double amount;
};

struct ValueTrio
{
    wxString color;
    wxString label;
    double amount;
};

struct WebsiteNews
{
    mmDateN m_dateN;
    wxString m_titleN;
    wxString m_linkN;
    wxString m_descriptionN;
};

//----------------------------------------------------------------------------

const wxString inQuotes(const wxString& label, const wxString& delimiter);
wxString removeQuotes(wxString s);
void csv2tab_separated_values(wxString& line, const wxString& delimit);
void correctEmptyFileExt(const wxString& ext, wxString & fileName );

void mmLoadColorsFromDatabase(const bool def = false);
wxString formatHTML(wxString& html);

inline wxString getTranslation(bool translate, const wxString& text)
{
    return translate ? wxGetTranslation(text) : text;
}

//----------------------------------------------------------------------------

bool getNewsRSS(std::vector<WebsiteNews>& websiteNews_a);
enum yahoo_price_type { FIAT = 0, SHARES };
bool getOnlineCurrencyRates(wxString& msg, const int64 curr_id = -1, const bool used_only = true);
bool get_yahoo_prices(
    std::map<wxString, double>& symbols,
    std::map<wxString, double>& out,
    const wxString& base_currency_symbol,
    wxString& output,
    int type
);
bool getCoincapInfoFromSymbol(const wxString& symbol, wxString& out_id, double& price_usd, wxString& output);
bool getCoincapAssetHistory(
    const wxString& asset_id, mmDate begin_date,
    std::map<mmDate, double>& date_rate_m, wxString& msg
);

wxString cleanseNumberString(const wxString& str, const bool decimal);
double cleanseNumberStringToDouble(const wxString& str, const bool decimal);
const wxString mmPlatformType();

//All components version in TXT, HTML, ABOUT
const wxString getProgramDescription(const int type = 0);
void DoWindowsFreezeThaw(wxWindow* w);
const wxString md2html(const wxString& md);
const wxString getVFname4print(const wxString& name, const wxString& data);
void clearVFprintedFiles(const wxString& name);
const wxRect GetDefaultMonitorRect();

//* Date Functions----------------------------------------------------------*//

const wxDateTime getUserDefinedFinancialYear(bool prevDayRequired = false);
const std::map<wxString, wxString>& date_formats_regex();
const wxString mmGetDateTimeForDisplay(
    const wxString& datetime_iso,
    const wxString& format = PrefModel::instance().getDateFormat()
);
const wxString mmGetDateForDisplay(
    const wxString& datetime_iso,
    const wxString& format = PrefModel::instance().getDateFormat()
);
const wxString mmGetTimeForDisplay(const wxString& datetime_iso);
bool mmParseDisplayStringToDate(
    wxDateTime& date,
    const wxString& sDate,
    const wxString& sDateMask
);
extern const std::map<int, std::pair<wxConvAuto, wxString>> g_encoding;

//----------------------------------------------------------------------------

CURLcode http_get_data(const wxString& site, wxString& output, const wxString& useragent = wxEmptyString);
CURLcode http_post_data(const wxString& site, const wxString& data, const wxString& contentType, wxString& output);
CURLcode http_download_file(const wxString& site, const wxString& path);
CURLcode getYahooFinanceQuotes(const wxString& URL, wxString& json_data);

//----------------------------------------------------------------------------

extern const std::vector<std::pair<wxString, wxString>> g_date_formats_map();
class mmDateParser
{
private:
    static constexpr int s_max_attempts = 3;

private:
    // initialized by constructor
    wxDateTime m_today;
    wxDateTime m_month_ago;

    // initialized by constructor; updated by doHandleStatistics()
    // format : date format, like %d/%m/%Y
    // mask   : human readable date format, like DD/MM/YYYY
    std::vector<std::pair<wxString, wxString>> m_format_mask_a; // (format, mask)
    std::map<wxString, int> m_format_stat_m;                    // (format, statistics)

    // set by doFinalizeStatistics()
    wxString m_max_format; // max statistics
    wxString m_max_mask;   // max statistics

    int m_error_count = 0;

public:
    mmDateParser();
    ~mmDateParser();

    auto getDateFormat() const -> const wxString { return m_max_format; }
    auto getDateMask() const -> const wxString { return m_max_mask; }
    int  getErrorCount() const { return m_error_count; }
    bool isDateFormatFound() const {
        return m_format_mask_a.size() < g_date_formats_map().size();
    }
    void doHandleStatistics(const wxString& dateStr);
    void doFinalizeStatistics();
};

// used where differences occur between platforms
wxImageList* createImageList(const int size = 0);

void mmToolTip(wxWindow* widget,const wxString& tip);

// escape HTML characters
wxString HTMLEncode(const wxString& input);

void mmSetSize(wxWindow* w);
void mmFontSize(wxWindow* widget);

wxChar ExtractHotkeyChar(const wxString& input, wxChar defaultChar = '\0');
