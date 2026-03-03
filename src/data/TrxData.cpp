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
// This is only sample code re-used from "table/TrxTable.cpp".

#include "TrxData.h"

TrxData::TrxData()
{
    TRANSID = -1;
    ACCOUNTID = -1;
    TOACCOUNTID = -1;
    PAYEEID = -1;
    TRANSAMOUNT = 0.0;
    CATEGID = -1;
    FOLLOWUPID = -1;
    TOTRANSAMOUNT = 0.0;
    COLOR = -1;
}

// Convert TrxData to TrxRow
TrxRow TrxData::to_row() const
{
    TrxRow row;

    row.TRANSID = TRANSID;
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
    row.LASTUPDATEDTIME = LASTUPDATEDTIME;
    row.DELETEDTIME = DELETEDTIME;
    row.FOLLOWUPID = FOLLOWUPID;
    row.TOTRANSAMOUNT = TOTRANSAMOUNT;
    row.COLOR = COLOR;

    return row;
}

// Convert TrxRow to TrxData
TrxData& TrxData::from_row(const TrxRow& row)
{
    TRANSID = row.TRANSID; // int64
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
    LASTUPDATEDTIME = row.LASTUPDATEDTIME; // wxString
    DELETEDTIME = row.DELETEDTIME; // wxString
    FOLLOWUPID = row.FOLLOWUPID; // int64
    TOTRANSAMOUNT = row.TOTRANSAMOUNT; // double
    COLOR = row.COLOR; // int64

    return *this;
}

bool TrxData::equals(const TrxData* other) const
{
    if ( TRANSID != other->TRANSID) return false;
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
    if (!LASTUPDATEDTIME.IsSameAs(other->LASTUPDATEDTIME)) return false;
    if (!DELETEDTIME.IsSameAs(other->DELETEDTIME)) return false;
    if ( FOLLOWUPID != other->FOLLOWUPID) return false;
    if ( TOTRANSAMOUNT != other->TOTRANSAMOUNT) return false;
    if ( COLOR != other->COLOR) return false;

    return true;
}
