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

#include "mmDateParser.h"

mmDateParser::mmDateParser() :
    m_today(wxDateTime::Today()),
    m_month_ago(wxDateTime::Today().Subtract(wxDateSpan::Months(1))),
    m_format_mask_a(g_date_formats_map())
{
    m_format_stat_m.clear();
}

mmDateParser::~mmDateParser()
{
}

void mmDateParser::doFinalizeStatistics()
{
    auto result = std::max_element(
        m_format_stat_m.begin(),
        m_format_stat_m.end(),
        [](const std::pair<wxString, int>& p1, const std::pair<wxString, int>& p2) {
            return p1.second < p2.second;
        }
    );

    if (result != m_format_stat_m.end()) {
        m_max_format = result->first;
        const auto& format_mask_a = g_date_formats_map();
        wxString format = m_max_format;
        const auto it = std::find_if(format_mask_a.begin(), format_mask_a.end(),
            [&format](const std::pair<wxString, wxString>& format_mask) {
                return format_mask.first == format;
            }
        );
        m_max_mask = it->second;
    }
    else
        wxLogDebug("No date string has been handled");
}

void mmDateParser::doHandleStatistics(const wxString& date_s)
{
    if (m_error_count > s_max_attempts || m_format_mask_a.size() <= 1)
        return;

    wxArrayString invalidFormat_a;
    std::vector<std::pair<wxString, wxString>> format_mask_a = m_format_mask_a;
    for (const auto& format_mask : format_mask_a) {
        const wxString format = format_mask.first;
        wxDateTime dateTime = m_today;
        if (mmParseDisplayStringToDate(dateTime, date_s, format)) {
            // Increase the date format rating if parsed date is recent
            // Decrease the data format rating if parsed date is in future
            m_format_stat_m[format] +=
                (dateTime < m_month_ago) ? 1 :
                (dateTime <= m_today)    ? 2 :
                -1;
        }
        else {
            invalidFormat_a.Add(format);
            m_format_stat_m.erase(format);
        }
    }

    if (invalidFormat_a.size() < m_format_mask_a.size()) {
        for (const wxString& invalidFormat : invalidFormat_a) {
            auto it = std::find_if(m_format_mask_a.begin(), m_format_mask_a.end(),
                [&invalidFormat](const std::pair<wxString, wxString>& format_mask) {
                    return format_mask.first == invalidFormat;
                }
            );
            m_format_mask_a.erase(it);
        }
    }
    else {
        m_error_count++;
    }
}

