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
// This is only sample code re-used from "table/TagLinkTable.h".
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
#include "table/TagLinkTable.h"

// User-friendly representation of a record in table TAGLINK_V1.
struct TagLinkData
{
    int64 TAGLINKID; // primary key
    wxString REFTYPE;
    int64 REFID;
    int64 TAGID;

    explicit TagLinkData();
    explicit TagLinkData(wxSQLite3ResultSet& q);
    TagLinkData(const TagLinkData& other) = default;

    int64 id() const { return TAGLINKID; }
    void id(const int64 id) { TAGLINKID = id; }
    TagLinkRow to_row() const;
    TagLinkData& from_row(const TagLinkRow& row);
    void to_insert_stmt(wxSQLite3Statement& stmt, int64 id) const;
    void to_update_stmt(wxSQLite3Statement& stmt) const;
    TagLinkData& from_select_result(wxSQLite3ResultSet& q);
    wxString to_json() const;
    void as_json(PrettyWriter<StringBuffer>& json_writer) const;
    row_t to_html_row() const;
    void to_html_template(html_template& t) const;
    void destroy() { delete this; }

    TagLinkData& clone_from(const TagLinkData& other);
    bool equals(const TagLinkData* other) const;
    bool operator< (const TagLinkData& other) const { return id() < other.id(); }
    bool operator< (const TagLinkData* other) const { return id() < other->id(); }

    struct SorterByTAGLINKID
    {
        bool operator()(const TagLinkData& x, const TagLinkData& y)
        {
            return x.TAGLINKID < y.TAGLINKID;
        }
    };

    struct SorterByREFTYPE
    {
        bool operator()(const TagLinkData& x, const TagLinkData& y)
        {
            return x.REFTYPE < y.REFTYPE;
        }
    };

    struct SorterByREFID
    {
        bool operator()(const TagLinkData& x, const TagLinkData& y)
        {
            return x.REFID < y.REFID;
        }
    };

    struct SorterByTAGID
    {
        bool operator()(const TagLinkData& x, const TagLinkData& y)
        {
            return x.TAGID < y.TAGID;
        }
    };
};

inline TagLinkData::TagLinkData(wxSQLite3ResultSet& q) :
    TagLinkData()
{
    from_select_result(q);
}

inline void TagLinkData::to_insert_stmt(wxSQLite3Statement& stmt, int64 id) const
{
    to_row().to_insert_stmt(stmt, id);
}

inline void TagLinkData::to_update_stmt(wxSQLite3Statement& stmt) const
{
    to_row().to_update_stmt(stmt);
}

inline TagLinkData& TagLinkData::from_select_result(wxSQLite3ResultSet& q)
{
    return from_row(TagLinkRow().from_select_result(q));
}

inline wxString TagLinkData::to_json() const
{
    return to_row().to_json();
}

inline void TagLinkData::as_json(PrettyWriter<StringBuffer>& json_writer) const
{
    to_row().as_json(json_writer);
}

inline row_t TagLinkData::to_html_row() const
{
    return to_row().to_html_row();
}

inline void TagLinkData::to_html_template(html_template& t) const
{
    to_row().to_html_template(t);
}

inline TagLinkData& TagLinkData::clone_from(const TagLinkData& other)
{
    *this = other;
    id(-1);
    return *this;
}
