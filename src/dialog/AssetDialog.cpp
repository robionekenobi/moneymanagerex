/*******************************************************
 Copyright (C) 2013 - 2016, 2020, 2022 Nikolay Akimov
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

#include "AssetDialog.h"

#include "base/_defs.h"
#include <wx/valnum.h>

#include "base/_constants.h"
#include "util/mmPath.h"
#include "util/mmImage.h"
#include "util/mmNavigatorList.h"
#include "util/mmAttachment.h"
#include "util/_util.h"
#include "util/_simple.h"
#include "util/mmTextCtrl.h"
#include "util/mmCalcValidator.h"

#include "AccountDialog.h"
#include "AttachmentDialog.h"
#include "TrxLinkDialog.h"

wxIMPLEMENT_DYNAMIC_CLASS(AssetDialog, wxDialog);

wxBEGIN_EVENT_TABLE( AssetDialog, wxDialog )
    EVT_BUTTON(wxID_OK,         AssetDialog::onOk)
    EVT_BUTTON(wxID_CANCEL,     AssetDialog::onCancel)
    EVT_BUTTON(wxID_FILE,       AssetDialog::onAttachments)
    EVT_CHOICE(IDC_COMBO_TYPE,  AssetDialog::onChangeAppreciationType)
    EVT_CHOICE(IDC_COMPOUNDING, AssetDialog::onChangeCompounding)
    EVT_CHILD_FOCUS(            AssetDialog::onChangeFocus)
    EVT_CLOSE(                  AssetDialog::onQuit)
wxEND_EVENT_TABLE()

// -- constructor

AssetDialog::AssetDialog(
    wxWindow* parent_win,
    AssetData* asset_n,
    bool show_trx_panel
) :
    m_asset_n(asset_n),
    m_show_trx_panel(show_trx_panel)
{
    m_dialog_heading =
        m_show_trx_panel ? _t("Add Asset Transaction") :
        m_asset_n        ? _t("Edit Asset") :
                           _t("New Asset");

    this->SetFont(parent_win->GetFont());
    create(parent_win, wxID_ANY, m_dialog_heading);
}

AssetDialog::AssetDialog(
    wxWindow* parent_win,
    const TrxLinkData* tl_n,
    TrxData* trx_n
) :
    m_tl_n(tl_n),
    m_trx_n(trx_n),
    m_show_trx_panel(true)
{
    if (m_tl_n) {
        m_asset_n = AssetModel::instance().unsafe_get_idN_data_n(m_tl_n->m_ref_id);
    }

    m_dialog_heading = m_tl_n
        ? _t("Edit Asset Transaction")
        : _t("Add Asset Transaction");

    create(parent_win, wxID_ANY, m_dialog_heading);
}

bool AssetDialog::create(
    wxWindow* parent_win,
    wxWindowID win_id,
    const wxString& caption,
    const wxPoint& pos,
    const wxSize& size,
    long style
) {
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);

    if (!wxDialog::Create(parent_win, win_id, caption, pos, size, style))
        return false;

    createControls();
    dataToControls();

    GetSizer()->Fit(this);
    this->SetInitialSize();
    GetSizer()->SetSizeHints(this);
    SetIcon(mmPath::getProgramIcon());
    Centre();
    mmThemeAutoColour(this);

    return true;
}

void AssetDialog::createControls()
{
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(main_sizer);

    wxBoxSizer* panel_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* left_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* right_sizer = new wxBoxSizer(wxVERTICAL);

    main_sizer->Add(panel_sizer, wxSizerFlags(g_flagsV));
    panel_sizer->Add(left_sizer, 0);
    panel_sizer->Add(right_sizer, 0);

    // Asset Details Panel
    wxStaticBox* details_frame = new wxStaticBox(this, wxID_ANY, _t("Asset Details"));
    wxStaticBoxSizer* details_frame_sizer = new wxStaticBoxSizer(details_frame, wxVERTICAL);
    left_sizer->Add(details_frame_sizer, g_flagsV);

    wxPanel* asset_details_panel = new wxPanel(details_frame, wxID_STATIC);
    details_frame_sizer->Add(asset_details_panel, g_flagsV);

    wxFlexGridSizer* itemFlexGridSizer6 = new wxFlexGridSizer(0, 2, 0, 0);
    asset_details_panel->SetSizer(itemFlexGridSizer6);

    wxStaticText* n = new wxStaticText(asset_details_panel, wxID_STATIC, _t("Name"));
    itemFlexGridSizer6->Add(n, g_flagsH);
    n->SetFont(this->GetFont().Bold());

    w_name_text = new wxTextCtrl(asset_details_panel, wxID_ANY, wxGetEmptyString());
    mmToolTip(w_name_text, _t("Enter the name of the asset"));
    itemFlexGridSizer6->Add(w_name_text, g_flagsExpand);

    itemFlexGridSizer6->Add(
        new wxStaticText(asset_details_panel, wxID_STATIC, _t("Date")),
        g_flagsH
    );
    w_date_picker = new mmDatePicker(asset_details_panel, wxID_ANY);
    itemFlexGridSizer6->Add(w_date_picker->mmGetLayout(false));
    mmToolTip(w_date_picker, _t("Specify the date of purchase of asset"));

    itemFlexGridSizer6->Add(
        new wxStaticText(asset_details_panel, wxID_STATIC, _t("Asset Type")),
        g_flagsH
    );

    w_type_choice = new wxChoice(asset_details_panel, wxID_STATIC);
    for (int i = 0; i < AssetType::size; ++i) {
        wxString type_name = AssetType(i).name();
        w_type_choice->Append(
            wxGetTranslation(type_name),
            new wxStringClientData(type_name)
        );
    }

    mmToolTip(w_type_choice, _t("Select type of asset"));
    w_type_choice->SetSelection(AssetType().id());
    itemFlexGridSizer6->Add(w_type_choice, g_flagsExpand);

    wxStaticText* v = new wxStaticText(asset_details_panel, wxID_STATIC, _t("Initial Value"));
    itemFlexGridSizer6->Add(v, g_flagsH);
    v->SetFont(this->GetFont().Bold());

    w_value_text = new mmTextCtrl(
        asset_details_panel, IDC_VALUE, wxGetEmptyString(),
        wxDefaultPosition, wxSize(150,-1), wxALIGN_RIGHT|wxTE_PROCESS_ENTER,
        mmCalcValidator()
    );
    mmToolTip(w_value_text, _t("Enter the current value of the asset"));
    itemFlexGridSizer6->Add(w_value_text, g_flagsExpand);

    wxStaticText* c = new wxStaticText(asset_details_panel, wxID_STATIC, _t("Current Value"));
    itemFlexGridSizer6->Add(c, g_flagsH);
    c->SetFont(this->GetFont().Bold());

    m_current_value_text = new mmTextCtrl(
        asset_details_panel, IDC_CURR_VAL, wxGetEmptyString(),
        wxDefaultPosition, wxSize(150,-1), wxALIGN_RIGHT|wxTE_PROCESS_ENTER,
        mmCalcValidator()
    );
    m_current_value_text->Enable(false);
    mmToolTip(m_current_value_text, _t("The current value of the asset"));
    itemFlexGridSizer6->Add(m_current_value_text, g_flagsExpand);

    itemFlexGridSizer6->Add(
        new wxStaticText(asset_details_panel, wxID_STATIC, _t("Change in Value")),
        g_flagsH
    );

    w_change_choice = new wxChoice(asset_details_panel, IDC_COMBO_TYPE);
    for (mmChoiceId i = 0; i < AssetChange::size; ++i) {
        wxString change_name = AssetChange(i).name();
        w_change_choice->Append(wxGetTranslation(change_name));
    }

    mmToolTip(w_change_choice, _t("Specify if the value of the asset changes over time"));
    w_change_choice->SetSelection(AssetChange::e_none);
    itemFlexGridSizer6->Add(w_change_choice, g_flagsExpand);

    w_compounding_label = new wxStaticText(
        asset_details_panel,
        wxID_STATIC,
        _t("Compounding Period")
    );
    itemFlexGridSizer6->Add(w_compounding_label, g_flagsH);
    w_compounding_choice = new wxChoice(asset_details_panel, IDC_COMPOUNDING);
    for(const auto& a : PrefModel::COMPOUNDING_NAME)
        w_compounding_choice->Append(wxGetTranslation(a.second));
    mmToolTip(w_compounding_choice,
        _t("Select the compounding period for the appreciation/depreciation rate")
    );
    m_compounding = static_cast<PrefModel::COMPOUNDING_ID>(
        PrefModel::instance().getAssetCompounding()
    );
    w_compounding_choice->SetSelection(m_compounding);
    itemFlexGridSizer6->Add(w_compounding_choice, g_flagsExpand);

    w_rate_label = new wxStaticText(
        asset_details_panel,
        wxID_STATIC,
        _t("% Rate")
    );
    itemFlexGridSizer6->Add(w_rate_label, g_flagsH);
    w_rate_text = new mmTextCtrl(
        asset_details_panel, IDC_RATE, wxGetEmptyString(),
        wxDefaultPosition, wxSize(150,-1), wxALIGN_RIGHT|wxTE_PROCESS_ENTER,
        mmCalcValidator()
    );
    w_rate_text->SetAltPrecision(3);
    mmToolTip(w_rate_text,
        _t("Enter the rate at which the asset changes its value in percentage per year")
    );
    itemFlexGridSizer6->Add(w_rate_text, g_flagsExpand);
    enableDisableRate(false);

    itemFlexGridSizer6->Add(
        new wxStaticText(asset_details_panel, wxID_STATIC, _t("Notes")),
        g_flagsH
    );

    w_att_btn = new wxBitmapButton(
        asset_details_panel, wxID_FILE,
        mmImage::bitmapBundle(mmImage::png::CLIP, mmImage::bitmapButtonSize),
        wxDefaultPosition,
        wxSize(w_change_choice->GetSize().GetY(), w_change_choice->GetSize().GetY())
    );
    itemFlexGridSizer6->Add(w_att_btn, wxSizerFlags(g_flagsV).Align(wxALIGN_RIGHT));
    mmToolTip(w_att_btn, _t("Organize attachments of this asset"));

    w_notes_text = new wxTextCtrl(details_frame, IDC_NOTES,
        wxGetEmptyString(),
        wxDefaultPosition, wxSize(220, 170),
        wxTE_MULTILINE
    );
    mmToolTip(w_notes_text, _t("Enter notes associated with this asset"));
    details_frame_sizer->Add(w_notes_text, 0, wxGROW | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    // Asset Transaction Panel
    wxString trans_frame_heading = _t("New Transaction Details");
    if (m_trx_n) {
        trans_frame_heading = _t("Edit Transaction Details");
    }

    w_tl_frame = new wxStaticBox(this, wxID_ANY, trans_frame_heading);
    wxStaticBoxSizer* transaction_frame_sizer = new wxStaticBoxSizer(
        w_tl_frame,
        wxVERTICAL
    );
    right_sizer->Add(transaction_frame_sizer, g_flagsV);

    w_tl_dlg = new TrxLinkDialog(w_tl_frame, m_trx_n, true, wxID_STATIC);
    transaction_frame_sizer->Add(w_tl_dlg, g_flagsV);
    if (m_tl_n && m_trx_n) {
        w_tl_dlg->CheckingType(
            TrxLinkModel::type_checking(m_trx_n->m_to_account_id_n)
        );
    }
    else if (m_asset_n) {
        w_tl_dlg->SetTransactionNumber(
            m_asset_n->m_id.ToString() + "_" + m_asset_n->m_name
        );
        w_tl_dlg->CheckingType(TrxLinkModel::AS_INCOME_EXPENSE);
    }

    if (!m_show_trx_panel)
        hideTransactionPanel();

    // Separation Line
    wxStaticLine* separation_line = new wxStaticLine(this, wxID_ANY,
        wxDefaultPosition, wxDefaultSize,
        wxLI_HORIZONTAL
    );
    main_sizer->Add(separation_line, 0, wxEXPAND | wxALL, 1);

    // Button Panel
    wxPanel* button_panel = new wxPanel(this, wxID_STATIC);
    wxBoxSizer* button_panel_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* ok_button = new wxButton(button_panel, wxID_OK, _t("&OK "));
    wxButton* cancel_button = new wxButton(button_panel, wxID_CANCEL, _t("&Cancel "));

    main_sizer->Add(button_panel, wxSizerFlags(g_flagsV).Center());
    button_panel->SetSizer(button_panel_sizer);
    button_panel_sizer->Add(ok_button, g_flagsV);

    button_panel_sizer->Add(cancel_button, g_flagsV);
    //cancel_button->SetFocus();
}

void AssetDialog::dataToControls()
{
    if (!m_asset_n)
        return;

    w_name_text->SetValue(m_asset_n->m_name);
    if (AccountModel::instance().get_name_data_n(m_asset_n->m_name))
        w_name_text->Enable(false);
    w_date_picker->setValue(m_asset_n->m_start_date.dateTime());
    w_type_choice->SetSelection(m_asset_n->m_type.id());
    // TODO: translate asset type
    if (AccountModel::instance().get_name_data_n(m_asset_n->m_type.name()))
        w_type_choice->Enable(false);

    auto bal = AssetModel::instance().get_data_value(*m_asset_n);
    w_value_text->SetValue(bal.first);
    m_current_value_text->SetValue(bal.second);

    mmChoiceId valueChangeType = m_asset_n->m_change.id();
    w_change_choice->SetSelection(valueChangeType);

    // m_asset_n->m_change_rate is the rate with daily compounding
    double valueChangeRate = m_asset_n->m_change_rate;
    if (valueChangeType != AssetChange::e_none &&
        m_compounding != PrefModel::COMPOUNDING_ID_DAY
    ) {
        valueChangeRate = convertRate(
            valueChangeType,
            valueChangeRate,
            PrefModel::COMPOUNDING_ID_DAY,
            m_compounding
        );
    }
    w_rate_text->SetValue(valueChangeRate, 3);
    enableDisableRate(valueChangeType != AssetChange::e_none);

    w_notes_text->SetValue(m_asset_n->m_notes);

    TrxLinkModel::DataA tl_a = TrxLinkModel::instance().find_ref_data_a(
        AssetModel::s_ref_type, m_asset_n->m_id
    );
    if (!tl_a.empty())
        w_value_text->Enable(false);

    // Set up the transaction if this is the first entry.
    if (tl_a.empty())
        w_tl_dlg->SetTransactionValue(bal.first);

    if (m_show_trx_panel) {
        w_name_text->Enable(false);
        w_date_picker->Enable(false);
        w_type_choice->Enable(false);
        w_value_text->Enable(false);
        w_change_choice->Enable(false);
    }

    if (m_trx_n && m_trx_n->is_deleted()) {
        w_change_choice->Enable(false);
        w_compounding_choice->Enable(false);
        w_rate_text->Enable(false);
        w_notes_text->Enable(false);
        w_att_btn->Enable(false);
    }
}

// -- methods

void AssetDialog::setTransactionAccountName(const wxString& account_name)
{
    w_tl_dlg->SetTransactionAccount(account_name);
}

void AssetDialog::setTransactionDate()
{
    w_tl_dlg->TransactionDate(w_date_picker->GetValue());
}

void AssetDialog::hideTransactionPanel()
{
    w_tl_frame->Hide();
    w_tl_dlg->Hide();
}

void AssetDialog::enableDisableRate(bool en)
{
    w_rate_label->Enable(en);
    // if (!en) w_rate_text->SetValue("0");
    w_rate_text->SetEditable(en);
    w_rate_text->Enable(en);
    w_compounding_label->Enable(en);
    w_compounding_choice->Enable(en);
}

void AssetDialog::createAssetAccount()
{
    // TODO: translate asset type
    const wxString name = m_asset_n->m_type.name();

    AccountData new_account_d = AccountData();
    new_account_d.m_name         = name;
    new_account_d.m_type_        = mmNavigatorList::instance().getAssetAccountStr();
    new_account_d.m_open_balance = 0;
    new_account_d.m_open_date    = m_asset_n->m_start_date;
    new_account_d.m_currency_id  = CurrencyModel::instance().get_base_data_n()->m_id;
    AccountModel::instance().add_data_n(new_account_d);

    AssetDialog dlg(this, m_asset_n, true);
    dlg.setTransactionAccountName(name);
    dlg.setTransactionDate();
    dlg.ShowModal();
}

double AssetDialog::convertRate(
    mmChoiceId changeType, double xRate, int xCompounding, int yCompounding
) {
    int sign = (changeType == AssetChange::e_depreciates) ? -1 : 1;
    int xN = PrefModel::COMPOUNDING_N[xCompounding].second;
    int yN = PrefModel::COMPOUNDING_N[yCompounding].second;
    // solve (1.0 + sign*xRate/(xN*100.0))^xN = (1.0 + sign*yRate/(yN*100.0))^yN
    double xMult = 1.0 + sign * xRate / (xN * 100.0);
    double yMult = (xMult > 0.0) ? pow(xMult, double(xN)/double(yN)) : 0.0;
    double yRate = sign * (yMult - 1.0) * (yN * 100.0);
    return (yRate > 0.0) ? yRate : 0.0;
}

// -- event handlers

void AssetDialog::onOk(wxCommandEvent& /*event*/)
{
    AssetType asset_type = AssetType();
    wxStringClientData* type_obj = static_cast<wxStringClientData *>(
        w_type_choice->GetClientObject(w_type_choice->GetSelection())
    );
    if (type_obj)
        asset_type = AssetType(type_obj->GetData());

    const wxString asset_name = w_name_text->GetValue().Trim();
    if (asset_name.empty()) {
        mmErrorDialogs::InvalidName(w_name_text);
        return;
    }

    double asset_value = 0.0;
    if (!w_value_text->checkValue(asset_value))
        return;

    mmChoiceId asset_change_id = w_change_choice->GetSelection();
    double asset_change_rate = 0.0;
    if (asset_change_id != AssetChange::e_none) {
        if (!w_rate_text->checkValue(asset_change_rate))
            return;
        if (m_compounding != PrefModel::COMPOUNDING_ID_DAY) {
            asset_change_rate = convertRate(asset_change_id, asset_change_rate, m_compounding);
        }
    }

    AssetData new_asset_d = AssetData();
    bool is_new = !m_asset_n;
    if (is_new) {
        m_asset_n = &new_asset_d;
    }

    m_asset_n->m_type          = asset_type;
    m_asset_n->m_status        = AssetStatus();
    m_asset_n->m_name          = asset_name;
    m_asset_n->m_start_date    = mmDate(w_date_picker->GetValue());
    m_asset_n->m_currency_id_n = -1;
    m_asset_n->m_value         = asset_value;
    m_asset_n->m_change        = AssetChange(asset_change_id);
    m_asset_n->m_change_mode   = AssetChangeMode();
    m_asset_n->m_change_rate   = asset_change_rate;
    m_asset_n->m_notes         = w_notes_text->GetValue().Trim();
    AssetModel::instance().unsafe_save_data_n(m_asset_n);

    if (is_new) {
        // m_asset_n points to new_asset_d; get a pointer into more persistent data.
        m_asset_n = AssetModel::instance().unsafe_get_idN_data_n(
            m_asset_n->m_id
        );

        mmAttachment::relocate_ref_all(
            AssetModel::s_ref_type, 0,
            AssetModel::s_ref_type, m_asset_n->m_id
        );
    }

    if (w_tl_dlg->ValidCheckingAccountEntry()) {
        int64 trx_id = w_tl_dlg->SaveChecking();
        if (trx_id < 0)
            return;

        if (!m_tl_n) {
            TrxLinkModel::instance().save_asset_record(
                trx_id, m_asset_n->m_id,
                w_tl_dlg->CheckingType()
            );
        }
        TrxLinkModel::instance().update_asset_value(m_asset_n);
    }
    else if (m_show_trx_panel) {
        mmErrorDialogs::MessageWarning(this, _t("Invalid Transaction"), m_dialog_heading);
        return;
    }

    if (is_new) {
        const AccountData* account_n = AccountModel::instance().get_name_data_n(
            asset_name
        );
        if (!account_n) {
            if (wxMessageBox(
                _t("Asset account not found.") + "\n\n" + _t("Do you want to create one?"),
                _t("New Asset"), wxYES_NO | wxICON_INFORMATION
            ) == wxYES) {
                createAssetAccount();
            }
        }
    }

    EndModal(wxID_OK);
}

