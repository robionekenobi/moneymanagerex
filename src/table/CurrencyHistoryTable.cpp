// -*- C++ -*-
//=============================================================================
/**
 *      Copyright: (c) 2013-2026 Guan Lisheng (guanlisheng@gmail.com)
 *      Copyright: (c) 2017-2018 Stefano Giorgio (stef145g)
 *      Copyright: (c) 2022      Mark Whalley (mark@ipx.co.uk)
 *      Copyright: (c) 2026      George Ef (george.a.ef@gmail.com)
 *
 *      CurrencyHistoryTable.cpp
 *
 *      Implementation of the interface to database table CURRENCYHISTORY_V1
 *
 *      @author [sqlite2cpp.py]
 *
 *      Revision History:
 *          AUTO GENERATED at 2026-02-25 08:58:12.230056.
 *          DO NOT EDIT!
 */
//=============================================================================

#include "_TableFactory.tpp"
#include "CurrencyHistoryTable.h"
#include "data/CurrencyHistoryData.h"

template class TableFactory<CurrencyHistoryTable, CurrencyHistoryData>;
template class mmCache<int64, CurrencyHistoryData>;

// List of column names in database table CURRENCYHISTORY_V1,
// in the order of CurrencyHistoryCol::COL_ID.
const wxArrayString CurrencyHistoryCol::COL_NAME_A = {
    "CURRHISTID",
    "CURRENCYID",
    "CURRDATE",
    "CURRVALUE",
    "CURRUPDTYPE"
};

const CurrencyHistoryCol::COL_ID CurrencyHistoryCol::PRIMARY_ID = COL_ID_CURRHISTID;
const wxString CurrencyHistoryCol::PRIMARY_NAME = COL_NAME_A[COL_ID_CURRHISTID];

CurrencyHistoryRow::CurrencyHistoryRow()
{
    CURRHISTID = -1;
    CURRENCYID = -1;
    CURRVALUE = 0.0;
    CURRUPDTYPE = -1;
}

// Bind a Row record to database insert statement.
void CurrencyHistoryRow::to_insert_stmt(wxSQLite3Statement& stmt, int64 id) const
{
    stmt.Bind(1, CURRENCYID);
    stmt.Bind(2, CURRDATE);
    stmt.Bind(3, CURRVALUE);
    stmt.Bind(4, CURRUPDTYPE);
    stmt.Bind(5, id);
}

CurrencyHistoryRow& CurrencyHistoryRow::from_select_result(wxSQLite3ResultSet& q)
{
    CURRHISTID = q.GetInt64(0);
    CURRENCYID = q.GetInt64(1);
    CURRDATE = q.GetString(2);
    CURRVALUE = q.GetDouble(3);
    CURRUPDTYPE = q.GetInt64(4);

    return *this;
}

// Return the data record as a json string
wxString CurrencyHistoryRow::to_json() const
{
    StringBuffer json_buffer;
    PrettyWriter<StringBuffer> json_writer(json_buffer);

    json_writer.StartObject();			
    as_json(json_writer);
    json_writer.EndObject();

    return json_buffer.GetString();
}

// Add the field data as json key:value pairs
void CurrencyHistoryRow::as_json(PrettyWriter<StringBuffer>& json_writer) const
{
    json_writer.Key("CURRHISTID");
    json_writer.Int64(CURRHISTID.GetValue());

    json_writer.Key("CURRENCYID");
    json_writer.Int64(CURRENCYID.GetValue());

    json_writer.Key("CURRDATE");
    json_writer.String(CURRDATE.utf8_str());

    json_writer.Key("CURRVALUE");
    json_writer.Double(CURRVALUE);

    json_writer.Key("CURRUPDTYPE");
    json_writer.Int64(CURRUPDTYPE.GetValue());
}

row_t CurrencyHistoryRow::to_html_row() const
{
    row_t row;

    row(L"CURRHISTID") = CURRHISTID.GetValue();
    row(L"CURRENCYID") = CURRENCYID.GetValue();
    row(L"CURRDATE") = CURRDATE;
    row(L"CURRVALUE") = CURRVALUE;
    row(L"CURRUPDTYPE") = CURRUPDTYPE.GetValue();

    return row;
}

void CurrencyHistoryRow::to_html_template(html_template& t) const
{
    t(L"CURRHISTID") = CURRHISTID.GetValue();
    t(L"CURRENCYID") = CURRENCYID.GetValue();
    t(L"CURRDATE") = CURRDATE;
    t(L"CURRVALUE") = CURRVALUE;
    t(L"CURRUPDTYPE") = CURRUPDTYPE.GetValue();
}

bool CurrencyHistoryRow::equals(const CurrencyHistoryRow* other) const
{
    if ( CURRHISTID != other->CURRHISTID) return false;
    if ( CURRENCYID != other->CURRENCYID) return false;
    if (!CURRDATE.IsSameAs(other->CURRDATE)) return false;
    if ( CURRVALUE != other->CURRVALUE) return false;
    if ( CURRUPDTYPE != other->CURRUPDTYPE) return false;

    return true;
}

CurrencyHistoryTable::CurrencyHistoryTable()
{
    m_table_name = "CURRENCYHISTORY_V1";

    m_create_query = "CREATE TABLE CURRENCYHISTORY_V1(CURRHISTID INTEGER PRIMARY KEY, CURRENCYID INTEGER NOT NULL, CURRDATE TEXT NOT NULL, CURRVALUE NUMERIC NOT NULL, CURRUPDTYPE INTEGER, UNIQUE(CURRENCYID, CURRDATE))";

    m_drop_query = "DROP TABLE IF EXISTS CURRENCYHISTORY_V1";

    m_index_query_a = {
        "CREATE INDEX IF NOT EXISTS IDX_CURRENCYHISTORY_CURRENCYID_CURRDATE ON CURRENCYHISTORY_V1(CURRENCYID, CURRDATE)"
    };

    m_insert_query = "INSERT INTO CURRENCYHISTORY_V1(CURRENCYID, CURRDATE, CURRVALUE, CURRUPDTYPE, CURRHISTID) VALUES(?, ?, ?, ?, ?)";

    m_update_query = "UPDATE CURRENCYHISTORY_V1 SET CURRENCYID = ?, CURRDATE = ?, CURRVALUE = ?, CURRUPDTYPE = ? WHERE CURRHISTID = ?";

    m_delete_query = "DELETE FROM CURRENCYHISTORY_V1 WHERE CURRHISTID = ?";

    m_select_query = "SELECT CURRHISTID, CURRENCYID, CURRDATE, CURRVALUE, CURRUPDTYPE FROM CURRENCYHISTORY_V1";
}
