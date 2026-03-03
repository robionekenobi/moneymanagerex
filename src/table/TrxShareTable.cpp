// -*- C++ -*-
//=============================================================================
/**
 *      Copyright: (c) 2013-2026 Guan Lisheng (guanlisheng@gmail.com)
 *      Copyright: (c) 2017-2018 Stefano Giorgio (stef145g)
 *      Copyright: (c) 2022      Mark Whalley (mark@ipx.co.uk)
 *      Copyright: (c) 2026      George Ef (george.a.ef@gmail.com)
 *
 *      TrxShareTable.cpp
 *
 *      Implementation of the interface to database table SHAREINFO_V1
 *
 *      @author [sqlite2cpp.py]
 *
 *      Revision History:
 *          AUTO GENERATED at 2026-02-25 08:58:12.230056.
 *          DO NOT EDIT!
 */
//=============================================================================

#include "_TableFactory.tpp"
#include "TrxShareTable.h"
#include "data/TrxShareData.h"

template class TableFactory<TrxShareTable, TrxShareData>;
template class mmCache<int64, TrxShareData>;

// List of column names in database table SHAREINFO_V1,
// in the order of TrxShareCol::COL_ID.
const wxArrayString TrxShareCol::COL_NAME_A = {
    "SHAREINFOID",
    "CHECKINGACCOUNTID",
    "SHARENUMBER",
    "SHAREPRICE",
    "SHARECOMMISSION",
    "SHARELOT"
};

const TrxShareCol::COL_ID TrxShareCol::PRIMARY_ID = COL_ID_SHAREINFOID;
const wxString TrxShareCol::PRIMARY_NAME = COL_NAME_A[COL_ID_SHAREINFOID];

TrxShareRow::TrxShareRow()
{
    SHAREINFOID = -1;
    CHECKINGACCOUNTID = -1;
    SHARENUMBER = 0.0;
    SHAREPRICE = 0.0;
    SHARECOMMISSION = 0.0;
}

// Bind a Row record to database insert statement.
void TrxShareRow::to_insert_stmt(wxSQLite3Statement& stmt, int64 id) const
{
    stmt.Bind(1, CHECKINGACCOUNTID);
    stmt.Bind(2, SHARENUMBER);
    stmt.Bind(3, SHAREPRICE);
    stmt.Bind(4, SHARECOMMISSION);
    stmt.Bind(5, SHARELOT);
    stmt.Bind(6, id);
}

TrxShareRow& TrxShareRow::from_select_result(wxSQLite3ResultSet& q)
{
    SHAREINFOID = q.GetInt64(0);
    CHECKINGACCOUNTID = q.GetInt64(1);
    SHARENUMBER = q.GetDouble(2);
    SHAREPRICE = q.GetDouble(3);
    SHARECOMMISSION = q.GetDouble(4);
    SHARELOT = q.GetString(5);

    return *this;
}

// Return the data record as a json string
wxString TrxShareRow::to_json() const
{
    StringBuffer json_buffer;
    PrettyWriter<StringBuffer> json_writer(json_buffer);

    json_writer.StartObject();			
    as_json(json_writer);
    json_writer.EndObject();

    return json_buffer.GetString();
}

// Add the field data as json key:value pairs
void TrxShareRow::as_json(PrettyWriter<StringBuffer>& json_writer) const
{
    json_writer.Key("SHAREINFOID");
    json_writer.Int64(SHAREINFOID.GetValue());

    json_writer.Key("CHECKINGACCOUNTID");
    json_writer.Int64(CHECKINGACCOUNTID.GetValue());

    json_writer.Key("SHARENUMBER");
    json_writer.Double(SHARENUMBER);

    json_writer.Key("SHAREPRICE");
    json_writer.Double(SHAREPRICE);

    json_writer.Key("SHARECOMMISSION");
    json_writer.Double(SHARECOMMISSION);

    json_writer.Key("SHARELOT");
    json_writer.String(SHARELOT.utf8_str());
}

row_t TrxShareRow::to_html_row() const
{
    row_t row;

    row(L"SHAREINFOID") = SHAREINFOID.GetValue();
    row(L"CHECKINGACCOUNTID") = CHECKINGACCOUNTID.GetValue();
    row(L"SHARENUMBER") = SHARENUMBER;
    row(L"SHAREPRICE") = SHAREPRICE;
    row(L"SHARECOMMISSION") = SHARECOMMISSION;
    row(L"SHARELOT") = SHARELOT;

    return row;
}

void TrxShareRow::to_html_template(html_template& t) const
{
    t(L"SHAREINFOID") = SHAREINFOID.GetValue();
    t(L"CHECKINGACCOUNTID") = CHECKINGACCOUNTID.GetValue();
    t(L"SHARENUMBER") = SHARENUMBER;
    t(L"SHAREPRICE") = SHAREPRICE;
    t(L"SHARECOMMISSION") = SHARECOMMISSION;
    t(L"SHARELOT") = SHARELOT;
}

bool TrxShareRow::equals(const TrxShareRow* other) const
{
    if ( SHAREINFOID != other->SHAREINFOID) return false;
    if ( CHECKINGACCOUNTID != other->CHECKINGACCOUNTID) return false;
    if ( SHARENUMBER != other->SHARENUMBER) return false;
    if ( SHAREPRICE != other->SHAREPRICE) return false;
    if ( SHARECOMMISSION != other->SHARECOMMISSION) return false;
    if (!SHARELOT.IsSameAs(other->SHARELOT)) return false;

    return true;
}

TrxShareTable::TrxShareTable()
{
    m_table_name = "SHAREINFO_V1";

    m_create_query = "CREATE TABLE SHAREINFO_V1 (SHAREINFOID integer NOT NULL primary key, CHECKINGACCOUNTID integer NOT NULL, SHARENUMBER numeric, SHAREPRICE numeric, SHARECOMMISSION numeric, SHARELOT TEXT)";

    m_drop_query = "DROP TABLE IF EXISTS SHAREINFO_V1";

    m_index_query_a = {
        "CREATE INDEX IF NOT EXISTS IDX_SHAREINFO ON SHAREINFO_V1 (CHECKINGACCOUNTID)"
    };

    m_insert_query = "INSERT INTO SHAREINFO_V1(CHECKINGACCOUNTID, SHARENUMBER, SHAREPRICE, SHARECOMMISSION, SHARELOT, SHAREINFOID) VALUES(?, ?, ?, ?, ?, ?)";

    m_update_query = "UPDATE SHAREINFO_V1 SET CHECKINGACCOUNTID = ?, SHARENUMBER = ?, SHAREPRICE = ?, SHARECOMMISSION = ?, SHARELOT = ? WHERE SHAREINFOID = ?";

    m_delete_query = "DELETE FROM SHAREINFO_V1 WHERE SHAREINFOID = ?";

    m_select_query = "SELECT SHAREINFOID, CHECKINGACCOUNTID, SHARENUMBER, SHAREPRICE, SHARECOMMISSION, SHARELOT FROM SHAREINFO_V1";
}
