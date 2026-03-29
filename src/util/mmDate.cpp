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

#include "mmDate.h"
#include "mmCache.tpp"

template class mmCache<wxDateTime, wxString>;
template class mmCache<wxString, wxDateTime>;

// -- static

// Tolerance of half-day in date comparisons.
const wxTimeSpan mmDate::s_htol = wxTimeSpan::Hours(12);

mmCache<wxDateTime, wxString> mmDate::c_dateTime_isoDate =
    mmCache<wxDateTime, wxString>(mmDate::s_cache_cap);
mmCache<wxString, wxDateTime> mmDate::c_isoDate_dateTime =
    mmCache<wxString, wxDateTime>(mmDate::s_cache_cap);

// -- constructor

// isoDateTime can be an ISO datetime string of the form "YYYY-MM-DD'T'hh:mm:ss",
// or an ISO date string of the form "YYYY-MM-DD".
// wxString::BeforeFirst('T') returns the whole string if 'T' is not found.
mmDate::mmDate(const wxString& isoDateTime)
{
    wxString isoDate = isoDateTime.BeforeFirst('T');
    if (!isoDate.IsEmpty()) {
        m_isoDate = isoDate;
        c_dateTimeN = wxInvalidDateTime;
    }
    else {
        wxLogDebug("ERROR: mmDate::mmDate(): isoDate is empty");
        cache_dateTime(wxDateTime(12, 0, 0, 0));
    }
}

// isoDateTime can be an ISO datetime string of the form "YYYY-MM-DD'T'hh:mm:ss",
// or an ISO date string of the form "YYYY-MM-DD", or an empty string.
// wxString::BeforeFirst('T') returns the whole string if 'T' is not found.
mmDateN::mmDateN(const wxString& isoDateTimeN) :
    m_isoDateN(isoDateTimeN.BeforeFirst('T')),
    c_dateTimeN(wxInvalidDateTime)
{
}

mmDate::mmDate(wxDateTime dateTime)
{
    if (dateTime.IsValid()) {
        cache_dateTime(dateTime);
    }
    else {
        wxLogDebug("ERROR: mmDate::mmDate(): dateTime is invalid");
        cache_dateTime(wxDateTime(12, 0, 0, 0));
    }
}

mmDateN::mmDateN(wxDateTime dateTimeN)
{
    cache_dateTimeN(dateTimeN);
}

// -- methods

wxDateTime mmDate::cache_dateTime()
{
    const wxDateTime* dateTime_n = mmDate::c_isoDate_dateTime.get(m_isoDate);
    if (dateTime_n) {
        c_dateTimeN = *dateTime_n;
        return c_dateTimeN;
    }

    c_dateTimeN.ParseISOCombined(m_isoDate + "T12:00:00");
    if (c_dateTimeN.IsValid()) {
        mmDate::c_isoDate_dateTime.add(m_isoDate, c_dateTimeN);
    }
    else {
        wxLogDebug("ERROR: mmDate::cache_dateTime(): c_dateTimeN is invalid");
        c_dateTimeN = wxDateTime(12, 0, 0, 0);
    }

    return c_dateTimeN;
}

void mmDate::cache_dateTime(wxDateTime dateTime)
{
    if (dateTime.IsValid()) {
        c_dateTimeN = dateTime;
    }
    else {
        wxLogDebug("ERROR: mmDate::cache_dateTime(): dateTime is invalid");
        c_dateTimeN = wxDateTime(12, 0, 0, 0);
    }

    // Set time to noon (12:00:00).
    c_dateTimeN.SetHour(12).SetMinute(0).SetSecond(0).SetMillisecond(0);

    const wxString* isoDate_n = mmDate::c_dateTime_isoDate.get(c_dateTimeN);
    if (isoDate_n) {
        m_isoDate = *isoDate_n;
    }
    else {
        m_isoDate = c_dateTimeN.FormatISODate();
        mmDate::c_dateTime_isoDate.add(c_dateTimeN, m_isoDate);
    }
}

wxDateTime mmDateN::cache_dateTimeN()
{
    c_dateTimeN = has_value() ? value().cache_dateTime() : wxInvalidDateTime;
    return c_dateTimeN;
}

void mmDateN::cache_dateTimeN(wxDateTime dateTimeN)
{
    if (dateTimeN.IsValid()) {
        mmDate date = mmDate(dateTimeN);
        m_isoDateN = date.m_isoDate;
        c_dateTimeN = date.c_dateTimeN;
    }
    else {
        m_isoDateN = "";
        c_dateTimeN = wxInvalidDateTime;
    }
}

void mmDate::addDateSpan(wxDateSpan dateSpan)
{
    cache_dateTime(dateTime() + dateSpan);
}

void mmDate::subDateSpan(wxDateSpan dateSpan)
{
    cache_dateTime(dateTime() - dateSpan);
}

mmDate mmDate::plusDateSpan(wxDateSpan dateSpan)
{
    return mmDate(dateTime() + dateSpan);
}

mmDate mmDate::minusDateSpan(wxDateSpan dateSpan)
{
    return mmDate(dateTime() - dateSpan);
}

int mmDate::daysSince(mmDate& other)
{
    wxTimeSpan dt = dateTime().Subtract(other.dateTime()) + mmDate::s_htol;
    return dt.IsPositive() ? dt.GetDays() : dt.GetDays() - 1;
}

int mmDate::daysUntil(mmDate& other)
{
    wxTimeSpan dt = other.dateTime().Subtract(dateTime()) + mmDate::s_htol;
    return dt.IsPositive() ? dt.GetDays() : dt.GetDays() - 1;
}
