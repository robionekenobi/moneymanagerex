/*******************************************************
 Copyright (C) 2025-2026 George Ef (george.a.ef@gmail.com)

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

#include "mmChoice.h"

// -- mmChoiceKeyA

mmChoiceIdN mmChoiceKeyA::valid_id_n(mmChoiceIdN id_n) const
{
    wxASSERT(
        (id_n >= 0 && id_n < static_cast<mmChoiceId>(m_choice_a.size())) ||
        id_n == m_default_id_n
    );
    return id_n;
}

const wxString mmChoiceKeyA::get_key(mmChoiceId id) const
{
    wxASSERT(id >= 0 && id < static_cast<mmChoiceId>(m_choice_a.size()));
    wxASSERT(m_choice_a[id].id == id);
    return m_choice_a[id].key;
}

mmChoiceIdN mmChoiceKeyA::find_key_n(const wxString& key)
{
    if (const auto it = m_key_id_m.find(key); it != m_key_id_m.end())
        return it->second;

    mmChoiceIdN id_n = m_default_id_n;
    for (const Choice& choice : m_choice_a) {
        bool match = m_nocase
            ? (key.CmpNoCase(choice.key) == 0)
            : (key == choice.key);
        if (match) {
            id_n = choice.id;
            break;
        }
    }
    m_key_id_m.insert({key, id_n});
    return id_n;
}

// -- mmChoiceKeyNameA

mmChoiceIdN mmChoiceKeyNameA::valid_id_n(mmChoiceIdN id_n) const
{
    wxASSERT(
        (id_n >= 0 && id_n < static_cast<mmChoiceId>(m_choice_a.size())) ||
        id_n == m_default_id_n
    );
    return id_n;
}

const wxString mmChoiceKeyNameA::get_key(mmChoiceId id) const
{
    wxASSERT(id >= 0 && id < static_cast<mmChoiceId>(m_choice_a.size()));
    wxASSERT(m_choice_a[id].id == id);
    return m_choice_a[id].key;
}

const wxString mmChoiceKeyNameA::get_name(mmChoiceId id) const
{
    wxASSERT(id >= 0 && id < static_cast<mmChoiceId>(m_choice_a.size()));
    wxASSERT(m_choice_a[id].id == id);
    return m_choice_a[id].name;
}

mmChoiceIdN mmChoiceKeyNameA::find_key_n(const wxString& key)
{
    if (const auto it = m_key_id_m.find(key); it != m_key_id_m.end())
        return it->second;

    mmChoiceIdN id_n = m_default_id_n;
    for (const Choice& choice : m_choice_a) {
        bool match = m_nocase
            ? (key.CmpNoCase(choice.key) == 0)
            : (key == choice.key);
        if (match) {
            id_n = choice.id;
            break;
        }
    }
    m_key_id_m.insert({key, id_n});
    return id_n;
}

// -- mmChoiceKeyKeyNameA

mmChoiceIdN mmChoiceKeyKeyNameA::valid_id_n(mmChoiceIdN id_n) const
{
    wxASSERT(
        (id_n >= 0 && id_n < static_cast<mmChoiceId>(m_choice_a.size())) ||
        id_n == m_default_id_n
    );
    return id_n;
}

const wxString mmChoiceKeyKeyNameA::get_key(mmChoiceId id) const
{
    wxASSERT(id >= 0 && id < static_cast<mmChoiceId>(m_choice_a.size()));
    wxASSERT(m_choice_a[id].id == id);
    return m_choice_a[id].key;
}

const wxString mmChoiceKeyKeyNameA::get_name(mmChoiceId id) const
{
    wxASSERT(id >= 0 && id < static_cast<mmChoiceId>(m_choice_a.size()));
    wxASSERT(m_choice_a[id].id == id);
    return m_choice_a[id].name;
}

mmChoiceIdN mmChoiceKeyKeyNameA::find_key_n(const wxString& key)
{
    if (const auto it = m_key_id_m.find(key); it != m_key_id_m.end())
        return it->second;

    mmChoiceIdN id_n = m_default_id_n;
    for (const Choice& choice : m_choice_a) {
        bool match = m_nocase
            ? (key.CmpNoCase(choice.key) == 0 || key.CmpNoCase(choice.key2) == 0)
            : (key == choice.key || key == choice.key2);
        if (match) {
            id_n = choice.id;
            break;
        }
    }
    m_key_id_m.insert({key, id_n});
    return id_n;
}

// -- mmChoiceCodeNameA

mmChoiceIdN mmChoiceCodeNameA::valid_id_n(mmChoiceIdN id_n) const
{
    wxASSERT(
        (id_n >= 0 && id_n < static_cast<mmChoiceId>(m_choice_a.size())) ||
        id_n == m_default_id_n
    );
    return id_n;
}

int mmChoiceCodeNameA::get_code(mmChoiceId id) const
{
    wxASSERT(id >= 0 && id < static_cast<mmChoiceId>(m_choice_a.size()));
    wxASSERT(m_choice_a[id].id == id);
    return m_choice_a[id].code;
}

const wxString mmChoiceCodeNameA::get_name(mmChoiceId id) const
{
    wxASSERT(id >= 0 && id < static_cast<mmChoiceId>(m_choice_a.size()));
    wxASSERT(m_choice_a[id].id == id);
    return m_choice_a[id].name;
}

mmChoiceIdN mmChoiceCodeNameA::find_code_n(int code)
{
    if (const auto it = m_code_id_m.find(code); it != m_code_id_m.end())
        return it->second;

    mmChoiceIdN id_n = m_default_id_n;
    for (const Choice& choice : m_choice_a) {
        if (code == choice.code) {
            id_n = choice.id;
            break;
        }
    }
    m_code_id_m.insert({code, id_n});
    return id_n;
}

mmChoiceIdN mmChoiceCodeNameA::find_name_n(const wxString& name)
{
    if (const auto it = m_name_id_m.find(name); it != m_name_id_m.end())
        return it->second;

    mmChoiceIdN id_n = m_default_id_n;
    for (const Choice& choice : m_choice_a) {
        bool match = m_nocase
            ? (name.CmpNoCase(choice.name) == 0)
            : (name == choice.name);
        if (match) {
            id_n = choice.id;
            break;
        }
    }
    m_name_id_m.insert({name, id_n});
    return id_n;
}
