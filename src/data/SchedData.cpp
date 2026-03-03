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

// PLEASE EDIT!
// This is only sample code re-used from "table/SchedTable.cpp".

#include "SchedData.h"

SchedData::SchedData()
{
    BDID = -1;
    ACCOUNTID = -1;
    TOACCOUNTID = -1;
    PAYEEID = -1;
    TRANSAMOUNT = 0.0;
    CATEGID = -1;
    FOLLOWUPID = -1;
    TOTRANSAMOUNT = 0.0;
    REPEATS = -1;
    NUMOCCURRENCES = -1;
    COLOR = -1;
}

// Convert SchedData to SchedRow
SchedRow SchedData::to_row() const
{
    SchedRow row;

    row.BDID = BDID;
    row.ACCOUNTID = ACCOUNTID;
    row.TOACCOUNTID = TOACCOUNTID;
    row.PAYEEID = PAYEEID;
    row.TRANSCODE = TRANSCODE;
    row.TRANSAMOUNT = TRANSAMOUNT;
    row.STATUS = STATUS;
    row.TRANSACTIONNUMBER = TRANSACTIONNUMBER;
    row.NOTES = NOTES;
    row.CATEGID = CATEGID;
    row.TRANSDATE = TRANSDATE;
    row.FOLLOWUPID = FOLLOWUPID;
    row.TOTRANSAMOUNT = TOTRANSAMOUNT;
    row.REPEATS = REPEATS;
    row.NEXTOCCURRENCEDATE = NEXTOCCURRENCEDATE;
    row.NUMOCCURRENCES = NUMOCCURRENCES;
    row.COLOR = COLOR;

    return row;
}

// Convert SchedRow to SchedData
SchedData& SchedData::from_row(const SchedRow& row)
{
    BDID = row.BDID; // int64
    ACCOUNTID = row.ACCOUNTID; // int64
    TOACCOUNTID = row.TOACCOUNTID; // int64
    PAYEEID = row.PAYEEID; // int64
    TRANSCODE = row.TRANSCODE; // wxString
    TRANSAMOUNT = row.TRANSAMOUNT; // double
    STATUS = row.STATUS; // wxString
    TRANSACTIONNUMBER = row.TRANSACTIONNUMBER; // wxString
    NOTES = row.NOTES; // wxString
    CATEGID = row.CATEGID; // int64
    TRANSDATE = row.TRANSDATE; // wxString
    FOLLOWUPID = row.FOLLOWUPID; // int64
    TOTRANSAMOUNT = row.TOTRANSAMOUNT; // double
    REPEATS = row.REPEATS; // int64
    NEXTOCCURRENCEDATE = row.NEXTOCCURRENCEDATE; // wxString
    NUMOCCURRENCES = row.NUMOCCURRENCES; // int64
    COLOR = row.COLOR; // int64

    return *this;
}

bool SchedData::equals(const SchedData* other) const
{
    if ( BDID != other->BDID) return false;
    if ( ACCOUNTID != other->ACCOUNTID) return false;
    if ( TOACCOUNTID != other->TOACCOUNTID) return false;
    if ( PAYEEID != other->PAYEEID) return false;
    if (!TRANSCODE.IsSameAs(other->TRANSCODE)) return false;
    if ( TRANSAMOUNT != other->TRANSAMOUNT) return false;
    if (!STATUS.IsSameAs(other->STATUS)) return false;
    if (!TRANSACTIONNUMBER.IsSameAs(other->TRANSACTIONNUMBER)) return false;
    if (!NOTES.IsSameAs(other->NOTES)) return false;
    if ( CATEGID != other->CATEGID) return false;
    if (!TRANSDATE.IsSameAs(other->TRANSDATE)) return false;
    if ( FOLLOWUPID != other->FOLLOWUPID) return false;
    if ( TOTRANSAMOUNT != other->TOTRANSAMOUNT) return false;
    if ( REPEATS != other->REPEATS) return false;
    if (!NEXTOCCURRENCEDATE.IsSameAs(other->NEXTOCCURRENCEDATE)) return false;
    if ( NUMOCCURRENCES != other->NUMOCCURRENCES) return false;
    if ( COLOR != other->COLOR) return false;

    return true;
}
