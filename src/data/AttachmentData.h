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
//
// This is only sample code re-used from "table/AttachmentTable.h".
//
// The data structure can be refined by:
// * using more user-frielndly filed name
// * using stronger field types
// * adding enumerations for fields with limited choices
// * demultiplexing composite values in database columns
//
// See also an implementation in Swift:
//   https://github.com/moneymanagerex/mmex-ios/tree/master/MMEX/Data
// and an implementation in Java:
//   https://github.com/moneymanagerex/android-money-manager-ex/tree/master/app/src/main/java/com/money/manager/ex/domainmodel

#pragma once

#include "table/_TableBase.h"
#include "table/AttachmentTable.h"

// User-friendly representation of a record in table ATTACHMENT_V1.
struct AttachmentData
{
    int64 ATTACHMENTID; // primary key
    wxString REFTYPE;
    int64 REFID;
    wxString DESCRIPTION;
    wxString FILENAME;

    explicit AttachmentData();
    explicit AttachmentData(wxSQLite3ResultSet& q);
    AttachmentData(const AttachmentData& other) = default;

    int64 id() const { return ATTACHMENTID; }
    void id(const int64 id) { ATTACHMENTID = id; }
    AttachmentRow to_row() const;
    AttachmentData& from_row(const AttachmentRow& row);
    void to_insert_stmt(wxSQLite3Statement& stmt, int64 id) const;
    void to_update_stmt(wxSQLite3Statement& stmt) const;
    AttachmentData& from_select_result(wxSQLite3ResultSet& q);
    wxString to_json() const;
    void as_json(PrettyWriter<StringBuffer>& json_writer) const;
    row_t to_html_row() const;
    void to_html_template(html_template& t) const;
    void destroy() { delete this; }

    AttachmentData& clone_from(const AttachmentData& other);
    bool equals(const AttachmentData* other) const;
    bool operator< (const AttachmentData& other) const { return id() < other.id(); }
    bool operator< (const AttachmentData* other) const { return id() < other->id(); }

    struct SorterByATTACHMENTID
    {
        bool operator()(const AttachmentData& x, const AttachmentData& y)
        {
            return x.ATTACHMENTID < y.ATTACHMENTID;
        }
    };

    struct SorterByREFTYPE
    {
        bool operator()(const AttachmentData& x, const AttachmentData& y)
        {
            return x.REFTYPE < y.REFTYPE;
        }
    };

    struct SorterByREFID
    {
        bool operator()(const AttachmentData& x, const AttachmentData& y)
        {
            return x.REFID < y.REFID;
        }
    };

    struct SorterByDESCRIPTION
    {
        bool operator()(const AttachmentData& x, const AttachmentData& y)
        {
            return x.DESCRIPTION < y.DESCRIPTION;
        }
    };

    struct SorterByFILENAME
    {
        bool operator()(const AttachmentData& x, const AttachmentData& y)
        {
            return x.FILENAME < y.FILENAME;
        }
    };
};

inline AttachmentData::AttachmentData(wxSQLite3ResultSet& q) :
    AttachmentData()
{
    from_select_result(q);
}

inline void AttachmentData::to_insert_stmt(wxSQLite3Statement& stmt, int64 id) const
{
    to_row().to_insert_stmt(stmt, id);
}

inline void AttachmentData::to_update_stmt(wxSQLite3Statement& stmt) const
{
    to_row().to_update_stmt(stmt);
}

inline AttachmentData& AttachmentData::from_select_result(wxSQLite3ResultSet& q)
{
    return from_row(AttachmentRow().from_select_result(q));
}

inline wxString AttachmentData::to_json() const
{
    return to_row().to_json();
}

inline void AttachmentData::as_json(PrettyWriter<StringBuffer>& json_writer) const
{
    to_row().as_json(json_writer);
}

inline row_t AttachmentData::to_html_row() const
{
    return to_row().to_html_row();
}

inline void AttachmentData::to_html_template(html_template& t) const
{
    to_row().to_html_template(t);
}

inline AttachmentData& AttachmentData::clone_from(const AttachmentData& other)
{
    *this = other;
    id(-1);
    return *this;
}
