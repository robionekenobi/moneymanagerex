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

#include "base/defs.h"
#include "util/mmDate.h"
#include "util/mmSingleton.h"
#include "util/_util.h"
#include "table/_TableFactory.h"
#include "data/InfoData.h"

class InfoModel : public TableFactory<InfoTable, InfoData>
{
// -- constructor

public:
    InfoModel() :
        TableFactory<InfoTable, InfoData>() {}
    ~InfoModel() {}

public:
    static InfoModel& instance(wxSQLite3Database* db);
    static InfoModel& instance();

// -- methods

public:
    bool contains(const wxString& key);

    wxString getRaw(const wxString& key, const wxString& defaultValue);
    void saveRaw(const wxString& key, const wxString& newValue);

    wxString getString(const wxString& key, const wxString& defaultValue);
    void saveString(const wxString& key, const wxString& newValue);

    bool getBool(const wxString& key, bool defaultValue);
    void saveBool(const wxString& key, bool newValue);

    int getInt(const wxString& key, int defaultValue);
    void saveInt(const wxString& key, int newValue);

    int64 getInt64(const wxString& key, int64 defaultValue);
    void saveInt64(const wxString& key, int64 newValue);

    const wxSize getSize(const wxString& key);
    void saveSize(const wxString& key, const wxSize& newValue);

    const wxColour getColour(const wxString& key, const wxColour& defaultValue = wxColour(255, 255, 255));
    void saveColour(const wxString& key, const wxColour& newValue);

    void setDate(const wxString& key, const mmDate& newValue);

    Document getJdoc(const wxString& key, const wxString& defaultValue);
    void saveJdoc(const wxString& key, Document& newValue);
    void saveJdoc(const wxString& key, StringBuffer& newValue);

    const wxArrayString getArrayString(const wxString& key, bool sort = false);
    void saveArrayString(const wxString& key, const wxArrayString& a);

    int findArrayItem(const wxString& key, const wxString& label);
    void updateArrayItem(const wxString& key, int i, const wxString& newValue);
    void prependArrayItem(const wxString& key, const wxString& value, int limit);
    void eraseArrayItem(const wxString& key, int i);

public:
    bool getOpenCustomDialog(const wxString& refType);
    void saveOpenCustomDialog(const wxString& refType, bool newValue);

    wxSize getCustomDialogSize(const wxString& refType);
    void saveCustomDialogSize(const wxString& refType, const wxSize& newValue);

public:
    bool checkDBVersion();
    static loop_t to_loop_t();

public:
    // static support functions
    static void saveFilterString(Document &sdoc, const char* skey, wxString svalue);
    static void saveFilterBool(Document &sdoc, const char* skey, bool bvalue);
    static void saveFilterInt(Document &sdoc, const char* skey, int ivalue);
};
