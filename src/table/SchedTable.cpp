// -*- C++ -*-
//=============================================================================
/**
 *      Copyright: (c) 2013-2026 Guan Lisheng (guanlisheng@gmail.com)
 *      Copyright: (c) 2017-2018 Stefano Giorgio (stef145g)
 *      Copyright: (c) 2022      Mark Whalley (mark@ipx.co.uk)
 *      Copyright: (c) 2026      George Ef (george.a.ef@gmail.com)
 *
 *      SchedTable.cpp
 *
 *      Implementation of the interface to database table BILLSDEPOSITS_V1
 *
 *      @author [sqlite2cpp.py]
 *
 *      Revision History:
 *          AUTO GENERATED at 2026-02-25 08:58:12.230056.
 *          DO NOT EDIT!
 */
//=============================================================================

#include "_TableFactory.tpp"
#include "SchedTable.h"
#include "data/SchedData.h"

template class TableFactory<SchedTable, SchedData>;
template class mmCache<int64, SchedData>;

// List of column names in database table BILLSDEPOSITS_V1,
// in the order of SchedCol::COL_ID.
const wxArrayString SchedCol::COL_NAME_A = {
    "BDID",
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
    "FOLLOWUPID",
    "TOTRANSAMOUNT",
    "REPEATS",
    "NEXTOCCURRENCEDATE",
    "NUMOCCURRENCES",
    "COLOR"
};

const SchedCol::COL_ID SchedCol::PRIMARY_ID = COL_ID_BDID;
const wxString SchedCol::PRIMARY_NAME = COL_NAME_A[COL_ID_BDID];

SchedRow::SchedRow()
{
    BDID = -1;
    ACCOUNTID = -1;
    TOACCOUNTID = -1;
    PAYEEID = -1;
    TRANSAMOUNT = 0.0;
    CATEGID = -1;
    FOLLOWUPID = -1;
    TOTRANSAMOUNT = 0.0;
    REPEATS = -1;
    NUMOCCURRENCES = -1;
    COLOR = -1;
}

// Bind a Row record to database insert statement.
void SchedRow::to_insert_stmt(wxSQLite3Statement& stmt, int64 id) const
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
    stmt.Bind(11, FOLLOWUPID);
    stmt.Bind(12, TOTRANSAMOUNT);
    stmt.Bind(13, REPEATS);
    stmt.Bind(14, NEXTOCCURRENCEDATE);
    stmt.Bind(15, NUMOCCURRENCES);
    stmt.Bind(16, COLOR);
    stmt.Bind(17, id);
}

SchedRow& SchedRow::from_select_result(wxSQLite3ResultSet& q)
{
    BDID = q.GetInt64(0);
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
    FOLLOWUPID = q.GetInt64(11);
    TOTRANSAMOUNT = q.GetDouble(12);
    REPEATS = q.GetInt64(13);
    NEXTOCCURRENCEDATE = q.GetString(14);
    NUMOCCURRENCES = q.GetInt64(15);
    COLOR = q.GetInt64(16);

    return *this;
}

// Return the data record as a json string
wxString SchedRow::to_json() const
{
    StringBuffer json_buffer;
    PrettyWriter<StringBuffer> json_writer(json_buffer);

    json_writer.StartObject();			
    as_json(json_writer);
    json_writer.EndObject();

    return json_buffer.GetString();
}

// Add the field data as json key:value pairs
void SchedRow::as_json(PrettyWriter<StringBuffer>& json_writer) const
{
    json_writer.Key("BDID");
    json_writer.Int64(BDID.GetValue());

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

    json_writer.Key("FOLLOWUPID");
    json_writer.Int64(FOLLOWUPID.GetValue());

    json_writer.Key("TOTRANSAMOUNT");
    json_writer.Double(TOTRANSAMOUNT);

    json_writer.Key("REPEATS");
    json_writer.Int64(REPEATS.GetValue());

    json_writer.Key("NEXTOCCURRENCEDATE");
    json_writer.String(NEXTOCCURRENCEDATE.utf8_str());

    json_writer.Key("NUMOCCURRENCES");
    json_writer.Int64(NUMOCCURRENCES.GetValue());

    json_writer.Key("COLOR");
    json_writer.Int64(COLOR.GetValue());
}

