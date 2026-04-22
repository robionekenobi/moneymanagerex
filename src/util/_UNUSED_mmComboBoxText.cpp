/*******************************************************
 Copyright (C) 2014 Nikolay Akimov
 Copyright (C) 2022  Mark Whalley (mark@ipx.co.uk)

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

#include "_UNUSED_mmComboBoxText.h"

#include "_util.h"
#include "_simple.h"

#include "model/AccountModel.h"
#include "model/PayeeModel.h"
#include "import_export/webapp.h"

UNUSED_mmComboBoxText::UNUSED_mmComboBoxText(
    wxWindow* parent_win,
    wxWindowID win_id,
    const wxString& value,
    bool is_payee,
    const wxPoint& pos = wxDefaultPosition,
    const wxSize& size = wxDefaultSize,
    long style = 0,
    const wxValidator& validator = wxDefaultValidator,
    const wxString& name = wxTextCtrlNameStr
) :
    wxTextCtrl(parent_win, win_id, value, pos, size, style, validator, name),
    m_is_payee(is_payee)
{
    if (m_is_payee)
        this->AutoComplete(PayeeModel::instance().find_all_name_a());
    else
        this->AutoComplete(AccountModel::instance().find_all_name_a());
}

void UNUSED_mmComboBoxText::setSelection(int& id)
{
    if (m_is_payee) {
        for (const auto& payee_d : PayeeModel::instance().find_data_a(
            TableClause::ORDERBY(PayeeCol::NAME_PAYEENAME)
        )) {
            if (payee_d.m_id == id)
                this->ChangeValue(payee_d.m_name);
        }
    }
    else {
        for (const auto &acc : AccountModel::instance().find_data_a(
            TableClause::ORDERBY(AccountCol::NAME_ACCOUNTNAME)
        )) {
            if (acc.m_id == id)
                this->ChangeValue(acc.m_name);
        }
    }
}

int64 UNUSED_mmComboBoxText::getID()
{
    int64 id = -1;
    if (m_is_payee) {
        const PayeeData* payee_n = PayeeModel::instance().get_name_data_n(
            this->GetValue()
        );
        if (payee_n) {
            id = payee_n->m_id;
        }
        else {
            PayeeData new_payee_d = PayeeData();
            new_payee_d.m_name = this->GetValue();
            PayeeModel::instance().add_data_n(new_payee_d);
            mmWebApp::uploadPayee();
        }
    }
    else {
        const AccountData* account_n = AccountModel::instance().get_name_data_n(
            this->GetValue()
        );
        if (account_n) {
            id = account_n->m_id;
        }
        else {
            //TODO:
        }
    }
    return id;
}
