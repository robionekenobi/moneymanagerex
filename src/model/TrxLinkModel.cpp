/*******************************************************
 Copyright (C) 2013,2014 Guan Lisheng (guanlisheng@gmail.com)
 Copyright (C) 2016 Stefano Giorgio
 Copyright (C) 2025 Klaus Wich
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

#include "TrxLinkModel.h"
#include "TrxShareModel.h"
#include "CurrencyHistoryModel.h"

// -- static

TrxLinkModel::CHECKING_TYPE TrxLinkModel::type_checking(const int64 tt)
{
    if (tt == AS_INCOME_EXPENSE || tt == -1) {
        return AS_INCOME_EXPENSE;
    }
    else {
        return AS_TRANSFER;
    }
}

// -- constructor

// Initialize the global TrxLinkModel table.
// Reset the TrxLinkModel table or create the table if it does not exist.
TrxLinkModel& TrxLinkModel::instance(wxSQLite3Database* db)
{
    TrxLinkModel& ins = Singleton<TrxLinkModel>::instance();
    ins.reset_cache();
    ins.m_db = db;
    ins.ensure_table();

    return ins;
}

// Return the static instance of TrxLinkModel table
TrxLinkModel& TrxLinkModel::instance()
{
    return Singleton<TrxLinkModel>::instance();
}

// -- methods

bool TrxLinkModel::purge_trxId_all(const int64 trx_id)
{
    bool ok = true;

    for (int64 tl_id : find_id_a(
        TrxLinkCol::WHERE_CHECKINGACCOUNTID(OP_EQ, trx_id)
    )) {
        ok = ok && purge_id(tl_id);
    }

    return ok;
}

// Remove all records associated with the Translink list
bool TrxLinkModel::Z_purge_ref(RefTypeN ref_type, int64 ref_id)
{
    bool ok = true;

    // FIXME: purge tl_d
    for (const Data& tl_d : find_ref_data_a(ref_type, ref_id)) {
        ok = ok && TrxModel::instance().purge_id(tl_d.m_trx_id);
    }

    return ok;
}

// Return the link record for the checking account
// Equivalent SQL statements:
//     select * from TRANSLINK_V1 where CHECKINGACCOUNTID = checking_id;
const TrxLinkData* TrxLinkModel::get_trx_data_n(int64 trx_id)
{
    for (int64 tl_id : find_id_a(
        TrxLinkCol::WHERE_CHECKINGACCOUNTID(OP_EQ, trx_id)
    )) {
        return get_id_data_n(tl_id);
    }

    return nullptr;
}

// Return a list of translink records for the associated foreign table type.
// Equivalent SQL statements:
//    select * from TRANSLINK_V1 where LINKTYPE = "Asset" AND LINKRECORDID = ref_id;
//    select * from TRANSLINK_V1 where LINKTYPE = "Stock" AND LINKRECORDID = ref_id;
TrxLinkModel::DataA TrxLinkModel::find_ref_data_a(RefTypeN ref_type, int64 ref_id)
{
    return find_data_a(
        TrxLinkCol::WHERE_LINKTYPE(OP_EQ, ref_type.key_n()),
        TrxLinkCol::WHERE_LINKRECORDID(OP_EQ, ref_id)
    );
}

// Return the link record for the symbol
// Equivalent SQL statements:
//     SELECT * FROM TRANSLINK_V1 WHERE LINKTYPE = "Stock" AND LINKRECORDID IN
//       (SELECT STOCKID FROM STOCK_V1 WHERE SYMBOL = ?)
TrxLinkModel::DataA TrxLinkModel::find_symbol_data_a(const wxString stock_symbol)
{
    DataA symbol_tl_a;
    for (auto& stock_d : StockModel::instance().find_data_a(
        StockCol::WHERE_SYMBOL(OP_EQ, stock_symbol)
    )) {
        DataA stock_tl_a = find_ref_data_a(StockModel::s_ref_type, stock_d.m_id);
        symbol_tl_a.insert(symbol_tl_a.end(), stock_tl_a.begin(), stock_tl_a.end());
    }
    return symbol_tl_a;
}

size_t TrxLinkModel::find_stock_id_c(const int64 stock_id)
{
    return find_ref_data_a(StockModel::s_ref_type, stock_id).size();
}

void TrxLinkModel::update_asset_value(AssetData* asset_n)
{
    DataA tl_a = find_ref_data_a(
        AssetModel::s_ref_type, asset_n->m_id
    );
    double new_value = 0;
    for (const auto& tl_d : tl_a) {
        const TrxData* trx_n = TrxModel::instance().get_id_data_n(tl_d.m_trx_id);
        if (!trx_n || !trx_n->is_valid())
            continue;

        const CurrencyData* currency_n = AccountModel::instance().get_id_currency_p(
            trx_n->m_account_id
        );
        const double conv_rate = CurrencyHistoryModel::instance().get_id_date_rate(
            currency_n->m_id,
            trx_n->m_date()
        );

        if (trx_n->is_deposit()) {
            new_value -= trx_n->m_amount * conv_rate; // Withdrawal from asset value
        }
        else {
            new_value += trx_n->m_amount * conv_rate;  // Deposit to asset value
        }
    }

    if (asset_n->m_value != new_value) {
        asset_n->m_value = new_value;
        AssetModel::instance().unsafe_save_data_n(asset_n);
    }
}

void TrxLinkModel::save_record(
    int64 trx_id,
    RefTypeN ref_type,
    int64 ref_id,
    [[maybe_unused]] const CHECKING_TYPE checking_type
) {
    TrxLinkData new_tl_d = TrxLinkData();
    new_tl_d.m_trx_id   = trx_id;
    new_tl_d.m_ref_type = ref_type;
    new_tl_d.m_ref_id   = ref_id;
    add_data_n(new_tl_d);

    // set the checking entry to recognise it as a foreign transaction
    // set the checking type as AS_INCOME_EXPENSE = 32701 or AS_TRANSFER
    TrxData* trx_n = TrxModel::instance().unsafe_get_id_data_n(trx_id);
    // FIXME
    // trx_n->m_to_account_id_n = checking_type;
    TrxModel::instance().unsafe_save_trx_n(trx_n);
    //TrxLinkModel::instance().get_id_data_n(new_tl_d.m_id);
}

// Create the translink record as Asset
void TrxLinkModel::save_asset_record(
    int64 trx_id,
    int64 asset_id,
    const CHECKING_TYPE checking_type
){
    save_record(trx_id, AssetModel::s_ref_type, asset_id, checking_type);
}

// Create a translink record as Stock
void TrxLinkModel::save_stock_record(
    int64 trx_id,
    int64 stock_id,
    const CHECKING_TYPE checking_type
) {
    save_record(trx_id, StockModel::s_ref_type, stock_id, checking_type);
}
