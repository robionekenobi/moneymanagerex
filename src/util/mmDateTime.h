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

#include "mmDate.h"

// mmDateTime represents a datetime (date and time), with resolution of a second.
// The underlying data structure is an mmDate, which represents the date part,
// a string in ISO time format "hh:mm:ss", and a wxDateTime which represents
// the datetime (both the date part and the time part together).
// The date string (stored inside mmDate) and time string are always available
// after construction, and they are used for date or datetime comparisons.
// The date wxDateTime representation (stored inside mmDate) and the datetime
// wxDateTime representation are caclulated and cached on demand, (for date
// or datetime arithmetic), otherwise they are set to wxInvalidDateTime.
// For efficiency, the time part is stored separately from the date part.
// Notice that the MMEX application does not perform arithmetic on time,
// as it does on date, and most datetime operations have granularity of a day.
//
// mmDateTimeN is an optional (nullable) mmDateTime.
// The underlying null value is { mmDateN(), "", wxInvalidDateTime }.
//
// mmDateTime::invalid() is not a valid mmDateTime.
// See the comments on mmDate::invalid() in `mmDate.h`.
//
// An optional argument `withTime` is provided for convenience in a few methods.
// When time is disabled in MMEX application, transactions datetime (TRANSDATE)
// is stored in ISO datetime format, but the time part is not shown in GUI.
// New transactions get a dummy time (12:00:00), however existing transactions
// may have a non-default time (e.g., if they were created with time enabled).
// The invisible time part of existing transactions shall be preserved in the
// database, such that it re-appears in GUI when users enable time again.
// `withTime` can be used to selectively get information with or without time.

struct mmDateTime
{
    friend struct mmDateTimeN;

// -- state

private:
    mmDate m_date;
    wxString m_isoTime;
    wxDateTime c_dateTimeN;

// -- constructor

private:
    mmDateTime(mmDate date, const wxString isoTime, wxDateTime dateTimeN) :
        m_date(date), m_isoTime(isoTime), c_dateTimeN(dateTimeN) {}

public:
    mmDateTime(mmDate date, const wxString& isoTime = "12:00:00") :
        mmDateTime(date, isoTime, wxInvalidDateTime) {}
    mmDateTime(const wxString& isoDateTime);
    mmDateTime(wxDateTime dateTime);

public:
    static mmDateTime invalid() {
        return mmDateTime(mmDate::invalid(), "", wxInvalidDateTime);
    }
    static mmDateTime now(bool withTime = true) {
        return withTime
            ? mmDateTime(wxDateTime::Now())
            : mmDateTime(mmDate::today());
    }

// -- methods

public:
    auto date() const -> const mmDate& { return m_date; }
    auto isoDate() const -> const wxString { return m_date.isoDate(); }
    auto isoTime(bool withTime = true) const -> const wxString {
        return withTime ? m_isoTime : "12:00:00";
    }
    auto isoDateTime(bool withTime = true) const -> const wxString {
        return m_date.isoDate() + "T" + isoTime(withTime);
    }

private:
    auto cache_dateTime() -> wxDateTime;
    void cache_dateTime(wxDateTime dateTime);

public:
    auto dateTime(bool withTime = true) -> wxDateTime {
        return withTime
            ? (c_dateTimeN.IsValid() ? c_dateTimeN : cache_dateTime())
            : m_date.dateTime();
    }
    void addDateSpan(wxDateSpan dateSpan);
    void subDateSpan(wxDateSpan dateSpan);
    auto plusDateSpan(wxDateSpan dateSpan) -> mmDateTime;
    auto minusDateSpan(wxDateSpan dateSpan) -> mmDateTime;
    auto fromLocalToUtc() -> mmDateTime;
    auto fromUtcToLocal() -> mmDateTime;

// -- operators

public:
    bool operator== (const mmDateTime& other) const {
        return m_date == other.m_date && m_isoTime == other.m_isoTime;
    }
    bool operator!= (const mmDateTime& other) const {
        return m_date != other.m_date || m_isoTime != other.m_isoTime;
    }
    bool operator< (const mmDateTime& other) const {
        return m_date < other.m_date ||
            (m_date == other.m_date && m_isoTime < other.m_isoTime);
    }
    bool operator> (const mmDateTime& other) const {
        return m_date > other.m_date ||
            (m_date == other.m_date && m_isoTime > other.m_isoTime);
    }
    bool operator<= (const mmDateTime& other) const {
        return m_date < other.m_date ||
            (m_date == other.m_date && m_isoTime <= other.m_isoTime);
    }
    bool operator>= (const mmDateTime& other) const {
        return m_date > other.m_date ||
            (m_date == other.m_date && m_isoTime >= other.m_isoTime);
    }
};

struct mmDateTimeN
{
    friend struct mmDateTime;

// -- state

public:
    mmDateN m_dateN;
    wxString m_isoTimeN;
    wxDateTime c_dateTimeN;

// -- constructor

private:
    mmDateTimeN(mmDateN dateN, const wxString isoTimeN, wxDateTime dateTimeN) :
        m_dateN(dateN), m_isoTimeN(isoTimeN), c_dateTimeN(dateTimeN) {}

public:
    mmDateTimeN() :
        mmDateTimeN(mmDateN(), "", wxInvalidDateTime) {}
    mmDateTimeN(mmDateTime dateTime) :
        mmDateTimeN(mmDateN(dateTime.m_date), dateTime.m_isoTime, dateTime.c_dateTimeN) {}
    mmDateTimeN(mmDate date, const wxString& isoTime = "12:00:00") :
        mmDateTimeN(mmDateN(date), isoTime, wxInvalidDateTime) {}
    mmDateTimeN(const wxString& isoDateTimeN);
    mmDateTimeN(wxDateTime dateTimeN);

// -- methods

public:
    bool has_value() const { return m_dateN.has_value(); }
    auto value() const -> mmDateTime {
        return mmDateTime(m_dateN.value(), m_isoTimeN, c_dateTimeN);
    }
    auto value_or(mmDateTime def_dateTime) const -> mmDateTime {
        return has_value() ? value() : def_dateTime;
    }

public:
    auto dateN() const -> const mmDateN& { return m_dateN; }
    auto isoDateN() const -> const wxString { return m_dateN.isoDateN(); }
    auto isoTimeN(bool withTime = true) const -> const wxString {
        return has_value() ? (withTime ? m_isoTimeN : "12:00:00") : "";
    }
    auto isoDateTimeN(bool withTime = true) const -> const wxString {
        return has_value() ? m_dateN.isoDateN() + "T" + isoTimeN(withTime) : "";
    }

private:
    auto cache_dateTimeN() -> wxDateTime;
    void cache_dateTimeN(wxDateTime dateTimeN);

public:
    auto dateTimeN(bool withTime = true) -> wxDateTime {
        return withTime
            ? (!has_value() || c_dateTimeN.IsValid()) ? c_dateTimeN : cache_dateTimeN()
            : m_dateN.dateTimeN();
    }
    auto fromLocalToUtcN() -> mmDateTimeN;
    auto fromUtcToLocalN() -> mmDateTimeN;

public:
    bool operator== (const mmDateTimeN& other) const {
        return m_dateN == other.m_dateN && m_isoTimeN == other.m_isoTimeN;
    }
    bool operator!= (const mmDateTimeN& other) const {
        return m_dateN != other.m_dateN || m_isoTimeN != other.m_isoTimeN;
    }
};
