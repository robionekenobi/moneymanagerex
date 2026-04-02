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

#pragma once

#include <wx/datetime.h>
#include <wx/string.h>
#include <wx/log.h>
#include "_types.h"
#include "mmCache.h"

// mmDate represents the date part of a datetime (without time information).
// wxWidgets does not have a dedicated type for this purpose.
// The underlying data structure is a string in ISO date format "YYYY-MM-DD",
// and a wxDateTime with the time part set to noon (12:00:00) instead of zero,
// in order to avoid rounding errors. The string representation is always
// available after construction, and it is used for date comparisons,
// while the wxDateTime representation is caclulated and cached on demand
// (for date arithmetic), otherwise it is set to wxInvalidDateTime.
// Notice that the MMEX schema stores dates as strings, therefore loading,
// processing, and saving of data records does not require converions between
// date representations, unless date arithmetic is involved.
//
// mmDateN is an optional (nullable) mmDate.
// The underlying null value is { "", wxInvalidDateTime }.
//
// mmDate::invalid() is not a valid mmDate (it is not created by any other method).
// It is provided for temporary initialization of an mmDate variable,
// when it is known that this variable is re-initialized later before it is used
// (using mmDate::invalid() with other methods is undefined behavior).
// Initialization with a dummy date string, e.g., mmDate::min(), also serves
// the same purpose, but it may be more confusing to the reader.
// Notice that mmDate("") is equivalent to mmDate::today(), which has a conversion
// overhead (from wxDateTime to string).
//
// iso{Start,End} are helper functions for date comparisons.
// For a string `isoDateTime` in ISO date or datetime format,
// * start_date.isoStart() <= isoDateTime
//   checks if `isoDateTime` is in the same day as `start_date` or later
// * isoDateTime <= end_date.isoEnd()
//   checks if `isoDateTime` is in the same day as `end_date` or earlier
// * start_date.isoEnd() < isoDateTime
//   checks if `isoDateTime` is strictly later than `start_date`
// * isoDateTime < end_date.isoStart()
//   checks if `isoDateTime` is strictly earlier than `end_date`
// Notice that the strict comparisons `start_date.isoStart() < isoDateTime`
// and `isoDateTime < end_date.isoEnd()` do not have the intended meaning.

struct mmDate
{
    friend struct mmDateN;

// -- static

private:
    static const wxTimeSpan s_htol;
    static constexpr std::size_t s_cache_cap = 10000;
    static mmCache<wxDateTime, wxString> c_dateTime_isoDate;
    static mmCache<wxString, wxDateTime> c_isoDate_dateTime;

// -- state

private:
    wxString m_isoDate;
    wxDateTime c_dateTimeN;

// -- constructor

private:
    mmDate(const wxString& isoDate, wxDateTime dateTimeN) :
        m_isoDate(isoDate), c_dateTimeN(dateTimeN) {}

public:
    mmDate(const wxString& isoDateTime);
    mmDate(wxDateTime dateTime);

public:
    static mmDate invalid() { return mmDate("", wxInvalidDateTime); }
    static mmDate today() { return mmDate(wxDateTime(12, 0, 0, 0)); }
    static mmDate min() { return mmDate("1970-01-01"); }
    static mmDate max() { return mmDate("2999-12-31"); }

// -- methods

public:
    auto isoDate() const -> const wxString { return m_isoDate; }
    auto isoStart() const -> const wxString { return m_isoDate; }
    auto isoEnd() const -> const wxString { return m_isoDate + "~"; }

private:
    auto cache_dateTime() -> wxDateTime;
    void cache_dateTime(wxDateTime dateTime);

public:
    auto dateTime() -> wxDateTime {
        return c_dateTimeN.IsValid() ? c_dateTimeN : cache_dateTime();
    }
    void addDateSpan(wxDateSpan dateSpan);
    void subDateSpan(wxDateSpan dateSpan);
    auto plusDateSpan(wxDateSpan dateSpan) -> mmDate;
    auto minusDateSpan(wxDateSpan dateSpan) -> mmDate;
    int  daysSince(mmDate& other);
    int  daysUntil(mmDate& other);

// -- operators

public:
    bool operator== (const mmDate& other) const { return m_isoDate == other.m_isoDate; }
    bool operator!= (const mmDate& other) const { return m_isoDate != other.m_isoDate; }
    bool operator<  (const mmDate& other) const { return m_isoDate <  other.m_isoDate; }
    bool operator>  (const mmDate& other) const { return m_isoDate >  other.m_isoDate; }
    bool operator<= (const mmDate& other) const { return m_isoDate <= other.m_isoDate; }
    bool operator>= (const mmDate& other) const { return m_isoDate >= other.m_isoDate; }
};

struct mmDateN
{
// -- state

private:
    wxString m_isoDateN;
    wxDateTime c_dateTimeN;

// -- constructor

private:
    mmDateN(const wxString& isoDateN, wxDateTime dateTimeN) :
        m_isoDateN(isoDateN), c_dateTimeN(dateTimeN) {}

public:
    mmDateN() : mmDateN("", wxInvalidDateTime) {}
    mmDateN(mmDate date) : mmDateN(date.m_isoDate, date.c_dateTimeN) {}
    mmDateN(const wxString& isoDateTimeN);
    mmDateN(wxDateTime dateTimeN);

// -- methods

public:
    bool has_value() const { return !m_isoDateN.IsEmpty(); }
    auto value() const -> mmDate { return mmDate(m_isoDateN, c_dateTimeN); }
    auto value_or(mmDate def_date) const -> mmDate {
        return has_value() ? value() : def_date;
    }

public:
    auto isoDateN() const -> const wxString { return m_isoDateN; }
    auto isoStartN() const -> const wxString { return m_isoDateN; }
    auto isoEndN() const -> const wxString { return m_isoDateN + "~"; }

private:
    auto cache_dateTimeN() -> wxDateTime;
    void cache_dateTimeN(wxDateTime dateTimeN);

public:
    auto dateTimeN() -> wxDateTime {
        return (!has_value() || c_dateTimeN.IsValid()) ? c_dateTimeN : cache_dateTimeN();
    }

// -- operators

public:
    bool operator== (const mmDateN& other) const { return m_isoDateN == other.m_isoDateN; }
    bool operator!= (const mmDateN& other) const { return m_isoDateN != other.m_isoDateN; }
};
