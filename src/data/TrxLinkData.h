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
// This is only sample code re-used from "table/TrxLinkTable.h".
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
#include "table/TrxLinkTable.h"

// User-friendly representation of a record in table TRANSLINK_V1.
struct TrxLinkData
{
    int64 TRANSLINKID; // primary key
    int64 CHECKINGACCOUNTID;
    wxString LINKTYPE;
    int64 LINKRECORDID;

    explicit TrxLinkData();
    explicit TrxLinkData(wxSQLite3ResultSet& q);
    TrxLinkData(const TrxLinkData& other) = default;

    int64 id() const { return TRANSLINKID; }
    void id(const int64 id) { TRANSLINKID = id; }
    TrxLinkRow to_row() const;
    TrxLinkData& from_row(const TrxLinkRow& row);
    void to_insert_stmt(wxSQLite3Statement& stmt, int64 id) const;
    void to_update_stmt(wxSQLite3Statement& stmt) const;
    TrxLinkData& from_select_result(wxSQLite3ResultSet& q);
    wxString to_json() const;
    void as_json(PrettyWriter<StringBuffer>& json_writer) const;
    row_t to_html_row() const;
    void to_html_template(html_template& t) const;
    void destroy() { delete this; }

    TrxLinkData& clone_from(const TrxLinkData& other);
    bool equals(const TrxLinkData* other) const;
    bool operator< (const TrxLinkData& other) const { return id() < other.id(); }
    bool operator< (const TrxLinkData* other) const { return id() < other->id(); }

    struct SorterByTRANSLINKID
    {
        bool operator()(const TrxLinkData& x, const TrxLinkData& y)
        {
            return x.TRANSLINKID < y.TRANSLINKID;
        }
    };

    struct SorterByCHECKINGACCOUNTID
    {
        bool operator()(const TrxLinkData& x, const TrxLinkData& y)
        {
            return x.CHECKINGACCOUNTID < y.CHECKINGACCOUNTID;
        }
    };

    struct SorterByLINKTYPE
    {
        bool operator()(const TrxLinkData& x, const TrxLinkData& y)
        {
            return x.LINKTYPE < y.LINKTYPE;
        }
    };

    struct SorterByLINKRECORDID
    {
        bool operator()(const TrxLinkData& x, const TrxLinkData& y)
        {
            return x.LINKRECORDID < y.LINKRECORDID;
        }
    };
};

inline TrxLinkData::TrxLinkData(wxSQLite3ResultSet& q) :
    TrxLinkData()
{
    from_select_result(q);
}

inline void TrxLinkData::to_insert_stmt(wxSQLite3Statement& stmt, int64 id) const
{
    to_row().to_insert_stmt(stmt, id);
}

inline void TrxLinkData::to_update_stmt(wxSQLite3Statement& stmt) const
{
    to_row().to_update_stmt(stmt);
}

inline TrxLinkData& TrxLinkData::from_select_result(wxSQLite3ResultSet& q)
{
    return from_row(TrxLinkRow().from_select_result(q));
}

inline wxString TrxLinkData::to_json() const
{
    return to_row().to_json();
}

inline void TrxLinkData::as_json(PrettyWriter<StringBuffer>& json_writer) const
{
    to_row().as_json(json_writer);
}

inline row_t TrxLinkData::to_html_row() const
{
    return to_row().to_html_row();
}

inline void TrxLinkData::to_html_template(html_template& t) const
{
    to_row().to_html_template(t);
}

inline TrxLinkData& TrxLinkData::clone_from(const TrxLinkData& other)
{
    *this = other;
    id(-1);
    return *this;
}
