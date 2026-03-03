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
// This is only sample code re-used from "table/TrxShareTable.cpp".

#include "TrxShareData.h"

TrxShareData::TrxShareData()
{
    SHAREINFOID = -1;
    CHECKINGACCOUNTID = -1;
    SHARENUMBER = 0.0;
    SHAREPRICE = 0.0;
    SHARECOMMISSION = 0.0;
}

// Convert TrxShareData to TrxShareRow
TrxShareRow TrxShareData::to_row() const
{
    TrxShareRow row;

    row.SHAREINFOID = SHAREINFOID;
    row.CHECKINGACCOUNTID = CHECKINGACCOUNTID;
    row.SHARENUMBER = SHARENUMBER;
    row.SHAREPRICE = SHAREPRICE;
    row.SHARECOMMISSION = SHARECOMMISSION;
    row.SHARELOT = SHARELOT;

    return row;
}

// Convert TrxShareRow to TrxShareData
TrxShareData& TrxShareData::from_row(const TrxShareRow& row)
{
    SHAREINFOID = row.SHAREINFOID; // int64
    CHECKINGACCOUNTID = row.CHECKINGACCOUNTID; // int64
    SHARENUMBER = row.SHARENUMBER; // double
    SHAREPRICE = row.SHAREPRICE; // double
    SHARECOMMISSION = row.SHARECOMMISSION; // double
    SHARELOT = row.SHARELOT; // wxString

    return *this;
}

bool TrxShareData::equals(const TrxShareData* other) const
{
    if ( SHAREINFOID != other->SHAREINFOID) return false;
    if ( CHECKINGACCOUNTID != other->CHECKINGACCOUNTID) return false;
    if ( SHARENUMBER != other->SHARENUMBER) return false;
    if ( SHAREPRICE != other->SHAREPRICE) return false;
    if ( SHARECOMMISSION != other->SHARECOMMISSION) return false;
    if (!SHARELOT.IsSameAs(other->SHARELOT)) return false;

    return true;
}
