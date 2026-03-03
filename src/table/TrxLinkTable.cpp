// -*- C++ -*-
//=============================================================================
/**
 *      Copyright: (c) 2013-2026 Guan Lisheng (guanlisheng@gmail.com)
 *      Copyright: (c) 2017-2018 Stefano Giorgio (stef145g)
 *      Copyright: (c) 2022      Mark Whalley (mark@ipx.co.uk)
 *      Copyright: (c) 2026      George Ef (george.a.ef@gmail.com)
 *
 *      TrxLinkTable.cpp
 *
 *      Implementation of the interface to database table TRANSLINK_V1
 *
 *      @author [sqlite2cpp.py]
 *
 *      Revision History:
 *          AUTO GENERATED at 2026-02-25 08:58:12.230056.
 *          DO NOT EDIT!
 */
//=============================================================================

#include "_TableFactory.tpp"
#include "TrxLinkTable.h"
#include "data/TrxLinkData.h"

template class TableFactory<TrxLinkTable, TrxLinkData>;
template class mmCache<int64, TrxLinkData>;

// List of column names in database table TRANSLINK_V1,
// in the order of TrxLinkCol::COL_ID.
const wxArrayString TrxLinkCol::COL_NAME_A = {
    "TRANSLINKID",
    "CHECKINGACCOUNTID",
    "LINKTYPE",
    "LINKRECORDID"
};

const TrxLinkCol::COL_ID TrxLinkCol::PRIMARY_ID = COL_ID_TRANSLINKID;
const wxString TrxLinkCol::PRIMARY_NAME = COL_NAME_A[COL_ID_TRANSLINKID];

TrxLinkRow::TrxLinkRow()
{
    TRANSLINKID = -1;
    CHECKINGACCOUNTID = -1;
    LINKRECORDID = -1;
}

// Bind a Row record to database insert statement.
void TrxLinkRow::to_insert_stmt(wxSQLite3Statement& stmt, int64 id) const
{
    stmt.Bind(1, CHECKINGACCOUNTID);
    stmt.Bind(2, LINKTYPE);
    stmt.Bind(3, LINKRECORDID);
    stmt.Bind(4, id);
}

TrxLinkRow& TrxLinkRow::from_select_result(wxSQLite3ResultSet& q)
{
    TRANSLINKID = q.GetInt64(0);
    CHECKINGACCOUNTID = q.GetInt64(1);
    LINKTYPE = q.GetString(2);
    LINKRECORDID = q.GetInt64(3);

    return *this;
}

// Return the data record as a json string
wxString TrxLinkRow::to_json() const
{
    StringBuffer json_buffer;
    PrettyWriter<StringBuffer> json_writer(json_buffer);

    json_writer.StartObject();			
    as_json(json_writer);
    json_writer.EndObject();

    return json_buffer.GetString();
}

// Add the field data as json key:value pairs
void TrxLinkRow::as_json(PrettyWriter<StringBuffer>& json_writer) const
{
    json_writer.Key("TRANSLINKID");
    json_writer.Int64(TRANSLINKID.GetValue());

    json_writer.Key("CHECKINGACCOUNTID");
    json_writer.Int64(CHECKINGACCOUNTID.GetValue());

    json_writer.Key("LINKTYPE");
    json_writer.String(LINKTYPE.utf8_str());

    json_writer.Key("LINKRECORDID");
    json_writer.Int64(LINKRECORDID.GetValue());
}

row_t TrxLinkRow::to_html_row() const
{
    row_t row;

    row(L"TRANSLINKID") = TRANSLINKID.GetValue();
    row(L"CHECKINGACCOUNTID") = CHECKINGACCOUNTID.GetValue();
    row(L"LINKTYPE") = LINKTYPE;
    row(L"LINKRECORDID") = LINKRECORDID.GetValue();

    return row;
}

void TrxLinkRow::to_html_template(html_template& t) const
{
    t(L"TRANSLINKID") = TRANSLINKID.GetValue();
    t(L"CHECKINGACCOUNTID") = CHECKINGACCOUNTID.GetValue();
    t(L"LINKTYPE") = LINKTYPE;
    t(L"LINKRECORDID") = LINKRECORDID.GetValue();
}

bool TrxLinkRow::equals(const TrxLinkRow* other) const
{
    if ( TRANSLINKID != other->TRANSLINKID) return false;
    if ( CHECKINGACCOUNTID != other->CHECKINGACCOUNTID) return false;
    if (!LINKTYPE.IsSameAs(other->LINKTYPE)) return false;
    if ( LINKRECORDID != other->LINKRECORDID) return false;

    return true;
}

TrxLinkTable::TrxLinkTable()
{
    m_table_name = "TRANSLINK_V1";

    m_create_query = "CREATE TABLE TRANSLINK_V1 (TRANSLINKID  integer NOT NULL primary key, CHECKINGACCOUNTID integer NOT NULL, LINKTYPE TEXT NOT NULL /* Asset, Stock */, LINKRECORDID integer NOT NULL)";

    m_drop_query = "DROP TABLE IF EXISTS TRANSLINK_V1";

    m_index_query_a = {
        "CREATE INDEX IF NOT EXISTS IDX_CHECKINGACCOUNT ON TRANSLINK_V1 (CHECKINGACCOUNTID)",
        "CREATE INDEX IF NOT EXISTS IDX_LINKRECORD ON TRANSLINK_V1 (LINKTYPE, LINKRECORDID)"
    };

    m_insert_query = "INSERT INTO TRANSLINK_V1(CHECKINGACCOUNTID, LINKTYPE, LINKRECORDID, TRANSLINKID) VALUES(?, ?, ?, ?)";

    m_update_query = "UPDATE TRANSLINK_V1 SET CHECKINGACCOUNTID = ?, LINKTYPE = ?, LINKRECORDID = ? WHERE TRANSLINKID = ?";

    m_delete_query = "DELETE FROM TRANSLINK_V1 WHERE TRANSLINKID = ?";

    m_select_query = "SELECT TRANSLINKID, CHECKINGACCOUNTID, LINKTYPE, LINKRECORDID FROM TRANSLINK_V1";
}
