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
// This is only sample code re-used from "table/TagLinkTable.cpp".

#include "TagLinkData.h"

TagLinkData::TagLinkData()
{
    TAGLINKID = -1;
    REFID = -1;
    TAGID = -1;
}

// Convert TagLinkData to TagLinkRow
TagLinkRow TagLinkData::to_row() const
{
    TagLinkRow row;

    row.TAGLINKID = TAGLINKID;
    row.REFTYPE = REFTYPE;
    row.REFID = REFID;
    row.TAGID = TAGID;

    return row;
}

// Convert TagLinkRow to TagLinkData
TagLinkData& TagLinkData::from_row(const TagLinkRow& row)
{
    TAGLINKID = row.TAGLINKID; // int64
    REFTYPE = row.REFTYPE; // wxString
    REFID = row.REFID; // int64
    TAGID = row.TAGID; // int64

    return *this;
}

bool TagLinkData::equals(const TagLinkData* other) const
{
    if ( TAGLINKID != other->TAGLINKID) return false;
    if (!REFTYPE.IsSameAs(other->REFTYPE)) return false;
    if ( REFID != other->REFID) return false;
    if ( TAGID != other->TAGID) return false;

    return true;
}
