/*******************************************************
Copyright (C) 2014 Nikolay Akimov

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

#include <vector>
#include "qif_import.h"

bool mmQIFImport::isLineOK(const wxString& line)
{
    return wxString("!DNPAT^MLSE$C/UI").Contains(line.Left(1));
}

wxString mmQIFImport::getLineData(const wxString& line)
{
    return line.substr(1, line.Length() - 1);
}

wxString mmQIFImport::getFileLine(wxTextInputStream& textFile, int& lineNumber)
{
    wxString textLine = textFile.ReadLine();
    lineNumber++;
    textLine.Trim(); // remove any trailing spaces

    return textLine;
}

wxString mmQIFImport::getFinancistoProject(wxString& sSubCateg)
{
    //Additional parsing for Financisto like apps
    wxString sProject = "";
    if (sSubCateg.Contains("/")) {
        wxStringTokenizer cattkz(sSubCateg, "/");

        sSubCateg = cattkz.GetNextToken();
        if (cattkz.HasMoreTokens())
            sProject = cattkz.GetNextToken().Trim();
    }
    return sProject;
}

qifAccountInfoType mmQIFImport::accountInfoType(const wxString& line)
{
    if (line.IsEmpty())
        return QIF_ID_UnknownInfo;

    wxChar fChar = line.GetChar(0);
    switch (fChar)
    {
    case 'N':
        return QIF_ID_Name;
    case 'T':
        return QIF_ID_AccountType;
    case 'D':
        return QIF_ID_Description;
    case 'L':
        return QIF_ID_CreditLimit;
    case '/':
        return QIF_ID_BalanceDate;
    case '$':
        return QIF_ID_Balance;
    case '^':
        return QIF_ID_EOT;
    default:
        return QIF_ID_UnknownInfo;
    }
}

qifLineType mmQIFImport::lineType(const wxString& line)
{
    if (line.IsEmpty())
        return QIF_ID_UnknownType;

    wxChar fChar = line.GetChar(0);
    switch (fChar)
    {
    case '!':
        return QIF_ID_AcctType;
    case 'D':
        return QIF_ID_Date;
    case 'N':
        return QIF_ID_TransNumber;
    case 'P':
        return QIF_ID_Payee;
    case 'A':
        return QIF_ID_Address;
    case 'T':
        return QIF_ID_Amount;
    case '^':
        return QIF_ID_EOTLT;
    case 'M':
        return QIF_ID_Memo;
    case 'L':
        return QIF_ID_Category;
    case 'S':
        return QIF_ID_CategorySplit;
    case 'E':
        return QIF_ID_MemoSplit;
    case '$':
        return QIF_ID_AmountSplit;
    case 'C':
        return QIF_ID_Status;
    default:
        return QIF_ID_UnknownType;
    }
}

bool mmQIFImport::handle_file(wxFileInputStream& input)
{
    wxTextInputStream text(input, "\x09", wxConvUTF8);

    bool qif_record_end = false;
    std::vector<QIF_Line> qif_record;  // each qif_record may contains mult lines
    while (input.IsOk() && !input.Eof()) {
        wxString line = text.ReadLine();
        QIF_Line qif_line;
        if (this->handle_line(line, qif_line)) {
            qif_record_end = (qif_line.first == QIF_ID_EOTLT);
            if (qif_line.first == QIF_ID_EOTLT) {
                QIF_Transaction tran;
                // process qif_record
                this->handle_qif_record(qif_record, tran);
                // release qif_record
                qif_record.clear();
            }
            else {
                qif_record.push_back(qif_line);
            }
        }
        else {
            // TODO
        }
    }

    if (!qif_record_end) {
        // TODO incomplete file ?
    }

    return true;
}

bool mmQIFImport::handle_file(const wxString& input_file)
{
    wxFileInputStream input(input_file);
    return this->handle_file(input);
}

bool mmQIFImport::handle_line(const wxString& line, QIF_Line& qif_line)
{
    qif_line.first = lineType(line);
    qif_line.second = getLineData(line);

    return qif_line.first != QIF_ID_UnknownType;
}

bool mmQIFImport::handle_qif_record(const QIF_Record& qif_record, QIF_Transaction& tran)
{
    for (const auto& line : qif_record)
    {
        this->handle_qif_line(line, tran);
    }
    return true;
}

bool mmQIFImport::handle_qif_line(const QIF_Line& qif_line, QIF_Transaction& tran)
{
    switch (qif_line.first)
    {
    case QIF_ID_Date:
        tran.D = qif_line.second;
        break;
    case QIF_ID_Memo:
        tran.M = qif_line.second;
        break;
    default:
        return false;
    }
    return true;
}
