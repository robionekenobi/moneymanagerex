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
// This is only sample code re-used from "table/AttachmentTable.cpp".

#include "AttachmentData.h"

AttachmentData::AttachmentData()
{
    ATTACHMENTID = -1;
    REFID = -1;
}

// Convert AttachmentData to AttachmentRow
AttachmentRow AttachmentData::to_row() const
{
    AttachmentRow row;

    row.ATTACHMENTID = ATTACHMENTID;
    row.REFTYPE = REFTYPE;
    row.REFID = REFID;
    row.DESCRIPTION = DESCRIPTION;
    row.FILENAME = FILENAME;

    return row;
}

// Convert AttachmentRow to AttachmentData
AttachmentData& AttachmentData::from_row(const AttachmentRow& row)
{
    ATTACHMENTID = row.ATTACHMENTID; // int64
    REFTYPE = row.REFTYPE; // wxString
    REFID = row.REFID; // int64
    DESCRIPTION = row.DESCRIPTION; // wxString
    FILENAME = row.FILENAME; // wxString

    return *this;
}

bool AttachmentData::equals(const AttachmentData* other) const
{
    if ( ATTACHMENTID != other->ATTACHMENTID) return false;
    if (!REFTYPE.IsSameAs(other->REFTYPE)) return false;
    if ( REFID != other->REFID) return false;
    if (!DESCRIPTION.IsSameAs(other->DESCRIPTION)) return false;
    if (!FILENAME.IsSameAs(other->FILENAME)) return false;

    return true;
}
