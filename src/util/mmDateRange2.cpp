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

#include "base/_constants.h"
#include "base/_types.h"
#include "base/mmStringBuilder.h"
#include "mmDateRange2.h"

#include "model/PrefModel.h"

const std::vector<mmDatePeriod::IdLabel> mmDatePeriod::s_id_label_a =
{
    { _A, "A" },
    { _Y, "Y" },
    { _Q, "Q" },
    { _M, "M" },
    { _W, "W" },
    { _T, "T" },
    { _S, "S" },
};
const mmDatePeriod::LabelIdM mmDatePeriod::s_label_id_m = makeLabelId();

mmDatePeriod::LabelIdM mmDatePeriod::makeLabelId()
{
    LabelIdM map;
    for (int i = 0; i < static_cast<int>(s_id_label_a.size()); ++i) {
        char c = s_id_label_a[i].label[0];
        map[c] = s_id_label_a[i].id;
    }
    return map;
}

mmDatePeriod::mmDatePeriod(Id id) :
    m_id(id)
{
}

mmDatePeriod::mmDatePeriod(char label)
{
    auto it = s_label_id_m.find(label);
    m_id = (it == s_label_id_m.end()) ? _A : it->second;
}

wxDateSpan mmDatePeriod::span(int offset, mmDatePeriod period)
{
    if (period == _Y)
        return wxDateSpan::Years(offset);
    else if (period == _Q)
        return wxDateSpan::Months(3*offset);
    else if (period == _M)
        return wxDateSpan::Months(offset);
    else if (period == _W)
        return wxDateSpan::Weeks(offset);
    else if (period == _T || period == _S)
        return wxDateSpan::Days(offset);
    else
        return wxDateSpan();
}

mmDateRange2::Range::Range(
    int so1, mmDatePeriod  sp1,
    int eo1, mmDatePeriod  ep1,
    int so2, mmDatePeriodN sp2,
    int eo2, mmDatePeriodN ep2,
    int f, wxString name
) :
    m_sp1(sp1),
    m_ep1(ep1)
{
    // m_so1/m_eo1 is not applicable if m_sp1/m_ep1 resp., is mmDatePeriod::_A
    m_so1 = (m_sp1 == mmDatePeriod::_A) ? 0 : so1;
    m_eo1 = (m_ep1 == mmDatePeriod::_A) ? 0 : eo1;
    // either both m_sp2 and m_ep2 are null, or none of them
    if (!sp2.has_value() || !ep2.has_value()) {
        // m_so2/m_eo2 are not applicable if m_sp2/m_ep2 are null
        m_sp2 = mmDatePeriodN(); m_so2 = 0;
        m_ep2 = mmDatePeriodN(); m_eo2 = 0;
    }
    else {
        // m_so2/m_eo2 is not applicable if m_sp2/m_ep2 resp., is mmDatePeriod::A
        m_sp2 = sp2; m_so2 = (m_sp2 == mmDatePeriod::_A) ? 0 : so2;
        m_ep2 = ep2; m_eo2 = (m_ep2 == mmDatePeriod::_A) ? 0 : eo2;
    }
    // m_f is either 0 (calendar) or 1 (financial)
    m_f = (f == 1) ? 1 : 0;
    m_name = name;
}

void mmDateRange2::Range::scanWhiteSpace(StringIt& buffer_i, StringIt buffer_end)
{
    const wxString ws = " \t\r\n"; 
    while (buffer_i != buffer_end && ws.Find(*buffer_i) != wxNOT_FOUND)
        ++buffer_i;
}

