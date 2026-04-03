/*******************************************************
 Copyright (C) 2017 - 2022 Nikolay Akimov
 Copyright (C) 2021-2022 Mark Whalley (mark@ipx.co.uk)

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
#include <wx/statline.h>

#include "base/_constants.h"
#include "util/mmPath.h"
#include "base/images_list.h"
#include "util/_simple.h"
#include "util/mmTextCtrl.h"
#include "util/mmCalcValidator.h"

#include "model/AccountModel.h"
#include "model/CurrencyHistoryModel.h"
#include "model/CurrencyModel.h"
#include "model/PayeeModel.h"
#include "model/TrxModel.h"

#include "manager/CategoryManager.h"
#include "manager/PayeeManager.h"
#include "TrxUpdateDialog.h"

wxIMPLEMENT_DYNAMIC_CLASS(TrxUpdateDialog, wxDialog);

wxBEGIN_EVENT_TABLE(TrxUpdateDialog, wxDialog)
    EVT_BUTTON(wxID_OK,             TrxUpdateDialog::OnOk)
    EVT_BUTTON(ID_BTN_CUSTOMFIELDS, TrxUpdateDialog::OnMoreFields)
    EVT_CHECKBOX(wxID_ANY,          TrxUpdateDialog::OnCheckboxClick)
    EVT_CHILD_FOCUS(                TrxUpdateDialog::onFocusChange)
    EVT_CHAR_HOOK(                  TrxUpdateDialog::OnComboKey)
    EVT_CHOICE(ID_TRANS_TYPE,       TrxUpdateDialog::OnTransTypeChanged)
wxEND_EVENT_TABLE()

TrxUpdateDialog::TrxUpdateDialog()
{
}

TrxUpdateDialog::~TrxUpdateDialog()
{
}

// Used to determine if we need to refresh the tag text ctrl after
// accelerator hints are shown which only occurs once.
static bool altRefreshDone;

TrxUpdateDialog::TrxUpdateDialog(
    wxWindow* parent,
    std::vector<int64>& trx_id_a
) :
    m_trx_id_a(trx_id_a)
{
    m_currency_n = CurrencyModel::instance().get_base_data_n(); // base currency if we need it

    // Determine the mix of transaction that have been selected
    for (const auto& trx_id : m_trx_id_a) {
        const TrxData* trx_n = TrxModel::instance().get_id_data_n(trx_id);
        const bool isTransfer = trx_n->is_transfer();

        if (!m_hasSplits) {
            TrxSplitModel::DataA tp_a = TrxSplitModel::instance().find(
                TrxSplitCol::TRANSID(trx_id)
            );
            if (!tp_a.empty())
                m_hasSplits = true;
        }

        if (!m_hasTransfers && isTransfer)
            m_hasTransfers = true;

        if (!m_hasNonTransfers && !isTransfer)
            m_hasNonTransfers = true;
    }

    m_custom_fields = new mmCustomDataTransaction(this, TrxModel::s_ref_type, 0, ID_CUSTOMFIELDS);

    this->SetFont(parent->GetFont());
    Create(parent);
}

bool TrxUpdateDialog::Create(
    wxWindow* parent,
    wxWindowID id,
    const wxString& caption,
    const wxPoint& pos,
    const wxSize& size, long style
) {
    altRefreshDone = false; // reset the ALT refresh indicator on new dialog creation
    SetExtraStyle(GetExtraStyle() | wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create(parent, id, wxGetTranslation(caption), pos, size, style);

    CreateControls();

    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    this->SetInitialSize();
    SetMinSize(wxSize(300, 400));
    SetIcon(mmPath::getProgramIcon());
    Centre();

    SetEvtHandlerEnabled(true);
    return true;
}

void TrxUpdateDialog::CreateControls()
{
    wxBoxSizer* box_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* box_sizer1 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* box_sizer2 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* custom_fields_box_sizer = new wxBoxSizer(wxVERTICAL);
    box_sizer->Add(box_sizer1, g_flagsExpand);
    box_sizer1->Add(box_sizer2, g_flagsExpand);
    box_sizer1->Add(custom_fields_box_sizer, g_flagsExpand);

    wxStaticBox* static_box = new wxStaticBox(this, wxID_ANY, _t("Specify"));
    wxStaticBoxSizer* box_sizer_left = new wxStaticBoxSizer(static_box, wxVERTICAL);
    wxFlexGridSizer* grid_sizer = new wxFlexGridSizer(0, 2, 0, 0);
    grid_sizer->AddGrowableCol(1, 1);
    box_sizer_left->Add(grid_sizer, g_flagsV);
    box_sizer2->Add(box_sizer_left, g_flagsExpand);

    // Date --------------------------------------------
    w_date_cb = new wxCheckBox(this, wxID_ANY, _t("Date")
        , wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);
    w_date_picker = new mmDatePickerCtrl(this, wxID_ANY);
    grid_sizer->Add(w_date_cb, g_flagsH);
    grid_sizer->Add(w_date_picker->mmGetLayout(false), wxSizerFlags(g_flagsH).Border(wxLEFT, 0));
    w_date_picker->Enable(false);

    if (PrefModel::instance().getUseTransDateTime())
    {
        // Time --------------------------------------------
        w_time_cb = new wxCheckBox(this, wxID_ANY, _t("Time"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);
        w_time_picker = new wxTimePickerCtrl(this, wxID_ANY);
        grid_sizer->Add(w_time_cb, g_flagsH);
        grid_sizer->Add(w_time_picker, g_flagsH);
        w_time_picker->Enable(false);
    }

    // Status --------------------------------------------
    w_status_cb = new wxCheckBox(this, wxID_ANY, _t("Status")
        , wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);

    w_status_choice = new wxChoice(this, wxID_ANY
        , wxDefaultPosition, wxDefaultSize);
    for (int i = 0; i < TrxStatus::size; ++i) {
        wxString name = TrxStatus(i).name();
        w_status_choice->Append(wxGetTranslation(name), new wxStringClientData(name));
    }

    w_status_choice->Enable(false);
    w_status_choice->Select(0);

    grid_sizer->Add(w_status_cb, g_flagsH);
    grid_sizer->Add(w_status_choice, g_flagsExpand);

    // Type --------------------------------------------
    w_type_cb = new wxCheckBox(this, wxID_ANY, _t("Type")
        , wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);

    w_type_choice = new wxChoice(this, ID_TRANS_TYPE
        , wxDefaultPosition, wxDefaultSize);
    for (int i = 0; i < TrxType::size; ++i) {
        if (!(m_hasSplits && i == TrxType::e_transfer)) {
            wxString name = TrxType(i).name();
            w_type_choice->Append(wxGetTranslation(name), new wxStringClientData(name));
        }
    }
    w_type_choice->Enable(false);
    w_type_choice->Select(0);


    grid_sizer->Add(w_type_cb, g_flagsH);
    grid_sizer->Add(w_type_choice, g_flagsExpand);

    // Amount Field --------------------------------------------
    w_amount_cb = new wxCheckBox(this, wxID_ANY, _t("Amount")
        , wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);
    w_amount_cb->Enable(!m_hasSplits);

    w_amount_text = new mmTextCtrl(this, wxID_ANY, ""
        , wxDefaultPosition, wxDefaultSize
        , wxALIGN_RIGHT | wxTE_PROCESS_ENTER, mmCalcValidator());
    w_amount_text->Enable(false);

    grid_sizer->Add(w_amount_cb, g_flagsH);
    grid_sizer->Add(w_amount_text, g_flagsExpand);

    // Payee --------------------------------------------
    w_payee_cb = new wxCheckBox(this, wxID_ANY, _t("Payee")
        , wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);
    w_payee_cb->Enable(!m_hasTransfers);

    w_payee_text = new mmComboBoxPayee(this, mmID_PAYEE, wxDefaultSize, -1, true);
    w_payee_text->Enable(false);
    w_payee_text->SetMinSize(wxSize(200, -1));

    grid_sizer->Add(w_payee_cb, g_flagsH);
    grid_sizer->Add(w_payee_text, g_flagsExpand);

    // Transfer to account --------------------------------------------
    m_transferAcc_checkbox = new wxCheckBox(this, wxID_ANY, _t("Transfer To")
        , wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);
    m_transferAcc_checkbox->Enable(!m_hasNonTransfers);

    w_account_text = new mmComboBoxAccount(this, ID_TRANS_ACC);
    w_account_text->SetMaxSize(w_payee_text->GetSize());
    w_account_text->Enable(false);

    grid_sizer->Add(m_transferAcc_checkbox, g_flagsH);
    grid_sizer->Add(w_account_text, g_flagsExpand);

    // Category -------------------------------------------------
    w_cat_cb = new wxCheckBox(this, wxID_VIEW_DETAILS, _t("Category")
        , wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);
    w_cat_cb->Enable(!m_hasSplits);

    w_cat_text = new mmComboBoxCategory(this, mmID_CATEGORY, wxDefaultSize, -1, true);
    w_cat_text->Enable(false);

    grid_sizer->Add(w_cat_cb, g_flagsH);
    grid_sizer->Add(w_cat_text, g_flagsExpand);

    // Tags ------------------------------------------------------
    w_tag_cb = new wxCheckBox(this, wxID_ANY, _t("Tags"));
    tag_append_checkbox_ = new wxCheckBox(this, wxID_ANY, _t("Append"));
    tag_append_checkbox_->SetValue(true);
    tag_append_checkbox_->Enable(false);

    w_tag_text = new mmTagTextCtrl(this);
    w_tag_text->Enable(false);
    grid_sizer->Add(w_tag_cb, g_flagsH);
    grid_sizer->Add(tag_append_checkbox_, g_flagsH);
    grid_sizer->AddSpacer(0);
    grid_sizer->Add(w_tag_text, g_flagsExpand);

    // Colours --------------------------------------------
    w_color_cb = new wxCheckBox(this, wxID_VIEW_DETAILS, _t("Color")
        , wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);
    w_color_btn = new mmColorButton(this, wxID_HIGHEST, w_cat_text->GetSize());
    mmToolTip(w_color_btn, _t("User Colors"));
    grid_sizer->Add(w_color_cb, g_flagsH);
    grid_sizer->Add(w_color_btn, g_flagsExpand);
    w_color_btn->Enable(false);

    // Notes --------------------------------------------
    w_notes_cb = new wxCheckBox(this, wxID_ANY, _t("Notes")
        , wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);
    m_append_checkbox = new wxCheckBox(this, wxID_ANY, _t("Append")
        , wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);
    m_append_checkbox->SetValue(true);

    w_notes_text = new wxTextCtrl(this, wxID_ANY, ""
        , wxDefaultPosition, wxSize(-1, w_cat_text->GetSize().GetHeight() * 5), wxTE_MULTILINE);
    w_notes_text->Enable(false);
    m_append_checkbox->Enable(false);

    grid_sizer->Add(w_notes_cb, g_flagsH);
    grid_sizer->Add(m_append_checkbox, g_flagsH);
    box_sizer_left->Add(w_notes_text, wxSizerFlags(g_flagsExpand).Border(wxLEFT | wxRIGHT | wxBOTTOM, 10));

    /*************************************************************
     Button Panel with OK/Cancel buttons
    *************************************************************/
    wxPanel* button_panel = new wxPanel(this, wxID_ANY
        , wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    box_sizer_left->Add(button_panel, wxSizerFlags(g_flagsV).Center());

    wxStdDialogButtonSizer* button_sizer = new wxStdDialogButtonSizer;
    button_panel->SetSizer(button_sizer);

    wxButton* button_ok = new wxButton(button_panel, wxID_OK, _t("&OK "));
    wxButton* button_cancel = new wxButton(button_panel
        , wxID_CANCEL, wxGetTranslation(g_CancelLabel));
    button_cancel->SetFocus();

    wxBitmapButton* button_hide = new wxBitmapButton(button_panel
        , ID_BTN_CUSTOMFIELDS, mmBitmapBundle(png::RIGHTARROW, mmBitmapButtonSize));
    mmToolTip(button_hide, _t("Show/Hide custom fields window"));
    if (m_custom_fields->GetCustomFieldsCount() == 0) {
        button_hide->Hide();
    }

    button_sizer->Add(button_ok, wxSizerFlags(g_flagsH).Border(wxBOTTOM | wxRIGHT, 10));
    button_sizer->Add(button_cancel, wxSizerFlags(g_flagsH).Border(wxBOTTOM | wxRIGHT, 10));
    button_sizer->Add(button_hide, wxSizerFlags(g_flagsH).Border(wxBOTTOM | wxRIGHT, 10));
    button_sizer->Realize();

    // Custom fields -----------------------------------

    m_custom_fields->FillCustomFields(custom_fields_box_sizer);
    if (m_custom_fields->GetActiveCustomFieldsCount() > 0) {
        wxCommandEvent evt(wxEVT_BUTTON, ID_BTN_CUSTOMFIELDS);
        this->GetEventHandler()->AddPendingEvent(evt);
    }

    Center();
    this->SetSizer(box_sizer);
}

