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

#include "mmTime.h"

mmTime::mmTime() :
    m_time("12:00:00")
{}

mmTime::mmTime(wxDateTime dateTime)
{
    if (dateTime.IsValid()) {
        m_time = dateTime.FormatISOTime();
    }
    else {
        wxLogDebug("ERROR: mmTime::mmTime(): dateTime is invalid");
        m_time = "12:00:00";
    }
}

// wxString::AfterLast('T') returns the whole string if 'T' is not found.
mmTime::mmTime(const wxString& isoDateTime) :
    m_time(isoDateTime.AfterLast('T'))
{
    if (m_time.IsEmpty()) {
        wxLogDebug("ERROR: mmTime::mmTime(): isoDateTime is invalid");
        m_time = "12:00:00";
    }
}

mmTimeN::mmTimeN() :
    m_timeN("")
{}

mmTimeN::mmTimeN(wxDateTime dateTimeN) :
    m_timeN(dateTimeN.IsValid() ? dateTimeN.FormatISOTime() : "")
{}

// wxString::AfterFirst('T') returns an empty string if 'T' is not found.
mmTimeN::mmTimeN(const wxString& isoDateTimeN) :
    m_timeN(isoDateTimeN.AfterFirst('T'))
{}
