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
// This is only sample code re-used from "table/FieldValueTable.h".
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
#include "table/FieldValueTable.h"

// User-friendly representation of a record in table CUSTOMFIELDDATA_V1.
struct FieldValueData
{
    int64 FIELDATADID; // primary key
    int64 FIELDID;
    int64 REFID;
    wxString CONTENT;

    explicit FieldValueData();
    explicit FieldValueData(wxSQLite3ResultSet& q);
    FieldValueData(const FieldValueData& other) = default;

    int64 id() const { return FIELDATADID; }
    void id(const int64 id) { FIELDATADID = id; }
    FieldValueRow to_row() const;
    FieldValueData& from_row(const FieldValueRow& row);
    void to_insert_stmt(wxSQLite3Statement& stmt, int64 id) const;
    void to_update_stmt(wxSQLite3Statement& stmt) const;
    FieldValueData& from_select_result(wxSQLite3ResultSet& q);
    wxString to_json() const;
    void as_json(PrettyWriter<StringBuffer>& json_writer) const;
    row_t to_html_row() const;
    void to_html_template(html_template& t) const;
    void destroy() { delete this; }

    FieldValueData& clone_from(const FieldValueData& other);
    bool equals(const FieldValueData* other) const;
    bool operator< (const FieldValueData& other) const { return id() < other.id(); }
    bool operator< (const FieldValueData* other) const { return id() < other->id(); }

    struct SorterByFIELDATADID
    {
        bool operator()(const FieldValueData& x, const FieldValueData& y)
        {
            return x.FIELDATADID < y.FIELDATADID;
        }
    };

    struct SorterByFIELDID
    {
        bool operator()(const FieldValueData& x, const FieldValueData& y)
        {
            return x.FIELDID < y.FIELDID;
        }
    };

    struct SorterByREFID
    {
        bool operator()(const FieldValueData& x, const FieldValueData& y)
        {
            return x.REFID < y.REFID;
        }
    };

    struct SorterByCONTENT
    {
        bool operator()(const FieldValueData& x, const FieldValueData& y)
        {
            return x.CONTENT < y.CONTENT;
        }
    };
};

inline FieldValueData::FieldValueData(wxSQLite3ResultSet& q) :
    FieldValueData()
{
    from_select_result(q);
}

inline void FieldValueData::to_insert_stmt(wxSQLite3Statement& stmt, int64 id) const
{
    to_row().to_insert_stmt(stmt, id);
}

inline void FieldValueData::to_update_stmt(wxSQLite3Statement& stmt) const
{
    to_row().to_update_stmt(stmt);
}

inline FieldValueData& FieldValueData::from_select_result(wxSQLite3ResultSet& q)
{
    return from_row(FieldValueRow().from_select_result(q));
}

inline wxString FieldValueData::to_json() const
{
    return to_row().to_json();
}

inline void FieldValueData::as_json(PrettyWriter<StringBuffer>& json_writer) const
{
    to_row().as_json(json_writer);
}

inline row_t FieldValueData::to_html_row() const
{
    return to_row().to_html_row();
}

inline void FieldValueData::to_html_template(html_template& t) const
{
    to_row().to_html_template(t);
}

inline FieldValueData& FieldValueData::clone_from(const FieldValueData& other)
{
    *this = other;
    id(-1);
    return *this;
}