void TrxUpdateDialog::OnOk(wxCommandEvent& WXUNUSED(event))
{
    double amount = 0;
    if (w_amount_cb->IsChecked() && !w_amount_text->checkValue(amount))
        return;

    wxString status = "";
    if (w_status_cb->IsChecked()) {
        wxStringClientData* status_obj = static_cast<wxStringClientData*>(
            w_status_choice->GetClientObject(w_status_choice->GetSelection())
        );
        if (status_obj)
            status = TrxStatus(status_obj->GetData()).key();
        else
            return;
    }

    TrxType trx_type = TrxType();
    if (w_type_cb->IsChecked()) {
        wxStringClientData* type_obj = static_cast<wxStringClientData*>(
            w_type_choice->GetClientObject(w_type_choice->GetSelection())
        );
        trx_type = TrxType(type_obj->GetData());
        if (trx_type.id() == TrxType::e_transfer) {
            if  (m_hasNonTransfers && !m_transferAcc_checkbox->IsChecked())
                return mmErrorDialogs::InvalidAccount(m_transferAcc_checkbox, true);
        } else {
            if (m_hasTransfers && !w_payee_cb->IsChecked())
                return mmErrorDialogs::InvalidPayee(w_payee_cb);
        }
    }

    int64 payee_id = -1;
    if (w_payee_cb->IsChecked()) {
        wxString payee_name = w_payee_text->GetValue();
        if (payee_name.IsEmpty())
            return mmErrorDialogs::InvalidPayee(w_payee_text);

        if (!w_payee_text->mmIsValid()) {
            wxMessageDialog msgDlg(this
                , wxString::Format(_t("Payee name has not been used before. Is the name correct?\n%s"), w_payee_text->GetValue())
                , _t("Confirm payee name")
                , wxYES_NO | wxYES_DEFAULT | wxICON_WARNING);
            if (msgDlg.ShowModal() == wxID_YES) {
                PayeeData new_payee_d = PayeeData();
                new_payee_d.m_name = w_payee_text->GetValue();
                PayeeModel::instance().add_data_n(new_payee_d);
                mmWebApp::uploadPayee();
                payee_id = new_payee_d.m_id;
            }
            else
                return;
        }
        else
            payee_id = w_payee_text->mmGetId();
    }

    if (w_tag_cb->IsChecked() && !w_tag_text->IsValid())
        return mmErrorDialogs::ToolTip4Object(w_tag_text,
            _t("Invalid value"),
            _t("Tags"),
            wxICON_ERROR
        );

    if (m_transferAcc_checkbox->IsChecked()) {
        if (!w_account_text->mmIsValid())
            return mmErrorDialogs::InvalidAccount(w_account_text);
    }

    if (w_cat_cb->IsChecked()) {
        if (!w_cat_text->mmIsValid())
            return mmErrorDialogs::InvalidCategory(w_cat_text);
    }
    int64 categ_id = w_cat_text->mmGetCategoryId();

    // const auto split = TrxSplitModel::instance().find_all_mTrxId();

    std::vector<int64> skip_trx;
    TrxModel::instance().db_savepoint();
    TagLinkModel::instance().db_savepoint();
    for (const auto& trx_id : m_trx_id_a) {
        TrxData* trx_n = TrxModel::instance().unsafe_get_id_data_n(trx_id);
        bool is_locked = TrxModel::instance().is_locked(*trx_n);

        if (is_locked) {
            skip_trx.push_back(trx_n->m_id);
            continue;
        }

        if (w_status_cb->IsChecked()) {
            trx_n->m_status = TrxStatus(status);
        }

        if (w_payee_cb->IsChecked()) {
            trx_n->m_payee_id_n = payee_id;
            trx_n->m_to_account_id_n = -1;
        }

        if (m_transferAcc_checkbox->IsChecked()) {
            trx_n->m_to_account_id_n = w_account_text->mmGetId();
            trx_n->m_payee_id_n = -1;
        }

        if (w_date_cb->IsChecked() || (w_time_picker && w_time_cb->IsChecked())) {
            wxString date_s = trx_n->m_isoDateTime();
            if (w_date_cb->IsChecked()) {
                date_s.replace(0, 10, w_date_picker->GetValue().FormatISODate());
                const AccountData* account = AccountModel::instance().get_id_data_n(
                    trx_n->m_account_id
                );
                const AccountData* to_account = AccountModel::instance().get_id_data_n(
                    trx_n->m_to_account_id_n
                );
                if ((mmDate(date_s) < account->m_open_date) ||
                    (to_account && (mmDate(date_s) < to_account->m_open_date))
                ) {
                    skip_trx.push_back(trx_n->m_id);
                    continue;
                }
            }

            if (w_time_picker && w_time_cb->IsChecked()) {
                if (date_s.Length() > 10)
                    date_s.replace(11, 8, w_time_picker->GetValue().FormatISOTime());
                else
                    date_s.Append("T" + w_time_picker->GetValue().FormatISOTime());
            }

            trx_n->m_datetime = mmDateTime(date_s);
        }

        if (w_color_cb->IsChecked()) {
            int color_id = w_color_btn->GetColorId();
            if (color_id < 0 || color_id > 7) {
                return mmErrorDialogs::ToolTip4Object(w_color_btn,
                    _t("Color"), _t("Invalid value"), wxICON_ERROR
                );
            }
            trx_n->m_color = color_id == 0 ? -1 : color_id ; 
        }

        if (w_notes_cb->IsChecked()) {
            if (m_append_checkbox->IsChecked()) {
                trx_n->m_notes += (trx_n->m_notes.Right(1) == "\n" || trx_n->m_notes.empty()
                    ? "" : "\n")
                    + w_notes_text->GetValue();
            }
            else {
                trx_n->m_notes = w_notes_text->GetValue();
            }
        }

        // Update tags
        if (w_tag_cb->IsChecked()) {
            TagLinkModel::DataA gl_a;
            wxArrayInt64 tag_id_a = w_tag_text->GetTagIDs();

            if (tag_append_checkbox_->IsChecked()) {
                // Since we are appending, start with the existing tags
                gl_a = TagLinkModel::instance().find(
                    TagLinkCol::REFTYPE(TrxModel::s_ref_type.key_n()),
                    TagLinkCol::REFID(trx_n->m_id)
                );
                // Remove existing tags from the new list to avoid duplicates
                for (const auto& gl_d : gl_a) {
                    auto index = std::find(tag_id_a.begin(), tag_id_a.end(), gl_d.m_tag_id);
                    if (index != tag_id_a.end())
                        tag_id_a.erase(index);
                }
            }
            // Create new taglinks for each tag ID
            for (const auto& tag_id : tag_id_a) {
                TagLinkData new_gl_d = TagLinkData();
                new_gl_d.m_tag_id   = tag_id;
                new_gl_d.m_ref_type = TrxModel::s_ref_type;
                new_gl_d.m_ref_id   = trx_n->m_id;
                gl_a.push_back(new_gl_d);
            }
            // Update the links for the transaction
            TagLinkModel::instance().update(
                TrxModel::s_ref_type, trx_n->m_id,
                gl_a
            );
        }

        if (w_amount_cb->IsChecked()) {
            trx_n->m_amount = amount;
        }

        if (w_cat_cb->IsChecked()) {
            trx_n->m_category_id_n = categ_id;
        }

        if (w_type_cb->IsChecked()) {
            trx_n->m_type = trx_type;
        }

        // Need to consider m_to_amount if material transaction change
        if (w_amount_cb->IsChecked() || w_type_cb->IsChecked() ||
            m_transferAcc_checkbox->IsChecked()
        ) {
            if (!trx_n->is_transfer()) {
                trx_n->m_to_amount = trx_n->m_amount;
            }
            else {
                const auto acc = AccountModel::instance().get_id_data_n(trx_n->m_account_id);
                const auto curr = CurrencyModel::instance().get_id_data_n(acc->m_currency_id);
                const auto to_acc = AccountModel::instance().get_id_data_n(trx_n->m_to_account_id_n);
                const auto to_curr = CurrencyModel::instance().get_id_data_n(to_acc->m_currency_id);
                if (curr == to_curr) {
                    trx_n->m_to_amount = trx_n->m_amount;
                }
                else {
                    double exch = 1;
                    const double convRateTo = CurrencyHistoryModel::instance().get_id_date_rate(
                        to_curr->m_id,
                        trx_n->m_date()
                    );
                    if (convRateTo > 0) {
                        const double convRate = CurrencyHistoryModel::instance().get_id_date_rate(
                            curr->m_id,
                            trx_n->m_date()
                        );
                        exch = convRate / convRateTo;
                    }
                    trx_n->m_to_amount = trx_n->m_amount * exch;
                }
            }
        }

        m_custom_fields->UpdateCustomValues(TrxModel::s_ref_type, trx_id);

        TrxModel::instance().unsafe_save_trx_n(trx_n);
    }
    TagLinkModel::instance().db_release_savepoint();
    TrxModel::instance().db_release_savepoint();
    if (!skip_trx.empty()) {
        const wxString detail = wxString::Format("%s\n%s: %zu\n%s: %zu",
            _t("This is due to some elements of the transaction or account detail not allowing the update"),
            _t("Updated"),
            m_trx_id_a.size() - skip_trx.size(),
            _t("Not updated"), skip_trx.size()
        );
        mmErrorDialogs::MessageWarning(this,
            detail,
            _t("Unable to update some transactions.")
        );
    }
    //TODO: enable report to detail transactions that are unable to be updated

    EndModal(wxID_OK);
}

