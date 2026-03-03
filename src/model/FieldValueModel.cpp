/*******************************************************
 Copyright (C) 2016 Gabriele-V

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
#include <wx/string.h>

#include "FieldModel.h"
#include "FieldValueModel.h"

FieldValueModel::FieldValueModel() :
    TableFactory<FieldValueTable, FieldValueData>()
{
}

FieldValueModel::~FieldValueModel()
{
}

/**
* Initialize the global FieldValueModel table.
* Reset the FieldValueModel table or create the table if it does not exist.
*/
FieldValueModel& FieldValueModel::instance(wxSQLite3Database* db)
{
    FieldValueModel& ins = Singleton<FieldValueModel>::instance();
    ins.reset_cache();
    ins.m_db = db;
    ins.ensure_table();

    return ins;
}

/** Return the static instance of FieldValueModel table */
FieldValueModel& FieldValueModel::instance()
{
    return Singleton<FieldValueModel>::instance();
}

const FieldValueData* FieldValueModel::get_key(int64 FieldID, int64 RefID)
{
    FieldValueModel::DataA items = this->find(
        FieldValueCol::FIELDID(FieldID),
        FieldValueCol::REFID(RefID)
    );
    if (!items.empty())
        return get_id_data_n(items[0].FIELDATADID);
    return nullptr;
}

std::map<int64, FieldValueModel::DataA> FieldValueModel::get_all_id(const wxString& reftype)
{
    FieldModel::DataA custom_fields = FieldModel::instance().find(
        FieldCol::REFTYPE(reftype)
    );
    std::map<int64, FieldValueModel::DataA> data;
    for (const auto& entry : custom_fields) {
        for (const auto& custom_field : find(FieldValueCol::FIELDID(entry.FIELDID))) {
            data[custom_field.REFID].push_back(custom_field);
        }
    }
    return data;
}

// Return all CustomFieldData value
wxArrayString FieldValueModel::allValue(const int64 FieldID)
{
    wxArrayString values;
    wxString PreviousValue;

    FieldValueModel::DataA items = this->find(FieldValueCol::FIELDID(FieldID));
    std::sort(items.begin(), items.end(), FieldValueData::SorterByCONTENT());

    for (const auto &item : items)
    {
        if (item.CONTENT != PreviousValue)
        {
            values.Add(item.CONTENT);
            PreviousValue = item.CONTENT;
        }
    }
    return values;
}

bool FieldValueModel::DeleteAllData(const wxString& RefType, int64 RefID)
{
    const auto& fields = FieldModel::instance().find(FieldCol::REFTYPE(RefType));

    for (const auto& field : fields)
    {
        const Data* data = FieldValueModel::instance().get_key(field.FIELDID, RefID);
        if (data)
            FieldValueModel::instance().purge_id(data->FIELDATADID);
    }
    return true;
}
