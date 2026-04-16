/*******************************************************
 Copyright (C) 2016 Guan Lisheng
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

#include "TagModel.h"

#include "TagLinkModel.h"
#include "AttachmentModel.h"
#include "TrxModel.h"
#include "_ModelAll.h"

// -- constructor

// Initialize the global TagModel.
TagModel& TagModel::instance(wxSQLite3Database* db)
{
    TagModel& ins = Singleton<TagModel>::instance();
    ins.reset_cache();
    ins.m_db = db;
    ins.ensure_table();

    return ins;
}

// Return the static instance of TagModel
TagModel& TagModel::instance()
{
    return Singleton<TagModel>::instance();
}

// -- override

bool TagModel::find_id_isUsed(int64 tag_id, bool ignore_deleted)
{
    bool is_used = false;

    if (!ignore_deleted) {
        // Fast path: search tag_id with a single query.
        is_used = is_used || TagLinkModel::instance().find_count(
            TagLinkCol::WHERE_TAGID(OP_EQ, tag_id)
        ) > 0;
    }
    else {
        // Slow path: fetch each tag link, until a match is found.
        // Alternatively, a single join query could be constructed and executed,
        // at the cost of more code (join queries are not supported by TableFactory).
        for (const TagLinkData& gl_d : TagLinkModel::instance().find_data_a(
            TagLinkCol::WHERE_TAGID(OP_EQ, tag_id)
        )) {
            is_used = is_used || ModelAll::instance().find_ref_count(
                gl_d.m_ref_type, gl_d.m_ref_id, true
            ) > 0;
            if (is_used)
                break;
        }
    }

    return is_used;
}

// -- methods

const TagData* TagModel::get_name_data_n(const wxString& name)
{
    const Data* tag_n = search_cache_n(TagCol::TAGNAME(name));
    if (tag_n)
        return tag_n;

    for (int64 tag_id : find_id_a(
        TagCol::WHERE_TAGNAME(OP_EQ, name)
    )) {
        tag_n = get_idN_data_n(tag_id);
    }

    return tag_n;
}

