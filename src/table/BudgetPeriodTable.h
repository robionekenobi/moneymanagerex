// -*- C++ -*-
//=============================================================================
/**
 *      Copyright: (c) 2013-2026 Guan Lisheng (guanlisheng@gmail.com)
 *      Copyright: (c) 2017-2018 Stefano Giorgio (stef145g)
 *      Copyright: (c) 2022      Mark Whalley (mark@ipx.co.uk)
 *      Copyright: (c) 2026      George Ef (george.a.ef@gmail.com)
 *
 *      BudgetPeriodTable.h
 *
 *      Interface to database table BUDGETYEAR_V1
 *
 *      @author [sqlite2cpp.py]
 *
 *      Revision History:
 *          AUTO GENERATED at 2026-04-11 13:27:18.900578.
 *          DO NOT EDIT!
 */
//=============================================================================

#pragma once

#include "_TableBase.h"

// Columns in database table BUDGETYEAR_V1
struct BudgetPeriodCol
{
    enum COL_ID
    {
        COL_ID_BUDGETYEARID = 0,
        COL_ID_BUDGETYEARNAME,
        COL_ID_size
    };

    static const wxArrayString s_col_name_a;
    static const COL_ID s_primary_id;
    static const wxString s_primary_name;

    static wxString col_id_name(COL_ID col_id) { return s_col_name_a[col_id]; }

    // convenience variables

    static const wxString NAME_BUDGETYEARID;
    static const wxString NAME_BUDGETYEARNAME;

    // convenience methods

    static TableClauseV<int64> WHERE_BUDGETYEARID(OP op, const int64& value) {
        return TableClause::WHERE<int64>(NAME_BUDGETYEARID, op, value);
    }

    static TableClauseV<wxString> WHERE_BUDGETYEARNAME(OP op, const wxString& value) {
        return TableClause::WHERE<wxString>(NAME_BUDGETYEARNAME, op, value);
    }

    // deprecated

    struct BUDGETYEARID : public TableOpV<int64>
    {
        static COL_ID col_id() { return COL_ID_BUDGETYEARID; }
        static wxString col_name() { return s_col_name_a[COL_ID_BUDGETYEARID]; }
        explicit BUDGETYEARID(const int64 &v): TableOpV<int64>(OP_EQ, v) {}
        explicit BUDGETYEARID(OP op, const int64 &v): TableOpV<int64>(op, v) {}
    };

    struct BUDGETYEARNAME : public TableOpV<wxString>
    {
        static COL_ID col_id() { return COL_ID_BUDGETYEARNAME; }
        static wxString col_name() { return s_col_name_a[COL_ID_BUDGETYEARNAME]; }
        explicit BUDGETYEARNAME(const wxString &v): TableOpV<wxString>(OP_EQ, v) {}
        explicit BUDGETYEARNAME(OP op, const wxString &v): TableOpV<wxString>(op, v) {}
    };
};

// A single record in database table BUDGETYEAR_V1
struct BudgetPeriodRow
{
    using Col = BudgetPeriodCol;

    int64 BUDGETYEARID; // primary key
    wxString BUDGETYEARNAME;

    explicit BudgetPeriodRow();
    explicit BudgetPeriodRow(wxSQLite3ResultSet& q);
    BudgetPeriodRow(const BudgetPeriodRow& other) = default;

    int64 id() const { return BUDGETYEARID; }
    void id(const int64 id) { BUDGETYEARID = id; }
    void to_insert_stmt(wxSQLite3Statement& stmt, int64 id) const;
    void to_update_stmt(wxSQLite3Statement& stmt) const;
    BudgetPeriodRow& from_select_result(wxSQLite3ResultSet& q);
    wxString to_json() const;
    void as_json(PrettyWriter<StringBuffer>& json_writer) const;
    row_t to_html_row() const;
    void to_html_template(html_template& t) const;
    void destroy() { delete this; }

    BudgetPeriodRow& clone_from(const BudgetPeriodRow& other);
    bool equals(const BudgetPeriodRow* other) const;
    bool operator< (const BudgetPeriodRow& other) const { return id() < other.id(); }
    bool operator< (const BudgetPeriodRow* other) const { return id() < other->id(); }

    template<typename C>
    bool match(const C&)
    {
        return false;
    }

    // TODO: check if col.m_operator == OP_EQ

    bool match(const Col::BUDGETYEARID& col)
    {
        return BUDGETYEARID == col.m_value;
    }

    bool match(const Col::BUDGETYEARNAME& col)
    {
        return BUDGETYEARNAME.CmpNoCase(col.m_value) == 0;
    }

    template<typename Arg1, typename... Args>
    bool match(const Arg1& arg1, const Args&... args)
    {
        return (match(arg1) && ... && match(args));
    }

    struct SorterByBUDGETYEARID
    {
        bool operator()(const BudgetPeriodRow& x, const BudgetPeriodRow& y)
        {
            return x.BUDGETYEARID < y.BUDGETYEARID;
        }
    };

    struct SorterByBUDGETYEARNAME
    {
        bool operator()(const BudgetPeriodRow& x, const BudgetPeriodRow& y)
        {
            return x.BUDGETYEARNAME < y.BUDGETYEARNAME;
        }
    };
};

// Interface to database table BUDGETYEAR_V1
struct BudgetPeriodTable : public TableBase
{
    using Row = BudgetPeriodRow;
    using Col = typename Row::Col;

    BudgetPeriodTable();
    ~BudgetPeriodTable() {}
};

inline BudgetPeriodRow::BudgetPeriodRow(wxSQLite3ResultSet& q)
{
    from_select_result(q);
}

inline void BudgetPeriodRow::to_update_stmt(wxSQLite3Statement& stmt) const
{
    to_insert_stmt(stmt, id());
}

inline BudgetPeriodRow& BudgetPeriodRow::clone_from(const BudgetPeriodRow& other)
{
    *this = other;
    id(-1);
    return *this;
}
