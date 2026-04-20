// -*- C++ -*-
//=============================================================================
/**
 *      Copyright: (c) 2013-2026 Guan Lisheng (guanlisheng@gmail.com)
 *      Copyright: (c) 2017-2018 Stefano Giorgio (stef145g)
 *      Copyright: (c) 2022      Mark Whalley (mark@ipx.co.uk)
 *      Copyright: (c) 2026      George Ef (george.a.ef@gmail.com)
 *
 *      TrxTable.cpp
 *
 *      Implementation of the interface to database table CHECKINGACCOUNT_V1
 *
 *      @author [sqlite2cpp.py]
 *
 *      Revision History:
 *          AUTO GENERATED at 2026-04-11 13:27:18.900578.
 *          DO NOT EDIT!
 */
//=============================================================================

#include "_TableFactory.tpp"
#include "TrxTable.h"
#include "data/TrxData.h"

template class TableFactory<TrxTable, TrxData>;
template class mmCache<int64, TrxData>;

// List of column names in database table CHECKINGACCOUNT_V1,
// in the order of TrxCol::COL_ID.
const wxArrayString TrxCol::s_col_name_a = {
    "TRANSID",
    "ACCOUNTID",
    "TOACCOUNTID",
    "PAYEEID",
    "TRANSCODE",
    "TRANSAMOUNT",
    "STATUS",
    "TRANSACTIONNUMBER",
    "NOTES",
    "CATEGID",
    "TRANSDATE",
    "LASTUPDATEDTIME",
    "DELETEDTIME",
    "FOLLOWUPID",
    "TOTRANSAMOUNT",
    "COLOR"
};

const TrxCol::COL_ID TrxCol::s_primary_id = COL_ID_TRANSID;
const wxString TrxCol::s_primary_name = s_col_name_a[COL_ID_TRANSID];

// convenience variables
const wxString TrxCol::NAME_TRANSID = s_col_name_a[COL_ID_TRANSID];
const wxString TrxCol::NAME_ACCOUNTID = s_col_name_a[COL_ID_ACCOUNTID];
const wxString TrxCol::NAME_TOACCOUNTID = s_col_name_a[COL_ID_TOACCOUNTID];
const wxString TrxCol::NAME_PAYEEID = s_col_name_a[COL_ID_PAYEEID];
const wxString TrxCol::NAME_TRANSCODE = s_col_name_a[COL_ID_TRANSCODE];
const wxString TrxCol::NAME_TRANSAMOUNT = s_col_name_a[COL_ID_TRANSAMOUNT];
const wxString TrxCol::NAME_STATUS = s_col_name_a[COL_ID_STATUS];
const wxString TrxCol::NAME_TRANSACTIONNUMBER = s_col_name_a[COL_ID_TRANSACTIONNUMBER];
const wxString TrxCol::NAME_NOTES = s_col_name_a[COL_ID_NOTES];
const wxString TrxCol::NAME_CATEGID = s_col_name_a[COL_ID_CATEGID];
const wxString TrxCol::NAME_TRANSDATE = s_col_name_a[COL_ID_TRANSDATE];
const wxString TrxCol::NAME_LASTUPDATEDTIME = s_col_name_a[COL_ID_LASTUPDATEDTIME];
const wxString TrxCol::NAME_DELETEDTIME = s_col_name_a[COL_ID_DELETEDTIME];
const wxString TrxCol::NAME_FOLLOWUPID = s_col_name_a[COL_ID_FOLLOWUPID];
const wxString TrxCol::NAME_TOTRANSAMOUNT = s_col_name_a[COL_ID_TOTRANSAMOUNT];
const wxString TrxCol::NAME_COLOR = s_col_name_a[COL_ID_COLOR];

TrxRow::TrxRow()
{
    TRANSID = -1;
    ACCOUNTID = -1;
    TOACCOUNTID = -1;
    PAYEEID = -1;
    TRANSAMOUNT = 0.0;
    CATEGID = -1;
    FOLLOWUPID = -1;
    TOTRANSAMOUNT = 0.0;
    COLOR = -1;
}

// Bind a Row record to database insert statement.
void TrxRow::to_insert_stmt(wxSQLite3Statement& stmt, int64 id) const
{
    stmt.Bind(1, ACCOUNTID);
    stmt.Bind(2, TOACCOUNTID);
    stmt.Bind(3, PAYEEID);
    stmt.Bind(4, TRANSCODE);
    stmt.Bind(5, TRANSAMOUNT);
    stmt.Bind(6, STATUS);
    stmt.Bind(7, TRANSACTIONNUMBER);
    stmt.Bind(8, NOTES);
    stmt.Bind(9, CATEGID);
    stmt.Bind(10, TRANSDATE);
    stmt.Bind(11, LASTUPDATEDTIME);
    stmt.Bind(12, DELETEDTIME);
    stmt.Bind(13, FOLLOWUPID);
    stmt.Bind(14, TOTRANSAMOUNT);
    stmt.Bind(15, COLOR);
    stmt.Bind(16, id);
}

