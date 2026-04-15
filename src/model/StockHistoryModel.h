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

#pragma once

#include "base/_defs.h"
#include "base/mmSingleton.h"
#include "table/_TableFactory.h"
#include "data/StockHistoryData.h"

class StockHistoryModel : public TableFactory<StockHistoryTable, StockHistoryData>
{
// -- static

public:
    static auto WHERE_DATE(OP op, const mmDate& date) -> TableClauseV<wxString>;

// -- constructor

public:
    StockHistoryModel() :
        TableFactory<StockHistoryTable, StockHistoryData>() {}
    ~StockHistoryModel() {}

public:
    static StockHistoryModel& instance(wxSQLite3Database* db);
    static StockHistoryModel& instance();

// -- override

public:
    // override TableFactory
    virtual bool purge_id(int64 id) override {
        return unsafe_remove_id(id);
    }

// -- methods

public:
    bool purge_symbol_all(const wxString& symbol);

    auto get_key_data_n(const wxString& symbol, const mmDate& date) -> const Data*;

    auto save_record(
        const wxString& symbol, const mmDate& date, double price, UpdateType update_type
    ) -> int64;
};
