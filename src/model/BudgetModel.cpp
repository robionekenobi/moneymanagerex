/*******************************************************
 Copyright (C) 2013,2014 James Higley
 Copyright (C) 2022 Mark Whalley (mark@ipx.co.uk)

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

#include "base/defs.h"
#include <wx/intl.h>

#include "table/BudgetTable.h"

#include "BudgetModel.h"
#include "BudgetPeriodModel.h"
#include "CategoryModel.h"
#include "PrefModel.h"

BudgetModel::BudgetModel() :
    TableFactory<BudgetTable, BudgetData>()
{
}

BudgetModel::~BudgetModel()
{
}

/**
* Initialize the global BudgetModel table.
* Reset the BudgetModel table or create the table if it does not exist.
*/
BudgetModel& BudgetModel::instance(wxSQLite3Database* db)
{
    BudgetModel& ins = Singleton<BudgetModel>::instance();
    ins.reset_cache();
    ins.m_db = db;
    ins.ensure_table();

    return ins;
}

/** Return the static instance of BudgetModel table */
BudgetModel& BudgetModel::instance()
{
    return Singleton<BudgetModel>::instance();
}

BudgetCol::PERIOD BudgetModel::FREQUENCY(OP op, BudgetFrequency freq)
{
    return BudgetCol::PERIOD(op, freq.name());
}

double BudgetModel::getEstimate(
    bool is_monthly,
    const BudgetFrequency freq,
    double amount
) {
    double estimated = amount * freq.times_per_year();
    if (is_monthly)
        estimated = estimated / 12;
    return estimated;
}

void BudgetModel::getBudgetEntry(
    int64 budgetYearID,
    std::map<int64, BudgetFrequency>& budgetFreq,
    std::map<int64, double>& budgetAmt,
    std::map<int64, wxString>& budgetNotes
) {
    //Set std::map with zerros
    for (const auto& category_d : CategoryModel::instance().find_all()) {
        budgetFreq[category_d.m_id] = BudgetFrequency(BudgetFrequency::e_none);
        budgetAmt[category_d.m_id]  = 0.0;
    }

    for (const auto& budget_d : find(
        BudgetCol::BUDGETYEARID(budgetYearID)
    )) {
        budgetFreq[budget_d.m_category_id]  = budget_d.m_frequency;
        budgetAmt[budget_d.m_category_id]   = budget_d.m_amount;
        budgetNotes[budget_d.m_category_id] = budget_d.m_notes;
    }
}

