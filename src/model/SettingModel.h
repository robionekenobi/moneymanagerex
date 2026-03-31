/*******************************************************
 Copyright (C) 2013,2014 Guan Lisheng (guanlisheng@gmail.com)
 Copyright (C) 2014 - 2022 Nikolay Akimov
 Copyright (C) 2021 Mark Whalley (mark@ipx.co.uk)
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

#include "base/defs.h"
#include "util/mmSingleton.h"
#include "table/_TableFactory.h"
#include "data/SettingData.h"

class SettingModel : public TableFactory<SettingTable, SettingData>
{
// -- contructor

public:
    SettingModel() :
        TableFactory<SettingTable, SettingData>() {}
    ~SettingModel() {}

public:
    static SettingModel& instance(wxSQLite3Database* db);
    static SettingModel& instance();

// -- methods

public:
    void setting_savepoint()
    {
        this->m_db->Savepoint("MMEX_Setting");
    }
    void setting_release_savepoint()
    {
        this->m_db->ReleaseSavepoint("MMEX_Setting");
    }
    void setting_rollback()
    {
        this->m_db->Rollback("MMEX_Setting");
    }

public:
    bool contains(const wxString& key);

    auto getRaw(const wxString& key, const wxString& defaultValue) -> const wxString;
    void saveRaw(const wxString& key, const wxString& newValue);

    auto getString(const wxString& key, const wxString& defaultValue) -> const wxString;
    void saveString(const wxString& key, const wxString& newValue);

    bool getBool(const wxString& key, bool defaultValue);
    void saveBool(const wxString& key, bool newValue);

    int  getInt(const wxString& key, int defaultValue);
    void saveInt(const wxString& key, int newValue);

    auto getColour(const wxString& key, const wxColour& defaultValue) -> const wxColour;
    void saveColour(const wxString& key, const wxColour& newValue);

    auto getJdoc(const wxString& key, const wxString& defaultValue) -> Document;
    void saveJdoc(const wxString& key, Document& newValue);
    void saveJdoc(const wxString& key, StringBuffer& newValue);

    auto getArrayString(const wxString& key) -> const wxArrayString;
    void saveArrayString(const wxString& key, const wxArrayString& a);

    void prependArrayItem(const wxString& key, const wxString& value, int limit);

    auto getViewAccounts() -> const wxString;
    void saveViewAccounts(const wxString& newValue);
    
    auto getTheme() -> const wxString;
    void saveTheme(const wxString& newValue);

    auto getLastDbPath() -> const wxString;

    void shrinkUsageTable();
    auto to_html_row() -> row_t;
};
