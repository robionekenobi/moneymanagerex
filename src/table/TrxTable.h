// -*- C++ -*-
//=============================================================================
/**
 *      Copyright: (c) 2013-2026 Guan Lisheng (guanlisheng@gmail.com)
 *      Copyright: (c) 2017-2018 Stefano Giorgio (stef145g)
 *      Copyright: (c) 2022      Mark Whalley (mark@ipx.co.uk)
 *      Copyright: (c) 2026      George Ef (george.a.ef@gmail.com)
 *
 *      TrxTable.h
 *
 *      Interface to database table CHECKINGACCOUNT_V1
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

// Columns in database table CHECKINGACCOUNT_V1
struct TrxCol
{
    enum COL_ID
    {
        COL_ID_TRANSID = 0,
        COL_ID_ACCOUNTID,
        COL_ID_TOACCOUNTID,
        COL_ID_PAYEEID,
        COL_ID_TRANSCODE,
        COL_ID_TRANSAMOUNT,
        COL_ID_STATUS,
        COL_ID_TRANSACTIONNUMBER,
        COL_ID_NOTES,
        COL_ID_CATEGID,
        COL_ID_TRANSDATE,
        COL_ID_LASTUPDATEDTIME,
        COL_ID_DELETEDTIME,
        COL_ID_FOLLOWUPID,
        COL_ID_TOTRANSAMOUNT,
        COL_ID_COLOR,
        COL_ID_size
    };

    static const wxArrayString COL_NAME_A;
    static const COL_ID PRIMARY_ID;
    static const wxString PRIMARY_NAME;

    static wxString col_name(COL_ID col_id) { return COL_NAME_A[col_id]; }

    struct TRANSID : public TableOpV<int64>
    {
        static COL_ID col_id() { return COL_ID_TRANSID; }
        static wxString col_name() { return COL_NAME_A[COL_ID_TRANSID]; }
        explicit TRANSID(const int64 &v): TableOpV<int64>(OP_EQ, v) {}
        explicit TRANSID(OP op, const int64 &v): TableOpV<int64>(op, v) {}
    };

    struct ACCOUNTID : public TableOpV<int64>
    {
        static COL_ID col_id() { return COL_ID_ACCOUNTID; }
        static wxString col_name() { return COL_NAME_A[COL_ID_ACCOUNTID]; }
        explicit ACCOUNTID(const int64 &v): TableOpV<int64>(OP_EQ, v) {}
        explicit ACCOUNTID(OP op, const int64 &v): TableOpV<int64>(op, v) {}
    };

    struct TOACCOUNTID : public TableOpV<int64>
    {
        static COL_ID col_id() { return COL_ID_TOACCOUNTID; }
        static wxString col_name() { return COL_NAME_A[COL_ID_TOACCOUNTID]; }
        explicit TOACCOUNTID(const int64 &v): TableOpV<int64>(OP_EQ, v) {}
        explicit TOACCOUNTID(OP op, const int64 &v): TableOpV<int64>(op, v) {}
    };

    struct PAYEEID : public TableOpV<int64>
    {
        static COL_ID col_id() { return COL_ID_PAYEEID; }
        static wxString col_name() { return COL_NAME_A[COL_ID_PAYEEID]; }
        explicit PAYEEID(const int64 &v): TableOpV<int64>(OP_EQ, v) {}
        explicit PAYEEID(OP op, const int64 &v): TableOpV<int64>(op, v) {}
    };

    struct TRANSCODE : public TableOpV<wxString>
    {
        static COL_ID col_id() { return COL_ID_TRANSCODE; }
        static wxString col_name() { return COL_NAME_A[COL_ID_TRANSCODE]; }
        explicit TRANSCODE(const wxString &v): TableOpV<wxString>(OP_EQ, v) {}
        explicit TRANSCODE(OP op, const wxString &v): TableOpV<wxString>(op, v) {}
    };

    struct TRANSAMOUNT : public TableOpV<double>
    {
        static COL_ID col_id() { return COL_ID_TRANSAMOUNT; }
        static wxString col_name() { return COL_NAME_A[COL_ID_TRANSAMOUNT]; }
        explicit TRANSAMOUNT(const double &v): TableOpV<double>(OP_EQ, v) {}
        explicit TRANSAMOUNT(OP op, const double &v): TableOpV<double>(op, v) {}
    };

    struct STATUS : public TableOpV<wxString>
    {
        static COL_ID col_id() { return COL_ID_STATUS; }
        static wxString col_name() { return COL_NAME_A[COL_ID_STATUS]; }
        explicit STATUS(const wxString &v): TableOpV<wxString>(OP_EQ, v) {}
        explicit STATUS(OP op, const wxString &v): TableOpV<wxString>(op, v) {}
    };

    struct TRANSACTIONNUMBER : public TableOpV<wxString>
    {
        static COL_ID col_id() { return COL_ID_TRANSACTIONNUMBER; }
        static wxString col_name() { return COL_NAME_A[COL_ID_TRANSACTIONNUMBER]; }
        explicit TRANSACTIONNUMBER(const wxString &v): TableOpV<wxString>(OP_EQ, v) {}
        explicit TRANSACTIONNUMBER(OP op, const wxString &v): TableOpV<wxString>(op, v) {}
    };

    struct NOTES : public TableOpV<wxString>
    {
        static COL_ID col_id() { return COL_ID_NOTES; }
        static wxString col_name() { return COL_NAME_A[COL_ID_NOTES]; }
        explicit NOTES(const wxString &v): TableOpV<wxString>(OP_EQ, v) {}
        explicit NOTES(OP op, const wxString &v): TableOpV<wxString>(op, v) {}
    };

    struct CATEGID : public TableOpV<int64>
    {
        static COL_ID col_id() { return COL_ID_CATEGID; }
        static wxString col_name() { return COL_NAME_A[COL_ID_CATEGID]; }
        explicit CATEGID(const int64 &v): TableOpV<int64>(OP_EQ, v) {}
        explicit CATEGID(OP op, const int64 &v): TableOpV<int64>(op, v) {}
    };

    struct TRANSDATE : public TableOpV<wxString>
    {
        static COL_ID col_id() { return COL_ID_TRANSDATE; }
        static wxString col_name() { return COL_NAME_A[COL_ID_TRANSDATE]; }
        explicit TRANSDATE(const wxString &v): TableOpV<wxString>(OP_EQ, v) {}
        explicit TRANSDATE(OP op, const wxString &v): TableOpV<wxString>(op, v) {}
    };

    struct LASTUPDATEDTIME : public TableOpV<wxString>
    {
        static COL_ID col_id() { return COL_ID_LASTUPDATEDTIME; }
        static wxString col_name() { return COL_NAME_A[COL_ID_LASTUPDATEDTIME]; }
        explicit LASTUPDATEDTIME(const wxString &v): TableOpV<wxString>(OP_EQ, v) {}
        explicit LASTUPDATEDTIME(OP op, const wxString &v): TableOpV<wxString>(op, v) {}
    };

    struct DELETEDTIME : public TableOpV<wxString>
    {
        static COL_ID col_id() { return COL_ID_DELETEDTIME; }
        static wxString col_name() { return COL_NAME_A[COL_ID_DELETEDTIME]; }
        explicit DELETEDTIME(const wxString &v): TableOpV<wxString>(OP_EQ, v) {}
        explicit DELETEDTIME(OP op, const wxString &v): TableOpV<wxString>(op, v) {}
    };

    struct FOLLOWUPID : public TableOpV<int64>
    {
        static COL_ID col_id() { return COL_ID_FOLLOWUPID; }
        static wxString col_name() { return COL_NAME_A[COL_ID_FOLLOWUPID]; }
        explicit FOLLOWUPID(const int64 &v): TableOpV<int64>(OP_EQ, v) {}
        explicit FOLLOWUPID(OP op, const int64 &v): TableOpV<int64>(op, v) {}
    };

    struct TOTRANSAMOUNT : public TableOpV<double>
    {
        static COL_ID col_id() { return COL_ID_TOTRANSAMOUNT; }
        static wxString col_name() { return COL_NAME_A[COL_ID_TOTRANSAMOUNT]; }
        explicit TOTRANSAMOUNT(const double &v): TableOpV<double>(OP_EQ, v) {}
        explicit TOTRANSAMOUNT(OP op, const double &v): TableOpV<double>(op, v) {}
    };

    struct COLOR : public TableOpV<int64>
    {
        static COL_ID col_id() { return COL_ID_COLOR; }
        static wxString col_name() { return COL_NAME_A[COL_ID_COLOR]; }
        explicit COLOR(const int64 &v): TableOpV<int64>(OP_EQ, v) {}
        explicit COLOR(OP op, const int64 &v): TableOpV<int64>(op, v) {}
    };
};

// A single record in database table CHECKINGACCOUNT_V1
struct TrxRow
{
    using Col = TrxCol;

    int64 TRANSID; // primary key
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
    wxString LASTUPDATEDTIME;
    wxString DELETEDTIME;
    int64 FOLLOWUPID;
    double TOTRANSAMOUNT;
    int64 COLOR;

    explicit TrxRow();
    explicit TrxRow(wxSQLite3ResultSet& q);
    TrxRow(const TrxRow& other) = default;

    int64 id() const { return TRANSID; }
    void id(const int64 id) { TRANSID = id; }
    void to_insert_stmt(wxSQLite3Statement& stmt, int64 id) const;
    void to_update_stmt(wxSQLite3Statement& stmt) const;
    TrxRow& from_select_result(wxSQLite3ResultSet& q);
    wxString to_json() const;
    void as_json(PrettyWriter<StringBuffer>& json_writer) const;
    row_t to_html_row() const;
    void to_html_template(html_template& t) const;
    void destroy() { delete this; }

    TrxRow& clone_from(const TrxRow& other);
    bool equals(const TrxRow* other) const;
    bool operator< (const TrxRow& other) const { return id() < other.id(); }
    bool operator< (const TrxRow* other) const { return id() < other->id(); }

    template<typename C>
    bool match(const C&)
    {
        return false;
    }

    // TODO: check if col.m_operator == OP_EQ

    bool match(const Col::TRANSID& col)
    {
        return TRANSID == col.m_value;
    }

    bool match(const Col::ACCOUNTID& col)
    {
        return ACCOUNTID == col.m_value;
    }

    bool match(const Col::TOACCOUNTID& col)
    {
        return TOACCOUNTID == col.m_value;
    }

    bool match(const Col::PAYEEID& col)
    {
        return PAYEEID == col.m_value;
    }

    bool match(const Col::TRANSCODE& col)
    {
        return TRANSCODE.CmpNoCase(col.m_value) == 0;
    }

    bool match(const Col::TRANSAMOUNT& col)
    {
        return TRANSAMOUNT == col.m_value;
    }

    bool match(const Col::STATUS& col)
    {
        return STATUS.CmpNoCase(col.m_value) == 0;
    }

    bool match(const Col::TRANSACTIONNUMBER& col)
    {
        return TRANSACTIONNUMBER.CmpNoCase(col.m_value) == 0;
    }

    bool match(const Col::NOTES& col)
    {
        return NOTES.CmpNoCase(col.m_value) == 0;
    }

    bool match(const Col::CATEGID& col)
    {
        return CATEGID == col.m_value;
    }

    bool match(const Col::TRANSDATE& col)
    {
        return TRANSDATE.CmpNoCase(col.m_value) == 0;
    }

    bool match(const Col::LASTUPDATEDTIME& col)
    {
        return LASTUPDATEDTIME.CmpNoCase(col.m_value) == 0;
    }

    bool match(const Col::DELETEDTIME& col)
    {
        return DELETEDTIME.CmpNoCase(col.m_value) == 0;
    }

    bool match(const Col::FOLLOWUPID& col)
    {
        return FOLLOWUPID == col.m_value;
    }

    bool match(const Col::TOTRANSAMOUNT& col)
    {
        return TOTRANSAMOUNT == col.m_value;
    }

    bool match(const Col::COLOR& col)
    {
        return COLOR == col.m_value;
    }

    template<typename Arg1, typename... Args>
    bool match(const Arg1& arg1, const Args&... args)
    {
        return (match(arg1) && ... && match(args));
    }

    struct SorterByTRANSID
    {
        bool operator()(const TrxRow& x, const TrxRow& y)
        {
            return x.TRANSID < y.TRANSID;
        }
    };

    struct SorterByACCOUNTID
    {
        bool operator()(const TrxRow& x, const TrxRow& y)
        {
            return x.ACCOUNTID < y.ACCOUNTID;
        }
    };

    struct SorterByTOACCOUNTID
    {
        bool operator()(const TrxRow& x, const TrxRow& y)
        {
            return x.TOACCOUNTID < y.TOACCOUNTID;
        }
    };

    struct SorterByPAYEEID
    {
        bool operator()(const TrxRow& x, const TrxRow& y)
        {
            return x.PAYEEID < y.PAYEEID;
        }
    };

    struct SorterByTRANSCODE
    {
        bool operator()(const TrxRow& x, const TrxRow& y)
        {
            return x.TRANSCODE < y.TRANSCODE;
        }
    };

    struct SorterByTRANSAMOUNT
    {
        bool operator()(const TrxRow& x, const TrxRow& y)
        {
            return x.TRANSAMOUNT < y.TRANSAMOUNT;
        }
    };

    struct SorterBySTATUS
    {
        bool operator()(const TrxRow& x, const TrxRow& y)
        {
            return x.STATUS < y.STATUS;
        }
    };

    struct SorterByTRANSACTIONNUMBER
    {
        bool operator()(const TrxRow& x, const TrxRow& y)
        {
            return x.TRANSACTIONNUMBER < y.TRANSACTIONNUMBER;
        }
    };

    struct SorterByNOTES
    {
        bool operator()(const TrxRow& x, const TrxRow& y)
        {
            return x.NOTES < y.NOTES;
        }
    };

    struct SorterByCATEGID
    {
        bool operator()(const TrxRow& x, const TrxRow& y)
        {
            return x.CATEGID < y.CATEGID;
        }
    };

    struct SorterByTRANSDATE
    {
        bool operator()(const TrxRow& x, const TrxRow& y)
        {
            return x.TRANSDATE < y.TRANSDATE;
        }
    };

    struct SorterByLASTUPDATEDTIME
    {
        bool operator()(const TrxRow& x, const TrxRow& y)
        {
            return x.LASTUPDATEDTIME < y.LASTUPDATEDTIME;
        }
    };

    struct SorterByDELETEDTIME
    {
        bool operator()(const TrxRow& x, const TrxRow& y)
        {
            return x.DELETEDTIME < y.DELETEDTIME;
        }
    };

    struct SorterByFOLLOWUPID
    {
        bool operator()(const TrxRow& x, const TrxRow& y)
        {
            return x.FOLLOWUPID < y.FOLLOWUPID;
        }
    };

    struct SorterByTOTRANSAMOUNT
    {
        bool operator()(const TrxRow& x, const TrxRow& y)
        {
            return x.TOTRANSAMOUNT < y.TOTRANSAMOUNT;
        }
    };

    struct SorterByCOLOR
    {
        bool operator()(const TrxRow& x, const TrxRow& y)
        {
            return x.COLOR < y.COLOR;
        }
    };
};

// Interface to database table CHECKINGACCOUNT_V1
struct TrxTable : public TableBase
{
    using Row = TrxRow;
    using Col = typename Row::Col;

    TrxTable();
    ~TrxTable() {}
};

inline TrxRow::TrxRow(wxSQLite3ResultSet& q)
{
    from_select_result(q);
}

inline void TrxRow::to_update_stmt(wxSQLite3Statement& stmt) const
{
    to_insert_stmt(stmt, id());
}

inline TrxRow& TrxRow::clone_from(const TrxRow& other)
{
    *this = other;
    id(-1);
    return *this;
}
