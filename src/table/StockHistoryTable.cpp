// -*- C++ -*-
//=============================================================================
/**
 *      Copyright: (c) 2013-2026 Guan Lisheng (guanlisheng@gmail.com)
 *      Copyright: (c) 2017-2018 Stefano Giorgio (stef145g)
 *      Copyright: (c) 2022      Mark Whalley (mark@ipx.co.uk)
 *      Copyright: (c) 2026      George Ef (george.a.ef@gmail.com)
 *
 *      StockHistoryTable.cpp
 *
 *      Implementation of the interface to database table STOCKHISTORY_V1
 *
 *      @author [sqlite2cpp.py]
 *
 *      Revision History:
 *          AUTO GENERATED at 2026-02-25 08:58:12.230056.
 *          DO NOT EDIT!
 */
//=============================================================================

#include "_TableFactory.tpp"
#include "StockHistoryTable.h"
#include "data/StockHistoryData.h"

template class TableFactory<StockHistoryTable, StockHistoryData>;
template class mmCache<int64, StockHistoryData>;

// List of column names in database table STOCKHISTORY_V1,
// in the order of StockHistoryCol::COL_ID.
const wxArrayString StockHistoryCol::COL_NAME_A = {
    "HISTID",
    "SYMBOL",
    "DATE",
    "VALUE",
    "UPDTYPE"
};

const StockHistoryCol::COL_ID StockHistoryCol::PRIMARY_ID = COL_ID_HISTID;
const wxString StockHistoryCol::PRIMARY_NAME = COL_NAME_A[COL_ID_HISTID];

StockHistoryRow::StockHistoryRow()
{
    HISTID = -1;
    VALUE = 0.0;
    UPDTYPE = -1;
}

// Bind a Row record to database insert statement.
void StockHistoryRow::to_insert_stmt(wxSQLite3Statement& stmt, int64 id) const
{
    stmt.Bind(1, SYMBOL);
    stmt.Bind(2, DATE);
    stmt.Bind(3, VALUE);
    stmt.Bind(4, UPDTYPE);
    stmt.Bind(5, id);
}

StockHistoryRow& StockHistoryRow::from_select_result(wxSQLite3ResultSet& q)
{
    HISTID = q.GetInt64(0);
    SYMBOL = q.GetString(1);
    DATE = q.GetString(2);
    VALUE = q.GetDouble(3);
    UPDTYPE = q.GetInt64(4);

    return *this;
}

// Return the data record as a json string
wxString StockHistoryRow::to_json() const
{
    StringBuffer json_buffer;
    PrettyWriter<StringBuffer> json_writer(json_buffer);

    json_writer.StartObject();			
    as_json(json_writer);
    json_writer.EndObject();

    return json_buffer.GetString();
}

// Add the field data as json key:value pairs
void StockHistoryRow::as_json(PrettyWriter<StringBuffer>& json_writer) const
{
    json_writer.Key("HISTID");
    json_writer.Int64(HISTID.GetValue());

    json_writer.Key("SYMBOL");
    json_writer.String(SYMBOL.utf8_str());

    json_writer.Key("DATE");
    json_writer.String(DATE.utf8_str());

    json_writer.Key("VALUE");
    json_writer.Double(VALUE);

    json_writer.Key("UPDTYPE");
    json_writer.Int64(UPDTYPE.GetValue());
}

row_t StockHistoryRow::to_html_row() const
{
    row_t row;

    row(L"HISTID") = HISTID.GetValue();
    row(L"SYMBOL") = SYMBOL;
    row(L"DATE") = DATE;
    row(L"VALUE") = VALUE;
    row(L"UPDTYPE") = UPDTYPE.GetValue();

    return row;
}

void StockHistoryRow::to_html_template(html_template& t) const
{
    t(L"HISTID") = HISTID.GetValue();
    t(L"SYMBOL") = SYMBOL;
    t(L"DATE") = DATE;
    t(L"VALUE") = VALUE;
    t(L"UPDTYPE") = UPDTYPE.GetValue();
}

bool StockHistoryRow::equals(const StockHistoryRow* other) const
{
    if ( HISTID != other->HISTID) return false;
    if (!SYMBOL.IsSameAs(other->SYMBOL)) return false;
    if (!DATE.IsSameAs(other->DATE)) return false;
    if ( VALUE != other->VALUE) return false;
    if ( UPDTYPE != other->UPDTYPE) return false;

    return true;
}

StockHistoryTable::StockHistoryTable()
{
    m_table_name = "STOCKHISTORY_V1";

    m_create_query = "CREATE TABLE STOCKHISTORY_V1(HISTID integer primary key, SYMBOL TEXT NOT NULL, DATE TEXT NOT NULL, VALUE numeric NOT NULL, UPDTYPE integer, UNIQUE(SYMBOL, DATE))";

    m_drop_query = "DROP TABLE IF EXISTS STOCKHISTORY_V1";

    m_index_query_a = {
        "CREATE INDEX IF NOT EXISTS IDX_STOCKHISTORY_SYMBOL ON STOCKHISTORY_V1(SYMBOL)"
    };

    m_insert_query = "INSERT INTO STOCKHISTORY_V1(SYMBOL, DATE, VALUE, UPDTYPE, HISTID) VALUES(?, ?, ?, ?, ?)";

    m_update_query = "UPDATE STOCKHISTORY_V1 SET SYMBOL = ?, DATE = ?, VALUE = ?, UPDTYPE = ? WHERE HISTID = ?";

    m_delete_query = "DELETE FROM STOCKHISTORY_V1 WHERE HISTID = ?";

    m_select_query = "SELECT HISTID, SYMBOL, DATE, VALUE, UPDTYPE FROM STOCKHISTORY_V1";
}
