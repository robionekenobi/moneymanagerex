/*******************************************************
 Copyright (C) 2014 Gabriele-V
 Copyright (C) 2022 Mark Whalley (mark@ipx.co.uk)

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

#include "mmAttachment.h"

#include "base/mmPlatform.h"
#include "mmPath.h"
#include "model/InfoModel.h"
#include "model/AttachmentModel.h"
#include "model/TrxModel.h"
#include "dialog/AttachmentDialog.h"

const wxString mmAttachment::s_path_sep = wxFileName::GetPathSeparator();

// Get the platform-specific attachments folder and substitute symbolic paths.
// Return either an empty string or a path ending in path separator
const wxString mmAttachment::getFolder()
{
    return mmPath::getPathAttachment(InfoModel::instance().getString(
        "ATTACHMENTSFOLDER:" + mmPlatform::platformType(),
        ""
    ));
}

const wxString mmAttachment::getMarker()
{
    return wxString::Format("[%s] ", _t("Att."));
}

bool mmAttachment::createReadmeFile(const wxString& folder)
{
    wxString readme_path = folder + s_path_sep + "readme.txt";
    if (wxFileExists(readme_path))
        return true;

    wxString readme_text;
    readme_text << _t("This directory and its files are automatically managed by MMEX.")
        << wxTextFile::GetEOL();
    readme_text << wxTextFile::GetEOL();
    readme_text << _t("Please do not remove, rename or modify manually directories and files.")
        << wxTextFile::GetEOL();

    bool ok = false;
    try {
        wxFile file(readme_path, wxFile::write);
        if (file.IsOpened()) {
            file.Write(readme_text);
            file.Close();
            ok = true;
        }
    }
    catch (...) {
    }

    return ok;
}

bool mmAttachment::openFile(const wxString& file)
{
    if (!wxFileExists(file)) {
        wxString msgStr = wxString() << _t("Unable to open file:") << "\n"
            << "'" << file << "'" << "\n"
            << "\n"
            << _t("Please verify that file exists and user has rights to read it.") << "\n";
        wxMessageBox(msgStr, _t("Open attachment failed"), wxICON_ERROR);
        return false;
    }

    return wxLaunchDefaultApplication(file);;
}

bool mmAttachment::copyFile(
    const wxString& src_file,
    const wxString& dst_file
) {
    if (wxFileExists(dst_file)) {
        bool found = (AttachmentModel::instance().find_count(
            AttachmentCol::WHERE_FILENAME(OP_EQ, wxFileNameFromPath(dst_file))
        ) > 0);

        wxString msgStr = wxString() << _t("Destination file already exists:") << "\n"
            << "'" << dst_file << "'" << "\n"
            << "\n"
            << (found
                ? _t("File already found in attachments")
                : _t("File not found in attachments. Please delete or rename it.")
            )
            << "\n";
        wxMessageBox(msgStr, _t("Destination file already exists"), wxICON_ERROR);

        return false;
    }

    wxString dst_folder = wxPathOnly(dst_file);
    if (!wxDirExists(dst_folder)) {
        if (!wxMkdir(dst_folder))
            return false;
        mmAttachment::createReadmeFile(dst_folder);
    }

    if (!wxCopyFile(src_file, dst_file))
        return false;

    if (InfoModel::instance().getBool("ATTACHMENTSDELETE", false))
        wxRemoveFile(src_file);

    return true;
}

bool mmAttachment::deleteFile(const wxString& file)
{
    if (file.IsEmpty())
        return true;

    if (!wxFileExists(file)) {
        wxString msgStr = wxString() << _t("Attachment not found:") << "\n"
            << "'" << file << "'" << "\n"
            << "\n"
            << _t("Do you want to delete the attachment in the database?") << "\n";
        return (wxMessageBox(
            msgStr,
            _t("Delete attachment failed"),
            wxYES_NO | wxNO_DEFAULT | wxICON_ERROR
        ) == wxYES);
    }

    if (!InfoModel::instance().getBool("ATTACHMENTSTRASH", false))
        return wxRemoveFile(file);

    const wxString folder = mmAttachment::getFolder();
    if (folder.IsEmpty()) {
        wxString msgStr = wxString() << _t("ATTACHMENTSFOLDER not configured:") << "\n"
            << "'" << file << "'" << "\n"
            << "\n"
            << _t("Do you want to delete the attachment in the database?") << "\n";
        return (wxMessageBox(
            msgStr,
            _t("Delete attachment failed"),
            wxYES_NO | wxNO_DEFAULT | wxICON_ERROR
        ) == wxYES);
    }

    const wxString deleted_folder = folder + "Deleted";

    if (!wxDirExists(deleted_folder)) {
        if (!wxMkdir(deleted_folder))
            return false;
        mmAttachment::createReadmeFile(deleted_folder);
    }

    const wxString deleted_file = deleted_folder + s_path_sep +
        mmDate::today().isoDate() + "_" + wxFileNameFromPath(file);

    return wxRenameFile(file, deleted_file);
}

bool mmAttachment::relocate_ref_all(
    RefTypeN old_ref_type, int64 old_ref_id,
    RefTypeN new_ref_type, int64 new_ref_id
) {
    AttachmentModel::DataA att_a = AttachmentModel::instance().find_data_a(
        AttachmentCol::WHERE_REFTYPE(OP_EQ, old_ref_type.key_n()),
        AttachmentCol::WHERE_REFID(OP_EQ, old_ref_id)
    );

    if (att_a.size() == 0)
        return false;

    const wxString old_folder = mmAttachment::getFolder() + old_ref_type.key_n() + s_path_sep;
    const wxString new_folder = mmAttachment::getFolder() + new_ref_type.key_n() + s_path_sep;

    for (AttachmentData& att_d : att_a) {
        wxString newFileName = att_d.m_filename;
        newFileName.Replace(
            att_d.m_ref_type_n.key_n() + "_" + wxString::Format("%lld", att_d.m_ref_id),
            new_ref_type.key_n() + "_" + wxString::Format("%lld", new_ref_id)
        );
        wxRenameFile(
            old_folder + att_d.m_filename,
            new_folder + newFileName
        );
        att_d.m_ref_type_n = new_ref_type;
        att_d.m_ref_id     = new_ref_id;
        att_d.m_filename   = newFileName;
    }
    AttachmentModel::instance().save_data_a(att_a);

    if (old_ref_type == TrxModel::s_ref_type)
        TrxModel::instance().save_timestamp(old_ref_id);
    if (new_ref_type == TrxModel::s_ref_type)
        TrxModel::instance().save_timestamp(new_ref_id);

    return true;
}

bool mmAttachment::clone_ref_all(
    RefTypeN ref_type,
    int64 src_ref_id,
    int64 dst_ref_id
) {
    const wxString folder = mmAttachment::getFolder() + ref_type.key_n() + s_path_sep;

    for (const AttachmentData& src_att_d : AttachmentModel::instance().find_data_a(
        AttachmentCol::WHERE_REFTYPE(OP_EQ, ref_type.key_n()),
        AttachmentCol::WHERE_REFID(OP_EQ, src_ref_id)
    )) {
        wxString dst_filename = src_att_d.m_filename;
        dst_filename.Replace(
            src_att_d.m_ref_type_n.key_n() + "_" + wxString::Format("%lld", src_att_d.m_ref_id),
            src_att_d.m_ref_type_n.key_n() + "_" + wxString::Format("%lld", dst_ref_id)
        );
        wxCopyFile(folder + src_att_d.m_filename, folder + dst_filename);
        AttachmentData new_att_d = AttachmentData();
        new_att_d.m_ref_type_n  = ref_type;
        new_att_d.m_ref_id      = dst_ref_id;
        new_att_d.m_filename    = dst_filename;
        new_att_d.m_description = src_att_d.m_description;
        AttachmentModel::instance().add_data_n(new_att_d);
    }

    if (ref_type == TrxModel::s_ref_type)
        TrxModel::instance().save_timestamp(dst_ref_id);

    return true;
}

void mmAttachment::openFromPanelIcon(
    wxWindow* parent,
    RefTypeN ref_type,
    int64 ref_id
) {
    int att_c = AttachmentModel::instance().find_ref_c(ref_type, ref_id);

    if (att_c == 1) {
        // CHECK: find_ref_c() and find_ref_data_a() apply different filters.
        // In case of inconsistency, att_a may be empty.
        AttachmentModel::DataA att_a = AttachmentModel::instance().find_ref_data_a(
            ref_type, ref_id
        );
        wxString file = mmAttachment::getFolder() +
            att_a[0].m_ref_type_n.key_n() + s_path_sep +
            att_a[0].m_filename;
        mmAttachment::openFile(file);
    }
    else {
        AttachmentDialog dlg(parent, ref_type, ref_id);
        dlg.ShowModal();
    }
}