row_t SchedRow::to_html_row() const
{
    row_t row;

    row(L"BDID") = BDID.GetValue();
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
    row(L"FOLLOWUPID") = FOLLOWUPID.GetValue();
    row(L"TOTRANSAMOUNT") = TOTRANSAMOUNT;
    row(L"REPEATS") = REPEATS.GetValue();
    row(L"NEXTOCCURRENCEDATE") = NEXTOCCURRENCEDATE;
    row(L"NUMOCCURRENCES") = NUMOCCURRENCES.GetValue();
    row(L"COLOR") = COLOR.GetValue();

    return row;
}

void SchedRow::to_html_template(html_template& t) const
{
    t(L"BDID") = BDID.GetValue();
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
    t(L"FOLLOWUPID") = FOLLOWUPID.GetValue();
    t(L"TOTRANSAMOUNT") = TOTRANSAMOUNT;
    t(L"REPEATS") = REPEATS.GetValue();
    t(L"NEXTOCCURRENCEDATE") = NEXTOCCURRENCEDATE;
    t(L"NUMOCCURRENCES") = NUMOCCURRENCES.GetValue();
    t(L"COLOR") = COLOR.GetValue();
}

bool SchedRow::equals(const SchedRow* other) const
{
    if ( BDID != other->BDID) return false;
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
    if ( FOLLOWUPID != other->FOLLOWUPID) return false;
    if ( TOTRANSAMOUNT != other->TOTRANSAMOUNT) return false;
    if ( REPEATS != other->REPEATS) return false;
    if (!NEXTOCCURRENCEDATE.IsSameAs(other->NEXTOCCURRENCEDATE)) return false;
    if ( NUMOCCURRENCES != other->NUMOCCURRENCES) return false;
    if ( COLOR != other->COLOR) return false;

    return true;
}

SchedTable::SchedTable()
{
    m_table_name = "BILLSDEPOSITS_V1";

    m_create_query = "CREATE TABLE BILLSDEPOSITS_V1(BDID integer primary key, ACCOUNTID integer NOT NULL, TOACCOUNTID integer, PAYEEID integer NOT NULL, TRANSCODE TEXT NOT NULL /* Withdrawal, Deposit, Transfer */, TRANSAMOUNT numeric NOT NULL, STATUS TEXT /* None, Reconciled, Void, Follow up, Duplicate */, TRANSACTIONNUMBER TEXT, NOTES TEXT, CATEGID integer, TRANSDATE TEXT, FOLLOWUPID integer, TOTRANSAMOUNT numeric, REPEATS integer, NEXTOCCURRENCEDATE TEXT, NUMOCCURRENCES integer, COLOR integer DEFAULT -1)";

    m_drop_query = "DROP TABLE IF EXISTS BILLSDEPOSITS_V1";

    m_index_query_a = {
        "CREATE INDEX IF NOT EXISTS IDX_BILLSDEPOSITS_ACCOUNT ON BILLSDEPOSITS_V1 (ACCOUNTID, TOACCOUNTID)"
    };

    m_insert_query = "INSERT INTO BILLSDEPOSITS_V1(ACCOUNTID, TOACCOUNTID, PAYEEID, TRANSCODE, TRANSAMOUNT, STATUS, TRANSACTIONNUMBER, NOTES, CATEGID, TRANSDATE, FOLLOWUPID, TOTRANSAMOUNT, REPEATS, NEXTOCCURRENCEDATE, NUMOCCURRENCES, COLOR, BDID) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

    m_update_query = "UPDATE BILLSDEPOSITS_V1 SET ACCOUNTID = ?, TOACCOUNTID = ?, PAYEEID = ?, TRANSCODE = ?, TRANSAMOUNT = ?, STATUS = ?, TRANSACTIONNUMBER = ?, NOTES = ?, CATEGID = ?, TRANSDATE = ?, FOLLOWUPID = ?, TOTRANSAMOUNT = ?, REPEATS = ?, NEXTOCCURRENCEDATE = ?, NUMOCCURRENCES = ?, COLOR = ? WHERE BDID = ?";

    m_delete_query = "DELETE FROM BILLSDEPOSITS_V1 WHERE BDID = ?";

    m_select_query = "SELECT BDID, ACCOUNTID, TOACCOUNTID, PAYEEID, TRANSCODE, TRANSAMOUNT, STATUS, TRANSACTIONNUMBER, NOTES, CATEGID, TRANSDATE, FOLLOWUPID, TOTRANSAMOUNT, REPEATS, NEXTOCCURRENCEDATE, NUMOCCURRENCES, COLOR FROM BILLSDEPOSITS_V1";
}