// the returned token is one of:
//   o : offset
//   p : period
//   . : .. (range)
//   , : , (subrange separator)
//   f : F (financial)
//   ; : ; or end
//   _ : error
// token_o is set if token==o; token_p is set if token==p
char mmDateRange2::Range::scanToken(
    StringIt& buffer_i, StringIt buffer_end,
    int& token_o, mmDatePeriod& token_p
) {
    scanWhiteSpace(buffer_i, buffer_end);
    if (buffer_i == buffer_end)
        return ';';
    char c;
    if (!(*buffer_i).IsAscii() || !(*buffer_i).GetAsChar(&c))
        return '_';

    if (c == ',' || c == ';') {
        ++buffer_i;
        return c;
    }

    if (c == 'F') {
        ++buffer_i;
        return 'f';
    }

    if (c == '.') {
        ++buffer_i;
        if (buffer_i == buffer_end || !(*buffer_i).IsAscii() || !(*buffer_i).GetAsChar(&c))
            return '_';
        if (c == '.') {
            ++buffer_i;
            return '.';
        }
        else {
            return '_';
        }
    }

    char c1 = 0;
    if (c == '+' || c == '-') {
        c1 = c;
        ++buffer_i;
        if (buffer_i == buffer_end || !(*buffer_i).IsAscii() || !(*buffer_i).GetAsChar(&c))
            return '_';
    }
    if (isdigit(c)) {
        StringIt str_start = buffer_i;
        do { ++buffer_i; } while (
            buffer_i != buffer_end && (*buffer_i).IsAscii() && (*buffer_i).GetAsChar(&c)
            && isdigit(c)
        );
        wxString str_o = "";
        str_o.append(str_start, buffer_i);
        token_o = wxAtoi(str_o);
        if (c1 == '-') token_o = -token_o;
        return 'o';
    }
    if (c1)
        return '_';

    if (auto it = mmDatePeriod::s_label_id_m.find(c); it != mmDatePeriod::s_label_id_m.end()) {
        ++buffer_i;
        token_p = it->second;
        return 'p';
    }

    return '_';
}

// return true if parse is successful
bool mmDateRange2::Range::parseLabel(StringIt& buffer_i, StringIt buffer_end)
{
    // range = subrange ("," subrange)? ("F")? (";" name)?
    // subrange = so? sp ".." eo? ep
    // subrange = so ".." eo p
    // subrange = o? p

    int so[2] = { 0, 0 }; mmDatePeriodN sp[2] = { mmDatePeriodN(), mmDatePeriodN() };
    int eo[2] = { 0, 0 }; mmDatePeriodN ep[2] = { mmDatePeriodN(), mmDatePeriodN() };
    int f = 0;
    int i = 0;     // index into {s,e}{o,p}[] (0: first subrange, 1: second subrange)
    int state = 0; // parse state: 0 (so) 1 (sp) 2 (..) 3 (eo) 4 (ep) 5 (f) 6 (;) 7

    while (1) {
        char token;                               // one of [op.,@f;_]
        int token_o = 0;                          // offset (if token == 'o')
        mmDatePeriod token_p = mmDatePeriod::_A;  // period (if token == 'p')
        token = scanToken(buffer_i, buffer_end, token_o, token_p);
        //wxLogDebug("DEBUG: state=%d, token=%c", state, token);
        if (state == 0 && token == 'o') {
            so[i] = token_o;
            state = 1;
            continue;
        }
        if ((state == 0 || state == 1) && token == 'p') {
            sp[i] = token_p;
            state = 2;
            continue;
        }
        if ((state == 1 || state == 2) && token == '.') {
            state = 3;
            continue;
        }
        if (state == 3 && token == 'o') {
            eo[i] = token_o;
            state = 4;
            continue;
        }
        if ((state == 3 || state == 4) && token == 'p') {
            if (!sp[i].has_value())
                sp[i] = token_p;
            ep[i] = token_p;
            state = 5;
            continue;
        }
        if (state == 2 && ((i == 0 && token == ',') || token == 'f' || token == ';')) {
            eo[i] = so[i];
            ep[i] = sp[i];
            state = 5;
        }
        if (i == 0 && state == 5 && token == ',') {
            i = 1;
            state = 0;
            continue;
        }
        if (state == 5 && token == 'f') {
            f = 1;
            state = 6;
            continue;
        }
        if ((state == 5 || state == 6) && token == ';') {
            state = 7;
            break;
        }
        break;
    }

    if (state != 7 || !sp[0].has_value() || !ep[0].has_value()) {
        wxLogDebug("ERROR: mmDateRange2::Range::parseLabel(): state=%d", state);
        return false;
    }

    m_so1 = so[0]; m_sp1 = sp[0].value(); m_so2 = so[1]; m_sp2 = sp[1];
    m_eo1 = eo[0]; m_ep1 = ep[0].value(); m_eo2 = eo[1]; m_ep2 = ep[1];
    m_f = f;

    return true;
}

