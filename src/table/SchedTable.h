// -*- C++ -*-
//=============================================================================
/**
 *      Copyright: (c) 2013-2026 Guan Lisheng (guanlisheng@gmail.com)
 *      Copyright: (c) 2017-2018 Stefano Giorgio (stef145g)
 *      Copyright: (c) 2022      Mark Whalley (mark@ipx.co.uk)
 *      Copyright: (c) 2026      George Ef (george.a.ef@gmail.com)
 *
 *      SchedTable.h
 *
 *      Interface to database table BILLSDEPOSITS_V1
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

// Columns in database table BILLSDEPOSITS_V1
struct SchedCol
{
    enum COL_ID
    {
        COL_ID_BDID = 0,
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
        COL_ID_FOLLOWUPID,
        COL_ID_TOTRANSAMOUNT,
        COL_ID_REPEATS,
        COL_ID_NEXTOCCURRENCEDATE,
        COL_ID_NUMOCCURRENCES,
        COL_ID_COLOR,
        COL_ID_size
    };

    static const wxArrayString COL_NAME_A;
    static const COL_ID PRIMARY_ID;
    static const wxString PRIMARY_NAME;

    static wxString col_name(COL_ID col_id) { return COL_NAME_A[col_id]; }

    struct BDID : public TableOpV<int64>
    {
        static COL_ID col_id() { return COL_ID_BDID; }
        static wxString col_name() { return COL_NAME_A[COL_ID_BDID]; }
        explicit BDID(const int64 &v): TableOpV<int64>(OP_EQ, v) {}
        explicit BDID(OP op, const int64 &v): TableOpV<int64>(op, v) {}
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

    struct REPEATS : public TableOpV<int64>
    {
        static COL_ID col_id() { return COL_ID_REPEATS; }
        static wxString col_name() { return COL_NAME_A[COL_ID_REPEATS]; }
        explicit REPEATS(const int64 &v): TableOpV<int64>(OP_EQ, v) {}
        explicit REPEATS(OP op, const int64 &v): TableOpV<int64>(op, v) {}
    };

    struct NEXTOCCURRENCEDATE : public TableOpV<wxString>
    {
        static COL_ID col_id() { return COL_ID_NEXTOCCURRENCEDATE; }
        static wxString col_name() { return COL_NAME_A[COL_ID_NEXTOCCURRENCEDATE]; }
        explicit NEXTOCCURRENCEDATE(const wxString &v): TableOpV<wxString>(OP_EQ, v) {}
        explicit NEXTOCCURRENCEDATE(OP op, const wxString &v): TableOpV<wxString>(op, v) {}
    };

    struct NUMOCCURRENCES : public TableOpV<int64>
    {
        static COL_ID col_id() { return COL_ID_NUMOCCURRENCES; }
        static wxString col_name() { return COL_NAME_A[COL_ID_NUMOCCURRENCES]; }
        explicit NUMOCCURRENCES(const int64 &v): TableOpV<int64>(OP_EQ, v) {}
        explicit NUMOCCURRENCES(OP op, const int64 &v): TableOpV<int64>(op, v) {}
    };

    struct COLOR : public TableOpV<int64>
    {
        static COL_ID col_id() { return COL_ID_COLOR; }
        static wxString col_name() { return COL_NAME_A[COL_ID_COLOR]; }
        explicit COLOR(const int64 &v): TableOpV<int64>(OP_EQ, v) {}
        explicit COLOR(OP op, const int64 &v): TableOpV<int64>(op, v) {}
    };
};

// A single record in database table BILLSDEPOSITS_V1
struct SchedRow
{
    using Col = SchedCol;

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

    explicit SchedRow();
    explicit SchedRow(wxSQLite3ResultSet& q);
    SchedRow(const SchedRow& other) = default;

    int64 id() const { return BDID; }
    void id(const int64 id) { BDID = id; }
    void to_insert_stmt(wxSQLite3Statement& stmt, int64 id) const;
    void to_update_stmt(wxSQLite3Statement& stmt) const;
    SchedRow& from_select_result(wxSQLite3ResultSet& q);
    wxString to_json() const;
    void as_json(PrettyWriter<StringBuffer>& json_writer) const;
    row_t to_html_row() const;
    void to_html_template(html_template& t) const;
    void destroy() { delete this; }

    SchedRow& clone_from(const SchedRow& other);
    bool equals(const SchedRow* other) const;
    bool operator< (const SchedRow& other) const { return id() < other.id(); }
    bool operator< (const SchedRow* other) const { return id() < other->id(); }

    template<typename C>
    bool match(const C&)
    {
        return false;
    }

    // TODO: check if col.m_operator == OP_EQ

    bool match(const Col::BDID& col)
    {
        return BDID == col.m_value;
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

    bool match(const Col::FOLLOWUPID& col)
    {
        return FOLLOWUPID == col.m_value;
    }

    bool match(const Col::TOTRANSAMOUNT& col)
    {
        return TOTRANSAMOUNT == col.m_value;
    }

    bool match(const Col::REPEATS& col)
    {
        return REPEATS == col.m_value;
    }

    bool match(const Col::NEXTOCCURRENCEDATE& col)
    {
        return NEXTOCCURRENCEDATE.CmpNoCase(col.m_value) == 0;
    }

    bool match(const Col::NUMOCCURRENCES& col)
    {
        return NUMOCCURRENCES == col.m_value;
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

    struct SorterByBDID
    {
        bool operator()(const SchedRow& x, const SchedRow& y)
        {
            return x.BDID < y.BDID;
        }
    };

    struct SorterByACCOUNTID
    {
        bool operator()(const SchedRow& x, const SchedRow& y)
        {
            return x.ACCOUNTID < y.ACCOUNTID;
        }
    };

    struct SorterByTOACCOUNTID
    {
        bool operator()(const SchedRow& x, const SchedRow& y)
        {
            return x.TOACCOUNTID < y.TOACCOUNTID;
        }
    };

    struct SorterByPAYEEID
    {
        bool operator()(const SchedRow& x, const SchedRow& y)
        {
            return x.PAYEEID < y.PAYEEID;
        }
    };

    struct SorterByTRANSCODE
    {
        bool operator()(const SchedRow& x, const SchedRow& y)
        {
            return x.TRANSCODE < y.TRANSCODE;
        }
    };

    struct SorterByTRANSAMOUNT
    {
        bool operator()(const SchedRow& x, const SchedRow& y)
        {
            return x.TRANSAMOUNT < y.TRANSAMOUNT;
        }
    };

    struct SorterBySTATUS
    {
        bool operator()(const SchedRow& x, const SchedRow& y)
        {
            return x.STATUS < y.STATUS;
        }
    };

    struct SorterByTRANSACTIONNUMBER
    {
        bool operator()(const SchedRow& x, const SchedRow& y)
        {
            return x.TRANSACTIONNUMBER < y.TRANSACTIONNUMBER;
        }
    };

    struct SorterByNOTES
    {
        bool operator()(const SchedRow& x, const SchedRow& y)
        {
            return x.NOTES < y.NOTES;
        }
    };

    struct SorterByCATEGID
    {
        bool operator()(const SchedRow& x, const SchedRow& y)
        {
            return x.CATEGID < y.CATEGID;
        }
    };

    struct SorterByTRANSDATE
    {
        bool operator()(const SchedRow& x, const SchedRow& y)
        {
            return x.TRANSDATE < y.TRANSDATE;
        }
    };

    struct SorterByFOLLOWUPID
    {
        bool operator()(const SchedRow& x, const SchedRow& y)
        {
            return x.FOLLOWUPID < y.FOLLOWUPID;
        }
    };

    struct SorterByTOTRANSAMOUNT
    {
        bool operator()(const SchedRow& x, const SchedRow& y)
        {
            return x.TOTRANSAMOUNT < y.TOTRANSAMOUNT;
        }
    };

    struct SorterByREPEATS
    {
        bool operator()(const SchedRow& x, const SchedRow& y)
        {
            return x.REPEATS < y.REPEATS;
        }
    };

    struct SorterByNEXTOCCURRENCEDATE
    {
        bool operator()(const SchedRow& x, const SchedRow& y)
        {
            return x.NEXTOCCURRENCEDATE < y.NEXTOCCURRENCEDATE;
        }
    };

    struct SorterByNUMOCCURRENCES
    {
        bool operator()(const SchedRow& x, const SchedRow& y)
        {
            return x.NUMOCCURRENCES < y.NUMOCCURRENCES;
        }
    };

    struct SorterByCOLOR
    {
        bool operator()(const SchedRow& x, const SchedRow& y)
        {
            return x.COLOR < y.COLOR;
        }
    };
};

// Interface to database table BILLSDEPOSITS_V1
struct SchedTable : public TableBase
{
    using Row = SchedRow;
    using Col = typename Row::Col;

    SchedTable();
    ~SchedTable() {}
};

inline SchedRow::SchedRow(wxSQLite3ResultSet& q)
{
    from_select_result(q);
}

inline void SchedRow::to_update_stmt(wxSQLite3Statement& stmt) const
{
    to_insert_stmt(stmt, id());
}

inline SchedRow& SchedRow::clone_from(const SchedRow& other)
{
    *this = other;
    id(-1);
    return *this;
}
