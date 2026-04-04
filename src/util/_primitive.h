/*******************************************************
 Copyright (C) 2006 Madhan Kanagavel
 Copyright (C) 2013-2022 Nikolay Akimov
 Copyright (C) 2021,2024 Mark Whalley (mark@ipx.co.uk)
 Copyright (C) 2025 George Ef (george.a.ef@gmail.com)

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
#include "base/_types.h"

// -- String

struct caseInsensitiveComparator {
    bool operator()(const wxString& lhs, const wxString& rhs) const {
        return lhs.CmpNoCase(rhs) < 0;
    }
};

int CaseInsensitiveCmp(const wxString &s1, const wxString &s2);
int CaseInsensitiveLocaleCmp(const wxString &s1, const wxString &s2);

const wxString mmTrimAmount(
    const wxString& value, const wxString& decimal, const wxString& replace_decimal = ""
);

// -- URI

bool isValidURI(const wxString& validate);

// -- DateTime

extern const wxString MONTHS[12];
extern const wxString MONTHS_SHORT[12];
extern const wxString g_days_of_week[7];
extern const wxString g_short_days_of_week[7];

inline const wxString mmGetMonthName(const wxDateTime::Month& month)
{
    return MONTHS[static_cast<int>(month - wxDateTime::Month::Jan)];
}

bool mmParseISODate(const wxString& in_str, wxDateTime& out_date);

wxDateTime DEPRECATED_parseDateTime(const wxString& str_date);

// -- Color

class mmColors
{
public:
    static wxColour userDefColor1;
    static wxColour userDefColor2;
    static wxColour userDefColor3;
    static wxColour userDefColor4;
    static wxColour userDefColor5;
    static wxColour userDefColor6;
    static wxColour userDefColor7;
};

const wxColor* bestFontColour(const wxColour& background);

wxColour getUDColour(const int c);