void mmDateRange2::Range::parseName(StringIt& buffer_i, StringIt buffer_end)
{
    m_name = "";
    scanWhiteSpace(buffer_i, buffer_end);
    m_name.append(buffer_i, buffer_end);
}

bool mmDateRange2::Range::parseLabelName(const wxString& buffer, const wxString& name)
{
    Range range = Range();
    StringIt buffer_i = buffer.begin();
    if (!range.parseLabel(buffer_i, buffer.end()))
        return false;
    if (!name.empty())
        range.m_name = name;
    else
        range.parseName(buffer_i, buffer.end());
    *this = range;
    return true;
}

const wxString mmDateRange2::Range::offsetRangeStr(int so, int eo, bool show_zero)
{
    if (so == eo) {
        return offsetStr(so, show_zero);
    }
    else {
        mmStringBuilder s;
        s.append(offsetStr(so, true));
        s.append("..");
        s.append(offsetStr(eo, true));
        return s.buffer;
    }
}

const wxString mmDateRange2::Range::getLabel() const
{
    mmStringBuilder sb;

    // first range
    if (m_sp1 == m_ep1) {
        sb.append(offsetRangeStr(m_so1, m_eo1)); sb.sep(); sb.append(m_sp1.label());
    }
    else {
        sb.append(offsetStr(m_so1)); sb.sep(); sb.append(m_sp1.label());
        sb.sep(); sb.append(".."); sb.sep();
        sb.append(offsetStr(m_eo1)); sb.sep(); sb.append(m_ep1.label());
    }

    // second range
    if (m_sp2.has_value() && m_ep2.has_value()) {
        sb.append(","); sb.sep();
        if (m_sp2 == m_ep2) {
            sb.append(offsetRangeStr(m_so2, m_eo2)); sb.sep(); sb.append(m_sp2.value().label());
        }
        else {
            sb.append(offsetStr(m_so2)); sb.sep(); sb.append(m_sp2.value().label());
            sb.sep(); sb.append(".."); sb.sep();
            sb.append(offsetStr(m_eo2)); sb.sep(); sb.append(m_ep2.value().label());
        }
    }

    // financial
    if (m_f == 1) {
        sb.sep(); sb.append("F");
    }

    return sb.buffer;
}

const wxString mmDateRange2::Range::getLabelName() const
{
    wxString s = getLabel();
    if (!m_name.empty()) {
        s.append("; ");
        s.append(m_name);
    }
    return s;
}

const wxString mmDateRange2::Range::checkingName() const
{
    wxString str = getLabel();
    // TODO
    return str;
}

const wxString mmDateRange2::Range::checkingDescription() const
{
    static mmStringBuilder sb;
    sb.reset();
    sb.append(getLabel());
    // TODO
    return sb.buffer;
}

mmDateRange2::Reporting::Reporting(
    int m, mmDatePeriod p
) :
    // m_p cannot be mmDatePeriod::_S (account statement date)
    // m_m cannot be 0; it is normalized to 1 if m_p is mmDatePeriod::_A
    m_m((m == 0 || p == mmDatePeriod::_A) ? 1 : m),
    m_p((p == mmDatePeriod::_S) ? mmDatePeriod::_T : p)
{
}

