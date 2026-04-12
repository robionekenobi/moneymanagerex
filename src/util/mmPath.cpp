/*******************************************************
Copyright (C) 2009 VaDiM
Copyright (C) 2026 Klaus Wich

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
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/icon.h>

#include "base/_constants.h"
#include "base/mmPlatform.h"
#include "mmPath.h"
#include "_util.h"
#include "../resources/mmexico.xpm"

#include "model/SettingModel.h"
#include "model/InfoModel.h"

wxFileName mmPath::getSettingsPathPortable()
{
    wxFileName f = mmPlatform::shareDir();
    f.SetFullName(mmPath::getSettingsFileName());
    return f;
}

// This routine is platform-independent.
// MMEX is a portable application which means ability to to run
// without installation, for example, from USB flash drive.
//
// If mmex finds mmexini.db3 in its folder, it assumes portable
// mode and getUserDir() in such case points to that folder.
//
// FIXME: security issue - temp files will be created on host filesystem.
const wxFileName mmPath::getUserDir(bool create)
{
    static wxFileName fname;

    if (!fname.IsOk()) {
        fname = mmPath::getSettingsPathPortable();

        bool portable_file_ok = fname.IsFileWritable() && fname.IsFileReadable();

        if (!portable_file_ok) {
            fname.AssignDir(wxStandardPaths::Get().GetUserDataDir());

            if (create && !fname.DirExists()) {
                // 0700 - octal, "111 000 000"
                portable_file_ok = fname.Mkdir(0700, wxPATH_MKDIR_FULL);
                wxASSERT(portable_file_ok);
            }
        }

        fname.SetFullName(wxGetEmptyString());
    }

    return fname;
}

const wxFileName mmPath::getLogDir(bool create)
{
    static wxFileName fname;

    if (!fname.IsOk()) {
        fname = mmPath::getUserDir(create);
        //FIXME: file not found ERROR
        //fname.AppendDir("logs");
    }

    return fname;
}

wxString mmPath::getPathDoc(EDocFile f, bool url)
{
    if (f < 0 || f >= DOC_FILES_MAX)
        f = HTML_INDEX;

    static const wxString files[DOC_FILES_MAX] = {
      "contrib.txt",
      "help%slicense.txt",
      "help%sindex.html",
      "help%sindex.html#section11.1",
      "help%sgrm.html",
      "help%sstocks_and_shares.html",
      "help%sbudget.html",
      "help%sindex.html#section17"
    };

    wxString path = files[f];
    wxString section;
    wxRegEx pattern(R"(^([^#]+)#([^#]+)$)");
    if (pattern.Matches(path)) {
        section = pattern.GetMatch(path, 2);
        path = pattern.GetMatch(path, 1);
    }

    wxString lang_code = PrefModel::instance().getLanguageCode();
    if (lang_code.empty() || lang_code == "en_US") {
        lang_code = "en_GB";
    }
    path = wxString::Format(path,
        wxFileName::GetPathSeparator() + lang_code + wxFileName::GetPathSeparator()
    );

    wxFileName helpIndexFile(mmPlatform::docDir());
    path.Prepend(helpIndexFile.GetPathWithSep());
    wxFileName helpFullPath(path);

    // Load the help file for the given language
    if (!helpFullPath.FileExists()) {
        section.clear();
        path = files[f];
        path.Replace("%s", wxFileName::GetPathSeparator());
        wxFileName help(mmPlatform::docDir());
        path.Prepend(help.GetPathWithSep());
    }

    if (url) {
        path.Prepend("file://");
    }

    if (!section.empty()) {
        path.Append("#" + section);
    }

    return path;
}

const wxString mmPath::getPathResource(EResFile f)
{
    static std::unordered_map<int, wxString> cache;
    const auto it = cache.find(f);
    if (it != cache.end()) {
        return it->second;
    }

    wxFileName fname = mmPlatform::resourceDir();
    std::vector<std::pair<int, wxString>> files = {
        { TRANS_SOUND1,       "drop.wav" },
        { TRANS_SOUND2,       "cash.wav" },
        { HOME_PAGE_TEMPLATE, "home_page.htt" },
        { MMEX_LOGO,          "mmex.svg" },
        { THEMESDIR,          "themes" },
        { REPORTS,            "reports" },
        { ICONDIR,            "icons" }
    };

    for (const auto& item : files) {
        fname.SetFullName(item.second);
        cache[item.first] = fname.GetFullPath();
    }

    return cache[f];
}

// use instead of getPathResource(PROGRAM_ICON)
const wxIcon& mmPath::getProgramIcon()
{
    static wxIcon icon(mmexico_xpm);
    return icon;
}

const wxString mmPath::getPathShared(ESharedFile f)
{
    static const wxString files[SHARED_FILES_MAX] = {
      "locale"
    };

    wxASSERT(f >= 0 && f < SHARED_FILES_MAX);

    wxString path = files[f];
    path.Replace("/", wxFILE_SEP_PATH);
    return path.Prepend(mmPlatform::shareDir().GetPathWithSep());
}

const wxString mmPath::getPathUser(EUserFile f)
{
    wxFileName fname = mmPath::getPathUserRaw(f, false);
    return fname.GetFullPath();
}

const wxFileName mmPath::getPathUserRaw(EUserFile f, bool create)
{
    static const wxString files[USER_FILES_MAX] = {
      mmPath::getSettingsFileName(),
      mmPath::getDirectory(),
      mmPath::getUserTheme(),
      mmPath::getIconDir()
    };

    wxASSERT(f >= 0 && f < USER_FILES_MAX);

    wxFileName fname = mmPath::getUserDir(true);
    if (f > DIRECTORY) {
        fname.AppendDir(files[f]);
        if (!fname.DirExists()) {
            if (create && !fname.Mkdir(wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL)) {
                wxLogError("Directory %s does not exist and could not be created!", fname.GetFullPath());
            }
            else {
                wxLogDebug("Directory %s does not exist!", fname.GetFullPath());
            }
        }
    }
    else {
        fname.SetFullName(files[f]);
    }

    return fname;
}

// This function transforms mnemonic paths to real one
// For example %USERPROFILE%\MyBudget will be transformed to C:\Users\James\MyBudget
const wxString mmPath::getPathAttachment(const wxString& folder)
{
    if (folder == wxEmptyString)
        return wxEmptyString;

    wxString path = folder;
    const wxString sep = wxFileName::GetPathSeparator();
    const wxString lastDBPath = SettingModel::instance().getLastDbPath();
    const wxString& lastDBFolder = wxFileName::FileName(lastDBPath).GetPath() + sep;
    const wxString& userFolder = mmPath::getUserDir(false).GetPath() + sep;

    if (folder.StartsWith(ATTACHMENTS_FOLDER_USERPROFILE, &path))
        path.Prepend(wxGetHomeDir() + sep);
    else if (folder.StartsWith(ATTACHMENTS_FOLDER_DOCUMENTS, &path))
        path.Prepend(wxStandardPaths::Get().GetDocumentsDir() + sep);
    else if (folder.StartsWith(ATTACHMENTS_FOLDER_DATABASE, &path))
        path.Prepend(lastDBFolder);
    else if (folder.StartsWith(ATTACHMENTS_FOLDER_APPDATA, &path))
        path.Prepend(userFolder);

    if (path.Last() != sep)
        path.Append(sep);
    if (InfoModel::instance().getBool("ATTACHMENTSSUBFOLDER", true))
        path += wxString::Format("MMEX_%s_Attachments%s",
            wxFileName::FileName(lastDBPath).GetName(),
            sep
        );

    return path;
}

bool mmPath::isPortableMode()
{
    wxFileName f = mmPath::getSettingsPathPortable();
    return (f.GetFullPath() == mmPath::getPathUser(SETTINGS));
}

const wxString mmPath::getTempFolder()
{
    const wxString path = mmPath::isPortableMode()
        ? mmPath::getUserDir(false).GetPath()
        : wxStandardPaths::Get().GetTempDir();
    const wxString folder = mmPath::isPortableMode()
        ? "tmp"
        : wxString::Format("%s_%s_tmp", mmPlatform::appName(), wxGetUserId());

    return wxString::Format("%s%s%s%s",
        path,
        wxString(wxFILE_SEP_PATH),
        folder,
        wxString(wxFILE_SEP_PATH)
    );
}
