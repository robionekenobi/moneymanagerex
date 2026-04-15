/*******************************************************
 Copyright (C) 2014 Gabriele-V
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
#include "data/AttachmentData.h"

class AttachmentModel : public TableFactory<AttachmentTable, AttachmentData>
{
// -- constructor

public:
    AttachmentModel() :
        TableFactory<AttachmentTable, AttachmentData>() {}
    ~AttachmentModel() {}

public:
    static AttachmentModel& instance(wxSQLite3Database* db);
    static AttachmentModel& instance();

// -- override

public:
    // override TableFactory
    virtual bool purge_id(int64 id) override {
        return unsafe_remove_id(id);
    }

// -- methods

public:
    bool purge_id_file(int64 att_id, bool purge_file = false);
    bool purge_ref_all(RefTypeN ref_type, const int64 ref_id/*, bool purge_file = false*/);

    int  find_ref_c(RefTypeN ref_type, const int64 ref_id);
    auto find_ref_data_a(RefTypeN ref_type, const int64 ref_id) -> const DataA;
    int  find_ref_last_num(RefTypeN ref_type, const int64 ref_id);
    auto find_refType_mRefId(RefTypeN ref_type) -> std::map<int64, DataA>;
    auto find_all_desc_a() -> wxArrayString;
};
