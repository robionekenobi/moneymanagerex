/*******************************************************
 Copyright (C) 2013,2014 Guan Lisheng (guanlisheng@gmail.com)
 Copyright (C) 2022 Mark Whalley (mark@ipx.co.uk)
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

#include "TrxSplitModel.h"
#include "CategoryModel.h"
#include "TrxModel.h"

// -- static

const RefTypeN TrxSplitModel::s_ref_type = RefTypeN(RefTypeN::e_trx_split);

// -- constructor

// Initialize the global TrxSplitModel table.
// Reset the TrxSplitModel table or create the table if it does not exist.
TrxSplitModel& TrxSplitModel::instance(wxSQLite3Database* db)
{
    TrxSplitModel& ins = Singleton<TrxSplitModel>::instance();
    ins.reset_cache();
    ins.m_db = db;
    ins.ensure_table();

    return ins;
}

// Return the static instance of TrxSplitModel table
TrxSplitModel& TrxSplitModel::instance()
{
    return Singleton<TrxSplitModel>::instance();
}

// -- override

bool TrxSplitModel::purge_id(int64 tp_id)
{
    bool ok = true;
    ok = ok && TagLinkModel::instance().purge_ref_all(s_ref_type, tp_id);
    ok = ok && unsafe_remove_id(tp_id);
    return ok;
}

// -- methods

bool TrxSplitModel::purge_trxId_all(const int64 trx_id)
{
    bool ok = true;

    for (int64 tp_id : find_id_a(
        TrxSplitCol::WHERE_TRANSID(OP_EQ, trx_id)
    )) {
        ok = ok && purge_id(tp_id);
    }

    return ok;
}

// This function is called by find_id_isUsed(), in the slow branch
// (when ignore_deleted is true), to check if a trx_id is not deleted.
std::size_t TrxSplitModel::find_id_count(int64 tp_id, bool ignore_deleted)
{
    for (const TrxSplitData& tp_d : find_data_a(
        TrxSplitCol::WHERE_SPLITTRANSID(OP_EQ, tp_id)
    )) {
        return TrxModel::instance().find_id_count(tp_d.m_trx_id, ignore_deleted);
    }
    return 0;
}

const TagLinkModel::DataA TrxSplitModel::find_id_gl_a(int64 tp_id)
{
    return TagLinkModel::instance().find_data_a(
        TagLinkCol::WHERE_REFTYPE(OP_EQ, TrxSplitModel::s_ref_type.key_n()),
        TagLinkCol::WHERE_REFID(OP_EQ, tp_id)
    );
}

std::map<int64, TrxSplitModel::DataA> TrxSplitModel::find_all_mTrxId()
{
    std::map<int64, DataA> trxId_dataA_m;
    for (const auto& tp_d : find_data_a(
        TableClause::ORDERBY(Col::s_primary_name)
    )) {
        trxId_dataA_m[tp_d.m_trx_id].push_back(tp_d);
    }
    return trxId_dataA_m;
}

double TrxSplitModel::get_total(const DataA& tp_a)
{
    double total = 0.0;
    for (const auto& tp_d : tp_a)
        total += tp_d.m_amount;
    return total;
}

double TrxSplitModel::get_total(const std::vector<Split>& split_a)
{
    double total = 0.0;
    for (auto& split_d : split_a)
        total += split_d.m_amount;
    return total;
}

const wxString TrxSplitModel::get_tooltip(
    const std::vector<Split>& split_a,
    const CurrencyData* currency_n
) {
    wxString tooltip = "";
    for (const auto& split_d : split_a) {
        tooltip += wxString::Format("%s = %s",
            CategoryModel::instance().get_id_fullname(split_d.m_category_id),
            CurrencyModel::instance().toCurrency(split_d.m_amount, currency_n)
        );
        if (!split_d.m_notes.IsEmpty()) {
            wxString notes = split_d.m_notes;
            notes.Replace("\n", " ");
            tooltip += wxString::Format(" (%s)", notes);
        }
        tooltip += "\n";
    }
    tooltip = tooltip.Left(tooltip.Len()-1);
    return tooltip;
}

int TrxSplitModel::update_trx(int64 trx_id, DataA& src_tp_a)
{
    bool save_timestamp = false;
    std::map<int, int64> row_id_map;

    DataA old_tp_a = find_data_a(
        TrxSplitCol::WHERE_TRANSID(OP_EQ, trx_id)
    );
    if (old_tp_a.size() != src_tp_a.size())
        save_timestamp = true;

    for (const auto& old_tp_d : old_tp_a) {
        if (!save_timestamp) {
            bool match = false;
            for (decltype(src_tp_a.size()) i = 0; i < src_tp_a.size(); ++i) {
                if (row_id_map.find(i) != row_id_map.end())
                    continue;
                match = (
                    src_tp_a[i].m_category_id == old_tp_d.m_category_id &&
                    src_tp_a[i].m_amount == old_tp_d.m_amount &&
                    src_tp_a[i].m_notes.IsSameAs(old_tp_d.m_notes)
                );
                if (match) {
                    row_id_map[i] = old_tp_d.m_id;
                    break;
                }
            }
            save_timestamp = save_timestamp || !match;
        }
        purge_id(old_tp_d.m_id);
    }

    for (auto& src_tp_d : src_tp_a) {
        Data new_tp_d = Data();
        new_tp_d.m_trx_id      = trx_id;
        new_tp_d.m_amount      = src_tp_d.m_amount;
        new_tp_d.m_category_id = src_tp_d.m_category_id;
        new_tp_d.m_notes       = src_tp_d.m_notes;
        add_data_n(new_tp_d);
        src_tp_d.m_id = new_tp_d.m_id;
    }

    if (save_timestamp)
        TrxModel::instance().save_timestamp(trx_id);
    
    return src_tp_a.size();
}

int TrxSplitModel::update_trx(int64 trx_id, const std::vector<Split>& split_a)
{
    DataA tp_a;
    for (const auto& split_d : split_a) {
        Data tp_d = Data();
        tp_d.m_category_id = split_d.m_category_id;
        tp_d.m_amount      = split_d.m_amount;
        tp_d.m_notes       = split_d.m_notes;
        tp_a.push_back(tp_d);
    }

    return update_trx(trx_id, tp_a);
}
