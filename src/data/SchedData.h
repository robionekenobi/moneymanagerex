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
// This is only sample code re-used from "table/SchedTable.h".
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
#include "table/SchedTable.h"

// User-friendly representation of a record in table BILLSDEPOSITS_V1.
struct SchedData
{
    int64 BDID; // primary key
    int64 ACCOUNTID;
    int64 TOACCOUNTID;
    int64 PAYEEID;
    wxString TRANSCODE;
    double TRANSAMOUNT;
    wxString STATUS;
    wxString TRANSACTIONNUMBER;
    wxString NOTES;
    int64 CATEGID;
    wxString TRANSDATE;
    int64 FOLLOWUPID;
    double TOTRANSAMOUNT;
    int64 REPEATS;
    wxString NEXTOCCURRENCEDATE;
    int64 NUMOCCURRENCES;
    int64 COLOR;

    explicit SchedData();
    explicit SchedData(wxSQLite3ResultSet& q);
    SchedData(const SchedData& other) = default;

    int64 id() const { return BDID; }
    void id(const int64 id) { BDID = id; }
    SchedRow to_row() const;
    SchedData& from_row(const SchedRow& row);
    void to_insert_stmt(wxSQLite3Statement& stmt, int64 id) const;
    void to_update_stmt(wxSQLite3Statement& stmt) const;
    SchedData& from_select_result(wxSQLite3ResultSet& q);
    wxString to_json() const;
    void as_json(PrettyWriter<StringBuffer>& json_writer) const;
    row_t to_html_row() const;
    void to_html_template(html_template& t) const;
    void destroy() { delete this; }

    SchedData& clone_from(const SchedData& other);
    bool equals(const SchedData* other) const;
    bool operator< (const SchedData& other) const { return id() < other.id(); }
    bool operator< (const SchedData* other) const { return id() < other->id(); }

    struct SorterByBDID
    {
        bool operator()(const SchedData& x, const SchedData& y)
        {
            return x.BDID < y.BDID;
        }
    };

    struct SorterByACCOUNTID
    {
        bool operator()(const SchedData& x, const SchedData& y)
        {
            return x.ACCOUNTID < y.ACCOUNTID;
        }
    };

    struct SorterByTOACCOUNTID
    {
        bool operator()(const SchedData& x, const SchedData& y)
        {
            return x.TOACCOUNTID < y.TOACCOUNTID;
        }
    };

    struct SorterByPAYEEID
    {
        bool operator()(const SchedData& x, const SchedData& y)
        {
            return x.PAYEEID < y.PAYEEID;
        }
    };

    struct SorterByTRANSCODE
    {
        bool operator()(const SchedData& x, const SchedData& y)
        {
            return x.TRANSCODE < y.TRANSCODE;
        }
    };

    struct SorterByTRANSAMOUNT
    {
        bool operator()(const SchedData& x, const SchedData& y)
        {
            return x.TRANSAMOUNT < y.TRANSAMOUNT;
        }
    };

    struct SorterBySTATUS
    {
        bool operator()(const SchedData& x, const SchedData& y)
        {
            return x.STATUS < y.STATUS;
        }
    };

    struct SorterByTRANSACTIONNUMBER
    {
        bool operator()(const SchedData& x, const SchedData& y)
        {
            return x.TRANSACTIONNUMBER < y.TRANSACTIONNUMBER;
        }
    };

    struct SorterByNOTES
    {
        bool operator()(const SchedData& x, const SchedData& y)
        {
            return x.NOTES < y.NOTES;
        }
    };

    struct SorterByCATEGID
    {
        bool operator()(const SchedData& x, const SchedData& y)
        {
            return x.CATEGID < y.CATEGID;
        }
    };

    struct SorterByTRANSDATE
    {
        bool operator()(const SchedData& x, const SchedData& y)
        {
            return x.TRANSDATE < y.TRANSDATE;
        }
    };

    struct SorterByFOLLOWUPID
    {
        bool operator()(const SchedData& x, const SchedData& y)
        {
            return x.FOLLOWUPID < y.FOLLOWUPID;
        }
    };

    struct SorterByTOTRANSAMOUNT
    {
        bool operator()(const SchedData& x, const SchedData& y)
        {
            return x.TOTRANSAMOUNT < y.TOTRANSAMOUNT;
        }
    };

    struct SorterByREPEATS
    {
        bool operator()(const SchedData& x, const SchedData& y)
        {
            return x.REPEATS < y.REPEATS;
        }
    };

    struct SorterByNEXTOCCURRENCEDATE
    {
        bool operator()(const SchedData& x, const SchedData& y)
        {
            return x.NEXTOCCURRENCEDATE < y.NEXTOCCURRENCEDATE;
        }
    };

    struct SorterByNUMOCCURRENCES
    {
        bool operator()(const SchedData& x, const SchedData& y)
        {
            return x.NUMOCCURRENCES < y.NUMOCCURRENCES;
        }
    };

    struct SorterByCOLOR
    {
        bool operator()(const SchedData& x, const SchedData& y)
        {
            return x.COLOR < y.COLOR;
        }
    };
};

inline SchedData::SchedData(wxSQLite3ResultSet& q) :
    SchedData()
{
    from_select_result(q);
}

inline void SchedData::to_insert_stmt(wxSQLite3Statement& stmt, int64 id) const
{
    to_row().to_insert_stmt(stmt, id);
}

inline void SchedData::to_update_stmt(wxSQLite3Statement& stmt) const
{
    to_row().to_update_stmt(stmt);
}

inline SchedData& SchedData::from_select_result(wxSQLite3ResultSet& q)
{
    return from_row(SchedRow().from_select_result(q));
}

inline wxString SchedData::to_json() const
{
    return to_row().to_json();
}

inline void SchedData::as_json(PrettyWriter<StringBuffer>& json_writer) const
{
    to_row().as_json(json_writer);
}

inline row_t SchedData::to_html_row() const
{
    return to_row().to_html_row();
}

inline void SchedData::to_html_template(html_template& t) const
{
    to_row().to_html_template(t);
}

inline SchedData& SchedData::clone_from(const SchedData& other)
{
    *this = other;
    id(-1);
    return *this;
}
