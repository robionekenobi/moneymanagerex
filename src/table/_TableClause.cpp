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

#include "_TableClause.h"

#include <vector>

// -- constructor

TableClauseD TableClause::RESULT(const wxString& result) {
    return TableClauseD(CLAUSE_ID_RESULT, result);
}

TableClauseD TableClause::WHERE(const wxString& expr) {
    return TableClauseD(CLAUSE_ID_WHERE, expr);
}

TableClauseD TableClause::BEGIN_AND() {
    return TableClauseD(CLAUSE_ID_PAREN, "AND");
}
TableClauseD TableClause::BEGIN_OR() {
    return TableClauseD(CLAUSE_ID_PAREN, "OR");
}
TableClauseD TableClause::END() {
    return TableClauseD(CLAUSE_ID_PAREN, "");
}

TableClauseD TableClause::GROUPBY(const wxString& result) {
    return TableClauseD(CLAUSE_ID_GROUP, result);
}

TableClauseD TableClause::ORDERBY(const wxString& result, bool desc) {
    return TableClauseD(CLAUSE_ID_ORDER, result + (desc ? " DESC" : " ASC"));
}

TableClauseD TableClause::LIMIT(int limit, int offset) {
    return TableClauseD(CLAUSE_ID_LIMIT, (offset == 0)
        ? wxString::Format("%d", limit)
        : wxString::Format("%d OFFSET %d", limit, offset)
    );
}

TableClauseD TableClause::EMPTY() {
    return TableClauseD(CLAUSE_ID_EMPTY, "");
}

// -- methods

TableClause TableClause::where_op(const wxString& col, OP op)
{
    wxString expr = "";
    int mult = 0;
    switch (op)
    {
        case OP_EQ:  expr = col +  " = ?"; mult = 1; break;
        case OP_NE:  expr = col + " != ?"; mult = 1; break;
        case OP_GT:  expr = col +  " > ?"; mult = 1; break;
        case OP_LT:  expr = col +  " < ?"; mult = 1; break;
        case OP_GE:  expr = col + " >= ?"; mult = 1; break;
        case OP_LE:  expr = col + " <= ?"; mult = 1; break;
        case OP_EQN: expr = "IFNULL(" + col + ", ?)" +  " = ?"; mult = 2; break;
        case OP_NEN: expr = "IFNULL(" + col + ", ?)" + " != ?"; mult = 2; break;
        case OP_LK:  expr = col + " LIKE ?"; mult = 1; break;
    }
    return TableClause(CLAUSE_ID_WHERE, expr, mult);
}

// The clause value is enclosed in quotes and copied verbatim to each placeholder.
// Use this function only if it is known that the clause value is a small label,
// without quotes or other special characters.
TableClauseD TableClause::eval(const TableClauseV<wxString>& clause)
{
    wxString text = clause.m_text;
    text.Replace("?", wxString::Format("'%s'", clause.m_value), true);
    return TableClauseD(clause.m_id, text);
}

TableClauseD TableClause::eval(const TableClauseV<int>& clause)
{
    wxString text = clause.m_text;
    text.Replace("?", wxString::Format("%d", clause.m_value), true);
    return TableClauseD(clause.m_id, text);
}

TableClauseD TableClause::eval(const TableClauseV<int64>& clause)
{
    wxString text = clause.m_text;
    text.Replace("?", wxString::Format("%lld", clause.m_value.GetValue()), true);
    return TableClauseD(clause.m_id, text);
}

// The clause value is formatted to a string and then copied to each placeholder.
// Depending on rounding errors and the wanted accouracy, this may create
// inconsistencies with floating-point comparisons in SQLite engine.
// Use this function only if precision in not critical, or if it is known
// that the clause value is a round number (e.g., 0.0).
// Floating-point comparisons can anyway be tricky if precision is critical.
TableClauseD TableClause::eval(const TableClauseV<double>& clause)
{
    wxString text = clause.m_text;
    text.Replace("?", wxString::Format("%f", clause.m_value), true);
    return TableClauseD(clause.m_id, text);
}

// This is a helper function used in the implementation of variadic merge().
// Merge the clauses in non-empty vector clause_na and return a single clause.
// All clauses shall be of the same type (_PAREN is coalesced to _WHERE).
// The first clause shall not be void. Other clauses cannot have _VOID type
// (since the first clause is not void), but can be void (with empty text).
// The type of the returned clause is the type of the first clause, after coalescence.
// The multiplicity of the returned clause is the multiplicity of the first clause.
TableClause TableClause::merge(const std::vector<const TableClause*>& clause_na)
{
    if (clause_na.empty()) {
        wxLogError("TableClause::merge: clause_na is empty");
        return TableClause::EMPTY();
    }
    if (clause_na[0]->is_void()) {
        wxLogError("TableClause::merge: clause_na[0] is void");
        return TableClause::EMPTY();
    }

    CLAUSE_ID id = collate_id(clause_na[0]->m_id);
    int mult = clause_na[0]->m_mult;
    wxString text = "";

    bool is_empty = true;
    std::vector<wxString> op_a;
    if (collate_id(id) == CLAUSE_ID_WHERE)
        op_a.push_back("AND");

    for (const TableClause* clause_n : clause_na) {
        const TableClause& clause = *clause_n;
        if (collate_id(clause.m_id) != id) {
            wxLogError("TableClause::merge: cannot merge clauses of different type");
            continue;
        }
        if (clause.is_void())
            continue;

        switch (clause.m_id)
        {
        case CLAUSE_ID_RESULT:
        case CLAUSE_ID_GROUP:
        case CLAUSE_ID_ORDER:
            if (!is_empty)
                text += ", ";
            text += clause.m_text;
            is_empty = false;
            break;
        case CLAUSE_ID_WHERE:
            if (!is_empty)
                text += " " + op_a.back() + " ";
            text += clause.m_text;
            is_empty = false;
            break;
        case CLAUSE_ID_PAREN:
            if (!clause.m_text.IsEmpty()) {
                if (!is_empty)
                    text += " " + op_a.back() + " ";
                text += "(";
                op_a.push_back(clause.m_text);
                is_empty = true;
            }
            else if (op_a.size() > 1) {
                if (is_empty)
                    wxLogError("TableClause::merge: empty condition inside PAREN");
                text += ")";
                op_a.pop_back();
                is_empty = false;
            }
            else {
                wxLogError("TableClause::merge: unexpected END");
                // ignore and continue
            }
            break;
        case CLAUSE_ID_LIMIT:
            if (!is_empty)
                wxLogWarning("TableClause::merge: multiple LIMIT arguments");
            // keep the last
            text = clause.m_text;
            is_empty = false;
            break;
        default:
            break;
        }
    }

    // close PAREN
    if (op_a.size() > 1) {
        wxLogWarning("TableClause::merge: missing END");
        for (std::size_t op_i = 1; op_i < op_a.size(); ++op_i)
            text += ")";
    }

    return TableClause(id, text, mult);
}