// return true if parse is successful
bool mmDateRange2::Reporting::parseLabel(StringIt& buffer_i, StringIt buffer_end)
{
    // reporting = m? p

    int m = 1; mmDatePeriod p = mmDatePeriod::_A;
    int state = 0; // parse state: 0 (m) 1 (p) 2 (;) 3

    while (1) {
        char token;                               // one of [op;_]
        int token_o = 0;                          // offset (if token == 'o')
        mmDatePeriod token_p = mmDatePeriod::_A;  // period (if token == 'p')
        token = mmDateRange2::Range::scanToken(buffer_i, buffer_end, token_o, token_p);
        //wxLogDebug("DEBUG: state=%d, token=%c", state, token);
        if (state == 0 && token == 'o') {
            // m_m cannot be 0
            m = (token_o == 0) ? 1 : token_o;
            state = 1;
            continue;
        }
        if ((state == 0 || state == 1) && token == 'p') {
            // m_m is normalized to 1 if m_p is mmDatePeriod::_A
            if (token_p == mmDatePeriod::_A)
                m = 1;
            // m_p cannot be mmDatePeriod::_S
            p = (token_p == mmDatePeriod::_S) ? mmDatePeriod::_T : token_p;
            state = 2;
            continue;
        }
        if ((state == 0 || state == 2) && token == ';') {
            state = 3;
            break;
        }
        break;
    }

    if (state != 3) {
        wxLogDebug("ERROR: mmDateRange2::Reporting::parseLabel(): state=%d", state);
        return false;
    }

    m_m = m;
    m_p = p;

    return true;
}

const wxString mmDateRange2::Reporting::getLabel() const
{
    mmStringBuilder sb;
    sb.append(multiplierStr(m_m)); sb.sep(); sb.append(m_p.label());
    return sb.buffer;
}

mmDateRange2::mmDateRange2(
    mmDateN sDateN,
    mmDate  tDate,
    mmDateN defStartDateN,
    mmDateN defEndDateN
) :
    m_firstDay{
        PrefModel::instance().getReportingFirstDay(),
        PrefModel::instance().getFinancialFirstDay()
    },
    m_firstMonth{
        wxDateTime::Jan,
        PrefModel::instance().getFinancialFirstMonth()
    },
    m_firstWeekday(
        PrefModel::instance().getReportingFirstWeekday()
    ),
    m_sDateN(sDateN),
    m_tDate(tDate),
    m_defStartDateN(defStartDateN),
    m_defEndDateN(defEndDateN),
    m_range(Range()),
    m_reporting(Reporting())
{
}

#ifndef NDEBUG
mmDateRange2::mmDateRange2(
    int firstDay_0, int firstDay_1,
    wxDateTime::Month firstMonth_0, wxDateTime::Month firstMonth_1,
    wxDateTime::WeekDay firstWeekday,
    mmDateN sDateN,
    mmDate  tDate,
    mmDateN defStartDateN,
    mmDateN defEndDateN
) :
    m_firstDay{firstDay_0, firstDay_1},
    m_firstMonth{firstMonth_0, firstMonth_1},
    m_firstWeekday(firstWeekday),
    m_sDateN(sDateN),
    m_tDate(tDate),
    m_defStartDateN(defStartDateN),
    m_defEndDateN(defEndDateN),
    m_range(Range()),
    m_reporting(Reporting())
{
}
#endif

// return true if parse is successful
bool mmDateRange2::parseRange(const wxString& buffer, const wxString& name)
{
    Range range = Range();
    if (!range.parseLabelName(buffer, name))
        return false;
    m_range = range;
    return true;
}

// return true if parse is successful
bool mmDateRange2::parseReporting(const wxString& buffer)
{
    Reporting reporting = Reporting();
    StringIt buffer_i = buffer.begin();
    if (!reporting.parseLabel(buffer_i, buffer.end()))
        return false;
    m_reporting = reporting;
    return true;
}

