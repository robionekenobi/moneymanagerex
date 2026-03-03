/*******************************************************
 Copyright (C) 2013,2014 Guan Lisheng (guanlisheng@gmail.com)

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

BudgetPeriodModel::BudgetPeriodModel() :
    TableFactory<BudgetPeriodTable, BudgetPeriodData>()
{
}

BudgetPeriodModel::~BudgetPeriodModel()
{
}

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

bool BudgetPeriodModel::purge_id(int64 id)
{
    for (const BudgetData& budget_d : BudgetModel::instance().find(
        BudgetCol::BUDGETYEARID(id)
    ))
        BudgetModel::instance().purge_id(budget_d.m_period_id);
    return unsafe_remove_id(id);
}

const wxString BudgetPeriodModel::get_id_name(int64 period_id)
{
    const Data* bp_n = get_id_data_n(period_id);
    return bp_n ? bp_n->m_name : "";
}

int64 BudgetPeriodModel::get_name_id(const wxString& period_name)
{
    // TODO: lookup period_name in cache
    for (const auto& bp_d : find_all()) {
        if (bp_d.m_name == period_name)
            return bp_d.m_id;
    }
    return -1;
}

int64 BudgetPeriodModel::ensure_name(const wxString& period_name)
{
    int64 period_id = get_name_id(period_name);
    if (period_id < 0) {
        Data new_bp_d = Data();
        new_bp_d.m_name = period_name;
        add_data_n(new_bp_d);
        period_id = new_bp_d.id();
    }
    return period_id;
}
