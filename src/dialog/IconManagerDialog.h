/*******************************************************
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

#include <wx/wx.h>
#include <wx/dnd.h>
#include <wx/scrolwin.h>
#include <functional>
#include <string>
#include "base/defs.h"

bool iconDownloadFavicon(const wxString& websiteUrl, std::string& outputName, std::string& ext);

wxString iconExtractServerName(const wxString& url);

bool iconSetButtonIconFromFile(wxBitmapButton* button, const wxString& filePath);


class IconDropTarget : public wxFileDropTarget
{
public:
    std::function<void(const wxArrayString&)> onDrop;
    bool OnDropFiles(wxCoord, wxCoord, const wxArrayString& filenames) override;
};

class IconManagerDialog : public wxDialog
{
public:
    IconManagerDialog(wxWindow* parent, const wxString& dir);

private:
    wxString m_dir;

    wxScrolledWindow* m_scroll;
    wxFlexGridSizer* m_gridSizer;
    wxStaticBitmap* m_preview;
    wxStaticText* m_preview_label;
    wxButton* m_delBtn;
    wxButton* m_renameBtn;
    wxString m_selectedFile;
    wxFont m_labelFont;

    // Core
    void LoadIcons();
    wxBitmap LoadBitmap(const wxString& path, int size);
    void UpdatePreview(const wxString& path);

    void OnResize(wxSizeEvent& event);

    // Actions
    void OnAdd(wxCommandEvent&);
    void OnAddFavIcon(wxCommandEvent&);
    void OnDelete(wxCommandEvent&);
    void OnRename(wxCommandEvent&);
    void AddFiles(const wxArrayString& files);

    // Context menu
    void OnRightClick(wxMouseEvent& event, const wxString& path);

    // Other
    void SetPreviewToIdle();
    wxDECLARE_EVENT_TABLE();
};
