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

#include <map>
#include "base/_defs.h"
#include "_util.h"

class mmDateParser
{
// -- static

private:
    static constexpr int s_max_attempts = 3;

// -- state

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
    auto getDateFormat() const -> const wxString { return m_max_format; }
    auto getDateMask() const -> const wxString { return m_max_mask; }
    int  getErrorCount() const { return m_error_count; }

// -- constructor

public:
    mmDateParser();
    ~mmDateParser();

// -- method

public:
    void doHandleStatistics(const wxString& dateStr);
    void doFinalizeStatistics();
    bool isDateFormatFound() const {
        return m_format_mask_a.size() < g_date_formats_map().size();
    }
};

