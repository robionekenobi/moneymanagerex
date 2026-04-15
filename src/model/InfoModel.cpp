/*******************************************************
 Copyright (C) 2013,2014 Guan Lisheng (guanlisheng@gmail.com)
 Copyright (C) 2022 Mark Whalley (mark@ipx.co.uk)
 Copyright (C) 2025 Klaus Wich
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

#include "base/_constants.h"
#include "util/_util.h"

#include "InfoModel.h"

// -- constructor

// Initialize the global InfoModel.
// Reset the InfoModel or create the table if it does not exist.
InfoModel& InfoModel::instance(wxSQLite3Database* db)
{
    InfoModel& ins = Singleton<InfoModel>::instance();
    ins.reset_cache();
    ins.m_db = db;
    ins.ensure_table();
    ins.preload_cache();

    if (!ins.contains("MMEXVERSION")) {
        ins.saveString("MMEXVERSION", mmex::version::string);
        ins.saveString("DATAVERSION", mmex::DATAVERSION);
        ins.setDate("CREATEDATE", mmDate::today());
        ins.saveString("DATEFORMAT", mmex::DEFDATEFORMAT);
    }
    return ins;
}

// Return the static instance of InfoModel
InfoModel& InfoModel::instance()
{
    return Singleton<InfoModel>::instance();
}

// -- methods

// Returns true if key setting found
bool InfoModel::contains(const wxString& key)
{
    return (find_count(
        InfoCol::WHERE_INFONAME(OP_EQ, key)
    ) > 0);
}

// Raw (the raw value stored in Infotable is always string)
void InfoModel::saveRaw(const wxString& key, const wxString& newValue)
{
    // search in cache
    const Data* info_n = search_cache_n(InfoCol::INFONAME(key));
    if (!info_n) {
        // not found in cache; search in db
        for (int64 info_id : find_id_a(
            InfoCol::WHERE_INFONAME(OP_EQ, key)
        )) {
            info_n = get_id_data_n(info_id);
            break;
        }
    }

    Data info_d = info_n ? *info_n : Data();
    if (!info_n)
        info_d.m_name = key;
    info_d.m_value = newValue;
    save_data_n(info_d);
}

wxString InfoModel::getRaw(const wxString& key, const wxString& defaultValue)
{
    // search in cache
    const Data* info_n = search_cache_n(InfoCol::INFONAME(key));
    if (info_n)
        return info_n->m_value;

    // search in db
    for (const Data& info_d : find_data_a(
        InfoCol::WHERE_INFONAME(OP_EQ, key)
    )) {
        return info_d.m_value;
    }

    // not found
    return defaultValue;
}

// String
void InfoModel::saveString(const wxString& key, const wxString& newValue)
{
    saveRaw(key, newValue);
}
wxString InfoModel::getString(const wxString& key, const wxString& defaultValue)
{
    return getRaw(key, defaultValue);
}

// Bool
void InfoModel::saveBool(const wxString& key, bool newValue)
{
    saveRaw(key, wxString::Format("%s", newValue ? "TRUE" : "FALSE"));
}
bool InfoModel::getBool(const wxString& key, bool defaultValue)
{
    const wxString rawValue = getRaw(key, "");
    if (rawValue == "1" || rawValue.CmpNoCase("TRUE") == 0)
        return true;
    else if (rawValue == "0" || rawValue.CmpNoCase("FALSE") == 0)
        return false;
    else
        return defaultValue;
}

// Int
void InfoModel::saveInt(const wxString& key, int newValue)
{
    saveRaw(key, wxString::Format("%d", newValue));
}
int InfoModel::getInt(const wxString& key, int defaultValue)
{
    const wxString rawValue = getRaw(key, "");
    if (!rawValue.IsEmpty() && rawValue.IsNumber())
        return wxAtoi(rawValue);
    return defaultValue;
}

// Int64
void InfoModel::saveInt64(const wxString& key, int64 newValue)
{
    saveRaw(key, wxString::Format("%lld", newValue));
}
int64 InfoModel::getInt64(const wxString& key, int64 defaultValue)
{
    const wxString rawValue = getString(key, "");
    if (!rawValue.IsEmpty() && rawValue.IsNumber())
        return int64(wxAtol(rawValue));
    return defaultValue;
}

// Size
void InfoModel::saveSize(const wxString& key, const wxSize& newValue)
{
    saveRaw(key, wxString::Format("%i,%i", newValue.GetWidth(), newValue.GetHeight()));
}
const wxSize InfoModel::getSize(const wxString& key)
{
    const wxString rawValue = getRaw(key, "");
    if (!rawValue.IsEmpty()) {
        wxRegEx pattern("^([0-9]+),([0-9]+)$");
        if (pattern.Matches(rawValue)) {
            const auto& x = pattern.GetMatch(rawValue, 1);
            const auto& y = pattern.GetMatch(rawValue, 2);
            return wxSize(wxAtoi(x), wxAtoi(y));
        }
    }
    return wxDefaultSize;
}

// Colour
void InfoModel::saveColour(const wxString& key, const wxColour& newValue)
{
    saveRaw(key, wxString::Format("%d,%d,%d",
        newValue.Red(), newValue.Green(), newValue.Blue()
    ));
}
const wxColour InfoModel::getColour(const wxString& key, const wxColour& defaultValue)
{
    const wxString rawValue = getRaw(key, "");
    if (!rawValue.IsEmpty()) {
        wxRegEx pattern("([0-9]{1,3}),([0-9]{1,3}),([0-9]{1,3})");
        if (pattern.Matches(rawValue)) {
            const wxString r = pattern.GetMatch(rawValue, 1);
            const wxString g = pattern.GetMatch(rawValue, 2);
            const wxString b = pattern.GetMatch(rawValue, 3);
            return wxColour(wxAtoi(r), wxAtoi(g), wxAtoi(b));
        }
        else {
            return wxColour(rawValue);
        }
    }
    return defaultValue;
}

// Date
void InfoModel::setDate(const wxString& key, const mmDate& newValue)
{
    saveRaw(key, newValue.isoDate());
}

//-------------------------------------------------------------------
// Jdoc
void InfoModel::saveJdoc(const wxString& key, Document& newValue)
{
    wxString j_str = JSON_PrettyFormated(newValue);
    saveRaw(key, j_str);
}
void InfoModel::saveJdoc(const wxString& key, StringBuffer& newValue)
{
    wxString j_str = wxString::FromUTF8(newValue.GetString());
    saveRaw(key, j_str);
}
Document InfoModel::getJdoc(const wxString& key, const wxString& defaultValue)
{
    Document j_doc;
    wxString j_str = getRaw(key, defaultValue);
    j_doc.Parse(j_str.utf8_str());
    if (j_doc.HasParseError()) {
        j_doc.Parse(defaultValue.utf8_str());
    }
    return j_doc;
}


// ArrayString
void InfoModel::saveArrayString(const wxString& key, const wxArrayString& a)
{
    StringBuffer json_buffer;
    PrettyWriter<StringBuffer> json_writer(json_buffer);
    json_writer.StartArray();
    for (const auto& value : a) {
        json_writer.String(value.utf8_str());
    }
    json_writer.EndArray();
    const wxString& json_string = wxString::FromUTF8(json_buffer.GetString());
    saveRaw(key, json_string);
    wxLogDebug("InfoModel::saveArrayString(%s): %s", key, json_string);
}

const wxArrayString InfoModel::getArrayString(const wxString& key, bool sort)
{
    wxString rawValue = getRaw(key, "");
    Document j_doc;
    if (rawValue.IsEmpty() ||
        j_doc.Parse(rawValue.utf8_str()).HasParseError() ||
        !j_doc.IsArray()
    )
        return wxArrayString();

    wxArrayString a;
    wxLogDebug("{{{ InfoModel::getArrayString(%s)", key);
    for (rapidjson::SizeType i = 0; i < j_doc.Size(); i++) {
        wxASSERT(j_doc[i].IsString());
        const auto value = wxString::FromUTF8(j_doc[i].GetString());
        wxLogDebug("%s", value);
        a.Add(value);
    }
    wxLogDebug("}}}");

    // Crude sort of JSON (case sensitive), could be improved by actually sorting by a field
    // but this should be sufficient if you want to sort by first element
    if (sort)
        a.Sort();
    return a;
}

// Search through a set of JSON data for a particular label
int InfoModel::findArrayItem(const wxString& key, const wxString& label)
{
    // Important: do not sort
    wxArrayString a = InfoModel::instance().getArrayString(key);
    int i = 0;
    for (const auto& data : a) {
        Document j_doc;
        if (j_doc.Parse(data.utf8_str()).HasParseError())
            j_doc.Parse("{}");
        Value& j_label = GetValueByPointerWithDefault(j_doc, "/LABEL", "");
        const wxString& s_label = j_label.IsString() ? wxString::FromUTF8(j_label.GetString()) : "";
        if (s_label == label)
            return i;
        ++i;
    }
    return wxNOT_FOUND;
}

void InfoModel::updateArrayItem(const wxString& key, int i, const wxString& newValue)
{
    wxString rawValue = getRaw(key, "");
    Document j_doc;
    if (rawValue.IsEmpty() ||
        j_doc.Parse(rawValue.utf8_str()).HasParseError() ||
        !j_doc.IsArray()
    )
        return;

    StringBuffer json_buffer;
    PrettyWriter<StringBuffer> json_writer(json_buffer);
    json_writer.StartArray();
    for (SizeType j = 0; j < j_doc.Size(); j++) {
        json_writer.String(
            (i == static_cast<int>(j)) ? newValue.utf8_str() : j_doc[j].GetString()
        );
    }
    json_writer.EndArray();
    const wxString& json_string = wxString::FromUTF8(json_buffer.GetString());

    saveRaw(key, json_string);
    wxLogDebug("InfoModel::updateArrayItem(%s, %d): %s", key, i, json_string);
}

void InfoModel::prependArrayItem(const wxString& key, const wxString& value, int limit)
{
    const Data* info_n = search_cache_n(InfoCol::INFONAME(key));
    if (!info_n) {
        for (int64 info_id : find_id_a(
            InfoCol::WHERE_INFONAME(OP_EQ, key)
        )) {
            info_n = get_id_data_n(info_id);
        }
    }

    Data info_d = info_n ? *info_n : Data();
    if (!info_n)
        info_d.m_name = key;

    wxArrayString a;
    if (!value.empty() && limit != 0)
        a.Add(value);

    Document j_doc;
    if (!j_doc.Parse(info_d.m_value.utf8_str()).HasParseError()
        && j_doc.IsArray()
    ) {
        int i = 1;
        for (auto& v : j_doc.GetArray()) {
            if (i >= limit && limit != -1) break;
            if (v.IsString()) {
                const auto item = wxString::FromUTF8(v.GetString());
                if (a.Index(item) == wxNOT_FOUND) {
                    a.Add(item);
                    i++;
                }
            }
        }
    }

    StringBuffer json_buffer;
    PrettyWriter<StringBuffer> json_writer(json_buffer);
    json_writer.StartArray();
    for (const auto& entry : a) {
        json_writer.String(entry.utf8_str());
    }
    json_writer.EndArray();

    info_d.m_value = wxString::FromUTF8(json_buffer.GetString());
    save_data_n(info_d);
}

void InfoModel::eraseArrayItem(const wxString& key, int i)
{
    wxString rawValue = getRaw(key, "");
    Document j_doc;
    if (rawValue.IsEmpty() ||
        j_doc.Parse(rawValue.utf8_str()).HasParseError() ||
        !j_doc.IsArray()
    )
        return;

    j_doc.Erase(j_doc.Begin() + i);
    StringBuffer json_buffer;
    PrettyWriter<StringBuffer> json_writer(json_buffer);
    j_doc.Accept(json_writer);

    const wxString json_string = wxString::FromUTF8(json_buffer.GetString());
    saveRaw(key, json_string);
    wxLogDebug("InfoModel::eraseArrayItem(%s, %d): %s", key, i, json_string);
}

//-------------------------------------------------------------------
// CUSTOMDIALOG_OPEN
void InfoModel::saveOpenCustomDialog(const wxString& refType, bool newValue)
{
    saveBool("CUSTOMDIALOG_OPEN:" + refType, newValue);
}
bool InfoModel::getOpenCustomDialog(const wxString& refType)
{
    return getBool("CUSTOMDIALOG_OPEN:" + refType, false);
}

// CUSTOMDIALOG_SIZE
void InfoModel::saveCustomDialogSize(const wxString& refType, const wxSize& newValue)
{
    wxString rawValue;
    rawValue << newValue.GetWidth() << ";" << newValue.GetHeight();
    saveRaw("CUSTOMDIALOG_SIZE:" + refType, rawValue);
}
wxSize InfoModel::getCustomDialogSize(const wxString& refType)
{
    wxString rawValue = getRaw("CUSTOMDIALOG_SIZE:" + refType, "0;0");
    return wxSize(wxAtoi(rawValue.BeforeFirst(';')), wxAtoi(rawValue.AfterFirst(';')));
}

//-------------------------------------------------------------------
bool InfoModel::checkDBVersion()
{
    if (!contains("DATAVERSION"))
        return false;
    return getInt("DATAVERSION", 0) >= mmex::MIN_DATAVERSION;
}

loop_t InfoModel::to_loop_t()
{
    loop_t loop;
    for (const auto& info_d : find_data_a(
        TableClause::ORDERBY(InfoCol::s_primary_name)
    ))
        loop += info_d.to_html_row();
    return loop;
}

//--- static support functions ---------------------------------------------
void InfoModel::saveFilterString(Document &sdoc, const char* skey, wxString svalue)
{
    if (sdoc.HasMember(skey)) {
        sdoc[skey].SetString(svalue.utf8_str(), sdoc.GetAllocator());
    }
    else {
        auto& allocator = sdoc.GetAllocator();
        rapidjson::Value field_key(skey, allocator);
        rapidjson::Value value(svalue.utf8_str(), allocator);
        sdoc.AddMember(field_key, value, allocator);
    }
};

void InfoModel::saveFilterBool(Document &sdoc, const char* skey, bool bvalue)
{
    if (sdoc.HasMember(skey)) {
        sdoc[skey].SetBool(bvalue);
    }
    else {
        auto& allocator = sdoc.GetAllocator();
        rapidjson::Value field_key(skey, allocator);
        sdoc.AddMember(field_key, bvalue, allocator);
    }
};

void InfoModel::saveFilterInt(Document &sdoc, const char* skey, int ivalue)
{
    if (sdoc.HasMember(skey)) {
        sdoc[skey].SetInt(ivalue);
    }
    else {
        auto& allocator = sdoc.GetAllocator();
        rapidjson::Value field_key(skey, allocator);
        sdoc.AddMember(field_key, ivalue, allocator);
    }
};
