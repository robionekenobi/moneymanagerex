// -*- C++ -*-
//=============================================================================
/**
 *      Copyright: (c) 2013-2026 Guan Lisheng (guanlisheng@gmail.com)
 *      Copyright: (c) 2017-2018 Stefano Giorgio (stef145g)
 *      Copyright: (c) 2022      Mark Whalley (mark@ipx.co.uk)
 *      Copyright: (c) 2026      George Ef (george.a.ef@gmail.com)
 *
 *      TagTable.h
 *
 *      Interface to database table TAG_V1
 *
 *      @author [sqlite2cpp.py]
 *
 *      Revision History:
 *          AUTO GENERATED at 2026-02-25 08:58:12.230056.
 *          DO NOT EDIT!
 */
//=============================================================================

#pragma once

#include "_TableBase.h"

// Columns in database table TAG_V1
struct TagCol
{
    enum COL_ID
    {
        COL_ID_TAGID = 0,
        COL_ID_TAGNAME,
        COL_ID_ACTIVE,
        COL_ID_size
    };

    static const wxArrayString COL_NAME_A;
    static const COL_ID PRIMARY_ID;
    static const wxString PRIMARY_NAME;

    static wxString col_name(COL_ID col_id) { return COL_NAME_A[col_id]; }

    struct TAGID : public TableOpV<int64>
    {
        static COL_ID col_id() { return COL_ID_TAGID; }
        static wxString col_name() { return COL_NAME_A[COL_ID_TAGID]; }
        explicit TAGID(const int64 &v): TableOpV<int64>(OP_EQ, v) {}
        explicit TAGID(OP op, const int64 &v): TableOpV<int64>(op, v) {}
    };

    struct TAGNAME : public TableOpV<wxString>
    {
        static COL_ID col_id() { return COL_ID_TAGNAME; }
        static wxString col_name() { return COL_NAME_A[COL_ID_TAGNAME]; }
        explicit TAGNAME(const wxString &v): TableOpV<wxString>(OP_EQ, v) {}
        explicit TAGNAME(OP op, const wxString &v): TableOpV<wxString>(op, v) {}
    };

    struct ACTIVE : public TableOpV<int64>
    {
        static COL_ID col_id() { return COL_ID_ACTIVE; }
        static wxString col_name() { return COL_NAME_A[COL_ID_ACTIVE]; }
        explicit ACTIVE(const int64 &v): TableOpV<int64>(OP_EQ, v) {}
        explicit ACTIVE(OP op, const int64 &v): TableOpV<int64>(op, v) {}
    };
};

// A single record in database table TAG_V1
struct TagRow
{
    using Col = TagCol;

    int64 TAGID; // primary key
    wxString TAGNAME;
    int64 ACTIVE;

    explicit TagRow();
    explicit TagRow(wxSQLite3ResultSet& q);
    TagRow(const TagRow& other) = default;

    int64 id() const { return TAGID; }
    void id(const int64 id) { TAGID = id; }
    void to_insert_stmt(wxSQLite3Statement& stmt, int64 id) const;
    void to_update_stmt(wxSQLite3Statement& stmt) const;
    TagRow& from_select_result(wxSQLite3ResultSet& q);
    wxString to_json() const;
    void as_json(PrettyWriter<StringBuffer>& json_writer) const;
    row_t to_html_row() const;
    void to_html_template(html_template& t) const;
    void destroy() { delete this; }

    TagRow& clone_from(const TagRow& other);
    bool equals(const TagRow* other) const;
    bool operator< (const TagRow& other) const { return id() < other.id(); }
    bool operator< (const TagRow* other) const { return id() < other->id(); }

    template<typename C>
    bool match(const C&)
    {
        return false;
    }

    // TODO: check if col.m_operator == OP_EQ

    bool match(const Col::TAGID& col)
    {
        return TAGID == col.m_value;
    }

    bool match(const Col::TAGNAME& col)
    {
        return TAGNAME.CmpNoCase(col.m_value) == 0;
    }

    bool match(const Col::ACTIVE& col)
    {
        return ACTIVE == col.m_value;
    }

    template<typename Arg1, typename... Args>
    bool match(const Arg1& arg1, const Args&... args)
    {
        return (match(arg1) && ... && match(args));
    }

    struct SorterByTAGID
    {
        bool operator()(const TagRow& x, const TagRow& y)
        {
            return x.TAGID < y.TAGID;
        }
    };

    struct SorterByTAGNAME
    {
        bool operator()(const TagRow& x, const TagRow& y)
        {
            return x.TAGNAME < y.TAGNAME;
        }
    };

    struct SorterByACTIVE
    {
        bool operator()(const TagRow& x, const TagRow& y)
        {
            return x.ACTIVE < y.ACTIVE;
        }
    };
};

// Interface to database table TAG_V1
struct TagTable : public TableBase
{
    using Row = TagRow;
    using Col = typename Row::Col;

    TagTable();
    ~TagTable() {}
};

inline TagRow::TagRow(wxSQLite3ResultSet& q)
{
    from_select_result(q);
}

inline void TagRow::to_update_stmt(wxSQLite3Statement& stmt) const
{
    to_insert_stmt(stmt, id());
}

inline TagRow& TagRow::clone_from(const TagRow& other)
{
    *this = other;
    id(-1);
    return *this;
}
