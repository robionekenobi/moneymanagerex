/*******************************************************
 Copyright (C) 2013,2014 Guan Lisheng (guanlisheng@gmail.com)
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

#include "BudgetPeriodModel.h"
#include "BudgetModel.h"

// Initialize the global BudgetPeriodModel table.
// Reset the BudgetPeriodModel table or create the table if it does not exist.
BudgetPeriodModel& BudgetPeriodModel::instance(wxSQLite3Database* db)
{
    BudgetPeriodModel& ins = Singleton<BudgetPeriodModel>::instance();
    ins.reset_cache();
    ins.m_db = db;
    ins.ensure_table();

    return ins;
}

// Return the static instance of BudgetPeriodModel table
BudgetPeriodModel& BudgetPeriodModel::instance()
{
    return Singleton<BudgetPeriodModel>::instance();
}

// -- override

bool BudgetPeriodModel::find_id_isUsed(int64 bp_id, [[maybe_unused]] bool ignore_deleted)
{
    return BudgetModel::instance().find_count(
        BudgetCol::WHERE_BUDGETYEARID(OP_EQ, bp_id)
    ) > 0;
}

bool BudgetPeriodModel::purge_id(int64 bp_id)
{
    bool ok = true;

    // FIXME: Do not remove budget; let the user do it manually.
    ok = ok && purge_id_dep(bp_id);

    ok = ok && unsafe_remove_id(bp_id);

    return ok;
}

// -- methods

bool BudgetPeriodModel::purge_id_dep(int64 bp_id)
{
    bool ok = true;
    db_savepoint();

    for (int64 budget_id : BudgetModel::instance().find_id_a(
        BudgetCol::WHERE_BUDGETYEARID(OP_EQ, bp_id)
    )) {
        ok = ok && BudgetModel::instance().purge_id(budget_id);
    }

    db_release_savepoint();
    return ok;
}

const wxString BudgetPeriodModel::get_id_name_n(int64 bp_id)
{
    const Data* bp_n = get_idN_data_n(bp_id);
    return bp_n ? bp_n->m_name : "";
}

int64 BudgetPeriodModel::get_name_id_n(const wxString& bp_name)
{
    // TODO: lookup bp_name in cache
    for (int64 bp_id : find_id_a(
        Col::WHERE_BUDGETYEARNAME(OP_EQ, bp_name)
    )) {
        return bp_id;
    }
    return -1;
}

int64 BudgetPeriodModel::ensure_name(const wxString& bp_name)
{
    int64 bp_id_n = get_name_id_n(bp_name);
    if (bp_id_n < 0) {
        Data new_bp_d = Data();
        new_bp_d.m_name = bp_name;
        add_data_n(new_bp_d);
        bp_id_n = new_bp_d.m_id;
    }
    return bp_id_n;
}