void BudgetModel::getBudgetStats(
    std::map<int64, std::map<int, double>>& budgetStats,
    mmDateRange* date_range,
    bool groupByMonth
) {
    //Set std::map with zeros
    for (const auto& category_d : CategoryModel::instance().find_all()) {
        for (int month = 0; month < 12; month++) {
            budgetStats[category_d.m_id][month] = 0.0;
        }
    }

    //Calculations
    const wxDateTime start_date(date_range->start_date());
    std::map<int64, double> monthlyBudgetValue;
    std::map<int64, double> yearlyBudgetValue;
    std::map<int64, double> yearDeduction;
    std::map<std::pair<int, int64>, bool> isBudgeted;
    std::map<int64, int> budgetedMonths;
    const wxString year = wxString::Format("%i", start_date.GetYear());
    int64 budgetYearID = BudgetPeriodModel::instance().get_name_id(year);
    for (const Data& budget_d : find(
        BudgetCol::BUDGETYEARID(budgetYearID)
    )) {
        int64 category_id = budget_d.m_category_id;
        // Determine the monhly budgeted amounts
        monthlyBudgetValue[category_id] = getEstimate(true, budget_d.m_frequency, budget_d.m_amount);
        // Determine the yearly budgeted amounts
        yearlyBudgetValue[category_id] = getEstimate(false, budget_d.m_frequency, budget_d.m_amount);
        // Store the yearly budget to use in reporting. Monthly budgets are stored in index 0-11, so use index 12 for year
        budgetStats[category_id][12] = yearlyBudgetValue[category_id];
    }
    bool budgetOverride = PrefModel::instance().getBudgetOverride();
    bool budgetDeductMonthly = PrefModel::instance().getBudgetDeductMonthly();
    for (int month = 0; month < 12; month++) {
        const wxString budgetYearMonth = wxString::Format("%s-%02d", year, month + 1);
        budgetYearID = BudgetPeriodModel::instance().get_name_id(budgetYearMonth);

        //fill with amount from monthly budgets first
        for (const Data& budget_d : find(
            BudgetCol::BUDGETYEARID(budgetYearID)
        )) {
            int64 category_id = budget_d.m_category_id;
            std::pair<int, int64> month_categ = std::make_pair(month, category_id);
            if (!isBudgeted[month_categ]) {
                isBudgeted[month_categ] = true;
                budgetedMonths[category_id]++;
            }
            budgetStats[category_id][month] = getEstimate(true, budget_d.m_frequency, budget_d.m_amount);
            yearDeduction[category_id] += budgetStats[category_id][month];
        }
    }
    // Now go month by month and add the yearly budget
    for (int month = 0; month < 12; month++) {
        // If user selected to deduct monthly budgeted amounts 
        if (budgetDeductMonthly)
            for (const auto& categoryBudget : yearlyBudgetValue) {
                if (yearDeduction[categoryBudget.first] / categoryBudget.second >= 1) continue;
                //Deduct the monthly total from the yearly budget
                double adjusted_amount = categoryBudget.second - yearDeduction[categoryBudget.first];
                if (!budgetOverride)
                    // If user doesn't override the budget, add 1/12 of the adjusted amount to every period
                    budgetStats[categoryBudget.first][month] += adjusted_amount / 12;
                else if (!isBudgeted[std::make_pair(month, categoryBudget.first)])
                    // Otherwise if n months have a defined budget, add 1/(12-n) of the adjusted amount only to the (12-n) non-budgeted periods
                    budgetStats[categoryBudget.first][month] = adjusted_amount / (12 - budgetedMonths[categoryBudget.first]);
            }
        else
            // If the user is not deducting the monthly budget from the yearly budget
            for (const auto& categoryBudget : monthlyBudgetValue) {
                if (!budgetOverride)
                    // If user doesn't override their budget, add 1/12 of the yearly amount to every period
                    budgetStats[categoryBudget.first][month] += categoryBudget.second;
                else if (!isBudgeted[std::make_pair(month, categoryBudget.first)])
                    // Otherwise fill 1/12 of the yearly amount only in non-budgeted periods
                    budgetStats[categoryBudget.first][month] = categoryBudget.second;
            }
    }
    if (!groupByMonth) {
        std::map<int64, std::map<int,double> > yearlyBudgetStats;
        for (const auto& category_d : CategoryModel::instance().find_all()) {
            yearlyBudgetStats[category_d.m_id][0] = 0.0;
        }

        for (const auto& cat : budgetStats)
            for(int month = 0; month < 12; month++)
                yearlyBudgetStats[cat.first][0] += budgetStats[cat.first][month];

        budgetStats = yearlyBudgetStats;
    }
}

void BudgetModel::copyBudgetYear(int64 newYearID, int64 baseYearID)
{
    std::map<int64, double> yearDeduction;
    int budgetedMonths = 0;
    bool optionDeductMonthly = PrefModel::instance().getBudgetDeductMonthly();
    const wxString baseBudgetYearName = BudgetPeriodModel::instance().get_id_data_n(baseYearID)->m_name;
    const wxString newBudgetYearName = BudgetPeriodModel::instance().get_id_data_n(newYearID)->m_name;

    // Only deduct monthly amounts if a monthly budget is being created based on a yearly budget
    optionDeductMonthly &= (baseBudgetYearName.length() == 4 && newBudgetYearName.length() > 4);

    if (optionDeductMonthly) {
        for (int month = 0; month < 12; month++) {
            const wxString budgetYearMonth = wxString::Format("%s-%02d", newBudgetYearName.SubString(0,3), month + 1);
            int64 budgetYearID = BudgetPeriodModel::instance().get_name_id(budgetYearMonth);
            BudgetModel::DataA budget_a = find(
                BudgetCol::BUDGETYEARID(budgetYearID)
            );
            if (!budget_a.empty())
                budgetedMonths++;
            //calculate deduction
            for (const auto& budget_d : budget_a) {
                yearDeduction[budget_d.m_category_id] += getEstimate(true, budget_d.m_frequency, budget_d.m_amount);
            }
        }
    }

    for (const Data& budget_d : find(
        BudgetCol::BUDGETYEARID(baseYearID)
    )) {
        Data new_budget_d = Data();
        new_budget_d.clone_from(budget_d);
        new_budget_d.m_period_id = newYearID;
        double yearAmount = getEstimate(false, budget_d.m_frequency, budget_d.m_amount);
        if (optionDeductMonthly && budgetedMonths > 0) {
            new_budget_d.m_frequency = BudgetFrequency(BudgetFrequency::e_monthly);
            new_budget_d.m_amount    = (yearDeduction[new_budget_d.m_category_id] / yearAmount < 1)
                ? (yearAmount - yearDeduction[new_budget_d.m_category_id]) / (12 - budgetedMonths)
                : 0;
        }
        add_data_n(new_budget_d);
    }
}
