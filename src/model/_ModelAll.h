/*******************************************************
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

#pragma once

// ModelAll contains methods which span across different ${Table}Model.
// These methods could be added in one of the individual ${Table}Model;
// collecting them in a separate class slightly improves readability.
//
// ModelAll could be stateless, and all its method could be static.
// A stateful design, containing a database handle m_db, is preferred here.
// As in other ${Table}Model classes, all methods depending on m_db are defined
// as member methods, and they are accessed through ModelAll::instance().
// This allows a future extension, in which the application can open
// more than database simultaneously.

#include "base/_defs.h"
#include "base/mmSingleton.h"
#include "data/_DataEnum.h"

class ModelAll
{
// -- state

private:
    wxSQLite3Database* m_db;

// -- constructor

public:
    ModelAll() {}
    ~ModelAll() {}

public:
    static ModelAll& instance(wxSQLite3Database* db);
    static ModelAll& instance();

// -- methods

public:
    auto find_ref_count(
        RefTypeN ref_type, int64 ref_id,
        bool ignore_deleted = false
    ) -> std::size_t;
};