void TrxUpdateDialog::SetPayeeTransferControls()
{
    wxStringClientData* trans_obj = static_cast<wxStringClientData*>(
        w_type_choice->GetClientObject(w_type_choice->GetSelection())
    );
    bool transfer = (TrxType(trans_obj->GetData()).id() == TrxType::e_transfer);

    w_payee_cb->Enable(!transfer);
    m_transferAcc_checkbox->Enable(transfer);
    if (transfer) {
        w_payee_cb->SetValue(false);
        w_payee_text->Enable(false);
    }
    else {
        m_transferAcc_checkbox->SetValue(false);
        w_account_text->Enable(false);
    }
}

void TrxUpdateDialog::OnTransTypeChanged(wxCommandEvent&)
{
    SetPayeeTransferControls();
}

void TrxUpdateDialog::OnCheckboxClick(wxCommandEvent& event)
{
    w_date_picker->Enable(w_date_cb->IsChecked());
    w_status_choice->Enable(w_status_cb->IsChecked());
    w_type_choice->Enable(w_type_cb->IsChecked());
    w_payee_text->Enable(w_payee_cb->IsChecked());
    w_account_text->Enable(m_transferAcc_checkbox->IsChecked());
    w_cat_text->Enable(w_cat_cb->IsChecked());
    w_amount_text->Enable(w_amount_cb->IsChecked());
    w_color_btn->Enable(w_color_cb->IsChecked());
    w_notes_text->Enable(w_notes_cb->IsChecked());
    m_append_checkbox->Enable(w_notes_cb->IsChecked());
    w_tag_text->Enable(w_tag_cb->IsChecked());
    tag_append_checkbox_->Enable(w_tag_cb->IsChecked());

    if (w_type_cb->IsChecked()) {
        SetPayeeTransferControls();
    } else {
        w_payee_cb->Enable(!m_hasTransfers);
        m_transferAcc_checkbox->Enable(!m_hasNonTransfers);
    }

    if (w_time_picker)
        w_time_picker->Enable(w_time_cb->IsChecked());

    event.Skip();
}

