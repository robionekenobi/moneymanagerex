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

#include "AttachmentModel.h"

#include "util/mmAttachment.h"

// -- constructor

// Initialize the global AttachmentModel table.
// Reset the AttachmentModel table or create the table if it does not exist.
AttachmentModel& AttachmentModel::instance(wxSQLite3Database* db)
{
    AttachmentModel& ins = Singleton<AttachmentModel>::instance();
    ins.reset_cache();
    ins.m_db = db;
    ins.ensure_table();

    return ins;
}

// Return the static instance of AttachmentModel table
AttachmentModel& AttachmentModel::instance()
{
    return Singleton<AttachmentModel>::instance();
}

// -- override

bool AttachmentModel::purge_id(int64 att_id)
{
    bool ok = true;
    db_savepoint();

    for (const Data& att_d : find_data_a(
        AttachmentCol::WHERE_ATTACHMENTID(OP_EQ, att_id)
    )) {
        ok = ok && mmAttachment::deleteFile(get_data_file(att_d));
        ok = ok && unsafe_remove_id(att_d.m_id);
    }

    db_release_savepoint();
    return ok;
}

// -- methods

bool AttachmentModel::purge_ref_all(RefTypeN ref_type, const int64 ref_id)
{
    bool ok = true;
    db_savepoint();

    for (const Data& att_d : find_data_a(
        AttachmentCol::WHERE_REFTYPE(OP_EQ, ref_type.key_n()),
        AttachmentCol::WHERE_REFID(OP_EQ, ref_id)
    )) {
        ok = ok && mmAttachment::deleteFile(get_data_file(att_d));
        ok = ok && unsafe_remove_id(att_d.m_id);
    }

    db_release_savepoint();
    return ok;
}

const wxString AttachmentModel::get_data_file(const AttachmentData& att_d)
{
    wxString folder = mmAttachment::getFolder();
    return !folder.IsEmpty()
        ? folder + att_d.m_ref_type_n.key_n() + mmAttachment::s_path_sep + att_d.m_filename
        : "";
}

// Return the number of attachments linked to a specific object
int AttachmentModel::find_ref_c(RefTypeN ref_type, const int64 ref_id)
{
    return AttachmentModel::instance().find_count(
        AttachmentCol::WHERE_REFTYPE(OP_EQ, ref_type.key_n()),
        AttachmentCol::WHERE_REFID(OP_EQ, ref_id)
    );
}

// Return a dataset with attachments linked to a specific object
const AttachmentModel::DataA AttachmentModel::find_ref_data_a(
    RefTypeN ref_type,
    const int64 ref_id
) {
    DataA att_a;
    for (const Data& att_d : find_data_a(
        TableClause::ORDERBY(Col::NAME_DESCRIPTION)
    )) {
        if (att_d.m_ref_type_n.key_n().Lower().Matches(
            ref_type.key_n().Lower().Append("*")
        ) && att_d.m_ref_id == ref_id)
            att_a.push_back(att_d);
    }
    return att_a;
}

// Return the last attachment number linked to a specific object
int AttachmentModel::find_ref_last_num(RefTypeN ref_type, const int64 ref_id)
{
    int max_num = 0;
    for (const auto& att_d : find_ref_data_a(ref_type, ref_id)) {
        wxString att_filename = att_d.m_filename;
        int num = wxAtoi(att_filename.SubString(
            att_filename.Find("Attach") + 6,
            att_filename.Find(".") - 1
        ));
        if (max_num < num)
            max_num = num;
    }
    return max_num;
}

// Return a dataset with attachments linked to a specific type
std::map<int64, AttachmentModel::DataA> AttachmentModel::find_refType_mRefId(
    RefTypeN ref_type
) {
    std::map<int64, AttachmentModel::DataA> refId_dataA_m;
    for (const auto& att_d : find_data_a(
        AttachmentCol::WHERE_REFTYPE(OP_EQ, ref_type.key_n())
    )) {
        refId_dataA_m[att_d.m_ref_id].push_back(att_d);
    }

    return refId_dataA_m;
}

// Return all attachments descriptions
wxArrayString AttachmentModel::find_all_desc_a()
{
    wxArrayString desc_a;
    wxString prev_desc;
    for (const auto& att_d : find_data_a(
        TableClause::ORDERBY(Col::NAME_DESCRIPTION)
    )) {
        if (att_d.m_description != prev_desc) {
            desc_a.Add(att_d.m_description);
            prev_desc = att_d.m_description;
        }
    }
    return desc_a;
}
