/*******************************************************
Copyright (C) 2015 Gabriele-V

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
********************************************************/

#include "dbcheck.h"

#include "util/mmNavigatorList.h"
#include "model/AccountModel.h"
#include "model/AttachmentModel.h"
#include "model/SchedModel.h"
#include "model/CategoryModel.h"
#include "model/PayeeModel.h"
#include "model/StockModel.h"

bool dbCheck::checkDB()
{
    bool result = true;
    result = checkAccounts();

    return result;
}

bool dbCheck::checkAccounts()
{
    bool result = true;

    // Transactions
    for (const auto& trx_d : TrxModel::instance().find_data_a()) {
        if (!AccountModel::instance().get_idN_data_n(trx_d.m_account_id) || (
            trx_d.is_transfer() &&
            !AccountModel::instance().get_idN_data_n(trx_d.m_to_account_id_n)
        )) {
            result = false;
        }
    }

    // BillsDeposits
    for (const auto& sched_d : SchedModel::instance().find_data_a()) {
        if (!AccountModel::instance().get_idN_data_n(sched_d.m_account_id) || (
            sched_d.is_transfer() &&
            !AccountModel::instance().get_idN_data_n(sched_d.m_to_account_id_n)
        )) {
            result = false;
        }
    }

    // Stocks
    for (const auto& stock_d : StockModel::instance().find_data_a()) {
        const auto& account_n = AccountModel::instance().get_idN_data_n(stock_d.m_account_id_n);
        if (!account_n ||
            AccountModel::type_id(*account_n) != mmNavigatorItem::TYPE_ID_INVESTMENT
        ) {
            result = false;
        }
    }

    return result;
}

bool dbCheck::checkAttachments()
{
    return true;
}

bool dbCheck::checkBudgets()
{
    return true;
}

bool dbCheck::checkBudgetYears()
{
    return true;
}

bool dbCheck::checkCategories()
{
    return true;
}

bool dbCheck::checkCurrencies()
{
    return true;
}

bool dbCheck::checkPayees()
{
    return true;
}

bool dbCheck::checkStocks()
{
    return true;
}

bool dbCheck::checkSubcategories()
{
    return true;
}

bool dbCheck::checkTransactions()
{
    return true;
}
