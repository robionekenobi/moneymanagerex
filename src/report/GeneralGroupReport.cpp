/*******************************************************
 Copyright (C) 2006 Madhan Kanagavel
 Copyright (C) 2012 Stefano Giorgio
 Copyright (C) 2013 - 2022 Nikolay Akimov
 Copyright (C) 2014 James Higley
 Copyright (C) 2014 Guan Lisheng (guanlisheng@gmail.com)
 Copyright (C) 2022 Mark Whalley (mark@ipx.co.uk)

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

#include "GeneralGroupReport.h"

#include "util/mmImage.h"
#include "util/mmTreeItemData.h"
#include "util/mmMultiChoice.h"
#include "model/BudgetPeriodModel.h"
#include "report/_all.h"
#include "app/mmFrame.h"

const char* GeneralGroupReport::group_report_template = R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8" />
    <meta http - equiv = "Content-Type" content = "text/html" />
    <title><TMPL_VAR REPORTNAME></title>
    <script>
        window.Promise || document.write('<script src="memory:polyfill.min.js"><\/script>');
        window.Promise || document.write('<script src="memory:classlist.min.js"><\/script>');
        window.Promise || document.write('<script src="memory:resize-observer.js"><\/script>');
        window.Promise || document.write('<script src="memory:findindex.min.js"><\/script>');
        window.Promise || document.write('<script src="memory:umd.min.js"><\/script>');
    </script>
    <script src = "memory:apexcharts.min.js"></script>
    <script src = "memory:sorttable.js"></script>
    <link href = "memory:master.css" rel = "stylesheet" />
</head>
<body>

<div class = "container">
<h3><TMPL_VAR REPORTNAME></h3>
<TMPL_VAR TODAY><hr>
<div class = "row">
<div class = "col-xs-2"></div>
<div class = "col-xs-8">

<table class = "table">
    <thead>
        <tr>
            <th>REPORTID</th>
            <th>REPORTNAME</th>
        </tr>
    </thead>
    <tbody>
        <TMPL_LOOP NAME=CONTENTS>
            <tr>
            <td><TMPL_VAR REPORTID></td>
            <td><TMPL_VAR REPORTNAME></td>
            </tr>
        </TMPL_LOOP>
    </tbody>
</table>
</div></div></div></body>
</html>
)";

GeneralGroupReport::GeneralGroupReport(const wxString& groupname) :
    ReportBase(_n("General Group Report")),
    m_group_name(groupname)
{
    m_sub_reports = ReportModel::instance().find(ReportCol::GROUPNAME(groupname));
}

wxString GeneralGroupReport::getHTMLText()
{
    loop_t contents;
    for (const auto & report : m_sub_reports)
        contents += report.to_html_row();
    wxString report_template = group_report_template;
    mm_html_template report(formatHTML(report_template));
    report(L"REPORTNAME") = this->getTitle() + " For " + this->m_group_name;
    report(L"CONTENTS") = contents;

    wxString out = wxEmptyString;
    try {
        out = report.Process();
    }
    catch (const syntax_ex& e) {
        return e.what();
    }
    catch (...) {
        return _t("Caught exception");
    }

    return out;
}
