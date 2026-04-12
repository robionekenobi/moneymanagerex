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

#include <curl/curl.h>
#include <iostream>
#include <fstream>
#include <regex>
#include <filesystem>
#include <wx/string.h>

//#include "base/paths.h"
#include "base/platfdep.h"
#include "util/_icon.h"
#include "base/images_list.h"


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

// HTTP GET → String
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

// Basis-URL extrahieren (https://example.com)
std::string _getBaseUrl(const std::string& url)
{
    std::regex re(R"(^(https?:\/\/[^\/]+))");
    std::smatch match;

    if (std::regex_search(url, match, re))
        return match[1];

    return "";
}

// Relative URL → Absolute URL
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
    //if (url.find(".ico") != std::string::npos) return ".ico";

    return ".ico"; // fallback
}


bool _cleanPngFile(const std::string& filePath)
{
    wxImage image;
    if (!image.LoadFile(filePath))
        return false;

    if (!image.HasAlpha())
        image.InitAlpha();

    image.SetOption(wxIMAGE_OPTION_PNG_COMPRESSION_LEVEL, 9);

    return image.SaveFile(filePath, wxBITMAP_TYPE_PNG);
}


// ---- Exported functions -----------------------------------
bool iconDownloadFavicon(const std::string& websiteUrl, std::string& outputName, std::string& ext)
{
    std::string html;
    if (!_httpGet(websiteUrl, html)) {
        wxLogDebug("Download Fav Icon: could not download HTML page");
        return false;
    }

    std::string base = _getBaseUrl(websiteUrl);
    std::string iconUrl = _extractFaviconUrl(html);

    if (!iconUrl.empty()) {
        iconUrl = _makeAbsolute(base, iconUrl);
    }
    else {
        iconUrl = base + "/favicon.ico";         // fallback
    }

    ext = _detectExtension(iconUrl);
    outputName += ext;

    if (!_downloadFile(iconUrl, outputName)) {
        wxLogDebug("Download Fav Icon: could not load Fav Icon");
        return false;
    }

    if (ext == ".png") {
        if (!_cleanPngFile(outputName)) {
            wxLogDebug("Download Fav Icon: could not cleanup PNG: %s", outputName);
        }
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
    wxImage image;
    if (!image.LoadFile(filePath)) {
        wxLogError("Could not load Icon: %s", filePath);
        return false;
    }

    // Optional: Bild auf Button-Größe skalieren
    wxSize btnSize = button->GetSize();
    image = image.Scale(btnSize.GetWidth(), btnSize.GetHeight(), wxIMAGE_QUALITY_HIGH);

    wxBitmap bitmap(image);

    button->SetBitmapMargins(2, 2);
    button->SetBitmap(bitmap);

    return true;
}


bool iconLoadBitmapBundleFromDirectory(wxVector<wxBitmapBundle>& images, std::map<int, wxString> & index, std::map<wxString, int> & revindex, const wxString& dirPath, const int size)
{
    wxDir dir(dirPath);
    if (!dir.IsOpened()) {
        wxLogDebug("Icon directory could not be opened: %s", dirPath);
        return false;
    }

    wxString filename;
    bool cont = dir.GetFirst(&filename);
    index = {};
    revindex = {};
    int bidx = acc_img::MAX_ACC_ICON;
    while (cont) {
        wxFileName fullPath(dirPath, filename);
        if (fullPath.FileExists()) {
            wxImage image;
            if (image.LoadFile(fullPath.GetFullPath())) {
                wxImage img = image.Scale(size, size, wxIMAGE_QUALITY_HIGH);
                images.emplace_back(wxBitmap(img));
                index[bidx] = filename;
                revindex[filename] = bidx++;
            }
        }
        cont = dir.GetNext(&filename);
    }
    return true;
}


bool iconGetFavIconResPath(wxFileName& resDir, bool create)
{
    resDir = mmex::GetResourceDir();
    if (create && !resDir.DirExists()) {
        if (!resDir.Mkdir()) {
            wxLogDebug("Could not create Resource Directory '%s'", resDir.GetPathWithSep());
            return false;
        }
    }
    if (resDir.AppendDir("favicons")) {
        if (create && !resDir.DirExists()) {
            if (!resDir.Mkdir()) {
                wxLogDebug("Could not create Icon Directory '%s'", resDir.GetPathWithSep());
                return false;
            }
        }
    }
    else {
        wxLogDebug("Could not add sub dir 'favicons' in Resource Directory '%s'", resDir.GetPathWithSep());
        return false;
    }
    if (create && !resDir.IsDirWritable()) {
        wxLogDebug("Directory '%s' is not writable , can not store favicons", resDir.GetPathWithSep());
        return false;
    }

    return true;
}
