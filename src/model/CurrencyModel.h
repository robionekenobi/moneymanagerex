/*******************************************************
 Copyright (C) 2013,2014 Guan Lisheng (guanlisheng@gmail.com)
 Copyright (C) 2022  Mark Whalley (mark@ipx.co.uk)

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

#include <map>

#include "base/defs.h"
#include "util/mmChoice.h"

#include "table/CurrencyTable.h"
#include "data/CurrencyData.h"

#include "_ModelBase.h"
#include "InfoModel.h" // detect base currency setting BASECURRENCYID

class CurrencyModel : public TableFactory<CurrencyTable, CurrencyData>
{
public:
    /**
    Initialize the global CurrencyModel table on initial call.
    Resets the global table on subsequent calls.
    * Return the static instance address for CurrencyModel table
    * Note: Assigning the address to a local variable can destroy the instance.
    */
    static CurrencyModel& instance(wxSQLite3Database* db);

    /**
    * Return the static instance address for CurrencyModel table
    * Note: Assigning the address to a local variable can destroy the instance.
    */
    static CurrencyModel& instance();

public:
    static CurrencyCol::CURRENCY_TYPE CURRENCY_TYPE(OP op, CurrencyType currency_type);

    // Return the Data record of the base currency.
    static const Data* GetBaseCurrency();
    static bool GetBaseCurrencySymbol(wxString& base_currency_symbol);

    // Resets all BASECONVRATE to 1
    static void ResetBaseConversionRates();

    static std::map<wxDateTime,int> DateUsed(int64 CurrencyID);

    // Add prefix and suffix characters to string value
    static const wxString toCurrency(double value, const Data* currency = GetBaseCurrency(), int precision = -1);
 
    // convert value to a string with required precision. Currency is used only for percision
    static const wxString toStringNoFormatting(
        double value, const Data* currency = GetBaseCurrency(), int precision = -1
    );
    // convert value to a currency formatted string with required precision
    static const wxString toString(
        double value, const Data* currency = GetBaseCurrency(), int precision = -1
    );
    // Reset currency string like 1.234,56 to standard number format like 1234.56
    static const wxString fromString2CLocale(
        const wxString &s, const Data* currency = CurrencyModel::GetBaseCurrency()
    );
    static bool fromString(wxString s, double& val, const Data* currency = GetBaseCurrency());
    static int precision(const Data* r);
    static int precision(const Data& r);
    static int precision(int64 account_id);
    static bool is_used(int64 currency_id);

public:
    // Remove the Data record from memory and the database.
    // Delete also all currency history
    bool purge_id(int64 id) override;

    const wxArrayString all_currency_names();
    const std::map<wxString, int64> all_currency();
    const wxArrayString all_currency_symbols();

    // Return the currency Data record for the given symbol
    const CurrencyData* GetCurrencyRecord(const wxString& currency_symbol);

public:
    CurrencyModel();
    ~CurrencyModel();
};
