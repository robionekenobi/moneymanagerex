/*******************************************************
 Copyright (C) 2016 Gabriele-V
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

#include "base/_defs.h"
#include <wx/string.h>

#include "FieldModel.h"
#include "FieldValueModel.h"

// -- static

TableClauseV<int64> FieldValueModel::WHERE_REFTYPEID(RefTypeN ref_type, int64 ref_id)
{
    return FieldValueCol::WHERE_REFID(OP_EQ, FieldValueData::encode_REFID(ref_type, ref_id));
}

// -- constructor

// Initialize the global FieldValueModel table.
// Reset the FieldValueModel table or create the table if it does not exist.
FieldValueModel& FieldValueModel::instance(wxSQLite3Database* db)
{
    FieldValueModel& ins = Singleton<FieldValueModel>::instance();
    ins.reset_cache();
    ins.m_db = db;
    ins.ensure_table();

    return ins;
}

// Return the static instance of FieldValueModel table
FieldValueModel& FieldValueModel::instance()
{
    return Singleton<FieldValueModel>::instance();
}

// -- methods

std::size_t FieldValueModel::find_fieldId_c(int64 field_id)
{
    return FieldValueModel::instance().find_count(
        FieldValueCol::WHERE_FIELDID(OP_EQ, field_id)
    );
}

bool FieldValueModel::purge_fieldId_all(int64 field_id)
{
    bool ok = true;

    db_savepoint();
    for (int64 fv_id : find_id_a(
        FieldValueCol::WHERE_FIELDID(OP_EQ, field_id)
    )) {
        ok = ok && purge_id(fv_id);
    }
    db_release_savepoint();

    return ok;
}

bool FieldValueModel::purge_ref_all(RefTypeN ref_type, int64 ref_id)
{
    bool ok = true;

    db_savepoint();
    for (int64 fv_id : find_id_a(
        FieldValueModel::WHERE_REFTYPEID(ref_type, ref_id)
    )) {
        ok = ok && purge_id(fv_id);
    }
    db_release_savepoint();

    return ok;
}

const FieldValueData* FieldValueModel::get_key_data_n(
    int64 field_id,
    RefTypeN ref_type, int64 ref_id
) {
    for (int64 fv_id : find_id_a(
        FieldValueCol::WHERE_FIELDID(OP_EQ, field_id),
        FieldValueModel::WHERE_REFTYPEID(ref_type, ref_id)
    )) {
        return get_id_data_n(fv_id);
    }

    return nullptr;
}

std::map<int64, FieldValueModel::DataA> FieldValueModel::find_refType_mRefId(
    RefTypeN ref_type
) {
    std::map<int64, FieldValueModel::DataA> refId_dataA_m;
    for (const Data& fv_d : find_data_a(
        TableClause::ORDERBY(FieldValueCol::s_primary_name)
    )) {
        if (fv_d.m_ref_type.id_n() == ref_type.id_n())
            refId_dataA_m[fv_d.m_ref_id].push_back(fv_d);
    }
    return refId_dataA_m;
}