mmDateN mmDateRange2::periodStartN(mmDate date, mmDatePeriod period) const
{
    if (period == mmDatePeriod::_A)
        return mmDateN();
    wxDateTime s = date.dateTime();
    if (period == mmDatePeriod::_Y || period == mmDatePeriod::_Q || period == mmDatePeriod::_M) {
        if (s.GetDay() < m_firstDay[m_range.m_f])
            s -= wxDateSpan::Months(1);
        s.SetDay(m_firstDay[m_range.m_f]);
        if (period == mmDatePeriod::_Y) {
            if (s.GetMonth() < m_firstMonth[m_range.m_f])
                s -= wxDateSpan::Years(1);
            s.SetMonth(m_firstMonth[m_range.m_f]);
        }
        else if (period == mmDatePeriod::_Q) {
            int m = (s.GetMonth() - m_firstMonth[m_range.m_f] + 12) % 3;
            if (m > 0)
                s -= wxDateSpan::Months(m);
        }
    }
    else if (period == mmDatePeriod::_W) {
        int d = (s.GetWeekDay() - m_firstWeekday + 7) % 7;
        if (d > 0)
            s -= wxDateSpan::Days(d);
    }
    return mmDateN(s);
}

mmDateN mmDateRange2::periodEndN(mmDate date, mmDatePeriod period) const
{
    if (period == mmDatePeriod::_A)
        return mmDateN();
    wxDateTime e = date.dateTime();
    if (period == mmDatePeriod::_Y || period == mmDatePeriod::_Q || period == mmDatePeriod::_M) {
        if (e.GetDay() >= m_firstDay[m_range.m_f])
            e += wxDateSpan::Months(1);
        e.SetDay(m_firstDay[m_range.m_f]);
        if (period == mmDatePeriod::_Y) {
            if (e.GetMonth() > m_firstMonth[m_range.m_f])
                e += wxDateSpan::Years(1);
            e.SetMonth(m_firstMonth[m_range.m_f]);
        }
        else if (period == mmDatePeriod::_Q) {
            int m = (m_firstMonth[m_range.m_f] - e.GetMonth() + 12) % 3;
            if (m > 0)
                e += wxDateSpan::Months(m);
        }
        e -= wxDateSpan::Days(1);
    }
    else if (period == mmDatePeriod::_W) {
        int d = (m_firstWeekday - e.GetWeekDay() + 6) % 7;
        if (d > 0)
            e += wxDateSpan::Days(d);
    }
    return mmDateN(e);
}

mmDateN mmDateRange2::rangeStartN() const
{
    if (m_range.m_sp1 == mmDatePeriod::_A || m_range.m_sp2 == mmDatePeriod::_A)
        return m_defStartDateN;
    mmDateN s1N = (m_range.m_sp1 == mmDatePeriod::_S) ? m_sDateN : m_tDate;
    if (!s1N.has_value())
        return mmDateN();
    mmDate s1 = s1N.value();
    if (m_range.m_so1 != 0)
        s1.addDateSpan(mmDatePeriod::span(m_range.m_so1, m_range.m_sp1));
    s1 = periodStartN(s1, m_range.m_sp1).value();
    if (!m_range.m_sp2.has_value())
        return s1;
    mmDateN s2N = (m_range.m_sp2 == mmDatePeriod::_S) ? m_sDateN : m_tDate;
    if (!s2N.has_value())
        return mmDateN();
    mmDate s2 = s2N.value();
    if (m_range.m_so2 != 0)
        s2.addDateSpan(mmDatePeriod::span(m_range.m_so2, m_range.m_sp2.value()));
    mmDatePeriod p = m_range.m_sp1.toInt() > m_range.m_sp2.value().toInt() ? m_range.m_sp1
        : m_range.m_sp2.value();
    s2 = periodStartN(s2, p).value();
    return s1 <= s2 ? s1 : s2;
}

