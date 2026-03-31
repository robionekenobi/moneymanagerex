/*******************************************************
 Copyright (C) 2026 George Ef (george.a.ef@gmail.com)

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

#include "_Repeat.h"

// default: manual mode, repeat once
Repeat::Repeat() :
    m_mode(RepeatMode()), m_freq(RepeatFreq()), m_num(1), m_x(-1)
{
}

Repeat::Repeat(RepeatMode mode, RepeatFreq freq, int num) :
    m_mode(mode), m_freq(freq), m_num(-1), m_x(-1)
{
    if (m_freq.is_once()) {
        m_num = 1;
        m_x   = -1;
    }
    else if (!m_freq.has_x() && (num > 0 || num == -1)) {
        m_num = num;
        m_x   = -1;
    }
    else if (m_freq.is_in_x() && num > 0) {
        m_num = 2;
        m_x   = num;
    }
    else if (m_freq.is_every_x() && num > 0) {
        m_num = -1;
        m_x   = num;
    }
    else {
        // default values in case of error
        m_mode = RepeatMode();
        m_freq = RepeatFreq();
        m_num  = 1;
        m_x    = -1;
    }
}

Repeat Repeat::from_row(int64 row_REPEATS, int64 row_NUMOCCURRENCES)
{
    int repeats = static_cast<int>(row_REPEATS.GetValue());
    int num     = static_cast<int>(row_NUMOCCURRENCES.GetValue());

    return Repeat(
        RepeatMode::from_code(repeats / Repeat::encoding_base),
        RepeatFreq::from_code(repeats % Repeat::encoding_base),
        num
    );
}

void Repeat::to_row(int64& row_REPEATS, int64& row_NUMOCCURRENCES) const
{
    int repeats = m_mode.code() * Repeat::encoding_base + m_freq.code();
    int num     = m_freq.has_x() ? m_x : m_num;

    row_REPEATS        = static_cast<int64>(repeats);
    row_NUMOCCURRENCES = static_cast<int64>(num);
}

mmDate Repeat::next_date(mmDate& date, bool reverse)
{
    int k = reverse ? -1 : 1;

    wxDateTime d = date.dateTime();
    if (m_freq.id() == RepeatFreq::e_1_day)
        d.Add(wxDateSpan::Days(1 * k));
    else if (m_freq.id() == RepeatFreq::e_1_week)
        d.Add(wxTimeSpan::Weeks(1 * k));
    else if (m_freq.id() == RepeatFreq::e_2_weeks)
        d.Add(wxTimeSpan::Weeks(2 * k));
    else if (m_freq.id() == RepeatFreq::e_4_weeks)
        d.Add(wxDateSpan::Weeks(4 * k));
    else if (m_freq.id() == RepeatFreq::e_1_month)
        d.Add(wxDateSpan::Months(1 * k));
    else if (m_freq.id() == RepeatFreq::e_month_last_day ||
             m_freq.id() == RepeatFreq::e_month_last_business_day)
    {
        d.Add(wxDateSpan::Months(k));
        d.SetToLastMonthDay(d.GetMonth(), d.GetYear());
        if (m_freq.id() == RepeatFreq::e_month_last_business_day) {
            // last weekday of month
            if (d.GetWeekDay() == wxDateTime::Sun ||
                d.GetWeekDay() == wxDateTime::Sat
            )
                d.SetToPrevWeekDay(wxDateTime::Fri);
        }
    }
    else if (m_freq.id() == RepeatFreq::e_2_months)
        d.Add(wxDateSpan::Months(2 * k));
    else if (m_freq.id() == RepeatFreq::e_3_months)
        d.Add(wxDateSpan::Months(3 * k));
    else if (m_freq.id() == RepeatFreq::e_4_months)
        d.Add(wxDateSpan::Months(4 * k));
    else if (m_freq.id() == RepeatFreq::e_6_months)
        d.Add(wxDateSpan::Months(6 * k));
    else if (m_freq.id() == RepeatFreq::e_1_year)
        d.Add(wxDateSpan::Years(1 * k));
    else if (m_freq.id() == RepeatFreq::e_in_x_days)
        d.Add(wxDateSpan::Days(m_x * k));
    else if (m_freq.id() == RepeatFreq::e_in_x_months)
        d.Add(wxDateSpan::Months(m_x * k));
    else if (m_freq.id() == RepeatFreq::e_every_x_days)
        d.Add(wxDateSpan::Days(m_x * k));
    else if (m_freq.id() == RepeatFreq::e_every_x_months)
        d.Add(wxDateSpan::Months(m_x * k));

    mmDate next_date = mmDate(d);

    if (0) wxLogDebug("init date: %s -> next date: %s",
        date.isoDate(), next_date.isoDate()
    );
    return next_date;
}

void Repeat::next_repeat() {
    if (m_num > 0)
        m_num -= 1;

    if (m_freq.is_in_x() && m_num == 1) {
        m_freq = RepeatFreq(RepeatFreq::e_once);
        m_x = -1;
    }
}
