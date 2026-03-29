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

#include "mmDateTime.h"

// -- constructor

// isoDateTime can be an ISO datetime string of the form "YYYY-MM-DD'T'hh:mm:ss",
// or an ISO date string of the form "YYYY-MM-DD".
// wxString::AfterFirst('T') returns an empty string if 'T' is not found.
mmDateTime::mmDateTime(const wxString& isoDateTime) :
    m_date(isoDateTime),
    c_dateTimeN(wxInvalidDateTime)
{
    wxString isoTimeN = isoDateTime.AfterFirst('T');
    m_isoTime = !isoTimeN.IsEmpty() ? isoTimeN : "12:00:00";
}

// isoDateTime can be an ISO datetime string of the form "YYYY-MM-DD'T'hh:mm:ss",
// or an ISO date string of the form "YYYY-MM-DD", or an empty string.
// wxString::AfterFirst('T') returns an empty string if 'T' is not found.
mmDateTimeN::mmDateTimeN(const wxString& isoDateTimeN) :
    m_dateN(mmDateN(isoDateTimeN)),
    c_dateTimeN(wxInvalidDateTime)
{
    if (m_dateN.has_value()) {
        wxString isoTimeN = isoDateTimeN.AfterFirst('T');
        m_isoTimeN = !isoTimeN.IsEmpty() ? isoTimeN : "12:00:00";
    }
    else {
        m_isoTimeN = "";
    }
}

// m_date is temporary initialized to an invalid value.
mmDateTime::mmDateTime(wxDateTime dateTime) :
    m_date(mmDateN().value())
{
    if (dateTime.IsValid()) {
        cache_dateTime(dateTime);
    }
    else {
        wxLogDebug("ERROR: mmDateTime::mmDateTime(): dateTime is invalid");
        cache_dateTime(wxDateTime::Now());
    }
}

mmDateTimeN::mmDateTimeN(wxDateTime dateTimeN)
{
    if (dateTimeN.IsValid()) {
        c_dateTimeN = dateTimeN;
        c_dateTimeN.SetMillisecond(0);
        m_dateN = mmDateN(c_dateTimeN);
        m_isoTimeN = c_dateTimeN.FormatISOTime();
    }
    else {
        m_dateN = mmDateN();
        m_isoTimeN = "";
        c_dateTimeN = wxInvalidDateTime;
    }
}

// -- methods

wxDateTime mmDateTime::cache_dateTime()
{
    c_dateTimeN.ParseISOCombined(m_date.isoDate() + "T" + m_isoTime);
    if (!c_dateTimeN.IsValid()) {
        wxLogDebug("ERROR: mmDateTime::cache_dateTime(): c_dateTimeN is invalid");
        c_dateTimeN = wxDateTime::Now();
    }

    return c_dateTimeN;
}

void mmDateTime::cache_dateTime(wxDateTime dateTime)
{
    if (dateTime.IsValid()) {
        c_dateTimeN = dateTime;
    }
    else {
        wxLogDebug("ERROR: mmDateTime::mmDateTime(): dateTime is invalid");
        c_dateTimeN = wxDateTime::Now();
    }

    c_dateTimeN.SetMillisecond(0);

    m_date = mmDate(c_dateTimeN);
    m_isoTime = c_dateTimeN.FormatISOTime();
}

wxDateTime mmDateTimeN::cache_dateTimeN()
{
    c_dateTimeN = has_value() ? value().cache_dateTime() : wxInvalidDateTime;
    return c_dateTimeN;
}

void mmDateTimeN::cache_dateTimeN(wxDateTime dateTimeN)
{
    if (dateTimeN.IsValid()) {
        mmDateTime dateTime = mmDateTime(dateTimeN);
        m_dateN = dateTime.m_date;
        m_isoTimeN = dateTime.m_isoTime;
        c_dateTimeN = dateTime.c_dateTimeN;
    }
    else {
        m_dateN = mmDateN();
        m_isoTimeN = "";
        c_dateTimeN = wxInvalidDateTime;
    }
}

void mmDateTime::addDateSpan(wxDateSpan dateSpan)
{
    m_date.addDateSpan(dateSpan);
    c_dateTimeN = wxInvalidDateTime;
}

void mmDateTime::subDateSpan(wxDateSpan dateSpan)
{
    m_date.subDateSpan(dateSpan);
    c_dateTimeN = wxInvalidDateTime;
}

mmDateTime mmDateTime::plusDateSpan(wxDateSpan dateSpan)
{
    return mmDateTime(m_date.plusDateSpan(dateSpan), m_isoTime);
}

mmDateTime mmDateTime::minusDateSpan(wxDateSpan dateSpan)
{
    return mmDateTime(m_date.minusDateSpan(dateSpan), m_isoTime);
}

mmDateTime mmDateTime::fromUtcToLocal()
{
    return mmDateTime(dateTime().FromUTC());
}

mmDateTime mmDateTime::fromLocalToUtc()
{
    return mmDateTime(dateTime().ToUTC());
}

mmDateTimeN mmDateTimeN::fromUtcToLocalN()
{
    return has_value()
        ? mmDateTimeN(dateTimeN().FromUTC())
        : mmDateTimeN();
}

mmDateTimeN mmDateTimeN::fromLocalToUtcN()
{
    return has_value()
        ? mmDateTimeN(dateTimeN().ToUTC())
        : mmDateTimeN();
}