mmDateN mmDateRange2::rangeEndN() const
{
    if (m_range.m_ep1 == mmDatePeriod::_A || m_range.m_ep2 == mmDatePeriod::_A)
        return m_defEndDateN;
    mmDateN e1N = (m_range.m_ep1 == mmDatePeriod::_S) ? m_sDateN : m_tDate;
    if (!e1N.has_value())
        return mmDateN();
    mmDate e1 = e1N.value();
    if (m_range.m_eo1 != 0)
        e1.addDateSpan(mmDatePeriod::span(m_range.m_eo1, m_range.m_ep1));
    e1 = periodEndN(e1, m_range.m_ep1).value();
    if (!m_range.m_ep2.has_value())
        return e1;
    mmDateN e2N = (m_range.m_ep2 == mmDatePeriod::_S) ? m_sDateN : m_tDate;
    if (!e2N.has_value())
        return mmDateN();
    mmDate e2 = e2N.value();
    if (m_range.m_eo2 != 0)
        e2.addDateSpan(mmDatePeriod::span(m_range.m_eo2, m_range.m_ep2.value()));
    mmDatePeriod p = m_range.m_ep1.toInt() > m_range.m_ep2.value().toInt()
        ? m_range.m_ep1
        : m_range.m_ep2.value();
    e2 = periodEndN(e2, p).value();
    return e2 <= e1 ? e1 : e2;
}

mmDateN mmDateRange2::reportingNextN() const
{
    mmDateN sN = rangeStartN();
    mmDateN eN = rangeEndN();
    if (!sN.has_value() || !eN.has_value())
        return eN;

    mmDate s = sN.value();
    mmDate e = eN.value();
    if (s > e)
        return mmDateN();

    if (m_reporting.m_p == mmDatePeriod::_A)
        return e;

    if (m_reporting.m_m > 0) {
        // return the end of the multi-period aligned at s
        // (i.e., its first period contains s)
        mmDate next = periodEndN(s, m_reporting.m_p).value();
        if (m_reporting.m_m > 1) {
            next.addDateSpan(mmDatePeriod::span(m_reporting.m_m - 1, m_reporting.m_p));
            next = periodEndN(next, m_reporting.m_p).value();
        }
        return next <= e ? next : e;
    }
    else { // if (m_reporting.m_m < 0)
        // return the end of the multi-period aligned at e
        // (i.e., its last period contains e)
        mmDate next = periodEndN(e, m_reporting.m_p).value();
        mmDate next1 = next.plusDateSpan(mmDatePeriod::span(m_reporting.m_m, m_reporting.m_p));
        while (s <= next1) {
            next = next1;
            next1.addDateSpan(mmDatePeriod::span(m_reporting.m_m, m_reporting.m_p));
            next1 = periodEndN(next1, m_reporting.m_p).value();
        }
        return next <= e ? next : e;
    }
}

const wxString mmDateRange2::checkingTooltip() const
{
    static mmStringBuilder sb;
    sb.reset();

    mmDateN s = rangeStartN();
    mmDateN e = rangeEndN();
    if (s.has_value())
        sb.append(s.value().isoDate());
    sb.sep(); sb.append(".."); sb.sep();
    if (e.has_value())
        sb.append(e.value().isoDate());
    sb.flush();

    sb.append("\n");
    sb.append(m_range.checkingDescription());
    return sb.buffer;
}

const wxString mmDateRange2::reportingTooltip() const
{
    // TODO
    return "";
}

mmDateRange2::ReportingIterator::ReportingIterator(const mmDateRange2* a) :
    m_a(a),
    m_count(0),
    m_nextDateN(a->reportingNextN()),
    m_lastDateN(a->rangeEndN())
{
}

