/*******************************************************
Copyright (C) 2014 Nikolay Akimov
Copyright (C) 2015 Stefano Giorgio
Copyright (C) 2022 Mark Whalley (mark@ipx.co.uk)
Copyright (C) 2026 Klaus Wich

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

#include "base/_defs.h"
#include "util/mmImage.h"
#include "util/mmSingleChoice.h"
#include "util/_util.h"
#include "util/_simple.h"
#include "util/mmCalcValidator.h"

#include "model/_all.h"
#include "model/PrefModel.h"

#include "manager/CategoryManager.h"
#include "manager/PayeeManager.h"
#include "AttachmentDialog.h"
#include "SplitDialog.h"
#include "TrxLinkDialog.h"

wxBEGIN_EVENT_TABLE(TrxLinkDialog, wxPanel)
    EVT_BUTTON(ID_TRANS_ACCOUNT_BUTTON,      TrxLinkDialog::OnTransAccountButton)
    EVT_BUTTON(ID_TRANS_PAYEE_BUTTON,        TrxLinkDialog::OnTransPayeeButton)
    EVT_BUTTON(mmID_CATEGORY_SPLIT,          TrxLinkDialog::OnCategs)
    EVT_COMBOBOX(ID_TRANS_CATEGORY_COMBOBOX, TrxLinkDialog::OnTransCategoryCombobox)
    EVT_BUTTON(wxID_FILE,                    TrxLinkDialog::OnAttachments)
wxEND_EVENT_TABLE()

TrxLinkDialog::TrxLinkDialog()
{
}

TrxLinkDialog::TrxLinkDialog(
    wxWindow* parent,
    TrxData* transaction_n,
    bool enable_revalue,
    wxWindowID win_id,
    const wxPoint& pos,
    const wxSize& size,
    long style,
    const wxString& name
) :
    m_transaction_n(transaction_n),
    m_enable_revalue(enable_revalue)
{
    if (m_transaction_n) {
        for (const auto& tp_d : TrxSplitModel::instance().find_data_a(
            TrxSplitCol::WHERE_TRANSID(OP_EQ, m_transaction_n->m_id)
        )) {
            wxArrayInt64 tag_id_a;
            for (const auto& gl_d : TagLinkModel::instance().find_data_a(
                TagLinkCol::WHERE_REFTYPE(OP_EQ, TrxSplitModel::s_ref_type.key_n()),
                TagLinkCol::WHERE_REFID(OP_EQ, tp_d.m_id)
            )) {
                tag_id_a.push_back(gl_d.m_tag_id);
            }
            m_local_splits.push_back({
                tp_d.m_category_id, tp_d.m_amount, tp_d.m_notes, tag_id_a
            });
        }
    }

    Create(parent, win_id, pos, size, style, name);
    DataToControls();
    BindEventsAndTrigger();
    UsageModel::instance().pageview(this);
}

TrxLinkDialog::~TrxLinkDialog()
{
}

bool TrxLinkDialog::Create(
    wxWindow* parent,
    wxWindowID win_id,
    const wxPoint &pos,
    const wxSize &size,
    long style,
    const wxString &name
) {
    wxPanel::Create(parent, win_id, pos, size, style, name);

    const wxSize std_size(230, -1);
    const wxSize std_half_size(110, -1);

    wxBoxSizer* main_panel_sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(main_panel_sizer);

    wxFlexGridSizer* transPanelSizer = new wxFlexGridSizer(0, 2, 0, 0);
    main_panel_sizer->Add(transPanelSizer);

    // Trans Date --------------------------------------------
    w_date_picker = new mmDatePicker(this, ID_TRANS_DATE_SELECTOR);
    mmToolTip(w_date_picker, _t("Specify the date of the transaction"));

    wxBoxSizer* date_sizer = new wxBoxSizer(wxHORIZONTAL);
    date_sizer->Add(w_date_picker->mmGetLayout());

    transPanelSizer->Add(new wxStaticText(this, wxID_STATIC, _t("Date")), g_flagsH);
    transPanelSizer->Add(date_sizer);

    // Account ------------------------------------------------
    transPanelSizer->Add(new wxStaticText(this, ID_TRANS_ACCOUNT_BUTTON_TEXT, _t("Account")), g_flagsH);
    w_account_btn = new wxButton(this, ID_TRANS_ACCOUNT_BUTTON, _t("Select Account")
        , wxDefaultPosition, std_size);
    mmToolTip(w_account_btn, _t("Specify the associated Account that will contain this transaction"));
    transPanelSizer->Add(w_account_btn, g_flagsH);

    // Type --------------------------------------------
    w_type_choice = new wxChoice(this, ID_TRANS_TYPE, wxDefaultPosition, std_half_size);
    for (int i = 0; i < TrxType::size; ++i) {
        if (i != TrxType::e_transfer || this->m_enable_revalue) {
            wxString type = TrxType(i).trade_name();
            w_type_choice->Append(wxGetTranslation(type), new wxStringClientData(type));
        }
    }

    w_type_choice->SetSelection(TrxType::e_withdrawal);
    mmToolTip(w_type_choice, _t("Withdraw funds from or deposit funds to this Account."));

    // transfer indicator (refined)
    w_transfer_cb = new wxCheckBox(this, ID_TRANS_TRANSFER,
        _t("&Transfer"),
        wxDefaultPosition, wxDefaultSize,
        wxCHK_2STATE
    );
    CheckingType(TrxLinkModel::AS_INCOME_EXPENSE);
    mmToolTip(w_transfer_cb, _t("Funds transfer from/to this account. Uncheck to set as Expense/Income."));

    wxBoxSizer* type_sizer = new wxBoxSizer(wxHORIZONTAL);
    type_sizer->Add(w_type_choice, g_flagsH);
    type_sizer->Add(w_transfer_cb, g_flagsH);

    transPanelSizer->Add(new wxStaticText(this, wxID_STATIC, _t("Type")), g_flagsH);
    transPanelSizer->Add(type_sizer);

    // Amount ------------------------------------------------
    wxStaticText* entered_amount_text = new wxStaticText(this, wxID_STATIC, _t("Amount"));
    w_amount_text = new mmTextCtrl(this, ID_TRANS_ENTERED_AMOUNT,
        "",
        wxDefaultPosition, std_half_size,
        wxALIGN_RIGHT | wxTE_PROCESS_ENTER,
        mmCalcValidator()
    );
    mmToolTip(w_amount_text, _t("Specify the amount for this transaction"));

    const CurrencyData* currency = CurrencyModel::instance().get_base_data_n();
    if (m_account_id > 0) {
        currency = AccountModel::instance().get_id_currency_p(m_account_id);
    }
    w_currency_btn = new wxButton(this, ID_TRANS_CURRENCY_BUTTON, currency->m_symbol
        , wxDefaultPosition, std_half_size);
    mmToolTip(w_currency_btn, _t("Currency used for this transaction."));

    wxBoxSizer* entered_amount_sizer = new wxBoxSizer(wxHORIZONTAL);
    entered_amount_sizer->Add(w_amount_text, g_flagsH);
    entered_amount_sizer->Add(w_currency_btn, g_flagsH);

    transPanelSizer->Add(entered_amount_text, g_flagsH);
    transPanelSizer->Add(entered_amount_sizer, wxSizerFlags(g_flagsExpand).Border(0));

    // Status --------------------------------------------
    w_status_choice = new wxChoice(this, ID_TRANS_STATUS_SELECTOR,
        wxDefaultPosition, std_half_size
    );

    for (int i = 0; i < TrxStatus::size; ++i) {
        wxString status_name = TrxStatus(i).name();
        w_status_choice->Append(
            wxGetTranslation(status_name),
            new wxStringClientData(status_name)
        );
    }

    w_status_choice->SetSelection(TrxStatus::e_reconciled);
    mmToolTip(w_status_choice, _t("Specify the status for this transaction"));

    transPanelSizer->Add(new wxStaticText(this, wxID_STATIC, _t("Status")), g_flagsH);
    transPanelSizer->Add(w_status_choice, g_flagsH);

    // Payee ------------------------------------------------
    w_payee_label = new wxStaticText(this, ID_TRANS_PAYEE_BUTTON_TEXT, _t("Payee"));
    w_payee_btn = new wxButton(this, ID_TRANS_PAYEE_BUTTON, _t("Select Payee"), wxDefaultPosition, std_size, 0);
    mmToolTip(w_payee_btn, _t("Specify a person, Company or Organisation for this transaction."));
    transPanelSizer->Add(w_payee_label, g_flagsH);
    transPanelSizer->Add(w_payee_btn, g_flagsH);

    // Category ---------------------------------------------
    wxStaticText* category_button_text = new wxStaticText(this, wxID_STATIC, _t("Category"));
    transPanelSizer->Add(category_button_text, g_flagsH);

    w_cat_text = new mmComboBoxCategory(this, ID_TRANS_CATEGORY_COMBOBOX, std_size, m_category_id, true);
    mmToolTip(w_cat_text, _t("Specify the category for this transaction"));

    wxBitmapButton* bSplit_ = new wxBitmapButton(this, mmID_CATEGORY_SPLIT, mmImage::bitmapBundle(mmImage::png::NEW_TRX, mmImage::bitmapButtonSize));
    bSplit_->Hide(); // TODO
    wxBoxSizer* split_sizer = new wxBoxSizer(wxHORIZONTAL);
    split_sizer->Add(w_cat_text, g_flagsH);
    split_sizer->Add(bSplit_, g_flagsH);

    transPanelSizer->Add(split_sizer, wxSizerFlags(g_flagsExpand).Border(0));

    // Number ---------------------------------------------
    w_number_text = new wxTextCtrl(this, ID_TRANS_ENTERED_NUMBER,
        "",
        wxDefaultPosition, std_size
    );
    mmToolTip(w_number_text, _t("Specify a transaction code or associated check details"));

    transPanelSizer->Add(new wxStaticText(this, wxID_STATIC, _t("Number")), g_flagsH);
    transPanelSizer->Add(w_number_text, g_flagsH);

    // Notes ---------------------------------------------
    transPanelSizer->Add(new wxStaticText(this, wxID_STATIC, _t("Notes")), g_flagsH);

    // Attachment ---------------------------------------------
    w_att_btn = new wxBitmapButton(this, wxID_FILE, mmImage::bitmapBundle(mmImage::png::CLIP, mmImage::bitmapButtonSize));
    mmToolTip(w_att_btn, _t("Manage transaction attachments"));
    //TODO: w_att_btn Enable/disable
    //w_att_btn->Enable(false);

    // Frequent Notes ---------------------------------------------
    w_freq_notes_btn = new wxButton(this, ID_TRANS_FREQUENT_NOTES, "..."
        , wxDefaultPosition, wxSize(w_att_btn->GetSize().GetX(), -1));
    mmToolTip(w_freq_notes_btn, _t("Select one of the frequently used notes"));

    wxBoxSizer* right_align_sizer = new wxBoxSizer(wxHORIZONTAL);
    right_align_sizer->Add(w_att_btn, g_flagsH);
    right_align_sizer->Add(w_freq_notes_btn, g_flagsH);

    w_notes_text = new wxTextCtrl(this, ID_TRANS_ENTERED_NOTES, "", wxDefaultPosition, wxSize(220, 96), wxTE_MULTILINE);
    mmToolTip(w_notes_text, _t("Specify any text notes you want to add to this transaction."));

    transPanelSizer->Add(right_align_sizer, wxSizerFlags(g_flagsH).Align(wxALIGN_RIGHT).Border(wxALL, 0));
    main_panel_sizer->Add(w_notes_text, wxSizerFlags(g_flagsExpand).Border(wxTOP, 5));

    return true;
}

void TrxLinkDialog::DataToControls()
{
    if (!m_transaction_n)
        return;

    wxDateTime trans_date = m_transaction_n->m_datetime.dateTime();
    TransactionDate(trans_date);

    m_transaction_id = m_transaction_n->m_id;
    m_account_id = m_transaction_n->m_account_id;
    w_account_btn->SetLabelText(AccountModel::instance().get_id_name(m_account_id));
    w_type_choice->SetSelection(m_transaction_n->m_type.id());

    if (m_account_id > 0) {
        const CurrencyData* currency = AccountModel::instance().get_id_currency_p(m_account_id);
        w_currency_btn->SetLabelText(currency->m_symbol);
        w_amount_text->SetCurrency(currency);
    }

    SetTransactionValue(m_transaction_n->m_amount);
    w_status_choice->SetSelection(m_transaction_n->m_status.id());

    m_payee_id = m_transaction_n->m_payee_id_n;
    w_payee_btn->SetLabelText(PayeeModel::instance().get_id_name(m_payee_id));

    m_category_id = m_transaction_n->m_category_id_n;
    w_cat_text->SetValue(CategoryModel::instance().get_id_fullname(m_category_id));

    w_number_text->SetValue(m_transaction_n->m_number);
    w_notes_text->SetValue(m_transaction_n->m_notes);

    bool has_split = !(m_local_splits.size() <= 1);
    if (has_split) {
        w_cat_text->Enable(!has_split);
        w_cat_text->SetLabelText(_t("Split Transaction"));

        SetTransactionValue(TrxSplitModel::instance().get_total(m_local_splits));
    }

    if (m_transaction_n->is_deleted()) {
        w_date_picker->Enable(false);
        w_account_btn->Enable(false);
        w_type_choice->Enable(false);
        w_status_choice->Enable(false);
        w_transfer_cb->Enable(false);
        w_amount_text->Enable(false);
        w_currency_btn->Enable(false);
        w_payee_btn->Enable(false);
        w_cat_text->Enable(false);
        w_number_text->Enable(false);
        w_att_btn->Enable(false);
        w_notes_text->Enable(false);
        w_freq_notes_btn->Enable(false);
    }
}

void TrxLinkDialog::BindEventsAndTrigger()
{
    w_type_choice->Bind(wxEVT_CHOICE, [this](wxCommandEvent&)
    {
        int selection = w_type_choice->GetSelection();
        w_payee_label->Show(selection != TrxType::e_transfer);
        w_payee_btn->Show(selection != TrxType::e_transfer);

        this->Layout();
    });
    wxCommandEvent evt(wxEVT_CHOICE, ID_TRANS_TYPE);
    wxPostEvent(w_type_choice, evt);

    w_freq_notes_btn->Bind(wxEVT_BUTTON, &TrxLinkDialog::OnFrequentNotes, this);
}

void TrxLinkDialog::SetLastPayeeAndCategory(const int64 account_id)
{
    if (PrefModel::instance().getTransPayeeNone() != PrefModel::LASTUSED)
        return;

    for (const TrxData& trx_d : TrxModel::instance().find_data_a(
        TrxModel::WHERE_TYPE(OP_NE, TrxType(TrxType::e_transfer)),
        TrxCol::WHERE_ACCOUNTID(OP_EQ, account_id),
        TableClause::ORDERBY(TrxCol::NAME_TRANSID, true),
        TableClause::LIMIT(1)
    )) {
        const PayeeData* last_payee_n = PayeeModel::instance().get_id_data_n(
            trx_d.m_payee_id_n
        );
        if (last_payee_n) {
            w_payee_btn->SetLabelText(last_payee_n->m_name);
            m_payee_id = last_payee_n->m_id;
            if (PrefModel::instance().getTransCategoryNone() == PrefModel::LASTUSED &&
                CategoryModel::instance().get_id_active(last_payee_n->m_category_id_n)
            ) {
                m_category_id = last_payee_n->m_category_id_n;
                w_cat_text->SetLabelText(CategoryModel::instance().get_id_fullname(
                    last_payee_n->m_category_id_n
                ));
            }
        }
        break;
    }
}

void TrxLinkDialog::OnTransAccountButton(wxCommandEvent& WXUNUSED(event))
{
    const auto& account_a = AccountModel::instance().find_all_name_a();
    mmSingleChoice scd(this,
        _t("Select the required account"),
        _t("Account Selection"),
        account_a
    );

    if (scd.ShowModal() == wxID_OK) {
        SetTransactionAccount(scd.GetStringSelection());
    }
}

void TrxLinkDialog::OnTransPayeeButton(wxCommandEvent& WXUNUSED(event))
{
    mmPayeeDialog dlg(this, true);
    if (dlg.ShowModal() != wxID_OK)
        return;

    m_payee_id = dlg.getPayeeId();
    const PayeeData* payee_n = PayeeModel::instance().get_id_data_n(m_payee_id);
    if (!payee_n)
        return;

    w_payee_btn->SetLabelText(payee_n->m_name);

    // Only for new transactions: if user want to autofill last category used
    // for payee and category has not been set.
    if (PrefModel::instance().getTransCategoryNone() == PrefModel::LASTUSED &&
        m_category_id < 0 &&
        m_subcategory_id < 0 &&
        CategoryModel::instance().get_id_active(payee_n->m_category_id_n) &&
        payee_n->m_category_id_n > 0
    ) {
        m_category_id = payee_n->m_category_id_n;
        w_cat_text->SetLabelText(
            CategoryModel::instance().get_id_fullname(m_category_id)
        );
    }
}

void TrxLinkDialog::OnTransCategoryCombobox(wxCommandEvent& WXUNUSED(event))
{
    if (!w_cat_text->GetValue().empty()) {
        m_category_id = w_cat_text->mmGetCategoryId();
        return;
    }
    CategoryManager dlg(this, true, m_category_id);
    if (dlg.ShowModal() == wxID_OK) {
        m_category_id = dlg.getCategId();
        w_cat_text->SetLabelText(CategoryModel::instance().get_id_fullname(m_category_id));
    }
}

void TrxLinkDialog::OnFrequentNotes(wxCommandEvent& WXUNUSED(event))
{
    std::vector<wxString> frequent_notes;
    TrxModel::instance().getFrequentUsedNotes(frequent_notes);
    wxMenu menu;
    int id = wxID_HIGHEST;
    for (const auto& entry : frequent_notes) {
        const wxString& label = entry.Mid(0, 30) + (entry.size() > 30 ? "..." : "");
        int menu_id = ++id;
        menu.Append(menu_id, label);
        Bind(wxEVT_MENU,
            [this, notes = frequent_notes, i = menu_id - wxID_HIGHEST](wxCommandEvent&) {
                if (i > 0 && i <= static_cast<int>(notes.size()))
                    w_notes_text->ChangeValue(notes[i - 1]);
            },
            menu_id
        );
    }

    if (!frequent_notes.empty())
        PopupMenu(&menu);
}

void TrxLinkDialog::OnAttachments(wxCommandEvent& WXUNUSED(event))
{
    int64 RefId = m_transaction_id;

    if (RefId < 0)
        RefId = 0;

    AttachmentDialog dlg(this, TrxModel::s_ref_type, RefId);
    dlg.ShowModal();
}

bool TrxLinkDialog::ValidCheckingAccountEntry()
{
    m_category_id = w_cat_text->mmGetCategoryId();  // update from selection
    return m_account_id != -1 &&
        (m_payee_id != -1 || TransactionType() == TrxType::e_transfer) &&
        m_category_id != -1 &&
        !w_amount_text->GetValue().IsEmpty();
}

wxDateTime TrxLinkDialog::TransactionDate()
{
    return w_date_picker->GetValue();
}

void TrxLinkDialog::TransactionDate(const wxDateTime& trans_date)
{
    w_date_picker->setValue(trans_date);
}

void TrxLinkDialog::SetTransactionValue(const double& trans_value, bool fixed_value)
{
    w_amount_text->SetValue(trans_value);
    if (fixed_value) {
        w_amount_text->Enable(false);
    }
}

void TrxLinkDialog::SetTransactionNumber(const wxString& trans_number)
{
    w_number_text->SetValue(trans_number);
}

void TrxLinkDialog::SetTransactionStatus(const int trans_status_enum)
{
    w_status_choice->SetSelection(trans_status_enum);
}

void TrxLinkDialog::SetTransactionPayee(const int64 payeeid)
{
    m_payee_id = payeeid;
    const PayeeData* payee_n = PayeeModel::instance().get_id_data_n(m_payee_id);
    if (payee_n)
        w_payee_btn->SetLabelText(payee_n->m_name);
}

void TrxLinkDialog::SetTransactionCategory(const int64 categid)
{
    m_category_id = categid;
    w_cat_text->SetLabelText(CategoryModel::instance().get_id_fullname(m_category_id));
}

void TrxLinkDialog::SetTransactionAccount(const wxString& trans_account)
{
    const AccountData* account = AccountModel::instance().get_name_data_n(trans_account);
    if (account) {
        w_account_btn->SetLabelText(account->m_name);
        m_account_id = account->m_id;
        SetLastPayeeAndCategory(m_account_id);
        const CurrencyData* currency = CurrencyModel::instance().get_id_data_n(account->m_currency_id);
        w_amount_text->SetCurrency(currency);
        w_currency_btn->SetLabelText(currency->m_symbol);
    }
}

TrxLinkModel::CHECKING_TYPE TrxLinkDialog::CheckingType()
{
    if (w_transfer_cb->IsChecked())
        return TrxLinkModel::AS_TRANSFER;
    else
        return TrxLinkModel::AS_INCOME_EXPENSE;
}

void TrxLinkDialog::CheckingType(TrxLinkModel::CHECKING_TYPE ct)
{
    w_transfer_cb->SetValue(false);
    if (ct == TrxLinkModel::AS_TRANSFER) {
        w_transfer_cb->SetValue(true);
    }
}

int64 TrxLinkDialog::SaveChecking()
{
    double initial_amount = 0;
    w_amount_text->checkValue(initial_amount);

    const AccountData* account = AccountModel::instance().get_id_data_n(m_account_id);
    wxDateTime trx_datetime = w_date_picker->GetValue();
    if (mmDate(trx_datetime) < account->m_open_date) {
        mmErrorDialogs::ToolTip4Object(w_account_btn,
            _t("The opening date for the account is later than the date of this transaction"),
            _t("Invalid Date")
        );
        return -1;
    }

    if (!m_transaction_n) {
        m_transaction_d = TrxData();
        // only set if new transaction, otherwise retain old value!
        m_transaction_d.m_color = 0;
        m_transaction_d.m_followup_id = 0;
        m_transaction_n = &m_transaction_d;
    }

    m_transaction_n->m_account_id = m_account_id;
    m_transaction_n->m_to_account_id_n = (
        TransactionType() == TrxType::e_transfer ||
        CheckingType() == TrxLinkModel::AS_TRANSFER
    ) ? m_account_id : -1; // Self Transfer as Revaluation

    m_transaction_n->m_datetime      = mmDateTime(trx_datetime);
    m_transaction_n->m_type          = TrxType(TransactionType());
    m_transaction_n->m_status        = TrxStatus(w_status_choice->GetStringSelection().Mid(0, 1));
    m_transaction_n->m_payee_id_n    = m_payee_id;
    m_transaction_n->m_category_id_n = m_category_id;
    m_transaction_n->m_amount        = initial_amount;
    m_transaction_n->m_to_amount     = m_transaction_n->m_amount;
    m_transaction_n->m_number        = w_number_text->GetValue();
    m_transaction_n->m_notes         = w_notes_text->GetValue();

    TrxModel::instance().unsafe_save_trx_n(m_transaction_n);
    return m_transaction_n->id();
}

int TrxLinkDialog::TransactionType()
{
    return w_type_choice->GetSelection();
}


void TrxLinkDialog::OnCategs(wxCommandEvent& WXUNUSED(event))
{
    if (m_local_splits.empty() && w_cat_text->mmIsValid()) {
        Split split_d;

        w_amount_text->GetDouble(split_d.m_amount);

        split_d.m_category_id = w_cat_text->mmGetCategoryId();
        split_d.m_notes       = w_notes_text->GetValue();
        m_local_splits.push_back(split_d);
    }

    SplitDialog dlg(this, m_local_splits, m_account_id);

    if (dlg.ShowModal() == wxID_OK) {
        m_local_splits = dlg.mmGetResult();

        if (m_local_splits.size() == 1) {
            w_cat_text->SetLabelText(
                CategoryModel::instance().get_id_fullname(m_local_splits[0].m_category_id)
            );
            w_amount_text->SetValue(m_local_splits[0].m_amount);
            w_notes_text->SetValue(m_local_splits[0].m_notes);

            m_local_splits.clear();
        }

        if (m_local_splits.empty()) {
            w_cat_text->Enable(true);
        }
        else {
            w_amount_text->SetValue(TrxSplitModel::instance().get_total(m_local_splits));

            w_cat_text->Enable(false);
            w_cat_text->SetLabelText(_t("Split Transaction"));
        }
    }
}
