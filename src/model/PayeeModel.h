/*******************************************************
 Copyright (C) 2013,2014 Guan Lisheng (guanlisheng@gmail.com)
 Copyright (C) 2022 Mark Whalley (mark@ipx.co.uk)
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

#pragma once

#include "base/_defs.h"
#include "base/mmSingleton.h"
#include "table/_TableFactory.h"
#include "data/_DataEnum.h"
#include "data/PayeeData.h"

class PayeeModel : public TableFactory<PayeeTable, PayeeData>
{
// -- static

public:
    static const RefTypeN s_ref_type;

// -- constructor

public:
    PayeeModel() :
        TableFactory<PayeeTable, PayeeData>() {}
    ~PayeeModel() {}

public:
    static PayeeModel& instance(wxSQLite3Database* db);
    static PayeeModel& instance();

// -- override

public:
    // override TableFactory
    virtual bool find_id_isUsed(int64 id, bool ignore_deleted = false) override;
    virtual bool purge_id(int64 id) override;

// -- methods

public:
    // lookup for given id
    auto get_id_name(int64 payee_id) -> const wxString;

    // lookup for given field
    auto get_name_data_n(const wxString& name) -> const Data*;

    // lookup for all Data
    auto find_all_name_a() -> const wxArrayString;
    auto find_all_name_id_m(bool only_active = false) -> const std::map<wxString, int64>;
    auto find_used_id_m() -> const std::set<int64>;
    auto find_pattern_data_a(const wxString& pattern, bool only_active = false) -> const DataA;
};
