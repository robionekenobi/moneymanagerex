// -*- C++ -*-
//=============================================================================
/**
 *      Copyright: (c) 2013-2026 Guan Lisheng (guanlisheng@gmail.com)
 *      Copyright: (c) 2017-2018 Stefano Giorgio (stef145g)
 *      Copyright: (c) 2022      Mark Whalley (mark@ipx.co.uk)
 *      Copyright: (c) 2026      George Ef (george.a.ef@gmail.com)
 *
 *      TrxSplitTable.h
 *
 *      Interface to database table SPLITTRANSACTIONS_V1
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

// Columns in database table SPLITTRANSACTIONS_V1
struct TrxSplitCol
{
    enum COL_ID
    {
        COL_ID_SPLITTRANSID = 0,
        COL_ID_TRANSID,
        COL_ID_CATEGID,
        COL_ID_SPLITTRANSAMOUNT,
        COL_ID_NOTES,
        COL_ID_size
    };

    static const wxArrayString s_col_name_a;
    static const COL_ID s_primary_id;
    static const wxString s_primary_name;

    static wxString col_id_name(COL_ID col_id) { return s_col_name_a[col_id]; }

    // convenience variables

    static const wxString NAME_SPLITTRANSID;
    static const wxString NAME_TRANSID;
    static const wxString NAME_CATEGID;
    static const wxString NAME_SPLITTRANSAMOUNT;
    static const wxString NAME_NOTES;

    // convenience methods

    static TableClauseV<int64> WHERE_SPLITTRANSID(OP op, const int64& value) {
        return TableClause::WHERE<int64>(NAME_SPLITTRANSID, op, value);
    }

    static TableClauseV<int64> WHERE_TRANSID(OP op, const int64& value) {
        return TableClause::WHERE<int64>(NAME_TRANSID, op, value);
    }

    static TableClauseV<int64> WHERE_CATEGID(OP op, const int64& value) {
        return TableClause::WHERE<int64>(NAME_CATEGID, op, value);
    }

    static TableClauseV<double> WHERE_SPLITTRANSAMOUNT(OP op, const double& value) {
        return TableClause::WHERE<double>(NAME_SPLITTRANSAMOUNT, op, value);
    }

    static TableClauseV<wxString> WHERE_NOTES(OP op, const wxString& value) {
        return TableClause::WHERE<wxString>(NAME_NOTES, op, value);
    }

    // deprecated

    struct SPLITTRANSID : public TableOpV<int64>
    {
        static COL_ID col_id() { return COL_ID_SPLITTRANSID; }
        static wxString col_name() { return s_col_name_a[COL_ID_SPLITTRANSID]; }
        explicit SPLITTRANSID(const int64 &v): TableOpV<int64>(OP_EQ, v) {}
        explicit SPLITTRANSID(OP op, const int64 &v): TableOpV<int64>(op, v) {}
    };

    struct TRANSID : public TableOpV<int64>
    {
        static COL_ID col_id() { return COL_ID_TRANSID; }
        static wxString col_name() { return s_col_name_a[COL_ID_TRANSID]; }
        explicit TRANSID(const int64 &v): TableOpV<int64>(OP_EQ, v) {}
        explicit TRANSID(OP op, const int64 &v): TableOpV<int64>(op, v) {}
    };

    struct CATEGID : public TableOpV<int64>
    {
        static COL_ID col_id() { return COL_ID_CATEGID; }
        static wxString col_name() { return s_col_name_a[COL_ID_CATEGID]; }
        explicit CATEGID(const int64 &v): TableOpV<int64>(OP_EQ, v) {}
        explicit CATEGID(OP op, const int64 &v): TableOpV<int64>(op, v) {}
    };

    struct SPLITTRANSAMOUNT : public TableOpV<double>
    {
        static COL_ID col_id() { return COL_ID_SPLITTRANSAMOUNT; }
        static wxString col_name() { return s_col_name_a[COL_ID_SPLITTRANSAMOUNT]; }
        explicit SPLITTRANSAMOUNT(const double &v): TableOpV<double>(OP_EQ, v) {}
        explicit SPLITTRANSAMOUNT(OP op, const double &v): TableOpV<double>(op, v) {}
    };

    struct NOTES : public TableOpV<wxString>
    {
        static COL_ID col_id() { return COL_ID_NOTES; }
        static wxString col_name() { return s_col_name_a[COL_ID_NOTES]; }
        explicit NOTES(const wxString &v): TableOpV<wxString>(OP_EQ, v) {}
        explicit NOTES(OP op, const wxString &v): TableOpV<wxString>(op, v) {}
    };
};

// A single record in database table SPLITTRANSACTIONS_V1
struct TrxSplitRow
{
    using Col = TrxSplitCol;

    int64 SPLITTRANSID; // primary key
    int64 TRANSID;
    int64 CATEGID;
    double SPLITTRANSAMOUNT;
    wxString NOTES;

    explicit TrxSplitRow();
    explicit TrxSplitRow(wxSQLite3ResultSet& q);
    TrxSplitRow(const TrxSplitRow& other) = default;

    int64 id() const { return SPLITTRANSID; }
    void id(const int64 id) { SPLITTRANSID = id; }
    void to_insert_stmt(wxSQLite3Statement& stmt, int64 id) const;
    void to_update_stmt(wxSQLite3Statement& stmt) const;
    TrxSplitRow& from_select_result(wxSQLite3ResultSet& q);
    wxString to_json() const;
    void as_json(PrettyWriter<StringBuffer>& json_writer) const;
    row_t to_html_row() const;
    void to_html_template(html_template& t) const;
    void destroy() { delete this; }

    TrxSplitRow& clone_from(const TrxSplitRow& other);
    bool equals(const TrxSplitRow* other) const;
    bool operator< (const TrxSplitRow& other) const { return id() < other.id(); }
    bool operator< (const TrxSplitRow* other) const { return id() < other->id(); }

    template<typename C>
    bool match(const C&)
    {
        return false;
    }

    // TODO: check if col.m_operator == OP_EQ

    bool match(const Col::SPLITTRANSID& col)
    {
        return SPLITTRANSID == col.m_value;
    }

    bool match(const Col::TRANSID& col)
    {
        return TRANSID == col.m_value;
    }

    bool match(const Col::CATEGID& col)
    {
        return CATEGID == col.m_value;
    }

    bool match(const Col::SPLITTRANSAMOUNT& col)
    {
        return SPLITTRANSAMOUNT == col.m_value;
    }

    bool match(const Col::NOTES& col)
    {
        return NOTES.CmpNoCase(col.m_value) == 0;
    }

    template<typename Arg1, typename... Args>
    bool match(const Arg1& arg1, const Args&... args)
    {
        return (match(arg1) && ... && match(args));
    }

    struct SorterBySPLITTRANSID
    {
        bool operator()(const TrxSplitRow& x, const TrxSplitRow& y)
        {
            return x.SPLITTRANSID < y.SPLITTRANSID;
        }
    };

    struct SorterByTRANSID
    {
        bool operator()(const TrxSplitRow& x, const TrxSplitRow& y)
        {
            return x.TRANSID < y.TRANSID;
        }
    };

    struct SorterByCATEGID
    {
        bool operator()(const TrxSplitRow& x, const TrxSplitRow& y)
        {
            return x.CATEGID < y.CATEGID;
        }
    };

    struct SorterBySPLITTRANSAMOUNT
    {
        bool operator()(const TrxSplitRow& x, const TrxSplitRow& y)
        {
            return x.SPLITTRANSAMOUNT < y.SPLITTRANSAMOUNT;
        }
    };

    struct SorterByNOTES
    {
        bool operator()(const TrxSplitRow& x, const TrxSplitRow& y)
        {
            return x.NOTES < y.NOTES;
        }
    };
};

// Interface to database table SPLITTRANSACTIONS_V1
struct TrxSplitTable : public TableBase
{
    using Row = TrxSplitRow;
    using Col = typename Row::Col;

    TrxSplitTable();
    ~TrxSplitTable() {}
};

inline TrxSplitRow::TrxSplitRow(wxSQLite3ResultSet& q)
{
    from_select_result(q);
}

inline void TrxSplitRow::to_update_stmt(wxSQLite3Statement& stmt) const
{
    to_insert_stmt(stmt, id());
}

inline TrxSplitRow& TrxSplitRow::clone_from(const TrxSplitRow& other)
{
    *this = other;
    id(-1);
    return *this;
}