void mmDateRange2::ReportingIterator::increment()
{
    // The iterator reaches the end when (m_nextDateN == m_lastDateN)
    // (this includes the special case of open end, in which m_lastDateN is null).
    // m_count is initialized to 0 and set to -1 at the end.
    // Notice that the iterator returns at least one mmDateN before it reaches the end.

    if (m_count == -1)
        return;
    if (!m_nextDateN.has_value() || m_nextDateN == m_lastDateN) {
        m_count = -1;
        return;
    }
    if (!m_nextDateN.has_value() || !m_lastDateN.has_value()) {
        // this should not happen
        wxLogDebug("ERROR in mmDateRange2::ReportingIterator: only one of m_nextDateN, m_lastDateN is null");
        m_count = -1;
        return;
    }

    int rm = m_a->m_reporting.m_m;
    if (rm < 0) rm = -rm;
    mmDatePeriod rp = m_a->m_reporting.m_p;
    // assertion: rp is not mmDatePeriod::_A
    mmDate next1 = m_nextDateN.value().plusDateSpan(mmDatePeriod::span(rm, rp));
    next1 = m_a->periodEndN(next1, rp).value();
    if (m_lastDateN.value() < next1)
        next1 = m_lastDateN.value();

    if (next1 <= m_nextDateN.value()) {
        // this should not happen
        // m_nextDateN must strictly increase in each step
        wxLogDebug("ERROR in mmDateRange2::ReportingIterator: cannot increment");
        m_count = -1;
        return;
    }

    ++m_count;
    m_nextDateN = next1;
}

