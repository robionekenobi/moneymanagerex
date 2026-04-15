/*******************************************************
 Copyright (C) 2014 Gabriele-V
 Copyright (C) 2021, 2022 Mark Whalley (mark@ipx.co.uk)

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
#include "data/_DataEnum.h"

class mmAttachment
{
public:
    static const wxString s_path_sep;

public:
    static auto getFolder() -> const wxString;
    static auto getMarker() -> const wxString;
    static bool createReadmeFile(const wxString& folder);
    static bool openFile(const wxString& file);
    static bool copyFile(const wxString& src_file, const wxString& dst_file);
    static bool deleteFile(const wxString& file);

    static bool relocate_ref_all(
        RefTypeN old_ref_type, int64 old_ref_id,
        RefTypeN new_ref_type, int64 new_ref_id
    );
    static bool clone_ref_all(RefTypeN ref_type, int64 src_ref_id, int64 dst_ref_id);
    static void openFromPanelIcon(
        wxWindow* parent,
        RefTypeN ref_type, int64 ref_id
    );
};