void AssetDialog::onCancel(wxCommandEvent& /*event*/)
{
    if (m_asset_rich_text)
        return;

    // FIXME: Avoid premature clone of attachments.
    if (!m_asset_n)
        AttachmentModel::instance().purge_ref_all(AssetModel::s_ref_type, 0);
    EndModal(wxID_CANCEL);
}

void AssetDialog::onAttachments(wxCommandEvent& /*event*/)
{
    int64 ref_id = m_asset_n ? m_asset_n->m_id : 0;
    AttachmentDialog dlg(this, AssetModel::s_ref_type, ref_id);
    dlg.ShowModal();
}

void AssetDialog::onChangeAppreciationType(wxCommandEvent& /*event*/)
{
    mmChoiceId selection = w_change_choice->GetSelection();
    // Disable for "None", Enable for "Appreciates" or "Depreciates"
    enableDisableRate(selection != AssetChange::e_none);
}

void AssetDialog::onChangeCompounding(wxCommandEvent& /*event*/)
{
    int selection = w_compounding_choice->GetSelection();
    if (selection == m_compounding)
        return;

    mmChoiceId change_id = w_change_choice->GetSelection();
    double change_rate = 0;
    if (change_id != AssetChange::e_none &&
        w_rate_text->checkValue(change_rate)
    ) {
        change_rate = convertRate(change_id, change_rate, m_compounding, selection);
        w_rate_text->SetValue(change_rate, 3);
    }

    m_compounding = static_cast<PrefModel::COMPOUNDING_ID>(selection);
}

void AssetDialog::onChangeFocus(wxChildFocusEvent& event)
{
    wxWindow *w = event.GetWindow();
    if (w)
        m_asset_rich_text = (w->GetId() == IDC_NOTES);
}

void AssetDialog::onQuit(wxCloseEvent& /*event*/)
{
    // FIXME: Avoid premature clone of attachments.
    if (!m_asset_n)
        AttachmentModel::instance().purge_ref_all(AssetModel::s_ref_type, 0);
    EndModal(wxID_CANCEL);
}
