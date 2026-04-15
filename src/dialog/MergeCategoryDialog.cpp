/*******************************************************
 Copyright (C) 2006 Madhan Kanagavel
 Copyright (C) 2011 Stefano Giorgio
 Copyright (C) 2016, 2020, 2022 Nikolay Akimov
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

#include "base/_constants.h"
#include "util/mmPath.h"
#include "util/_util.h"

#include "model/_all.h"

#include "manager/CategoryManager.h"
#include "MergeCategoryDialog.h"
#include "import_export/webapp.h"

wxIMPLEMENT_DYNAMIC_CLASS(MergeCategoryDialog, wxDialog);

wxBEGIN_EVENT_TABLE(MergeCategoryDialog, wxDialog)
    EVT_CHAR_HOOK(MergeCategoryDialog::OnComboKey)
    EVT_CHILD_FOCUS(MergeCategoryDialog::OnFocusChange)
    EVT_COMBOBOX(wxID_ANY,  MergeCategoryDialog::OnTextUpdated)
    EVT_BUTTON(wxID_OK,     MergeCategoryDialog::OnOk)
    EVT_BUTTON(wxID_CANCEL, MergeCategoryDialog::OnCancel)
wxEND_EVENT_TABLE()

MergeCategoryDialog::MergeCategoryDialog()
{}

MergeCategoryDialog::~MergeCategoryDialog()
{
    InfoModel::instance().saveSize("RELOCATECATEG_DIALOG_SIZE", GetSize());
}

MergeCategoryDialog::MergeCategoryDialog(wxWindow* parent
    , int64 sourceCatID, int64 sourceSubCatID)
    : m_sourceCatID(sourceCatID)
    , m_sourceSubCatID(sourceSubCatID)
{
    this->SetFont(parent->GetFont());
    Create(parent);
}

bool MergeCategoryDialog::Create(wxWindow* parent
    , wxWindowID id
    , const wxString& caption
    , const wxPoint& pos
    , const wxSize& size
    , long style
    , const wxString& name)
{
    style |= wxRESIZE_BORDER;
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create(parent, id, caption, pos, size, style, name);

    CreateControls();
    mmThemeAutoColour(this);
    IsOkOk();

    SetIcon(mmPath::getProgramIcon());
    Fit();
    wxSize sz = GetSize();
    SetSizeHints(sz.GetWidth(), sz.GetHeight(), -1, sz.GetHeight());
    Centre();
    mmSetSize(this);
    return true;
}

void MergeCategoryDialog::CreateControls()
{
    wxSizerFlags flagsH, flagsV, flagsExpand;
    flagsH.Align(wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL).Border(wxALL, 5).Center();
    flagsV.Align(wxALIGN_LEFT).Border(wxALL, 5).Center();
    flagsExpand.Align(wxALIGN_LEFT).Border(wxALL, 5).Expand();

    wxStaticText* headerText = new wxStaticText(this, wxID_STATIC
        , _t("Merge Categories"));
    wxStaticLine* lineTop = new wxStaticLine(this, wxID_STATIC);

    cbSourceCategory_ = new mmComboBoxCategory(this, wxID_LAST);
    cbSourceCategory_->SetMinSize(wxSize(200, -1));
    const CategoryData* cat_n = CategoryModel::instance().get_id_data_n(m_sourceCatID);
    if (cat_n)
        cbSourceCategory_->SetValue(CategoryModel::instance().get_id_fullname(m_sourceCatID));

    cbDestCategory_ = new mmComboBoxCategory(this, wxID_NEW, wxDefaultSize, -1, true);
    cbDestCategory_->SetMinSize(wxSize(200, -1));

    cbDeleteSourceCategory_ = new wxCheckBox(this, wxID_ANY,
        _t("&Delete source category after merge (if source category has no subcategories)")
    );

    wxStaticLine* lineBottom = new wxStaticLine(this, wxID_STATIC);

    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(topSizer);
    wxBoxSizer* boxSizer = new wxBoxSizer(wxVERTICAL);
    wxFlexGridSizer* request_sizer = new wxFlexGridSizer(0, 2, 0, 0);
    request_sizer->AddGrowableCol(0, 1);
    request_sizer->AddGrowableCol(1, 1);

    topSizer->Add(boxSizer, flagsExpand);

    boxSizer->Add(headerText, g_flagsV);
    boxSizer->Add(lineTop, g_flagsExpand);

    request_sizer->Add(new wxStaticText(this, wxID_STATIC, _t("Source:")), flagsH);
    request_sizer->Add(new wxStaticText(this, wxID_STATIC, _t("Target:")), flagsH);
    request_sizer->Add(cbSourceCategory_, flagsExpand);
    request_sizer->Add(cbDestCategory_, flagsExpand);

    boxSizer->Add(request_sizer, flagsExpand);
    boxSizer->Add(cbDeleteSourceCategory_, flagsExpand);
    boxSizer->Add(lineBottom, flagsExpand);

    m_info = new wxStaticText(this, wxID_STATIC, "");
    boxSizer->Add(m_info, flagsExpand);

    wxStaticLine* lineBottom2 = new wxStaticLine(this, wxID_STATIC);
    boxSizer->Add(lineBottom2, flagsExpand);

    wxButton* okButton = new wxButton(this, wxID_OK, _t("&Merge"));
    wxButton* cancelButton = new wxButton(this, wxID_CANCEL, _t("&Close"));
    cancelButton-> SetFocus();
    wxBoxSizer* buttonBoxSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonBoxSizer->Add(okButton, flagsH);
    buttonBoxSizer->Add(cancelButton, flagsH);
    boxSizer->Add(buttonBoxSizer, flagsV);
}

void MergeCategoryDialog::OnCancel(wxCommandEvent& WXUNUSED(event))
{
    EndModal(m_changedRecords > 0 ? wxID_OK : wxID_CANCEL);
}

void MergeCategoryDialog::OnOk(wxCommandEvent& WXUNUSED(event))
{
    const wxString& info = wxString::Format(_t("From %1$s to %2$s"),
        cbSourceCategory_->GetValue(),
        cbDestCategory_->GetValue()
    );
    if (wxMessageBox(
        _t("Please Confirm:") + "\n" + info,
        _t("Merge categories confirmation"),
        wxOK | wxCANCEL | wxICON_INFORMATION
    ) != wxOK)
        return;

    const int64 dst_cat_id = cbDestCategory_->mmGetCategoryId();

    for (auto& trx_d : TrxModel::instance().find_data_a(
        TrxCol::WHERE_CATEGID(OP_EQ, m_sourceCatID)
    )) {
        trx_d.m_category_id_n = dst_cat_id;
        TrxModel::instance().save_trx_n(trx_d);
        m_changedRecords++;
    }

    for (auto& tp_d : TrxSplitModel::instance().find_data_a(
        TrxSplitCol::WHERE_CATEGID(OP_EQ, m_sourceCatID)
    )) {
        tp_d.m_category_id = dst_cat_id;
        TrxSplitModel::instance().save_data_n(tp_d);
        m_changedRecords++;
    }

    for (auto& sched_d : SchedModel::instance().find_data_a(
        SchedCol::WHERE_CATEGID(OP_EQ, m_sourceCatID)
    )) {
        sched_d.m_category_id_n = dst_cat_id;
        SchedModel::instance().save_data_n(sched_d);
        m_changedRecords++;
    }

    for (auto& qp_d : SchedSplitModel::instance().find_data_a(
        SchedSplitCol::WHERE_CATEGID(OP_EQ, m_sourceCatID)
    )) {
        qp_d.m_category_id = dst_cat_id;
        SchedSplitModel::instance().save_data_n(qp_d);
        m_changedRecords++;
    }

    for (auto& payee_d : PayeeModel::instance().find_data_a(
        PayeeCol::WHERE_CATEGID(OP_EQ, m_sourceCatID)
    )) {
        payee_d.m_category_id_n = dst_cat_id;
        PayeeModel::instance().save_data_n(payee_d);
        m_changedRecords++;
    }
    mmWebApp::uploadPayee();

    for (auto& budget_d : BudgetModel::instance().find_data_a(
        BudgetCol::WHERE_CATEGID(OP_EQ, m_sourceCatID)
    )) {
        BudgetModel::instance().purge_id(budget_d.m_id);
        m_changedRecords++;
    }

    if (cbDeleteSourceCategory_->IsChecked()) {
        if (m_sourceSubCatID == -1) {
            const CategoryData* src_cat_n = CategoryModel::instance().get_id_data_n(
                m_sourceCatID
            );
            if (CategoryModel::instance().find_data_sub_a(*src_cat_n).empty())
                CategoryModel::instance().purge_id(m_sourceCatID);
        }

        cbSourceCategory_->mmDoReInitialize();
        cbDestCategory_->mmDoReInitialize();
        mmWebApp::uploadCategory();
    }

    IsOkOk();
}

void MergeCategoryDialog::IsOkOk()
{
    m_sourceCatID = cbSourceCategory_->mmGetCategoryId();
    int64 destCatID = cbDestCategory_->mmGetCategoryId();

    const int trx_c = (m_sourceCatID < 0 && m_sourceSubCatID < 0)
        ? 0
        : static_cast<int>(TrxModel::instance().find_count(
            TrxCol::WHERE_CATEGID(OP_EQ, m_sourceCatID)
        ));
    const int tp_c = static_cast<int>(TrxSplitModel::instance().find_count(
        TrxSplitCol::WHERE_CATEGID(OP_EQ, m_sourceCatID)
    ));
    const int sched_c = (m_sourceCatID < 0 && m_sourceSubCatID < 0)
        ? 0
        : static_cast<int>(SchedModel::instance().find_count(
            SchedCol::WHERE_CATEGID(OP_EQ, m_sourceCatID)
        ));
    const int qp_c = static_cast<int>(SchedSplitModel::instance().find_count(
        SchedSplitCol::WHERE_CATEGID(OP_EQ, m_sourceCatID)
    ));
    const int payee_c = (m_sourceCatID < 0 && m_sourceSubCatID < 0)
        ? 0
        : static_cast<int>(PayeeModel::instance().find_count(
            PayeeCol::WHERE_CATEGID(OP_EQ, m_sourceCatID)
        ));
    const int budget_c = static_cast<int>(BudgetModel::instance().find_count(
        BudgetCol::WHERE_CATEGID(OP_EQ, m_sourceCatID)
    ));
    const int total = trx_c + tp_c + sched_c + qp_c + payee_c + budget_c;

    wxString msgStr = wxString()
        << wxString::Format(_t("Records found in transactions: %i"), trx_c) << "\n"
        << wxString::Format(_t("Records found in split transactions: %i"), tp_c) << "\n"
        << wxString::Format(_t("Records found in scheduled transactions: %i"), sched_c) << "\n"
        << wxString::Format(_t("Records found in scheduled split transactions: %i"), qp_c) << "\n"
        << wxString::Format(_t("Records found as default payee category: %i"), payee_c) << "\n"
        << wxString::Format(_t("Records found in budget: %i"), budget_c);
    m_info->SetLabel(msgStr);

    bool e = true;
    if (total == 0)
        e = false;
    else if (m_sourceCatID == destCatID)
        e = false;
    else if (destCatID < 0 || m_sourceCatID < 0)
        e = false;
    wxButton* ok = wxStaticCast(FindWindow(wxID_OK), wxButton);
    ok->Enable(e);
}

void MergeCategoryDialog::OnComboKey(wxKeyEvent& event)
{
    if (event.GetKeyCode() != WXK_RETURN) {
        event.Skip();
        return;
    }
    
    auto id = event.GetId();
    switch (id)
    {
    case wxID_LAST: {
        auto category = cbSourceCategory_->GetValue();
        if (category.empty()) {
            CategoryManager dlg(this, true, -1);
            dlg.ShowModal();
            if (dlg.getRefreshRequested())
                cbSourceCategory_->mmDoReInitialize();
            category = CategoryModel::instance().get_id_fullname(dlg.getCategId());
            cbSourceCategory_->ChangeValue(category);
            return;
        }
        break;
    }
    case wxID_NEW: {
        auto category = cbDestCategory_->GetValue();
        if (category.empty()) {
            CategoryManager dlg(this, true, -1);
            dlg.ShowModal();
            if (dlg.getRefreshRequested())
                cbDestCategory_->mmDoReInitialize();
            category = CategoryModel::instance().get_id_fullname(dlg.getCategId());
            cbDestCategory_->ChangeValue(category);
            return;
        }
        break;
    }
    default:
        break;
    }

    event.Skip();
}

void MergeCategoryDialog::OnFocusChange(wxChildFocusEvent& event)
{
    cbDestCategory_->ChangeValue(cbDestCategory_->GetValue());
    cbSourceCategory_->ChangeValue(cbSourceCategory_->GetValue());
    IsOkOk();
    event.Skip();
}

void MergeCategoryDialog::OnTextUpdated(wxCommandEvent&)
{
    IsOkOk();
}
