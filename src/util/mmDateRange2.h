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

#include "base/defs.h"
#include <wx/datetime.h>
#include <optional>
#include <unordered_map>

#include "_primitive.h"
#include "mmDate.h"

class mmDatePeriod
{
public:
    enum Id
    {
        _A = 0,     // all
        _Y,         // year = 4 quarters = 12 months
        _Q,         // quarter = 3 months
        _M,         // month
        _W,         // week = 7 days
        _T,         // day (today)
        _S,         // day (account statement date)
        _min = _A,  // (min value)
        _max = _S,  // (max value)
    };

    typedef struct { Id id; const wxString label; } MapIdLabel;
    typedef std::unordered_map<char, Id> MapLabelId;
    static const MapIdLabel mapIdLabel[];
    static const MapLabelId mapLabelId;

private:
    static MapLabelId makeLabelId();

private:
    Id m_id;

public:
    mmDatePeriod(Id id = _A);
    mmDatePeriod(char label);

public:
    static wxDateSpan span(int offset, mmDatePeriod period);

public:
    int  toInt() const { return static_cast<int>(m_id); }
    auto label() const -> const wxString { return mapIdLabel[toInt()].label; }

public:
    bool operator== (mmDatePeriod other) const { return m_id == other.m_id; }
    bool operator== (Id other_id) const { return m_id == other_id; }
};

using mmDatePeriodN = std::optional<mmDatePeriod>;

// mmDateRange2 represents a date range relative to today.
// The range specification consists of the following:
//   two subranges
//   a reporting multiplier/period
//   a selector between calendar or financial year/quarter/month
//   a descriptive name
// Each subrange consists of a start offset/period and an end offset/period.
// To avoid instability, the date of today as well as a few user preferences,
// are cached at construction time.

class mmDateRange2
{
public:
    class Range
    {
        friend class mmDateRange2;

    private:
        int m_so1 = 0;                          // start offset of first subrange
        int m_eo1 = 0;                          // end   offset of first subrange
        mmDatePeriod m_sp1 = mmDatePeriod::_A;  // start period of first subrange
        mmDatePeriod m_ep1 = mmDatePeriod::_A;  // end   period of first subrange
        int m_so2 = 0;                          // start offset of second subrange
        int m_eo2 = 0;                          // end   offset of second subrange
        mmDatePeriodN m_sp2 = mmDatePeriodN();  // start period of second subrange
        mmDatePeriodN m_ep2 = mmDatePeriodN();  // end   period of second subrange
        int m_f = 0;                            // index in first*[] (0=calendar, 1=financial)
        wxString m_name = "";                   // specification name

    public:
        Range(
            int so1 = 0, mmDatePeriod  sp1 = mmDatePeriod::_A,
            int eo1 = 0, mmDatePeriod  ep1 = mmDatePeriod::_A,
            int so2 = 0, mmDatePeriodN sp2 = mmDatePeriodN(),
            int eo2 = 0, mmDatePeriodN ep2 = mmDatePeriodN(),
            int f = 0, wxString name = ""
        );

    public:
        void setName(const wxString& name) { m_name = name; }
        bool parseLabel(StringIt& buffer_i, StringIt buffer_end);
        void parseName(StringIt& buffer_i, StringIt buffer_end);
        bool parseLabelName(const wxString& buffer, const wxString& name = "");
        int  getF() const { return m_f; }
        auto getLabel() const -> const wxString;
        auto getName() const -> const wxString { return m_name; }
        auto getLabelName() const -> const wxString;
        auto checkingName() const -> const wxString;
        auto checkingDescription() const -> const wxString;
        bool hasPeriodS() const;

    private:
        static void scanWhiteSpace(StringIt& buffer_i, StringIt buffer_end);
        static char scanToken(StringIt& buffer_i, StringIt buffer_end, int& token_o, mmDatePeriod& token_p);
        static auto offsetStr(int offset, bool show_zero = false) -> const wxString;
        static auto offsetRangeStr(int so, int eo, bool show_zero = false) -> const wxString;
    };

public:
    class Reporting
    {
        friend class mmDateRange2;

    private:
        int m_m;           // multiplier
        mmDatePeriod m_p;  // reporting period

    public:
        Reporting(int m = 1, mmDatePeriod p = mmDatePeriod::_A);

    public:
        bool parseLabel(StringIt& buffer_i, StringIt buffer_end);
        auto getLabel() const -> const wxString;

