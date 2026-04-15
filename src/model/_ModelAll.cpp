/*******************************************************
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

#include "_ModelAll.h"

#include "TrxModel.h"
#include "TrxSplitModel.h"

// -- constructor

ModelAll& ModelAll::instance(wxSQLite3Database* db)
{
    ModelAll& ins = Singleton<ModelAll>::instance();
    ins.m_db = db;

    return ins;
}

ModelAll& ModelAll::instance()
{
    return Singleton<ModelAll>::instance();
}

// -- methods

// This function is called by find_id_isUsed(), in the slow branch
// (when ignore_deleted is true), to check if a reference is not deleted.
std::size_t ModelAll::find_ref_count(RefTypeN ref_type, int64 ref_id, bool ignore_deleted)
{
    if (ref_type == TrxModel::s_ref_type) {
        return TrxModel::instance().find_id_count(ref_id, ignore_deleted);
    }
    else if (ref_type == TrxSplitModel::s_ref_type) {
        return TrxSplitModel::instance().find_id_count(ref_id, ignore_deleted); 
    }
    else {
        return 1;
    }
}
