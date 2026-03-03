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
// This is only sample code re-used from "table/FieldTable.cpp".

#include "FieldData.h"

FieldData::FieldData()
{
    FIELDID = -1;
}

// Convert FieldData to FieldRow
FieldRow FieldData::to_row() const
{
    FieldRow row;

    row.FIELDID = FIELDID;
    row.REFTYPE = REFTYPE;
    row.DESCRIPTION = DESCRIPTION;
    row.TYPE = TYPE;
    row.PROPERTIES = PROPERTIES;

    return row;
}

// Convert FieldRow to FieldData
FieldData& FieldData::from_row(const FieldRow& row)
{
    FIELDID = row.FIELDID; // int64
    REFTYPE = row.REFTYPE; // wxString
    DESCRIPTION = row.DESCRIPTION; // wxString
    TYPE = row.TYPE; // wxString
    PROPERTIES = row.PROPERTIES; // wxString

    return *this;
}

bool FieldData::equals(const FieldData* other) const
{
    if ( FIELDID != other->FIELDID) return false;
    if (!REFTYPE.IsSameAs(other->REFTYPE)) return false;
    if (!DESCRIPTION.IsSameAs(other->DESCRIPTION)) return false;
    if (!TYPE.IsSameAs(other->TYPE)) return false;
    if (!PROPERTIES.IsSameAs(other->PROPERTIES)) return false;

    return true;
}