    private:
        static auto multiplierStr(int m, bool show_one = false) -> const wxString;
    };

private:
    int m_firstDay[2];                  // first day in PERIOD::[YQM] (1..28)
    wxDateTime::Month m_firstMonth[2];  // first month in PERIOD::[YQ] (0..11)
    wxDateTime::WeekDay m_firstWeekday; // first weekday in PERIOD::W (0=Sun, 1=Mon)
    mmDateN m_sDateN;                   // the date of PERIOD::S (account statement date)
    mmDate  m_tDate;                    // the date of PERIOD::T (today)
    mmDateN m_defStartDateN;            // default start date (if range start is open)
    mmDateN m_defEndDateN;              // default end date (if range end is open)
    Range m_range;                      // range specification
    Reporting m_reporting;              // reporting multiplier/period

public:
    mmDateRange2(
        mmDateN sDateN = mmDateN(),
        mmDate  tDate = mmDate::today(),
        mmDateN defStartDateN = mmDateN(),
        mmDateN defEndDateN = mmDateN()
    );

#ifndef NDEBUG
private:
    mmDateRange2(
        int firstDay_0, int firstDay_1,
        wxDateTime::Month firstMonth_0, wxDateTime::Month firstMonth_1,
        wxDateTime::WeekDay firstWeekday,
        mmDateN sDateN = mmDateN(),
        mmDate  tDate = mmDate::today(),
        mmDateN defStartDateN = mmDateN(),
        mmDateN defEndDateN = mmDateN()
    );
#endif

public:
    void setSDateN(mmDateN dateN = mmDateN()) { m_sDateN = dateN; }
    void setTDate(mmDate date = mmDate::today()) { m_tDate = date; }
    void setDefStartDateN(mmDateN dateN = mmDateN()) { m_defStartDateN = dateN; }
    void setDefEndDateN(mmDateN dateN = mmDateN()) { m_defEndDateN = dateN; }
    void setRange(const Range& range) { m_range = range; }
    void setReporting(const Reporting& reporting) { m_reporting = reporting; }
    int  getFirstDay() const { return m_firstDay[m_range.m_f]; }
    auto getFirstMonth() const -> wxDateTime::Month { return m_firstMonth[m_range.m_f]; }
    auto getFirstWeekday() const -> wxDateTime::WeekDay { return m_firstWeekday; }
    auto getSDateN() const -> mmDateN { return m_sDateN; }
    auto getTDate() const -> mmDate { return m_tDate; }
    auto getDefStartDateN() const -> mmDateN { return m_defStartDateN; }
    auto getDefEndDateN() const -> mmDateN { return m_defEndDateN; }
    auto getRange() const -> Range { return m_range; }
    auto getReporting() const -> Reporting { return m_reporting; }
    auto parseRange(const wxString& buffer, const wxString& name = "") -> bool;
    auto parseReporting(const wxString& buffer) -> bool;
    auto rangeLabel() const -> const wxString { return m_range.getLabel(); }
    auto rangeName() const -> const wxString { return m_range.getName(); }
    auto rangeLabelName() const -> const wxString { return m_range.getLabelName(); }
    auto reportingLabel() const -> const wxString { return m_reporting.getLabel(); }
    auto periodStartN(mmDate date, mmDatePeriod period) const -> mmDateN;
    auto periodEndN(mmDate date, mmDatePeriod period) const -> mmDateN;
    auto rangeStartN() const -> mmDateN;
    auto rangeEndN() const -> mmDateN;
    auto reportingNextN() const -> mmDateN;
    auto rangeStartIsoStartN() const -> const wxString { return rangeStartN().isoStartN(); }
    auto rangeEndIsoEndN() const -> const wxString { return rangeEndN().isoEndN(); }
    auto reportingNextIsoEndN() const -> const wxString { return reportingNextN().isoEndN(); }
    auto checkingTooltip() const -> const wxString;
    auto reportingTooltip() const -> const wxString;

public:
    struct ReportingIterator
    {
        using iterator_category = std::input_iterator_tag;
        using difference_type = int;
        using value_type = mmDateN;
        using pointer = const value_type*;
        using reference = const value_type&;

        friend class mmDateRange2;

    private:
        const mmDateRange2* m_a;
        int m_count;
        mmDateN m_nextDateN, m_lastDateN;

    public:
        ReportingIterator(const mmDateRange2* a);

    public:
        const mmDateN& operator*() { return m_nextDateN; }
        const mmDateN* operator->() { return &m_nextDateN; }
        ReportingIterator& operator++();
        ReportingIterator operator++(int);
        bool operator== (const ReportingIterator& other) { return m_count == other.m_count; }
        bool operator!= (const ReportingIterator& other) { return m_count != other.m_count; }
        bool operator== (int other_count) { return m_count == other_count; }
        bool operator!= (int other_count) { return m_count != other_count; }

    private:
        void increment();
    };

    ReportingIterator cbegin() const { return ReportingIterator(this); }
    int cend() const { return -1; }

#ifndef NDEBUG
public:
    static bool debug();
#endif
};

inline bool mmDateRange2::Range::hasPeriodS() const
{
    return
        m_sp1 == mmDatePeriod::_S || m_ep1 == mmDatePeriod::_S ||
        m_sp2 == mmDatePeriod::_S || m_ep2 == mmDatePeriod::_S;
}

inline const wxString mmDateRange2::Range::offsetStr(int offset, bool show_zero)
{
    return (offset != 0) ? wxString::Format("%+d", offset) : show_zero ? "0" : "";
}
inline const wxString mmDateRange2::Reporting::multiplierStr(int m, bool show_one)
{
    return (m != 1 || show_one) ? wxString::Format("%+d", m) : "";
}

inline mmDateRange2::ReportingIterator& mmDateRange2::ReportingIterator::operator++()
{
    increment();
    return *this;
}
inline mmDateRange2::ReportingIterator mmDateRange2::ReportingIterator::operator++(int)
{
    mmDateRange2::ReportingIterator tmp = *this;
    ++(*this);
    return tmp;
}