#ifndef NDEBUG
bool mmDateRange2::debug()
{
    bool ok = true;
    wxLogDebug("{{{ mmDateRange2::debug()");

    // check order in mmDatePeriod::s_id_label_a
    int n = static_cast<int>(mmDatePeriod::s_id_label_a.size());
    for (int i = 0; i < n; i++) {
        wxASSERT_MSG(
            static_cast<int>(mmDatePeriod::s_id_label_a[i].id) == i,
            "Wrong order in mmDatePeriod::s_id_label_a"
        );
    }

    // create a mmDateRange2 object; defStartDateN is null
    wxDateTime sDateTime, tDateTime, defEndDateTime;
    sDateTime.ParseISOCombined("2024-08-30T00:00:01"); // Fri
    tDateTime.ParseISOCombined("2025-01-30T00:00:01"); // Thu
    defEndDateTime.ParseISODate("2025-01-31"); // Fri
    mmDateRange2 dr = mmDateRange2(
        1, 6,
        wxDateTime::Month::Jan, wxDateTime::Month::Apr,
        wxDateTime::WeekDay::Mon,
        mmDate(sDateTime), mmDate(tDateTime),
        mmDateN(), mmDate(defEndDateTime)
    );
    wxLogDebug("INFO: sDateN=[%s]", dr.getSDateN().isoDateN());
    wxLogDebug("INFO: tDate=[%s]", dr.getTDate().isoDate());
    wxLogDebug("INFO: defStartDateN=[%s]", dr.getDefStartDateN().isoDateN());
    wxLogDebug("INFO: defEndDateN=[%s]", dr.getDefEndDateN().isoDateN());

    struct {
        wxString range_label; wxString reporting_label;
        wxString range_start; wxString range_end; int reporting_count;
    } test[] = {
        { "A",            "",     "",           "2025-01-31",  1 }, // -,   Fri
        { "Y",            "M",    "2025-01-01", "2025-12-31", 12 }, // Wed, Wed
        { "Q",            "",     "2025-01-01", "2025-03-31",  1 }, // Wed, Mon
        { "M",            "W",    "2025-01-01", "2025-01-31",  5 }, // Wed, Fri
        { "W",            "",     "2025-01-27", "2025-02-02",  1 }, // Mon, Mon
        { "T",            "",     "2025-01-30", "2025-01-30",  1 }, // Thu, Thu
        { "S",            "",     "2024-08-30", "2024-08-30",  1 }, // Fri, Fri
        { "Y F",          "2 W",  "2024-04-06", "2025-04-05", 27 }, // Sat, Sat
        { "Q F",          "",     "2025-01-06", "2025-04-05",  1 }, // Mon, Sat
        { "M F",          "",     "2025-01-06", "2025-02-05",  1 }, // Mon, Wed
        { "W F",          "",     "2025-01-27", "2025-02-02",  1 }, // Mon, Sun
        { "T F",          "",     "2025-01-30", "2025-01-30",  1 }, // Thu, Thu
        { "S F",          "",     "2024-08-30", "2024-08-30",  1 }, // Fri, Fri
        { "1 M",          "",     "2025-02-01", "2025-02-28",  1 }, // Sat, Fri
        { "-1..0 Y",      "Q",    "2024-01-01", "2025-12-31",  8 }, // Mon, Wed
        { "-1..+1 Q",     "M",    "2024-10-01", "2025-06-30",  9 }, // Tue, Mon
        { "A .. W",       "W",    "",           "2025-02-02",  1 }, // -,   Mon
        { "-1 Y .. M",    "Q",    "2024-01-01", "2025-01-31",  5 }, // Mon, Fri
        { "-1 Y, M",      "W",    "2024-01-01", "2025-01-31", 57 }, // Mon, Fri
        { "-1 M, Q .. Y", "-2 M", "2024-12-01", "2025-01-31",  1 }, // Sun, Fri
        { "W, -1..+1 Q",  "-2 M", "2024-10-28", "2025-05-04",  4 }, // Mon, Sun
    };
    for (int i = 0; i < static_cast<int>(sizeof(test)/sizeof(test[0])); ++i) {
        mmStringBuilder sb;
        if (!test[i].range_label.empty()) {
            sb.append(test[i].range_label);
        }
        if (!test[i].reporting_label.empty()) {
            sb.sep(); sb.append("@"); sb.sep(); sb.append(test[i].reporting_label);
        }
        wxString label = sb.buffer;
        //wxLogDebug("test[%d] [%s]", i, label);

        // parse range and reporeting labels
        if (!dr.parseRange(test[i].range_label)) {
            ok = false;
            wxLogDebug("ERROR in test[%d] [%s]: Cannot parse range label", i, label);
            continue;
        }
        if (!dr.parseReporting(test[i].reporting_label)) {
            ok = false;
            wxLogDebug("ERROR in test[%d] [%s]: Cannot parse reporting label", i, label);
            continue;
        }
        wxString range_label = dr.rangeLabel();
        if (range_label != test[i].range_label)
            wxLogDebug("test[%d] [%s]: range_label=[%s]", i, label, range_label);
        wxString reporting_label = dr.reportingLabel();
        if (reporting_label != test[i].reporting_label)
            wxLogDebug("test[%d] [%s]: reporting_label=[%s]", i, label, reporting_label);

        // check range start
        wxString range_start = dr.rangeStartN().isoDateN();
        if (range_start != test[i].range_start) {
            ok = false;
            wxLogDebug("ERROR in test[%d] [%s]: range_start=[%s], expected [%s]",
                i, label, range_start, test[i].range_start
            );
        }

        // check range end
        wxString range_end = dr.rangeEndN().isoDateN();
        if (range_end != test[i].range_end) {
            ok = false;
            wxLogDebug("ERROR in test[%d] [%s]: range_end=[%s], expected [%s]",
                i, label, range_end, test[i].range_end
            );
        }

        // check reporting iterator
        //wxLogDebug("test[%d] [%s]: reporting iterator", i, label);
        int reporting_count = 0;
        for (auto it = dr.cbegin(); it != dr.cend(); ++it) {
            ++reporting_count;
            //wxLogDebug("  count=%d, next=[%s]", reporting_count, (*it).isoDateN());
        }
        if (reporting_count != test[i].reporting_count) {
            ok = false;
            wxLogDebug("ERROR in test[%d] [%s]: reporting_count=%d, expected %d",
                i, label, reporting_count, test[i].reporting_count
            );
        }
    }

    wxLogDebug("}}}");
    return ok;
}
#endif
