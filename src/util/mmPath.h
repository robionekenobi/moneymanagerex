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

#pragma once

// mmex's files and dirs locations.
// All functions return absolute paths.

#include "base/_defs.h"

struct mmPath
{
    enum EDocFile
    {
        F_CONTRIB = 0,
        F_LICENSE,
        HTML_INDEX,
        HTML_WEBAPP,
        HTML_CUSTOM_SQL,
        HTML_INVESTMENT,
        HTML_BUDGET,
        HTML_REPORTS,
        DOC_FILES_MAX
    };

    enum EResFile
    {
        TRANS_SOUND1 = 0,
        TRANS_SOUND2,
        HOME_PAGE_TEMPLATE,
        MMEX_LOGO,
        THEMESDIR,
        REPORTS,
        ICONDIR,
        RES_FILES_MAX
    };

    enum ESharedFile
    {
        LANG_DIR = 0,
        SHARED_FILES_MAX
    };

    enum EUserFile
    {
        SETTINGS = 0,
        DIRECTORY,
        USERTHEMEDIR,
        USERICONS,
        USER_FILES_MAX
    };

    static auto getSettingsFileName() -> const wxString { return "mmexini.db3"; }
    static auto getUserTheme() -> const wxString { return "themes"; }
    static auto getDirectory() -> const wxString { return ""; }
    static auto getIconDir() -> const wxString { return "icons"; }
    static auto getSettingsPathPortable() -> wxFileName;
    static auto getUserDir(bool create) -> const wxFileName;
    static auto getLogDir(bool create) -> const wxFileName;

    static auto getPathDoc(EDocFile f, bool url = true) -> wxString;
    static auto getPathResource(EResFile f) -> const wxString;
    static auto getProgramIcon() -> const wxIcon&;
    static auto getPathShared(ESharedFile f) -> const wxString;
    static auto getPathUser(EUserFile f) -> const wxString;
    static const wxFileName getPathUserRaw(EUserFile f, bool create = false);

    static auto getPathImport(const wxString& importFolder) -> const wxString;
    static auto getPathAttachment(const wxString& folder) -> const wxString;
    static bool isPortableMode();
    static auto getTempFolder() -> const wxString;
};
