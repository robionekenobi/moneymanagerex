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

#include "dialog/IconManagerDialog.h"
#include "util/mmPath.h"
#include "util/mmImage.h"
#include "model/InfoModel.h"

#include <regex>
#include <filesystem>

// Download Fav ICON:
struct MemoryBuffer {
    std::string data;
};

size_t _writeToString(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t totalSize = size * nmemb;
    static_cast<MemoryBuffer*>(userp)->data.append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

size_t _writeToFile(void* ptr, size_t size, size_t nmemb, void* userdata)
{
    std::ofstream* out = static_cast<std::ofstream*>(userdata);
    out->write(static_cast<char*>(ptr), size * nmemb);
    return size * nmemb;
}

bool _httpGet(const std::string& url, std::string& response)
{
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    MemoryBuffer buffer;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _writeToString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0");

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
        return false;

    response = buffer.data;
    return true;
}

bool _downloadFile(const std::string& url, const std::string& outputFile)
{
    std::string tempFile = outputFile + ".tmp";

    CURL* curl = curl_easy_init();
    if (!curl) {
        return false;
    }

    std::ofstream file(tempFile, std::ios::binary);
    if (!file.is_open()) {
        curl_easy_cleanup(curl);
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _writeToFile);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);

    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);

    CURLcode res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    file.close();

    if (res != CURLE_OK || !std::filesystem::exists(tempFile) || std::filesystem::file_size(tempFile) == 0) {
        std::remove(tempFile.c_str());
        return false;
    }

    std::remove(outputFile.c_str());
    if (std::rename(tempFile.c_str(), outputFile.c_str()) != 0) {
        std::remove(tempFile.c_str());
        return false;
    }

    return true;
}

std::string _getBaseUrl(const std::string& url)
{
    std::regex re(R"(^(https?:\/\/[^\/]+))");
    std::smatch match;

    if (std::regex_search(url, match, re))
        return match[1];

    return "";
}

std::string _makeAbsolute(const std::string& base, const std::string& link)
{
    if (link.find("http") == 0)
        return link;

    if (!link.empty() && link[0] == '/')
        return base + link;

    return base + "/" + link;
}