TrxRow& TrxRow::from_select_result(wxSQLite3ResultSet& q)
{
    TRANSID = q.GetInt64(0);
    ACCOUNTID = q.GetInt64(1);
    TOACCOUNTID = q.GetInt64(2);
    PAYEEID = q.GetInt64(3);
    TRANSCODE = q.GetString(4);
    TRANSAMOUNT = q.GetDouble(5);
    STATUS = q.GetString(6);
    TRANSACTIONNUMBER = q.GetString(7);
    NOTES = q.GetString(8);
    CATEGID = q.GetInt64(9);
    TRANSDATE = q.GetString(10);
    LASTUPDATEDTIME = q.GetString(11);
    DELETEDTIME = q.GetString(12);
    FOLLOWUPID = q.GetInt64(13);
    TOTRANSAMOUNT = q.GetDouble(14);
    COLOR = q.GetInt64(15);

    return *this;
}

// Return the data record as a json string
wxString TrxRow::to_json() const
{
    StringBuffer json_buffer;
    PrettyWriter<StringBuffer> json_writer(json_buffer);

    json_writer.StartObject();			
    as_json(json_writer);
    json_writer.EndObject();

    return json_buffer.GetString();
}

// Add the field data as json key:value pairs
void TrxRow::as_json(PrettyWriter<StringBuffer>& json_writer) const
{
    json_writer.Key("TRANSID");
    json_writer.Int64(TRANSID.GetValue());

    json_writer.Key("ACCOUNTID");
    json_writer.Int64(ACCOUNTID.GetValue());

    json_writer.Key("TOACCOUNTID");
    json_writer.Int64(TOACCOUNTID.GetValue());

    json_writer.Key("PAYEEID");
    json_writer.Int64(PAYEEID.GetValue());

    json_writer.Key("TRANSCODE");
    json_writer.String(TRANSCODE.utf8_str());

    json_writer.Key("TRANSAMOUNT");
    json_writer.Double(TRANSAMOUNT);

    json_writer.Key("STATUS");
    json_writer.String(STATUS.utf8_str());

    json_writer.Key("TRANSACTIONNUMBER");
    json_writer.String(TRANSACTIONNUMBER.utf8_str());

    json_writer.Key("NOTES");
    json_writer.String(NOTES.utf8_str());

    json_writer.Key("CATEGID");
    json_writer.Int64(CATEGID.GetValue());

    json_writer.Key("TRANSDATE");
    json_writer.String(TRANSDATE.utf8_str());

    json_writer.Key("LASTUPDATEDTIME");
    json_writer.String(LASTUPDATEDTIME.utf8_str());

    json_writer.Key("DELETEDTIME");
    json_writer.String(DELETEDTIME.utf8_str());

    json_writer.Key("FOLLOWUPID");
    json_writer.Int64(FOLLOWUPID.GetValue());

    json_writer.Key("TOTRANSAMOUNT");
    json_writer.Double(TOTRANSAMOUNT);

    json_writer.Key("COLOR");
    json_writer.Int64(COLOR.GetValue());
}

row_t TrxRow::to_html_row() const
{
    row_t row;

    row(L"TRANSID") = TRANSID.GetValue();
    row(L"ACCOUNTID") = ACCOUNTID.GetValue();
    row(L"TOACCOUNTID") = TOACCOUNTID.GetValue();
    row(L"PAYEEID") = PAYEEID.GetValue();
    row(L"TRANSCODE") = TRANSCODE;
    row(L"TRANSAMOUNT") = TRANSAMOUNT;
    row(L"STATUS") = STATUS;
    row(L"TRANSACTIONNUMBER") = TRANSACTIONNUMBER;
    row(L"NOTES") = NOTES;
    row(L"CATEGID") = CATEGID.GetValue();
    row(L"TRANSDATE") = TRANSDATE;
    row(L"LASTUPDATEDTIME") = LASTUPDATEDTIME;
    row(L"DELETEDTIME") = DELETEDTIME;
    row(L"FOLLOWUPID") = FOLLOWUPID.GetValue();
    row(L"TOTRANSAMOUNT") = TOTRANSAMOUNT;
    row(L"COLOR") = COLOR.GetValue();

    return row;
}

