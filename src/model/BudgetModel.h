/*******************************************************
 Copyright (C) 2013,2014 James Higley
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

#pragma once

#include <float.h>
#include "base/_defs.h"
#include "base/mmSingleton.h"
#include "util/mmDateRange.h"
#include "table/_TableFactory.h"
#include "data/BudgetData.h"

class BudgetModel : public TableFactory<BudgetTable, BudgetData>
{
// -- static

public:
    static auto WHERE_FREQUENCY(OP op, BudgetFreq freq) -> TableClauseV<wxString>;

// -- constructor

public:
    BudgetModel() :
        TableFactory<BudgetTable, BudgetData>() {}
    ~BudgetModel() {}

    static BudgetModel& instance(wxSQLite3Database* db);
    static BudgetModel& instance();

// -- override

public:
    // override TableFactory
    virtual bool purge_id(int64 id) override {
        return unsafe_remove_id(id);
    }

// -- methods

public:
    void getBudgetEntry(
        int64 bp_id,
        std::map<int64, BudgetFreq>& freq_mCatId,
        std::map<int64, double>& amount_mCatId,
        std::map<int64, wxString>& notes_mCatId
    );
    void getBudgetStats(
        std::map<int64, std::map<int, double>>& budgetStats,
        mmDateRange* date_range,
        bool groupByMonth
    );
    void copyBudgetYear(int64 dst_bp_id, int64 src_bp_id);
};
