/*******************************************************
 Copyright (C) 2013,2014 Guan Lisheng (guanlisheng@gmail.com)
 Copyright (C) 2016 Stefano Giorgio

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

#include "base/defs.h"

#include "table/TrxShareTable.h"
#include "data/TrxShareData.h"

#include "_ModelBase.h"
#include "TrxSplitModel.h"

class TrxShareModel : public TableFactory<TrxShareTable, TrxShareData>
{
public:
    // Initialize the global TrxShareModel table on initial call.
    // Resets the global table on subsequent calls.
    // Return the static instance address for TrxShareModel table
    // Note: Assigning the address to a local variable can destroy the instance.
    static TrxShareModel& instance(wxSQLite3Database* db);

    // Return the static instance address for TrxShareModel table
    // Note: Assigning the address to a local variable can destroy the instance.
    static TrxShareModel& instance();

public:
    // Create a Share record if it does not exist.
    // Save the share record linked to the transaction.
    static void ShareEntry(
        int64 trx_id,
        double share_number,
        double share_price,
        double share_commission,
        const std::vector<Split>& commission_splits,
        const wxString& share_lot
    );

public:
    TrxShareModel();
    ~TrxShareModel();

    int64 find_trx_share_id(const int64 trx_id);
    TrxShareData* unsafe_get_trx_share_n(const int64 trx_id);
    void remove_trx_share(const int64 trx_id);
};
