/*******************************************************
Copyright (C) 2014 Nikolay Akimov
Copyright (C) 2015 Stefano Giorgio

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
#include "base/_constants.h"
#include "util/mmDatePicker.h"
#include "util/mmTextCtrl.h"
#include "util/_simple.h"

#include "model/TrxLinkModel.h"

class CurrencyModel;


class TrxLinkDialog : public wxPanel
{
    wxDECLARE_EVENT_TABLE();

private:
    enum
    {
        ID_TRANS_DATE_SELECTOR = wxID_HIGHEST + 10,
        ID_TRANS_DATE_CONTROLLER,
        ID_TRANS_STATUS_SELECTOR,
        ID_TRANS_ACCOUNT_BUTTON_TEXT,
        ID_TRANS_ACCOUNT_BUTTON,
        ID_TRANS_PAYEE_BUTTON_TEXT,
        ID_TRANS_PAYEE_BUTTON,
        ID_TRANS_CATEGORY_COMBOBOX,
        ID_TRANS_ENTERED_NUMBER,
        ID_TRANS_ENTERED_AMOUNT,
        ID_TRANS_ENTERED_NOTES,
        ID_TRANS_FREQUENT_NOTES,
        ID_TRANS_TRANSFER,
        ID_TRANS_CURRENCY_BUTTON,
        ID_TRANS_TYPE,
        mmID_CATEGORY_SPLIT,
    };

private:
    TrxData* m_transaction_n = nullptr;
    TrxData m_transaction_d;
    bool m_enable_revalue = false;
    int64 m_transaction_id = -1;
    int64 m_account_id = -1;
    int64 m_payee_id = -1;
    int64 m_category_id = -1;
    int64 m_subcategory_id = -1;
    std::vector<Split> m_local_splits;

    mmDatePicker*       w_date_picker    = nullptr;
    wxButton*           w_account_btn    = nullptr;
    wxChoice*           w_status_choice  = nullptr;
    wxChoice*           w_type_choice    = nullptr;
    mmTextCtrl*         w_amount_text    = nullptr;
    wxCheckBox*         w_transfer_cb    = nullptr;
    wxButton*           w_currency_btn   = nullptr;
    wxStaticText*       w_payee_label    = nullptr;
    wxButton*           w_payee_btn      = nullptr;
    mmComboBoxCategory* w_cat_text       = nullptr;
    wxTextCtrl*         w_number_text    = nullptr;
    wxTextCtrl*         w_notes_text     = nullptr;
    wxBitmapButton*     w_att_btn        = nullptr;
    wxButton*           w_freq_notes_btn = nullptr;

public:
    TrxLinkDialog();
    TrxLinkDialog(
        wxWindow *parent,
        TrxData* transaction_n,
        bool enable_revalue = false,
        wxWindowID win_id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL,
        const wxString &name = _t("Transaction Link Dialog")
    );
    ~TrxLinkDialog();

    int64 SaveChecking();
    bool ValidCheckingAccountEntry();
    wxDateTime TransactionDate();
    void TransactionDate(const wxDateTime& trans_date);

    void SetTransactionValue(const double& trans_value, bool fixed_value = false);
    void SetTransactionNumber(const wxString& trans_number);
    void SetTransactionAccount(const wxString& trans_account);
    void SetTransactionStatus(const int trans_status_enum);
    void SetTransactionPayee(const int64 payeeid);
    void SetTransactionCategory(const int64 categid);

    int TransactionType();
    TrxLinkModel::CHECKING_TYPE CheckingType();
    void CheckingType(TrxLinkModel::CHECKING_TYPE ct);

private:
    bool Create(
        wxWindow* parent,
        wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL,
        const wxString &name = _t("Transaction Link Dialog")
    );

    void DataToControls();
    void BindEventsAndTrigger();
    void SetLastPayeeAndCategory(const int64 account_id);

    void OnTransAccountButton(wxCommandEvent& WXUNUSED(event));
    void OnTransPayeeButton(wxCommandEvent& WXUNUSED(event));
    void OnTransCategoryCombobox(wxCommandEvent& WXUNUSED(event));
    void OnCategs(wxCommandEvent& event);

    void OnFrequentNotes(wxCommandEvent& event);
    void OnAttachments(wxCommandEvent& WXUNUSED(event));
};
