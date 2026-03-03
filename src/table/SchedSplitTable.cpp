// -*- C++ -*-
//=============================================================================
/**
 *      Copyright: (c) 2013-2026 Guan Lisheng (guanlisheng@gmail.com)
 *      Copyright: (c) 2017-2018 Stefano Giorgio (stef145g)
 *      Copyright: (c) 2022      Mark Whalley (mark@ipx.co.uk)
 *      Copyright: (c) 2026      George Ef (george.a.ef@gmail.com)
 *
 *      SchedSplitTable.cpp
 *
 *      Implementation of the interface to database table BUDGETSPLITTRANSACTIONS_V1
 *
 *      @author [sqlite2cpp.py]
 *
 *      Revision History:
 *          AUTO GENERATED at 2026-02-25 08:58:12.230056.
 *          DO NOT EDIT!
 */
//=============================================================================

#include "_TableFactory.tpp"
#include "SchedSplitTable.h"
#include "data/SchedSplitData.h"

template class TableFactory<SchedSplitTable, SchedSplitData>;
template class mmCache<int64, SchedSplitData>;

// List of column names in database table BUDGETSPLITTRANSACTIONS_V1,
// in the order of SchedSplitCol::COL_ID.
const wxArrayString SchedSplitCol::COL_NAME_A = {
    "SPLITTRANSID",
    "TRANSID",
    "CATEGID",
    "SPLITTRANSAMOUNT",
    "NOTES"
};

const SchedSplitCol::COL_ID SchedSplitCol::PRIMARY_ID = COL_ID_SPLITTRANSID;
const wxString SchedSplitCol::PRIMARY_NAME = COL_NAME_A[COL_ID_SPLITTRANSID];

SchedSplitRow::SchedSplitRow()
{
    SPLITTRANSID = -1;
    TRANSID = -1;
    CATEGID = -1;
    SPLITTRANSAMOUNT = 0.0;
}

// Bind a Row record to database insert statement.
void SchedSplitRow::to_insert_stmt(wxSQLite3Statement& stmt, int64 id) const
{
    stmt.Bind(1, TRANSID);
    stmt.Bind(2, CATEGID);
    stmt.Bind(3, SPLITTRANSAMOUNT);
    stmt.Bind(4, NOTES);
    stmt.Bind(5, id);
}

SchedSplitRow& SchedSplitRow::from_select_result(wxSQLite3ResultSet& q)
{
    SPLITTRANSID = q.GetInt64(0);
    TRANSID = q.GetInt64(1);
    CATEGID = q.GetInt64(2);
    SPLITTRANSAMOUNT = q.GetDouble(3);
    NOTES = q.GetString(4);

    return *this;
}

// Return the data record as a json string
wxString SchedSplitRow::to_json() const
{
    StringBuffer json_buffer;
    PrettyWriter<StringBuffer> json_writer(json_buffer);

    json_writer.StartObject();			
    as_json(json_writer);
    json_writer.EndObject();

    return json_buffer.GetString();
}

// Add the field data as json key:value pairs
void SchedSplitRow::as_json(PrettyWriter<StringBuffer>& json_writer) const
{
    json_writer.Key("SPLITTRANSID");
    json_writer.Int64(SPLITTRANSID.GetValue());

    json_writer.Key("TRANSID");
    json_writer.Int64(TRANSID.GetValue());

    json_writer.Key("CATEGID");
    json_writer.Int64(CATEGID.GetValue());

    json_writer.Key("SPLITTRANSAMOUNT");
    json_writer.Double(SPLITTRANSAMOUNT);

    json_writer.Key("NOTES");
    json_writer.String(NOTES.utf8_str());
}

row_t SchedSplitRow::to_html_row() const
{
    row_t row;

    row(L"SPLITTRANSID") = SPLITTRANSID.GetValue();
    row(L"TRANSID") = TRANSID.GetValue();
    row(L"CATEGID") = CATEGID.GetValue();
    row(L"SPLITTRANSAMOUNT") = SPLITTRANSAMOUNT;
    row(L"NOTES") = NOTES;

    return row;
}

void SchedSplitRow::to_html_template(html_template& t) const
{
    t(L"SPLITTRANSID") = SPLITTRANSID.GetValue();
    t(L"TRANSID") = TRANSID.GetValue();
    t(L"CATEGID") = CATEGID.GetValue();
    t(L"SPLITTRANSAMOUNT") = SPLITTRANSAMOUNT;
    t(L"NOTES") = NOTES;
}

bool SchedSplitRow::equals(const SchedSplitRow* other) const
{
    if ( SPLITTRANSID != other->SPLITTRANSID) return false;
    if ( TRANSID != other->TRANSID) return false;
    if ( CATEGID != other->CATEGID) return false;
    if ( SPLITTRANSAMOUNT != other->SPLITTRANSAMOUNT) return false;
    if (!NOTES.IsSameAs(other->NOTES)) return false;

    return true;
}

SchedSplitTable::SchedSplitTable()
{
    m_table_name = "BUDGETSPLITTRANSACTIONS_V1";

    m_create_query = "CREATE TABLE BUDGETSPLITTRANSACTIONS_V1(SPLITTRANSID integer primary key, TRANSID integer NOT NULL, CATEGID integer, SPLITTRANSAMOUNT numeric, NOTES TEXT)";

    m_drop_query = "DROP TABLE IF EXISTS BUDGETSPLITTRANSACTIONS_V1";

    m_index_query_a = {
        "CREATE INDEX IF NOT EXISTS IDX_BUDGETSPLITTRANSACTIONS_TRANSID ON BUDGETSPLITTRANSACTIONS_V1(TRANSID)"
    };

    m_insert_query = "INSERT INTO BUDGETSPLITTRANSACTIONS_V1(TRANSID, CATEGID, SPLITTRANSAMOUNT, NOTES, SPLITTRANSID) VALUES(?, ?, ?, ?, ?)";

    m_update_query = "UPDATE BUDGETSPLITTRANSACTIONS_V1 SET TRANSID = ?, CATEGID = ?, SPLITTRANSAMOUNT = ?, NOTES = ? WHERE SPLITTRANSID = ?";

    m_delete_query = "DELETE FROM BUDGETSPLITTRANSACTIONS_V1 WHERE SPLITTRANSID = ?";

    m_select_query = "SELECT SPLITTRANSID, TRANSID, CATEGID, SPLITTRANSAMOUNT, NOTES FROM BUDGETSPLITTRANSACTIONS_V1";
}
