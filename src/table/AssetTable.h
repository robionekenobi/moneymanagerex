// -*- C++ -*-
//=============================================================================
/**
 *      Copyright: (c) 2013-2026 Guan Lisheng (guanlisheng@gmail.com)
 *      Copyright: (c) 2017-2018 Stefano Giorgio (stef145g)
 *      Copyright: (c) 2022      Mark Whalley (mark@ipx.co.uk)
 *      Copyright: (c) 2026      George Ef (george.a.ef@gmail.com)
 *
 *      AssetTable.h
 *
 *      Interface to database table ASSETS_V1
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

// Columns in database table ASSETS_V1
struct AssetCol
{
    enum COL_ID
    {
        COL_ID_ASSETID = 0,
        COL_ID_STARTDATE,
        COL_ID_ASSETNAME,
        COL_ID_ASSETSTATUS,
        COL_ID_CURRENCYID,
        COL_ID_VALUECHANGEMODE,
        COL_ID_VALUE,
        COL_ID_VALUECHANGE,
        COL_ID_NOTES,
        COL_ID_VALUECHANGERATE,
        COL_ID_ASSETTYPE,
        COL_ID_size
    };

    static const wxArrayString COL_NAME_A;
    static const COL_ID PRIMARY_ID;
    static const wxString PRIMARY_NAME;

    static wxString col_name(COL_ID col_id) { return COL_NAME_A[col_id]; }

    struct ASSETID : public TableOpV<int64>
    {
        static COL_ID col_id() { return COL_ID_ASSETID; }
        static wxString col_name() { return COL_NAME_A[COL_ID_ASSETID]; }
        explicit ASSETID(const int64 &v): TableOpV<int64>(OP_EQ, v) {}
        explicit ASSETID(OP op, const int64 &v): TableOpV<int64>(op, v) {}
    };

    struct STARTDATE : public TableOpV<wxString>
    {
        static COL_ID col_id() { return COL_ID_STARTDATE; }
        static wxString col_name() { return COL_NAME_A[COL_ID_STARTDATE]; }
        explicit STARTDATE(const wxString &v): TableOpV<wxString>(OP_EQ, v) {}
        explicit STARTDATE(OP op, const wxString &v): TableOpV<wxString>(op, v) {}
    };

    struct ASSETNAME : public TableOpV<wxString>
    {
        static COL_ID col_id() { return COL_ID_ASSETNAME; }
        static wxString col_name() { return COL_NAME_A[COL_ID_ASSETNAME]; }
        explicit ASSETNAME(const wxString &v): TableOpV<wxString>(OP_EQ, v) {}
        explicit ASSETNAME(OP op, const wxString &v): TableOpV<wxString>(op, v) {}
    };

    struct ASSETSTATUS : public TableOpV<wxString>
    {
        static COL_ID col_id() { return COL_ID_ASSETSTATUS; }
        static wxString col_name() { return COL_NAME_A[COL_ID_ASSETSTATUS]; }
        explicit ASSETSTATUS(const wxString &v): TableOpV<wxString>(OP_EQ, v) {}
        explicit ASSETSTATUS(OP op, const wxString &v): TableOpV<wxString>(op, v) {}
    };

    struct CURRENCYID : public TableOpV<int64>
    {
        static COL_ID col_id() { return COL_ID_CURRENCYID; }
        static wxString col_name() { return COL_NAME_A[COL_ID_CURRENCYID]; }
        explicit CURRENCYID(const int64 &v): TableOpV<int64>(OP_EQ, v) {}
        explicit CURRENCYID(OP op, const int64 &v): TableOpV<int64>(op, v) {}
    };

    struct VALUECHANGEMODE : public TableOpV<wxString>
    {
        static COL_ID col_id() { return COL_ID_VALUECHANGEMODE; }
        static wxString col_name() { return COL_NAME_A[COL_ID_VALUECHANGEMODE]; }
        explicit VALUECHANGEMODE(const wxString &v): TableOpV<wxString>(OP_EQ, v) {}
        explicit VALUECHANGEMODE(OP op, const wxString &v): TableOpV<wxString>(op, v) {}
    };

    struct VALUE : public TableOpV<double>
    {
        static COL_ID col_id() { return COL_ID_VALUE; }
        static wxString col_name() { return COL_NAME_A[COL_ID_VALUE]; }
        explicit VALUE(const double &v): TableOpV<double>(OP_EQ, v) {}
        explicit VALUE(OP op, const double &v): TableOpV<double>(op, v) {}
    };

    struct VALUECHANGE : public TableOpV<wxString>
    {
        static COL_ID col_id() { return COL_ID_VALUECHANGE; }
        static wxString col_name() { return COL_NAME_A[COL_ID_VALUECHANGE]; }
        explicit VALUECHANGE(const wxString &v): TableOpV<wxString>(OP_EQ, v) {}
        explicit VALUECHANGE(OP op, const wxString &v): TableOpV<wxString>(op, v) {}
    };

    struct NOTES : public TableOpV<wxString>
    {
        static COL_ID col_id() { return COL_ID_NOTES; }
        static wxString col_name() { return COL_NAME_A[COL_ID_NOTES]; }
        explicit NOTES(const wxString &v): TableOpV<wxString>(OP_EQ, v) {}
        explicit NOTES(OP op, const wxString &v): TableOpV<wxString>(op, v) {}
    };

    struct VALUECHANGERATE : public TableOpV<double>
    {
        static COL_ID col_id() { return COL_ID_VALUECHANGERATE; }
        static wxString col_name() { return COL_NAME_A[COL_ID_VALUECHANGERATE]; }
        explicit VALUECHANGERATE(const double &v): TableOpV<double>(OP_EQ, v) {}
        explicit VALUECHANGERATE(OP op, const double &v): TableOpV<double>(op, v) {}
    };

    struct ASSETTYPE : public TableOpV<wxString>
    {
        static COL_ID col_id() { return COL_ID_ASSETTYPE; }
        static wxString col_name() { return COL_NAME_A[COL_ID_ASSETTYPE]; }
        explicit ASSETTYPE(const wxString &v): TableOpV<wxString>(OP_EQ, v) {}
        explicit ASSETTYPE(OP op, const wxString &v): TableOpV<wxString>(op, v) {}
    };
};

// A single record in database table ASSETS_V1
struct AssetRow
{
    using Col = AssetCol;

    int64 ASSETID; // primary key
    wxString STARTDATE;
    wxString ASSETNAME;
    wxString ASSETSTATUS;
    int64 CURRENCYID;
    wxString VALUECHANGEMODE;
    double VALUE;
    wxString VALUECHANGE;
    wxString NOTES;
    double VALUECHANGERATE;
    wxString ASSETTYPE;

    explicit AssetRow();
    explicit AssetRow(wxSQLite3ResultSet& q);
    AssetRow(const AssetRow& other) = default;

    int64 id() const { return ASSETID; }
    void id(const int64 id) { ASSETID = id; }
    void to_insert_stmt(wxSQLite3Statement& stmt, int64 id) const;
    void to_update_stmt(wxSQLite3Statement& stmt) const;
    AssetRow& from_select_result(wxSQLite3ResultSet& q);
    wxString to_json() const;
    void as_json(PrettyWriter<StringBuffer>& json_writer) const;
    row_t to_html_row() const;
    void to_html_template(html_template& t) const;
    void destroy() { delete this; }

    AssetRow& clone_from(const AssetRow& other);
    bool equals(const AssetRow* other) const;
    bool operator< (const AssetRow& other) const { return id() < other.id(); }
    bool operator< (const AssetRow* other) const { return id() < other->id(); }

    template<typename C>
    bool match(const C&)
    {
        return false;
    }

    // TODO: check if col.m_operator == OP_EQ

    bool match(const Col::ASSETID& col)
    {
        return ASSETID == col.m_value;
    }

    bool match(const Col::STARTDATE& col)
    {
        return STARTDATE.CmpNoCase(col.m_value) == 0;
    }

    bool match(const Col::ASSETNAME& col)
    {
        return ASSETNAME.CmpNoCase(col.m_value) == 0;
    }

    bool match(const Col::ASSETSTATUS& col)
    {
        return ASSETSTATUS.CmpNoCase(col.m_value) == 0;
    }

    bool match(const Col::CURRENCYID& col)
    {
        return CURRENCYID == col.m_value;
    }

    bool match(const Col::VALUECHANGEMODE& col)
    {
        return VALUECHANGEMODE.CmpNoCase(col.m_value) == 0;
    }

    bool match(const Col::VALUE& col)
    {
        return VALUE == col.m_value;
    }

    bool match(const Col::VALUECHANGE& col)
    {
        return VALUECHANGE.CmpNoCase(col.m_value) == 0;
    }

    bool match(const Col::NOTES& col)
    {
        return NOTES.CmpNoCase(col.m_value) == 0;
    }

    bool match(const Col::VALUECHANGERATE& col)
    {
        return VALUECHANGERATE == col.m_value;
    }

    bool match(const Col::ASSETTYPE& col)
    {
        return ASSETTYPE.CmpNoCase(col.m_value) == 0;
    }

    template<typename Arg1, typename... Args>
    bool match(const Arg1& arg1, const Args&... args)
    {
        return (match(arg1) && ... && match(args));
    }

    struct SorterByASSETID
    {
        bool operator()(const AssetRow& x, const AssetRow& y)
        {
            return x.ASSETID < y.ASSETID;
        }
    };

    struct SorterBySTARTDATE
    {
        bool operator()(const AssetRow& x, const AssetRow& y)
        {
            return x.STARTDATE < y.STARTDATE;
        }
    };

    struct SorterByASSETNAME
    {
        bool operator()(const AssetRow& x, const AssetRow& y)
        {
            return x.ASSETNAME < y.ASSETNAME;
        }
    };

    struct SorterByASSETSTATUS
    {
        bool operator()(const AssetRow& x, const AssetRow& y)
        {
            return x.ASSETSTATUS < y.ASSETSTATUS;
        }
    };

    struct SorterByCURRENCYID
    {
        bool operator()(const AssetRow& x, const AssetRow& y)
        {
            return x.CURRENCYID < y.CURRENCYID;
        }
    };

    struct SorterByVALUECHANGEMODE
    {
        bool operator()(const AssetRow& x, const AssetRow& y)
        {
            return x.VALUECHANGEMODE < y.VALUECHANGEMODE;
        }
    };

    struct SorterByVALUE
    {
        bool operator()(const AssetRow& x, const AssetRow& y)
        {
            return x.VALUE < y.VALUE;
        }
    };

    struct SorterByVALUECHANGE
    {
        bool operator()(const AssetRow& x, const AssetRow& y)
        {
            return x.VALUECHANGE < y.VALUECHANGE;
        }
    };

    struct SorterByNOTES
    {
        bool operator()(const AssetRow& x, const AssetRow& y)
        {
            return x.NOTES < y.NOTES;
        }
    };

    struct SorterByVALUECHANGERATE
    {
        bool operator()(const AssetRow& x, const AssetRow& y)
        {
            return x.VALUECHANGERATE < y.VALUECHANGERATE;
        }
    };

    struct SorterByASSETTYPE
    {
        bool operator()(const AssetRow& x, const AssetRow& y)
        {
            return x.ASSETTYPE < y.ASSETTYPE;
        }
    };
};

// Interface to database table ASSETS_V1
struct AssetTable : public TableBase
{
    using Row = AssetRow;
    using Col = typename Row::Col;

    AssetTable();
    ~AssetTable() {}
};

inline AssetRow::AssetRow(wxSQLite3ResultSet& q)
{
    from_select_result(q);
}

inline void AssetRow::to_update_stmt(wxSQLite3Statement& stmt) const
{
    to_insert_stmt(stmt, id());
}

inline AssetRow& AssetRow::clone_from(const AssetRow& other)
{
    *this = other;
    id(-1);
    return *this;
}