void TrxRow::to_html_template(html_template& t) const
{
    t(L"TRANSID") = TRANSID.GetValue();
    t(L"ACCOUNTID") = ACCOUNTID.GetValue();
    t(L"TOACCOUNTID") = TOACCOUNTID.GetValue();
    t(L"PAYEEID") = PAYEEID.GetValue();
    t(L"TRANSCODE") = TRANSCODE;
    t(L"TRANSAMOUNT") = TRANSAMOUNT;
    t(L"STATUS") = STATUS;
    t(L"TRANSACTIONNUMBER") = TRANSACTIONNUMBER;
    t(L"NOTES") = NOTES;
    t(L"CATEGID") = CATEGID.GetValue();
    t(L"TRANSDATE") = TRANSDATE;
    t(L"LASTUPDATEDTIME") = LASTUPDATEDTIME;
    t(L"DELETEDTIME") = DELETEDTIME;
    t(L"FOLLOWUPID") = FOLLOWUPID.GetValue();
    t(L"TOTRANSAMOUNT") = TOTRANSAMOUNT;
    t(L"COLOR") = COLOR.GetValue();
}

bool TrxRow::equals(const TrxRow* other) const
{
    if ( TRANSID != other->TRANSID) return false;
    if ( ACCOUNTID != other->ACCOUNTID) return false;
    if ( TOACCOUNTID != other->TOACCOUNTID) return false;
    if ( PAYEEID != other->PAYEEID) return false;
    if (!TRANSCODE.IsSameAs(other->TRANSCODE)) return false;
    if ( TRANSAMOUNT != other->TRANSAMOUNT) return false;
    if (!STATUS.IsSameAs(other->STATUS)) return false;
    if (!TRANSACTIONNUMBER.IsSameAs(other->TRANSACTIONNUMBER)) return false;
    if (!NOTES.IsSameAs(other->NOTES)) return false;
    if ( CATEGID != other->CATEGID) return false;
    if (!TRANSDATE.IsSameAs(other->TRANSDATE)) return false;
    if (!LASTUPDATEDTIME.IsSameAs(other->LASTUPDATEDTIME)) return false;
    if (!DELETEDTIME.IsSameAs(other->DELETEDTIME)) return false;
    if ( FOLLOWUPID != other->FOLLOWUPID) return false;
    if ( TOTRANSAMOUNT != other->TOTRANSAMOUNT) return false;
    if ( COLOR != other->COLOR) return false;

    return true;
}

TrxTable::TrxTable()
{
    m_table_name = "CHECKINGACCOUNT_V1";

    m_create_query = "CREATE TABLE CHECKINGACCOUNT_V1(TRANSID integer primary key, ACCOUNTID integer NOT NULL, TOACCOUNTID integer, PAYEEID integer NOT NULL, TRANSCODE TEXT NOT NULL /* Withdrawal, Deposit, Transfer */, TRANSAMOUNT numeric NOT NULL, STATUS TEXT /* None, Reconciled, Void, Follow up, Duplicate */, TRANSACTIONNUMBER TEXT, NOTES TEXT, CATEGID integer, TRANSDATE TEXT, LASTUPDATEDTIME TEXT, DELETEDTIME TEXT, FOLLOWUPID integer, TOTRANSAMOUNT numeric, COLOR integer DEFAULT -1)";

    m_drop_query = "DROP TABLE IF EXISTS CHECKINGACCOUNT_V1";

    m_index_query_a = {
        "CREATE INDEX IF NOT EXISTS IDX_CHECKINGACCOUNT_ACCOUNT ON CHECKINGACCOUNT_V1 (ACCOUNTID, TOACCOUNTID)",
        "CREATE INDEX IF NOT EXISTS IDX_CHECKINGACCOUNT_TRANSDATE ON CHECKINGACCOUNT_V1 (TRANSDATE)"
    };

    m_insert_query = "INSERT INTO CHECKINGACCOUNT_V1(ACCOUNTID, TOACCOUNTID, PAYEEID, TRANSCODE, TRANSAMOUNT, STATUS, TRANSACTIONNUMBER, NOTES, CATEGID, TRANSDATE, LASTUPDATEDTIME, DELETEDTIME, FOLLOWUPID, TOTRANSAMOUNT, COLOR, TRANSID) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

    m_update_query = "UPDATE CHECKINGACCOUNT_V1 SET ACCOUNTID = ?, TOACCOUNTID = ?, PAYEEID = ?, TRANSCODE = ?, TRANSAMOUNT = ?, STATUS = ?, TRANSACTIONNUMBER = ?, NOTES = ?, CATEGID = ?, TRANSDATE = ?, LASTUPDATEDTIME = ?, DELETEDTIME = ?, FOLLOWUPID = ?, TOTRANSAMOUNT = ?, COLOR = ? WHERE TRANSID = ?";

    m_delete_query = "DELETE FROM CHECKINGACCOUNT_V1 WHERE TRANSID = ?";

    m_select_query = "SELECT TRANSID, ACCOUNTID, TOACCOUNTID, PAYEEID, TRANSCODE, TRANSAMOUNT, STATUS, TRANSACTIONNUMBER, NOTES, CATEGID, TRANSDATE, LASTUPDATEDTIME, DELETEDTIME, FOLLOWUPID, TOTRANSAMOUNT, COLOR FROM CHECKINGACCOUNT_V1";
}
