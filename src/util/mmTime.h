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

// mmTime represents the time part of a datetime (without date information).
// The underlying data structure is a string of the form "hh:mm:ss".
// MMEX does not perform arithmetic on time, therefore a simple string
// representation is sufficient. The default time is noon ("12:00:00").
//
// mmTimeN is an optional (nullable) mmTime.
// The underlying null value is the empty string.
//
// The constructor mmTime(const wxString&) can be used with an ISO datetime string
// of the form "YYYY-MM-DD'T'hh:mm:ss", or with a time string of the form "hh:mm:ss".
// The constructor mmTimeN(const wxString&) can be used with an ISO datetime string,
// or with a time string of the form "'T'hh:mm:ss", or with an ISO date string of the
// form "YYYY-MM-DD", or with ""; in the last two cases it returns a null value.

struct mmTime
{
    friend struct mmTimeN;

protected:
    wxString m_time;

public:
    mmTime();
    mmTime(wxDateTime dateTime);
    mmTime(const wxString& isoDateTime);

public:
    auto isoTime() const -> const wxString { return m_time; }

public:
    bool operator== (const mmTime& other) const { return m_time == other.m_time; }
    bool operator!= (const mmTime& other) const { return m_time != other.m_time; }
    bool operator<  (const mmTime& other) const { return m_time <  other.m_time; }
    bool operator>  (const mmTime& other) const { return m_time >  other.m_time; }
    bool operator<= (const mmTime& other) const { return m_time <= other.m_time; }
    bool operator>= (const mmTime& other) const { return m_time >= other.m_time; }
};

struct mmTimeN
{
    friend struct mmTime;

private:
    wxString m_timeN;

public:
    mmTimeN();
    mmTimeN(wxDateTime dateTimeN);
    mmTimeN(const wxString& isoDateTimeN);

public:
    bool has_value() const { return !m_timeN.IsEmpty(); }
    auto value() const -> mmTime { return mmTime(m_timeN); }
    auto value_or(mmTime defTime) const -> mmTime { return has_value() ? value() : defTime; }

public:
    auto isoTimeN() const -> const wxString { return m_timeN; }

public:
    bool operator== (const mmTimeN& other) const;
    bool operator!= (const mmTimeN& other) const;
};

inline bool mmTimeN::operator== (const mmTimeN& other) const
{
    return (!has_value() && !other.has_value())
        || (has_value() && other.has_value() && value() == other.value());
}
inline bool mmTimeN::operator!= (const mmTimeN& other) const
{
    return (has_value() || other.has_value())
        && (!has_value() || !other.has_value() || value() != other.value());
}
