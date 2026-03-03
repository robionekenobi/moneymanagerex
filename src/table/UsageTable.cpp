// -*- C++ -*-
//=============================================================================
/**
 *      Copyright: (c) 2013-2026 Guan Lisheng (guanlisheng@gmail.com)
 *      Copyright: (c) 2017-2018 Stefano Giorgio (stef145g)
 *      Copyright: (c) 2022      Mark Whalley (mark@ipx.co.uk)
 *      Copyright: (c) 2026      George Ef (george.a.ef@gmail.com)
 *
 *      UsageTable.cpp
 *
 *      Implementation of the interface to database table USAGE_V1
 *
 *      @author [sqlite2cpp.py]
 *
 *      Revision History:
 *          AUTO GENERATED at 2026-02-25 08:58:12.230056.
 *          DO NOT EDIT!
 */
//=============================================================================

#include "_TableFactory.tpp"
#include "UsageTable.h"
#include "data/UsageData.h"

template class TableFactory<UsageTable, UsageData>;
template class mmCache<int64, UsageData>;

// List of column names in database table USAGE_V1,
// in the order of UsageCol::COL_ID.
const wxArrayString UsageCol::COL_NAME_A = {
    "USAGEID",
    "USAGEDATE",
    "JSONCONTENT"
};

const UsageCol::COL_ID UsageCol::PRIMARY_ID = COL_ID_USAGEID;
const wxString UsageCol::PRIMARY_NAME = COL_NAME_A[COL_ID_USAGEID];

UsageRow::UsageRow()
{
    USAGEID = -1;
}

// Bind a Row record to database insert statement.
void UsageRow::to_insert_stmt(wxSQLite3Statement& stmt, int64 id) const
{
    stmt.Bind(1, USAGEDATE);
    stmt.Bind(2, JSONCONTENT);
    stmt.Bind(3, id);
}

UsageRow& UsageRow::from_select_result(wxSQLite3ResultSet& q)
{
    USAGEID = q.GetInt64(0);
    USAGEDATE = q.GetString(1);
    JSONCONTENT = q.GetString(2);

    return *this;
}

// Return the data record as a json string
wxString UsageRow::to_json() const
{
    StringBuffer json_buffer;
    PrettyWriter<StringBuffer> json_writer(json_buffer);

    json_writer.StartObject();			
    as_json(json_writer);
    json_writer.EndObject();

    return json_buffer.GetString();
}

// Add the field data as json key:value pairs
void UsageRow::as_json(PrettyWriter<StringBuffer>& json_writer) const
{
    json_writer.Key("USAGEID");
    json_writer.Int64(USAGEID.GetValue());

    json_writer.Key("USAGEDATE");
    json_writer.String(USAGEDATE.utf8_str());

    json_writer.Key("JSONCONTENT");
    json_writer.String(JSONCONTENT.utf8_str());
}

row_t UsageRow::to_html_row() const
{
    row_t row;

    row(L"USAGEID") = USAGEID.GetValue();
    row(L"USAGEDATE") = USAGEDATE;
    row(L"JSONCONTENT") = JSONCONTENT;

    return row;
}

void UsageRow::to_html_template(html_template& t) const
{
    t(L"USAGEID") = USAGEID.GetValue();
    t(L"USAGEDATE") = USAGEDATE;
    t(L"JSONCONTENT") = JSONCONTENT;
}

bool UsageRow::equals(const UsageRow* other) const
{
    if ( USAGEID != other->USAGEID) return false;
    if (!USAGEDATE.IsSameAs(other->USAGEDATE)) return false;
    if (!JSONCONTENT.IsSameAs(other->JSONCONTENT)) return false;

    return true;
}

UsageTable::UsageTable()
{
    m_table_name = "USAGE_V1";

    m_create_query = "CREATE TABLE USAGE_V1 (USAGEID INTEGER NOT NULL PRIMARY KEY, USAGEDATE TEXT NOT NULL, JSONCONTENT TEXT NOT NULL)";

    m_drop_query = "DROP TABLE IF EXISTS USAGE_V1";

    m_index_query_a = {
        "CREATE INDEX IF NOT EXISTS IDX_USAGE_DATE ON USAGE_V1 (USAGEDATE)"
    };

    m_insert_query = "INSERT INTO USAGE_V1(USAGEDATE, JSONCONTENT, USAGEID) VALUES(?, ?, ?)";

    m_update_query = "UPDATE USAGE_V1 SET USAGEDATE = ?, JSONCONTENT = ? WHERE USAGEID = ?";

    m_delete_query = "DELETE FROM USAGE_V1 WHERE USAGEID = ?";

    m_select_query = "SELECT USAGEID, USAGEDATE, JSONCONTENT FROM USAGE_V1";
}