std::string _extractFaviconUrl(const std::string& html)
{
    std::regex re(R"(<link[^>]*rel=["']([^"']*icon[^"']*)["'][^>]*href=["']([^"']+)["'])", std::regex::icase);
    std::smatch match;
    if (std::regex_search(html, match, re)) {
        return match[2];
    }

    return "";
}

std::string _detectExtension(const std::string& url)
{
    if (url.find(".png") != std::string::npos) return ".png";
    if (url.find(".svg") != std::string::npos) return ".svg";
    if (url.find(".bmp") != std::string::npos) return ".bmp";
    if (url.find(".ico") != std::string::npos) return ".ico";

    return "";
}

bool verifyFile(const wxString (path))
{
    bool fileIsOk = true;
    if (path.Lower().EndsWith(".svg")) {
        wxBitmapBundle bundle = wxBitmapBundle::FromSVGFile(path, wxSize(32,32));
        fileIsOk = bundle.IsOk();
    }
    else {
        wxImage image;
        if (image.LoadFile(path)) {
            if (path.Lower().EndsWith(".png")) {  // Cleanup PNG if necessary
                if (!image.HasAlpha()) {
                    image.InitAlpha();
                }
                image.SetOption(wxIMAGE_OPTION_PNG_COMPRESSION_LEVEL, 9);
                fileIsOk = image.SaveFile(path, wxBITMAP_TYPE_PNG);
            }
        }
        else {
            fileIsOk = false;
        }
    }
    return fileIsOk;
}

// ---- Exported functions -----------------------------------
bool iconDownloadFavicon(const wxString& websiteUrl, std::string& outputName, std::string& ext)
{
    std::string url = (websiteUrl.Lower().StartsWith("http://") || websiteUrl.Lower().StartsWith("https://")) ? websiteUrl.utf8_string() : ("https://" + websiteUrl).utf8_string();
    std::string html;
    if (!_httpGet(url, html)) {
        wxLogDebug("Download Fav Icon: could not download HTML page");
        return false;
    }

    std::string base = _getBaseUrl(url);
    std::string iconUrl = _extractFaviconUrl(html);

    if (!iconUrl.empty()) {
        iconUrl = _makeAbsolute(base, iconUrl);
    }
    else {
        iconUrl = base + "/favicon.ico";         // fallback
    }

    ext = _detectExtension(iconUrl);
    if (ext == "") {
        return false;
    }
    outputName += ext;

    if (!_downloadFile(iconUrl, outputName)) {
        wxLogDebug("Download Fav Icon: could not load Fav Icon");
        return false;
    }

    if (!verifyFile(wxString(outputName))) {
        wxLogDebug("Download Fav Icon: Downloaded file can not be read => Delete it");
        std::remove(outputName.c_str());
        return false;
    }

    return true;
}

wxString iconExtractServerName(const wxString& url)
{
    wxString working = url;
    int schemePos = working.Find("://");
    if (schemePos != wxNOT_FOUND) {
        working = working.Mid(schemePos + 3);
    }

    int slashPos = working.Find('/');
    if (slashPos != wxNOT_FOUND) {
        working = working.Left(slashPos);
    }

    int colonPos = working.Find(':');
    if (colonPos != wxNOT_FOUND) {
        working = working.Left(colonPos);
    }

    working.Replace(".", "_");
    return working;
}

bool iconSetButtonIconFromFile(wxBitmapButton* button, const wxString& filePath)
{
    wxSize btnSize = button->GetSize();
    button->SetBitmapMargins(2, 2);
    bool result = true;

    if (filePath.Lower().EndsWith(".svg")) {
        wxBitmapBundle bundle = wxBitmapBundle::FromSVGFile(filePath, btnSize);
        if (bundle.IsOk()) {
             button->SetBitmap(bundle);
        }
        else {
            wxLogDebug("Could not load SVG Icon: %s", filePath);
            result = false;
        }
    }
    else {
        wxImage image;
        if (image.LoadFile(filePath)) {
            image = image.Scale(btnSize.GetWidth(), btnSize.GetHeight(), wxIMAGE_QUALITY_HIGH);
            wxBitmap bitmap(image);
            button->SetBitmap(bitmap);
        }
        else {
            wxLogDebug("Could not load Icon: %s", filePath);
            result = false;
        }
    }

    return result;
}

// =============== Icon Manager =============================
// -------------------------
// Drop Target
// -------------------------
bool IconDropTarget::OnDropFiles(wxCoord, wxCoord, const wxArrayString& filenames)
{
    if (onDrop) onDrop(filenames);
    return true;
}

// -------------------------
// IDs
// -------------------------
enum
{
    ID_CTX_DELETE = wxID_HIGHEST + 1,
    ID_CTX_RENAME,
    ID_FAVICON
};

// -------------------------
// IconManagerDialog
// -------------------------
wxBEGIN_EVENT_TABLE(IconManagerDialog, wxDialog)
    EVT_MENU(ID_CTX_DELETE, IconManagerDialog::OnDelete)
    EVT_MENU(ID_CTX_RENAME, IconManagerDialog::OnRename)
    EVT_SIZE(IconManagerDialog::OnResize)
wxEND_EVENT_TABLE()

// -------------------------
IconManagerDialog::IconManagerDialog(wxWindow* parent, const wxString& dir)
    : wxDialog(parent, wxID_ANY, _t("Custom icons"), wxDefaultPosition, wxSize(600, 500),
                wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
      m_dir(dir)
{
    wxBoxSizer* dialogSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    // LEFT: Icon Grid
    m_scroll = new wxScrolledWindow(this);
    m_gridSizer = new wxFlexGridSizer(0, 4, 10, 10);
    m_gridSizer->SetFlexibleDirection(wxBOTH);
    m_gridSizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);
    m_scroll->SetSizer(m_gridSizer);
    m_scroll->SetScrollRate(0,5);
    mainSizer->Add(m_scroll, 1, wxEXPAND | wxALL, 5);

    // RIGHT: Preview + Actions
    wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);

    m_preview = new wxStaticBitmap(this, wxID_ANY, wxBitmap(128,128));
    rightSizer->Add(m_preview, 0, wxALL | wxALIGN_CENTER, 10);

    m_preview_label = new wxStaticText(this, wxID_ANY, "",
                                                   wxDefaultPosition, wxSize(120,-1),
                                                   wxALIGN_CENTER_HORIZONTAL | wxST_ELLIPSIZE_END);
    m_labelFont = m_preview_label->GetFont();
    double fs = m_labelFont.GetFractionalPointSize();
    m_labelFont.SetFractionalPointSize(fs -2);
    m_preview_label->SetFont(m_labelFont);
    rightSizer->Add(m_preview_label, 0, wxALL | wxALIGN_CENTER, 10);

    m_renameBtn = new wxButton(this, ID_CTX_RENAME, _t("&Rename"));
    m_delBtn = new wxButton(this, wxID_DELETE, _t("&Delete"));

    rightSizer->Add(m_renameBtn, 0, wxALL | wxEXPAND, 5);
    rightSizer->Add(m_delBtn, 0, wxALL | wxEXPAND, 5);
    rightSizer->AddStretchSpacer();
    rightSizer->Add(new wxButton(this, wxID_ADD, _t("&Add Icon")), 0, wxALL | wxEXPAND, 5);
    rightSizer->Add(new wxButton(this, ID_FAVICON, _t("Add &Favicon")), 0, wxALL | wxEXPAND, 5);

    mainSizer->Add(rightSizer, 0, wxEXPAND);

    // add main area
    dialogSizer->Add(mainSizer, 1, wxEXPAND);

    // bottom area
    wxStaticLine* panelSeparatorLine = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    dialogSizer->Add(panelSeparatorLine, 0, wxGROW | wxLEFT | wxRIGHT, 10);

    wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    bottomSizer->AddStretchSpacer(1);
    wxButton* closeBtn = new wxButton(this, wxID_CLOSE);
    bottomSizer->Add(closeBtn, 0, wxALL | wxALIGN_CENTER, 10);
    bottomSizer->AddStretchSpacer(1);

    dialogSizer->Add(bottomSizer, 0, wxEXPAND);

    // set final
    SetSizer(dialogSizer);

    // Events
    Bind(wxEVT_BUTTON, &IconManagerDialog::OnRename, this, ID_CTX_RENAME);
    Bind(wxEVT_BUTTON, &IconManagerDialog::OnAdd, this, wxID_ADD);
    Bind(wxEVT_BUTTON, &IconManagerDialog::OnAddFavIcon, this, ID_FAVICON);
    Bind(wxEVT_BUTTON, &IconManagerDialog::OnDelete, this, wxID_DELETE);
    Bind(wxEVT_BUTTON, [this](wxCommandEvent&) { EndModal(wxID_OK); }, wxID_CLOSE);

    // Drag & Drop
    auto drop = new IconDropTarget();
    drop->onDrop = [this](const wxArrayString& files) { AddFiles(files); };
    m_scroll->SetDropTarget(drop);

    LoadIcons();
}

// -------------------------
void IconManagerDialog::OnResize(wxSizeEvent& event)
{
    int width = m_scroll->GetClientSize().GetWidth() - 50;
    if (width > 0)
    {
        int cols = std::max(1, width / 80);
        m_gridSizer->SetCols(cols);

        m_gridSizer->Layout();
        m_scroll->FitInside();
    }
    event.Skip();
}

// -------------------------
void IconManagerDialog::LoadIcons()
{
    m_gridSizer->Clear(true);
    SetPreviewToIdle();

    wxDir dir(m_dir);
    if (!dir.IsOpened()) return;

    wxString filename;
    bool cont = dir.GetFirst(&filename);

    while (cont)
    {
        wxString path = wxFileName(m_dir, filename).GetFullPath();
        wxBitmap bmp = LoadBitmap(path, 64);

        if (bmp.IsOk())
        {
            wxPanel* itemPanel = new wxPanel(m_scroll);
            wxBoxSizer* itemSizer = new wxBoxSizer(wxVERTICAL);

            wxBitmapButton* btn = new wxBitmapButton(itemPanel, wxID_ANY, bmp);
            btn->SetMinSize(wxSize(75,75));

            wxStaticText* label = new wxStaticText(itemPanel, wxID_ANY, filename,
                                                   wxDefaultPosition, wxSize(80,-1),
                                                   wxALIGN_CENTER_HORIZONTAL | wxST_ELLIPSIZE_END);

            label->SetFont(m_labelFont);

            itemSizer->Add(btn, 0, wxALIGN_CENTER | wxBOTTOM, 2);
            itemSizer->Add(label, 0, wxALIGN_CENTER);
            itemPanel->SetSizer(itemSizer);

            auto selectHandler = [this, path](wxCommandEvent&) {
                m_selectedFile = path;
                UpdatePreview(path);
            };
            btn->Bind(wxEVT_BUTTON, selectHandler);
            label->Bind(wxEVT_LEFT_DOWN, [this, path](wxMouseEvent&) {
                m_selectedFile = path;
                UpdatePreview(path);
            });

            auto rightClickHandler = [this, path](wxMouseEvent& evt){
                OnRightClick(evt, path);
            };
            btn->Bind(wxEVT_RIGHT_DOWN, rightClickHandler);
            label->Bind(wxEVT_RIGHT_DOWN, rightClickHandler);
            itemPanel->Bind(wxEVT_RIGHT_DOWN, rightClickHandler);

           m_gridSizer->Add(itemPanel, 0, wxALL | wxALIGN_CENTER, 5);
        }

        cont = dir.GetNext(&filename);
    }

    m_gridSizer->SetCols(3);
    m_scroll->Layout();
    m_scroll->FitInside();
}

// -------------------------
void IconManagerDialog::SetPreviewToIdle()
{
    m_delBtn->Enable(false);
    m_renameBtn->Enable(false);
    m_preview_label->SetLabel("");
    wxBitmap empty(128, 128);
    wxMemoryDC dc(empty);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    dc.SelectObject(wxNullBitmap);
    m_preview->SetBitmap(empty);
}

// -------------------------
wxBitmap IconManagerDialog::LoadBitmap(const wxString& path, int size)
{
    if (path.Lower().EndsWith(".svg")) {
        wxBitmapBundle bundle = wxBitmapBundle::FromSVGFile(path, wxSize(size,size));
        if (bundle.IsOk())
            return bundle.GetBitmap(wxSize(size,size));
    }
    else {
        wxImage img;
        if (img.LoadFile(path))
        {
            img = img.Scale(size, size, wxIMAGE_QUALITY_HIGH);
            return wxBitmap(img);
        }
    }
    return wxBitmap();
}

// -------------------------
void IconManagerDialog::UpdatePreview(const wxString& path)
{
    wxBitmap bmp = LoadBitmap(path, 128);
    if (bmp.IsOk()) {
        m_preview->SetBitmap(bmp);
    }
    wxFileName* fname = new wxFileName(path);
    m_preview_label->SetLabel(fname->GetFullName());
    m_delBtn->Enable(bmp.IsOk());
    m_renameBtn->Enable(bmp.IsOk());
}

// -------------------------
void IconManagerDialog::OnAdd(wxCommandEvent&)
{
    wxString defPath = InfoModel::instance().getString("ICON_MANAGER_DIALOG_ADD_FILE_PATH", "");
    wxFileDialog dlg(this, "Add Icons", defPath, "",
        "Images (*.png;*.jpg;*.bmp;*.svg)|*.png;*.jpg;*.bmp;*.svg",
        wxFD_OPEN | wxFD_MULTIPLE);

    if (dlg.ShowModal() == wxID_OK) {
        wxArrayString files;
        dlg.GetPaths(files);
        AddFiles(files);
        wxFileName fn = wxFileName(files[0]);
        InfoModel::instance().saveString("ICON_MANAGER_DIALOG_ADD_FILE_PATH", fn.GetPath());
    }
}

// -------------------------
void IconManagerDialog::OnAddFavIcon(wxCommandEvent&)
{
    wxTextEntryDialog dlg(this, _t("Please enter a valid URL"), _t("URL entry"));
    static const std::regex pattern( R"(^(?:(https?|ftp)://)?(www\.)?[^\s/$.?#].[^\s]*$)", std::regex::icase);
    wxString outUrl;
    bool success = true;
    while (true) {
        if (dlg.ShowModal() != wxID_OK)
            break;

        wxString url = dlg.GetValue().Trim().Trim(false);

        if (std::regex_match(std::string(url.mb_str()), pattern)) {
            outUrl = url;
            success = true;
            break;
        }
        else {
            wxMessageBox(_t("Invalid URL") + "\n" + _t("Please enter URL again"),
                         _t("Error"),
                         wxOK | wxICON_ERROR,
                         this);
        }
    }

    if (success && !outUrl.IsEmpty()) {
        std::string ext = "";
        wxFileName resDir = mmPath::getPathUserRaw(mmPath::USERICONS, true);
        if (resDir.IsOk()) {
            wxFileName path(resDir.GetPathWithSep(), iconExtractServerName(outUrl));
            std::string iconFileName = path.GetFullPath().utf8_string();
            if (iconDownloadFavicon(outUrl, iconFileName, ext)) {
                LoadIcons();
                m_selectedFile = path.GetFullPath();
                UpdatePreview(m_selectedFile);
            }
            else {
                wxMessageBox(_t("Could not download Favicon!"),
                         _t("Error"),
                         wxOK | wxICON_ERROR,
                         this);
            }
        }
    }
}

// -------------------------
void IconManagerDialog::AddFiles(const wxArrayString& files)
{
    wxString setFile = "";
    for (auto& f : files) {
        wxFileName src(f);
        wxFileName dst(m_dir, src.GetFullName());
        if (verifyFile(src.GetFullPath())) {
            wxCopyFile(src.GetFullPath(), dst.GetFullPath());
            setFile = dst.GetFullPath();
        }
        else {
            wxMessageBox(_t("Icon can not be added!") + '\n' +_t("Invalid icon file format"),
                         _t("Error"),
                         wxOK | wxICON_ERROR,
                         this);
        }
        NavTreeIconImages::instance().setChanged();
    }
    LoadIcons();
    if (!setFile.IsEmpty()) {
        m_selectedFile = setFile;
        UpdatePreview(m_selectedFile);
    }
}

// -------------------------
void IconManagerDialog::OnDelete(wxCommandEvent&)
{
    if (m_selectedFile.IsEmpty()) return;

    if (wxMessageBox("Delete selected icon?", "Confirm",
                     wxYES_NO | wxICON_WARNING) == wxYES) {
        wxRemoveFile(m_selectedFile);
        m_selectedFile.clear();
        NavTreeIconImages::instance().setChanged();
        LoadIcons();
    }
}

// -------------------------
void IconManagerDialog::OnRename(wxCommandEvent&)
{
    if (m_selectedFile.IsEmpty()) return;

    wxFileName fn(m_selectedFile);

    wxTextEntryDialog dlg(this, "New name:", "Rename", fn.GetFullName());
    if (dlg.ShowModal() == wxID_OK) {
        wxFileName newFn(m_dir, dlg.GetValue());

        if (wxRenameFile(fn.GetFullPath(), newFn.GetFullPath())) {
            NavTreeIconImages::instance().setChanged();
        }
        else {
            wxLogDebug("Rename failed");
            return;
        }

        LoadIcons();
    }
}

// -------------------------
// Context menu
// -------------------------
void IconManagerDialog::OnRightClick(wxMouseEvent& WXUNUSED(event), const wxString& path)
{
    m_selectedFile = path;

    wxMenu menu;
    menu.Append(ID_CTX_RENAME, "Rename");
    menu.Append(ID_CTX_DELETE, "Delete");

    PopupMenu(&menu);
}