void TrxUpdateDialog::onFocusChange(wxChildFocusEvent& event)
{
    w_cat_text->SetValue(w_cat_text->GetValue());


    int object_in_focus = -1;
    wxWindow *w = event.GetWindow();
    if (w) {
        object_in_focus = w->GetId();
    }

    if (object_in_focus == w_amount_text->GetId()) {
        w_amount_text->SelectAll();
    }
    else {
        w_amount_text->Calculate();
    }

    event.Skip();
}

void TrxUpdateDialog::OnMoreFields(wxCommandEvent& WXUNUSED(event))
{
    wxBitmapButton* button = static_cast<wxBitmapButton*>(FindWindow(ID_BTN_CUSTOMFIELDS));

    if (button)
        button->SetBitmap(mmBitmapBundle(
            m_custom_fields->IsCustomPanelShown()
                ? png::RIGHTARROW
                : png::LEFTARROW,
            mmBitmapButtonSize
        ));

    m_custom_fields->ShowHideCustomPanel();

    this->SetMinSize(wxSize(0, 0));
    this->Fit();
}

void TrxUpdateDialog::OnComboKey(wxKeyEvent& event)
{
    if (event.GetKeyCode() == WXK_RETURN) {
        auto id = event.GetId();
        switch (id) {
        case mmID_PAYEE:
        {
            const auto payeeName = w_payee_text->GetValue();
            if (payeeName.empty()) {
                mmPayeeDialog dlg(this, true);
                dlg.ShowModal();
                if (dlg.getRefreshRequested())
                    w_payee_text->mmDoReInitialize();
                int64 payee_id = dlg.getPayeeId();
                const PayeeData* payee_n = PayeeModel::instance().get_id_data_n(payee_id);
                if (payee_n) {
                    w_payee_text->ChangeValue(payee_n->m_name);
                    w_payee_text->SelectAll();
                }
                return;
            }
        }
        break;
        case mmID_CATEGORY:
        {
            auto category = w_cat_text->GetValue();
            if (category.empty()) {
                CategoryManager dlg(this, true, -1);
                dlg.ShowModal();
                if (dlg.getRefreshRequested())
                    w_cat_text->mmDoReInitialize();
                category = CategoryModel::instance().get_id_fullname(dlg.getCategId());
                w_cat_text->ChangeValue(category);
                w_cat_text->SelectAll();
                return;
            }
        }
        break;
        default:
            break;
        }
    }

    // The first time the ALT key is pressed accelerator hints are drawn, but custom painting on the tags button
    // is not applied. We need to refresh the tag ctrl to redraw the drop button with the correct image.
    if (event.AltDown() && !altRefreshDone) {
        w_tag_text->Refresh();
        altRefreshDone = true;
    }

    event.Skip();
}
