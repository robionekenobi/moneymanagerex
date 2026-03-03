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

// -- mmChoiceNameA --

mmChoiceIdN mmChoiceNameA::valid_id_n(mmChoiceIdN id_n) const
{
    wxASSERT(
        (id_n >= 0 && id_n < static_cast<mmChoiceId>(m_choice_a.size())) ||
        id_n == m_default_id_n
    );
    return id_n;
}

const wxString mmChoiceNameA::get_name(mmChoiceId id) const
{
    wxASSERT(id >= 0 && id < static_cast<mmChoiceId>(m_choice_a.size()));
    wxASSERT(m_choice_a[id].id == id);
    return m_choice_a[id].name;
}

mmChoiceIdN mmChoiceNameA::find_name_n(const wxString& name)
{
    if (const auto it = m_index_m.find(name); it != m_index_m.end())
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
    m_index_m.insert({name, id_n});
    return id_n;
}

// -- mmChoiceKeyNameA --

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

mmChoiceIdN mmChoiceKeyNameA::find_keyname_n(const wxString& keyname)
{
    if (const auto it = m_index_m.find(keyname); it != m_index_m.end())
        return it->second;

    mmChoiceIdN id_n = m_default_id_n;
    for (const Choice& choice : m_choice_a) {
        bool match = m_nocase
            ? (keyname.CmpNoCase(choice.key) == 0 || keyname.CmpNoCase(choice.name) == 0)
            : (keyname == choice.key || keyname == choice.name);
        if (match) {
            id_n = choice.id;
            break;
        }
    }
    m_index_m.insert({keyname, id_n});
    return id_n;
}
