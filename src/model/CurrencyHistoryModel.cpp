/*******************************************************
 Copyright (C) 2015 Gabriele-V
 Copyright (C) 2026 George Ef (george.a.ef@gmail.com)

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
********************************************************/

#include "CurrencyModel.h"
#include "CurrencyHistoryModel.h"
#include "PrefModel.h"

// -- static

TableClauseV<wxString> CurrencyHistoryModel::WHERE_DATE(OP op, const mmDate& date)
{
    return CurrencyHistoryCol::WHERE_CURRDATE(op, date.isoDate());
}

// -- constructor

// Initialize the global CurrencyHistoryModel table.
// Reset the CurrencyHistoryModel table or create the table if it does not exist.
CurrencyHistoryModel& CurrencyHistoryModel::instance(wxSQLite3Database* db)
{
    CurrencyHistoryModel& ins = Singleton<CurrencyHistoryModel>::instance();
    ins.m_db = db;
    ins.ensure_table();

    return ins;
}

// Return the static instance of CurrencyHistoryModel table
CurrencyHistoryModel& CurrencyHistoryModel::instance()
{
    return Singleton<CurrencyHistoryModel>::instance();
}

// -- methods

bool CurrencyHistoryModel::purge_currencyId_all(int64 currency_id)
{
    bool ok = true;
    db_savepoint();

    for (int64 uh_id : find_id_a(
        CurrencyHistoryCol::WHERE_CURRENCYID(OP_EQ, currency_id)
    )) {
        ok = ok && purge_id(uh_id);
    }

    db_release_savepoint();
    return ok;
}

// Clears the currency History table
bool CurrencyHistoryModel::purge_all()
{
    bool ok = true;
    db_savepoint();

    for (int64 uh_id : find_id_a()) {
        ok = ok && purge_id(uh_id);
    }

    db_release_savepoint();
    return ok;
}

const CurrencyHistoryData* CurrencyHistoryModel::get_key_data_n(
    int64 currency_id,
    const mmDate& date
) {
    const Data* uh_n = search_cache_n(
        CurrencyHistoryCol::CURRENCYID(currency_id),
        CurrencyHistoryCol::CURRDATE(OP_EQ, date.isoDate())
    );
    if (uh_n)
        return uh_n;

    for (int64 uh_id : find_id_a(
        CurrencyHistoryCol::WHERE_CURRENCYID(OP_EQ, currency_id),
        CurrencyHistoryModel::WHERE_DATE(OP_EQ, date)
    )) {
        uh_n = get_idN_data_n(uh_id);
        break;
    }

    return uh_n;
}

double CurrencyHistoryModel::get_id_date_rate(int64 currency_id_n, const mmDate& date)
{
    if (currency_id_n == CurrencyModel::instance().get_base_data_n()->m_id ||
        currency_id_n == -1
    )
        return 1.0;

    const CurrencyData* currency_n = CurrencyModel::instance().get_idN_data_n(currency_id_n);
    if (!PrefModel::instance().getUseCurrencyHistory())
        return currency_n->m_base_conv_rate;

    // Search for specified date
    for (const Data& uh_d : CurrencyHistoryModel::instance().find_data_a(
        CurrencyHistoryCol::WHERE_CURRENCYID(OP_EQ, currency_id_n),
        CurrencyHistoryModel::WHERE_DATE(OP_EQ, date)
    )) {
        return uh_d.m_base_conv_rate;
    }

    if (find_count(
        CurrencyHistoryCol::WHERE_CURRENCYID(OP_EQ, currency_id_n)
    ) == 0) {
        return currency_n->m_base_conv_rate;
    }

    // Rate not found for specified date, look at previous and next
    CurrencyHistoryModel::DataA prev_uh_a = find_data_a(
        CurrencyHistoryCol::WHERE_CURRENCYID(OP_EQ, currency_id_n),
        CurrencyHistoryModel::WHERE_DATE(OP_LE, date),
        TableClause::ORDERBY(CurrencyHistoryCol::NAME_CURRDATE, true),
        TableClause::LIMIT(1)
    );
    CurrencyHistoryModel::DataA next_uh_a = find_data_a(
        CurrencyHistoryCol::WHERE_CURRENCYID(OP_EQ, currency_id_n),
        CurrencyHistoryModel::WHERE_DATE(OP_GE, date),
        TableClause::ORDERBY(CurrencyHistoryCol::NAME_CURRDATE, false),
        TableClause::LIMIT(1)
    );

    if (!prev_uh_a.empty() && !next_uh_a.empty()) {
        mmDate date_copy = date;
        int prev_days = date_copy.daysSince(prev_uh_a[0].m_date);
        int next_days = date_copy.daysUntil(next_uh_a[0].m_date);
        return prev_days <= next_days
            ? prev_uh_a[0].m_base_conv_rate
            : next_uh_a[0].m_base_conv_rate;
    }
    else if (!prev_uh_a.empty()) {
        return prev_uh_a[0].m_base_conv_rate;
    }
    else if (!next_uh_a.empty()) {
        return next_uh_a[0].m_base_conv_rate;
    }

    return currency_n->m_base_conv_rate;
}

// Return the last rate for specified currency
double CurrencyHistoryModel::get_id_last_rate(int64 currency_id)
{
    if (PrefModel::instance().getUseCurrencyHistory()) {
        for (const Data& uh_d : find_data_a(
            CurrencyHistoryCol::WHERE_CURRENCYID(OP_EQ, currency_id),
            TableClause::ORDERBY(Col::NAME_CURRDATE, true),
            TableClause::ORDERBY(Col::s_primary_name, true),
            TableClause::LIMIT(1)
        )) {
            return uh_d.m_base_conv_rate;
        }
    }

    const CurrencyData* currency_n = CurrencyModel::instance().get_idN_data_n(
        currency_id
    );
    return currency_n->m_base_conv_rate;
}

// Adds or updates an element in currency history
int64 CurrencyHistoryModel::save_record(
    int64 currency_id,
    const mmDate& date,
    double price,
    UpdateType update_type
) {
    const Data *uh_n = get_key_data_n(currency_id, date);
    Data uh_d = uh_n ? *uh_n : Data();
    uh_d.m_currency_id    = currency_id;
    uh_d.m_date           = date;
    uh_d.m_base_conv_rate = price;
    uh_d.m_update_type    = update_type;
    save_data_n(uh_d);
    return uh_d.m_id;
}
