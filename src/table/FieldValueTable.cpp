// -*- C++ -*-
//=============================================================================
/**
 *      Copyright: (c) 2013-2026 Guan Lisheng (guanlisheng@gmail.com)
 *      Copyright: (c) 2017-2018 Stefano Giorgio (stef145g)
 *      Copyright: (c) 2022      Mark Whalley (mark@ipx.co.uk)
 *      Copyright: (c) 2026      George Ef (george.a.ef@gmail.com)
 *
 *      FieldValueTable.cpp
 *
 *      Implementation of the interface to database table CUSTOMFIELDDATA_V1
 *
 *      @author [sqlite2cpp.py]
 *
 *      Revision History:
 *          AUTO GENERATED at 2026-02-25 08:58:12.230056.
 *          DO NOT EDIT!
 */
//=============================================================================

#include "_TableFactory.tpp"
#include "FieldValueTable.h"
#include "data/FieldValueData.h"

template class TableFactory<FieldValueTable, FieldValueData>;
template class mmCache<int64, FieldValueData>;

// List of column names in database table CUSTOMFIELDDATA_V1,
// in the order of FieldValueCol::COL_ID.
const wxArrayString FieldValueCol::COL_NAME_A = {
    "FIELDATADID",
    "FIELDID",
    "REFID",
    "CONTENT"
};

const FieldValueCol::COL_ID FieldValueCol::PRIMARY_ID = COL_ID_FIELDATADID;
const wxString FieldValueCol::PRIMARY_NAME = COL_NAME_A[COL_ID_FIELDATADID];

FieldValueRow::FieldValueRow()
{
    FIELDATADID = -1;
    FIELDID = -1;
    REFID = -1;
}

// Bind a Row record to database insert statement.
void FieldValueRow::to_insert_stmt(wxSQLite3Statement& stmt, int64 id) const
{
    stmt.Bind(1, FIELDID);
    stmt.Bind(2, REFID);
    stmt.Bind(3, CONTENT);
    stmt.Bind(4, id);
}

FieldValueRow& FieldValueRow::from_select_result(wxSQLite3ResultSet& q)
{
    FIELDATADID = q.GetInt64(0);
    FIELDID = q.GetInt64(1);
    REFID = q.GetInt64(2);
    CONTENT = q.GetString(3);

    return *this;
}

// Return the data record as a json string
wxString FieldValueRow::to_json() const
{
    StringBuffer json_buffer;
    PrettyWriter<StringBuffer> json_writer(json_buffer);

    json_writer.StartObject();			
    as_json(json_writer);
    json_writer.EndObject();

    return json_buffer.GetString();
}

// Add the field data as json key:value pairs
void FieldValueRow::as_json(PrettyWriter<StringBuffer>& json_writer) const
{
    json_writer.Key("FIELDATADID");
    json_writer.Int64(FIELDATADID.GetValue());

    json_writer.Key("FIELDID");
    json_writer.Int64(FIELDID.GetValue());

    json_writer.Key("REFID");
    json_writer.Int64(REFID.GetValue());

    json_writer.Key("CONTENT");
    json_writer.String(CONTENT.utf8_str());
}

row_t FieldValueRow::to_html_row() const
{
    row_t row;

    row(L"FIELDATADID") = FIELDATADID.GetValue();
    row(L"FIELDID") = FIELDID.GetValue();
    row(L"REFID") = REFID.GetValue();
    row(L"CONTENT") = CONTENT;

    return row;
}

void FieldValueRow::to_html_template(html_template& t) const
{
    t(L"FIELDATADID") = FIELDATADID.GetValue();
    t(L"FIELDID") = FIELDID.GetValue();
    t(L"REFID") = REFID.GetValue();
    t(L"CONTENT") = CONTENT;
}

bool FieldValueRow::equals(const FieldValueRow* other) const
{
    if ( FIELDATADID != other->FIELDATADID) return false;
    if ( FIELDID != other->FIELDID) return false;
    if ( REFID != other->REFID) return false;
    if (!CONTENT.IsSameAs(other->CONTENT)) return false;

    return true;
}

FieldValueTable::FieldValueTable()
{
    m_table_name = "CUSTOMFIELDDATA_V1";

    m_create_query = "CREATE TABLE CUSTOMFIELDDATA_V1 (FIELDATADID INTEGER NOT NULL PRIMARY KEY, FIELDID INTEGER NOT NULL, REFID INTEGER NOT NULL, CONTENT TEXT, UNIQUE(FIELDID, REFID))";

    m_drop_query = "DROP TABLE IF EXISTS CUSTOMFIELDDATA_V1";

    m_index_query_a = {
        "CREATE INDEX IF NOT EXISTS IDX_CUSTOMFIELDDATA_REF ON CUSTOMFIELDDATA_V1 (FIELDID, REFID)"
    };

    m_insert_query = "INSERT INTO CUSTOMFIELDDATA_V1(FIELDID, REFID, CONTENT, FIELDATADID) VALUES(?, ?, ?, ?)";

    m_update_query = "UPDATE CUSTOMFIELDDATA_V1 SET FIELDID = ?, REFID = ?, CONTENT = ? WHERE FIELDATADID = ?";

    m_delete_query = "DELETE FROM CUSTOMFIELDDATA_V1 WHERE FIELDATADID = ?";

    m_select_query = "SELECT FIELDATADID, FIELDID, REFID, CONTENT FROM CUSTOMFIELDDATA_V1";
}
