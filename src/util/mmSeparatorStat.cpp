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

#include "mmSeparatorStat.h"
#include "base/_constants.h"
#include "model/InfoModel.h"

mmSeparatorStat::mmSeparatorStat()
{
    const wxString def_delim = InfoModel::instance().getString(
        "DELIMITER", mmex::DEFDELIMTER
    );

    m_sep_stat_m[";"] = 0;
    m_sep_stat_m[","] = 0;
    m_sep_stat_m["\t"] = 0;
    m_sep_stat_m["|"] = 0;
    m_sep_stat_m[def_delim] = 0;
}

mmSeparatorStat::~mmSeparatorStat()
{
}

bool mmSeparatorStat::search(const wxString& string)
{
    bool result = false;

    for (const auto& sep_stat : m_sep_stat_m) {
        bool skip = false;
        for (const auto& letter : string) {
            if (letter == '"') {
                skip = !skip;
            }
            if (!skip && letter == sep_stat.first) {
                m_sep_stat_m[sep_stat.first]++;
                result = true;
            }
        }
    }
    return result;
}

const wxString mmSeparatorStat::max() const
{
    auto x = std::max_element(m_sep_stat_m.begin(), m_sep_stat_m.end(),
        [](const std::pair<wxString, int>& p1, const std::pair<wxString, int>& p2) {
            return p1.second < p2.second;
        }
    );
    return x->first;
}
