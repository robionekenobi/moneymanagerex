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
// This is only sample code re-used from "table/FieldValueTable.cpp".

#include "FieldValueData.h"

FieldValueData::FieldValueData()
{
    FIELDATADID = -1;
    FIELDID = -1;
    REFID = -1;
}

// Convert FieldValueData to FieldValueRow
FieldValueRow FieldValueData::to_row() const
{
    FieldValueRow row;

    row.FIELDATADID = FIELDATADID;
    row.FIELDID = FIELDID;
    row.REFID = REFID;
    row.CONTENT = CONTENT;

    return row;
}

// Convert FieldValueRow to FieldValueData
FieldValueData& FieldValueData::from_row(const FieldValueRow& row)
{
    FIELDATADID = row.FIELDATADID; // int64
    FIELDID = row.FIELDID; // int64
    REFID = row.REFID; // int64
    CONTENT = row.CONTENT; // wxString

    return *this;
}

bool FieldValueData::equals(const FieldValueData* other) const
{
    if ( FIELDATADID != other->FIELDATADID) return false;
    if ( FIELDID != other->FIELDID) return false;
    if ( REFID != other->REFID) return false;
    if (!CONTENT.IsSameAs(other->CONTENT)) return false;

    return true;
}
