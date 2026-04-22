// -*- C++ -*-
//=============================================================================
/**
 *      Copyright: (c) 2013-2026 Guan Lisheng (guanlisheng@gmail.com)
 *      Copyright: (c) 2017-2018 Stefano Giorgio (stef145g)
 *      Copyright: (c) 2022      Mark Whalley (mark@ipx.co.uk)
 *      Copyright: (c) 2026      George Ef (george.a.ef@gmail.com)
 *
 *      BudgetPeriodTable.cpp
 *
 *      Implementation of the interface to database table BUDGETYEAR_V1
 *
 *      @author [sqlite2cpp.py]
 *
 *      Revision History:
 *          AUTO GENERATED at 2026-04-11 13:27:18.900578.
 *          DO NOT EDIT!
 */
//=============================================================================

#include "_TableFactory.tpp"
#include "BudgetPeriodTable.h"
#include "data/BudgetPeriodData.h"

template class TableFactory<BudgetPeriodTable, BudgetPeriodData>;
template class mmCache<int64, BudgetPeriodData>;

// List of column names in database table BUDGETYEAR_V1,
// in the order of BudgetPeriodCol::COL_ID.
const wxArrayString BudgetPeriodCol::s_col_name_a = {
    "BUDGETYEARID",
    "BUDGETYEARNAME"
};

const BudgetPeriodCol::COL_ID BudgetPeriodCol::s_primary_id = COL_ID_BUDGETYEARID;
const wxString BudgetPeriodCol::s_primary_name = s_col_name_a[COL_ID_BUDGETYEARID];

// convenience variables
const wxString BudgetPeriodCol::NAME_BUDGETYEARID = s_col_name_a[COL_ID_BUDGETYEARID];
const wxString BudgetPeriodCol::NAME_BUDGETYEARNAME = s_col_name_a[COL_ID_BUDGETYEARNAME];

BudgetPeriodRow::BudgetPeriodRow()
{
    BUDGETYEARID = -1;
}

// Bind a Row record to database insert statement.
void BudgetPeriodRow::to_insert_stmt(wxSQLite3Statement& stmt, int64 id) const
{
    stmt.Bind(1, BUDGETYEARNAME);
    stmt.Bind(2, id);
}

BudgetPeriodRow& BudgetPeriodRow::from_select_result(wxSQLite3ResultSet& q)
{
    BUDGETYEARID = q.GetInt64(0);
    BUDGETYEARNAME = q.GetString(1);

    return *this;
}

// Return the data record as a json string
wxString BudgetPeriodRow::to_json() const
{
    StringBuffer json_buffer;
    PrettyWriter<StringBuffer> json_writer(json_buffer);

    json_writer.StartObject();			
    as_json(json_writer);
    json_writer.EndObject();

    return json_buffer.GetString();
}

// Add the field data as json key:value pairs
void BudgetPeriodRow::as_json(PrettyWriter<StringBuffer>& json_writer) const
{
    json_writer.Key("BUDGETYEARID");
    json_writer.Int64(BUDGETYEARID.GetValue());

    json_writer.Key("BUDGETYEARNAME");
    json_writer.String(BUDGETYEARNAME.utf8_str());
}

row_t BudgetPeriodRow::to_html_row() const
{
    row_t row;

    row(L"BUDGETYEARID") = BUDGETYEARID.GetValue();
    row(L"BUDGETYEARNAME") = BUDGETYEARNAME;

    return row;
}

void BudgetPeriodRow::to_html_template(html_template& t) const
{
    t(L"BUDGETYEARID") = BUDGETYEARID.GetValue();
    t(L"BUDGETYEARNAME") = BUDGETYEARNAME;
}

bool BudgetPeriodRow::equals(const BudgetPeriodRow* other) const
{
    if ( BUDGETYEARID != other->BUDGETYEARID) return false;
    if (!BUDGETYEARNAME.IsSameAs(other->BUDGETYEARNAME)) return false;

    return true;
}

BudgetPeriodTable::BudgetPeriodTable()
{
    m_table_name = "BUDGETYEAR_V1";

    m_create_query = "CREATE TABLE BUDGETYEAR_V1(BUDGETYEARID integer primary key, BUDGETYEARNAME TEXT NOT NULL UNIQUE)";

    m_drop_query = "DROP TABLE IF EXISTS BUDGETYEAR_V1";

    m_index_query_a = {
        "CREATE INDEX IF NOT EXISTS IDX_BUDGETYEAR_BUDGETYEARNAME ON BUDGETYEAR_V1(BUDGETYEARNAME)"
    };

    m_insert_query = "INSERT INTO BUDGETYEAR_V1(BUDGETYEARNAME, BUDGETYEARID) VALUES(?, ?)";

    m_update_query = "UPDATE BUDGETYEAR_V1 SET BUDGETYEARNAME = ? WHERE BUDGETYEARID = ?";

    m_delete_query = "DELETE FROM BUDGETYEAR_V1 WHERE BUDGETYEARID = ?";

    m_select_query = "SELECT BUDGETYEARID, BUDGETYEARNAME FROM BUDGETYEAR_V1";
}
