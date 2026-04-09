/*******************************************************
 Copyright (C) 2025 George Ef (george.a.ef@gmail.com)

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

#include "base/_defs.h"
#include "base/_types.h"

// TableClause represents a query clause, e.g., a WHERE term in a select query.
// It is the base for two derived types and cannot be instantiated directly.
// TableClauseD represents a clause without a placeholder, while the template
// type TableClauseV<V> represents a clause with one or more placeholders,
// which are all bound to the same value of a parametric type V.
// The base type contains a type (e.g., a _RESULT clause represents a term
// in the SELECT part of the query, a _WHERE clause in the WHERE part, etc.),
// the text of the clause, which may contain `?` placeholders, and a multiplicity,
// which must be equal to the number of `?` placeholders in the text.
// TableClauseV<V> contains a value of type V, used to bind all placeholders
// in its text. To simplify code in template methods, TableClauseD also contains
// a dummy int value, which is declared but not used.
//
// A _PAREN clause represents an open or close parenthesis in a boolean expression.
// It is used to construct composite _WHERE clauses. The text in an open _PAREN
// clause is the logical operator which combines the following _WHERE terms
// (until the next close _PAREN clause), while the text in a close _PAREN is empty.

// OP represents an SQLite operator.
// It is used in TableClause::WHERE() to construct a _WHERE clause.
enum OP {
    OP_EQ = 0, // EQual
    OP_NE,     // Not Equal
    OP_GT,     // Greater Than
    OP_LT,     // Less Than
    OP_GE,     // Greater or Equal
    OP_LE,     // Less or Equal
    OP_EQN,    // EQual to Null or argument
    OP_NEN,    // Not (Equal to Null or argument)
};

// deprecated
template<typename V>
struct TableOpV
{
    OP m_operator;
    V m_value;

    TableOpV(OP op, const V& v): m_operator(op), m_value(v) {}
};

// forward declarations
struct TableClauseD;
template<typename V>
struct TableClauseV;

struct TableClause
{
    friend struct TableClauseD;
    template<typename V>
    friend struct TableClauseV;

public:
    enum CLAUSE_ID
    {
        CLAUSE_ID_RESULT, // m_text: result
        CLAUSE_ID_WHERE,  // m_text: boolean_expr; m_mult: may be > 0
        CLAUSE_ID_PAREN,  // m_text: ("AND" | "OR" | "")
        CLAUSE_ID_GROUP,  // m_text: result
        CLAUSE_ID_ORDER,  // m_text: result (| ASC | DESC)
        CLAUSE_ID_LIMIT,  // m_text: integer (| "OFFSET" integer)
        CLAUSE_ID_size
    };

    static CLAUSE_ID collate_id(CLAUSE_ID id) {
        return id == CLAUSE_ID_PAREN ? CLAUSE_ID_WHERE : id;
    }

// -- state

public:
    CLAUSE_ID m_id;  // clause type
    wxString m_text; // text to be copied in query; may contain ? placeholders
    int m_mult;      // number of ? placeholders in m_text

// -- constructor

private:
    TableClause(CLAUSE_ID id, const wxString& text = "", int mult = 0) :
        m_id(id), m_text(text), m_mult(mult) {}
    TableClause(const TableClause& other) { *this = other; }

public:
    static auto RESULT(const wxString& result) -> TableClauseD;
    static auto WHERE(const wxString& expr) -> TableClauseD;
    static auto BEGIN_AND() -> TableClauseD;
    static auto BEGIN_OR() -> TableClauseD;
    static auto END() -> TableClauseD;
    static auto GROUPBY(const wxString& result) -> TableClauseD;
    static auto ORDERBY(const wxString& result, bool desc = false) -> TableClauseD;
    static auto LIMIT(int limit, int offset = 0) -> TableClauseD;

    template<typename V>
    static auto WHERE(const wxString& col, OP op, const V& value) -> TableClauseV<V>;

// -- methods

public:
    static auto where_op(const wxString& col, OP op) -> TableClause;

    static auto eval(const TableClauseV<wxString>& clause) -> TableClauseD;
    static auto eval(const TableClauseV<int>& clause) -> TableClauseD;
    static auto eval(const TableClauseV<int64>& clause) -> TableClauseD;
    static auto eval(const TableClauseV<double>& clause) -> TableClauseD;

    static auto merge(const std::vector<const TableClause*>& clause_na) -> TableClause;
    template<typename... Args>
    static auto merge(const TableClause& arg1, const Args&... argsD) -> TableClause;
    template<typename... Args>
    static auto merge(const TableClauseD& arg1D, const Args&... argsD) -> TableClauseD;
    template<typename V, typename... Args>
    static auto merge(const TableClauseV<V>& arg1V, const Args&... argsD) -> TableClauseV<V>;
};

struct TableClauseD : public TableClause
{
public:
    int m_value = 0; // dummy bind value (not used)

public:
    TableClauseD(TableClause::CLAUSE_ID id, const wxString& text) :
        TableClause(id, text) {}
    TableClauseD(const TableClause& other) :
        TableClause(other) {}
};

template<typename V>
struct TableClauseV : public TableClause
{
public:
    V m_value; // bind value for each placeholder in m_text

public:
    TableClauseV(TableClause::CLAUSE_ID id, const wxString& text, int mult, const V& value) :
        TableClause(id, text, mult), m_value(value) {}
    TableClauseV(const TableClause& other, const V& value) :
        TableClause(other), m_value(value) {}
};

// -- template

template<typename V>
TableClauseV<V> TableClause::WHERE(const wxString& col, OP op, const V& value)
{
    return TableClauseV<V>(where_op(col, op), value);
}

template<typename... Args>
TableClause TableClause::merge(const TableClause& arg1, const Args&... argsD)
{
    static_assert(
        (std::is_same_v<Args, TableClauseD> && ...),
        "Args must be TableClauseD"
    );
    return merge({&arg1, &argsD...});
}

template<typename... Args>
TableClauseD TableClause::merge(const TableClauseD& arg1D, const Args&... argsD)
{
    static_assert(
        (std::is_same_v<Args, TableClauseD> && ...),
        "Args must be TableClauseD"
    );
    return TableClauseD(merge({&arg1D, &argsD...}));
}

template<typename V, typename... Args>
TableClauseV<V> TableClause::merge(const TableClauseV<V>& arg1V, const Args&... argsD)
{
    static_assert(
        (std::is_same_v<Args, TableClauseD> && ...),
        "Args must be TableClauseD"
    );
    return TableClauseV<V>(merge({&arg1V, &argsD...}), arg1V.m_value);
}
