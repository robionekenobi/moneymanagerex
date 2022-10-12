/*******************************************************
Copyright (C) 2006-2012 Madhan Kanagavel
Copyright (C) 2013 - 2016, 2020 - 2022 Nikolay Akimov
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

#include "splittransactionsdialog.h"
#include "categdialog.h"
#include "constants.h"
#include "images_list.h"
#include "util.h"
#include "paths.h"
#include "validators.h"

#include "model/Model_Account.h"
#include "model/Model_Category.h"

#include <wx/statline.h>

// mmEditSplitOther
// ------------------------------------------------------------------------------------------

wxIMPLEMENT_DYNAMIC_CLASS(mmEditSplitOther, wxDialog);

wxBEGIN_EVENT_TABLE(mmEditSplitOther, wxDialog)
    EVT_BUTTON(wxID_CANCEL, mmEditSplitOther::OnCancel)
    EVT_BUTTON(wxID_OK, mmEditSplitOther::OnOk)
wxEND_EVENT_TABLE()

mmEditSplitOther::mmEditSplitOther()
{
}
mmEditSplitOther::mmEditSplitOther(wxWindow *parent, Model_Currency::Data* currency
                        , Split* split, const wxString &name)
: m_split(split)
, m_currency(currency)
{
    long style = wxCAPTION | wxCLOSE_BOX | wxRESIZE_BORDER;
    if (!wxDialog::Create(parent, wxID_ANY, _("Edit Split Detail")
        , wxDefaultPosition, wxDefaultSize, style, name))
        return;

    CreateControls();
    mmSetSize(this);
    Centre();
    SetIcon(mmex::getProgramIcon());

    fillControls();
}

mmEditSplitOther::~mmEditSplitOther()
{
    Model_Infotable::instance().Set("EDITSPLITOTHER_DIALOG_SIZE", GetSize());
}

void mmEditSplitOther::CreateControls()
{
    wxBoxSizer* bSizer1 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(bSizer1);

    wxFlexGridSizer* fgSizer1 = new wxFlexGridSizer(0, 2, 0, 0);
    fgSizer1->AddGrowableCol(1, 0);
    bSizer1->Add(fgSizer1, g_flagsExpand);

    // Split Category
    fgSizer1->Add(new wxStaticText(this, wxID_STATIC, _("Category")), g_flagsH);
    wxString catName = Model_Category::full_name(m_split->CATEGID, m_split->SUBCATEGID);
    wxTextCtrl* category = new wxTextCtrl(this, wxID_ANY, catName);
    category->Disable();
    fgSizer1->Add(category, g_flagsExpand);

    // Split Amount
    fgSizer1->Add(new wxStaticText(this, wxID_STATIC, _("Amount")), g_flagsH);
    wxString amountStr = Model_Currency::toCurrency(m_split->SPLITTRANSAMOUNT, m_currency);
    wxTextCtrl* amount = new wxTextCtrl(this, wxID_ANY, amountStr);
    amount->Disable();
    fgSizer1->Add(amount, g_flagsExpand);

    // Notes
    fgSizer1->Add(new wxStaticText(this, wxID_STATIC, _("Notes")), g_flagsH);
    m_Notes = new wxTextCtrl(this, wxID_ANY, ""
        , wxDefaultPosition, wxSize(-1, -1), wxTE_MULTILINE);
    fgSizer1->Add(m_Notes, g_flagsExpand);
    mmToolTip(m_Notes, _("Enter notes to describe this split transaction"));

    //Buttons
    wxBoxSizer* bSizer3 = new wxBoxSizer(wxHORIZONTAL);
    bSizer1->Add(bSizer3, wxSizerFlags(g_flagsV).Center());
    wxButton* itemButtonOK = new wxButton(this, wxID_OK, _("&OK "));
    wxButton* itemButtonCancel = new wxButton(this, wxID_CANCEL, wxGetTranslation(g_CancelLabel));
    bSizer3->Add(itemButtonOK, g_flagsH);
    bSizer3->Add(itemButtonCancel, g_flagsH);

    Fit();
    wxSize sz = this->GetSize();
    SetSizeHints(sz.GetWidth(), sz.GetHeight(), -1, sz.GetHeight());
}

void mmEditSplitOther::fillControls()
{
    m_Notes->SetValue(m_split->NOTES);
}

void mmEditSplitOther::OnOk(wxCommandEvent& /*event*/)
{   
    m_split->NOTES = m_Notes->GetValue();
    EndModal(wxID_OK);
}

void mmEditSplitOther::OnCancel(wxCommandEvent& /*event*/)
{
    EndModal(wxID_OK);
}

// mmSplitTransactionDialog
// ------------------------------------------------------------------------------------------

#define STATIC_SPLIT_NUM 10

 wxBEGIN_EVENT_TABLE(mmSplitTransactionDialog, wxDialog)
     EVT_CHILD_FOCUS(mmSplitTransactionDialog::OnFocusChange)
     EVT_BUTTON(wxID_OK, mmSplitTransactionDialog::OnOk)
     EVT_BUTTON(mmID_SPLIT, mmSplitTransactionDialog::OnAddRow)
     EVT_BUTTON(mmID_REMOVE, mmSplitTransactionDialog::OnRemoveRow)
 wxEND_EVENT_TABLE()

mmSplitTransactionDialog::mmSplitTransactionDialog( )
{
}

mmSplitTransactionDialog::~mmSplitTransactionDialog()
{
     Model_Infotable::instance().Set("SPLITTRANSACTION_DIALOG_SIZE", GetSize());
}

mmSplitTransactionDialog::mmSplitTransactionDialog(wxWindow* parent
    , std::vector<Split>& split
    , int accountID
    , int transType
    , double totalAmount
    , bool is_view_only
)
    : m_orig_splits(split)
    , totalAmount_(totalAmount)
    , transType_(transType)
    , row_num_(static_cast<int>(split.size()))
    , is_view_only_(is_view_only)
{
    Model_Account::Data* account = Model_Account::instance().get(accountID);
    m_currency = account ? Model_Account::currency(account) : Model_Currency::GetBaseCurrency();
    m_splits = m_orig_splits;
    this->SetFont(parent->GetFont());
    Create(parent);
}

bool mmSplitTransactionDialog::Create(wxWindow* parent
    , wxWindowID id
    , const wxString& caption
    , const wxPoint& pos
    , const wxSize& size
    , long style
    , const wxString& name
    )
{
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style, name);

    CreateControls();
    FillControls();
    row_num_ = m_splits.size() - 1;
    activateNewRow();
    UpdateSplitTotal();

    mmSetSize(this);
    Centre();
    SetIcon(mmex::getProgramIcon());

    return TRUE;
}

void mmSplitTransactionDialog::CreateControls()
{
    SetEvtHandlerEnabled(false);
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(mainSizer);

    wxBoxSizer* boxSizerHead_ = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(boxSizerHead_, wxSizerFlags().Expand().Border(wxALL, 5));
    wxStaticText* categoryText = new wxStaticText(this, wxID_STATIC, _("Category"));
    categoryText->SetFont(this->GetFont().Bold());
    wxStaticText* amountText = new wxStaticText(this, wxID_STATIC, _("Amount"));
    amountText->SetFont(this->GetFont().Bold());
    wxStaticText* otherText = new wxStaticText(this, wxID_STATIC, _("Other"));
    otherText->SetFont(this->GetFont().Bold());
    boxSizerHead_->Add(categoryText, g_flagsExpand);
    boxSizerHead_->Add(amountText, g_flagsH);
    boxSizerHead_->Add(otherText, g_flagsH);

    slider_ = new wxScrolledWindow(this, wxNewId(), wxDefaultPosition, wxDefaultSize, wxVSCROLL);
    mainSizer->Add(slider_, wxSizerFlags().Align(wxALIGN_LEFT | wxEXPAND).Border(wxALL, 1).Proportion(0));

    wxBoxSizer* dialogMainSizerV = new wxBoxSizer(wxVERTICAL);
    slider_->SetSizer(dialogMainSizerV);

    flexGridSizer_ = new wxFlexGridSizer(0, 3, 0, 0);
    flexGridSizer_->AddGrowableCol(0, 0);
    dialogMainSizerV->Add(flexGridSizer_, g_flagsExpand);

    int size = static_cast<int>(m_splits.size());
    if (size < STATIC_SPLIT_NUM) size = STATIC_SPLIT_NUM;

    wxSize scrollSize;
    for (int row = 0; row < size; row++)
    {
        createNewRow(row <= m_splits.size() && !is_view_only_);
        if (row == (STATIC_SPLIT_NUM - 1))
        {
            slider_->Fit();
            scrollSize = slider_->GetSize();
        }
    }
    slider_->Fit();
    slider_->SetMinSize(scrollSize);
    slider_->SetScrollRate(1, 1);

    wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* plusAmountSizer = new wxBoxSizer(wxHORIZONTAL);
    bottomSizer->Add(plusAmountSizer, wxSizerFlags().Align(wxALIGN_LEFT).Border(wxALL, 5));

    wxButton* bAdd = new wxButton(this, mmID_SPLIT, _("Add Split"));
    bAdd->SetName("BUTTON");
    bAdd->Enable(!is_view_only_);
    plusAmountSizer->AddSpacer(mmBitmapButtonSize + 10);
    plusAmountSizer->Add(bAdd);

    wxButton* bRemove = new wxButton(this, mmID_REMOVE, _("Remove Split"));
    bRemove->SetName("BUTTON");
    bRemove->Enable(!is_view_only_);
    plusAmountSizer->AddSpacer(5);
    plusAmountSizer->Add(bRemove);

    wxBoxSizer* totalAmountSizer = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText* transAmountText = new wxStaticText(this, wxID_STATIC, _("Total:"));
    transAmount_ = new wxStaticText(this, wxID_STATIC, wxEmptyString);
    totalAmountSizer->Add(transAmountText, wxSizerFlags());
    totalAmountSizer->Add(transAmount_, wxSizerFlags().Border(wxLEFT, 5));
    bottomSizer->AddStretchSpacer();
    bottomSizer->Add(totalAmountSizer, wxSizerFlags().Border(wxALL, 5));
    mainSizer->Add(bottomSizer, g_flagsExpand);

    // OK Cancel buttons
    wxPanel* buttons_panel = new wxPanel(this, wxID_ANY);
    mainSizer->Add(buttons_panel, wxSizerFlags(g_flagsV).Center().Border(wxALL, 5));
    wxStdDialogButtonSizer*  buttons_sizer = new wxStdDialogButtonSizer;
    buttons_panel->SetSizer(buttons_sizer);

    wxSizerFlags flagsV = wxSizerFlags(g_flagsV).Border(wxLEFT | wxRIGHT | wxBOTTOM, 5).Center();
    wxBoxSizer* mainButtonSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* topRowButtonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* bottomRowButtonSizer = new wxBoxSizer(wxHORIZONTAL);
    mainButtonSizer->Add(topRowButtonSizer, flagsV);
    mainButtonSizer->Add(bottomRowButtonSizer, flagsV);
    buttons_sizer->Add(mainButtonSizer);

    itemButtonOK_ = new wxButton(buttons_panel, wxID_OK, _("&OK "));
    itemButtonOK_->SetName("BUTTON");
    wxButton* itemButtonCancel = new wxButton(buttons_panel, wxID_CANCEL, wxGetTranslation(g_CancelLabel));
    itemButtonCancel->SetName("BUTTON");

    bottomRowButtonSizer->Add(itemButtonOK_, g_flagsH);
    bottomRowButtonSizer->Add(itemButtonCancel, g_flagsH);
    itemButtonOK_->Enable(!is_view_only_);

    SetEvtHandlerEnabled(true);

    Fit();
    wxSize sz = this->GetSize();
    SetSizeHints(sz.GetWidth(), sz.GetHeight(), -1, sz.GetHeight());
}

void mmSplitTransactionDialog::FillControls(int focusRow)
{
    for (int row=0; row<m_splits_widgets.size(); row++)
    {
        if (row < m_splits.size())
        {
            m_splits_widgets.at(row).category->ChangeValue(
                    Model_Category::full_name(m_splits.at(row).CATEGID, m_splits.at(row).SUBCATEGID));
            if (m_splits.at(row).CATEGID == -1)
                m_splits_widgets.at(row).amount->SetValue("");
            else
                m_splits_widgets.at(row).amount->SetValue(m_splits.at(row).SPLITTRANSAMOUNT);
            UpdateExtraInfo(row);
            m_splits_widgets.at(row).category->Enable(true);
            m_splits_widgets.at(row).amount->Enable(true);
            m_splits_widgets.at(row).other->Enable(true);
            m_splits_widgets.at(row).category->SetFocus();
        } else
        {
            m_splits_widgets.at(row).category->ChangeValue("");
            m_splits_widgets.at(row).amount->SetValue("");
            m_splits_widgets.at(row).other->SetBitmap(mmBitmapBundle(png::GRM,mmBitmapButtonSize));
            m_splits_widgets.at(row).category->Enable(false);
            m_splits_widgets.at(row).amount->Enable(false);
            m_splits_widgets.at(row).other->Enable(false);

        }
        if (focusRow != -1)
            m_splits_widgets.at(focusRow).category->SetFocus();
    }
}

void mmSplitTransactionDialog::createNewRow(bool enabled)
{
    int row = m_splits_widgets.size();
    int catID = (row < m_splits.size()) ? m_splits.at(row).CATEGID : -1;
    int subCatID = (row < m_splits.size()) ? m_splits.at(row).SUBCATEGID : -1;
    mmComboBoxCategory* ncbc = new mmComboBoxCategory(slider_, wxID_HIGHEST + row
                                        , wxDefaultSize, catID, subCatID, true);
    ncbc->Enable(enabled);
        ncbc->Bind(wxEVT_CHAR_HOOK, &mmSplitTransactionDialog::OnComboKey, this);
        ncbc->SetMinSize(wxSize(250,-1));
    mmTextCtrl* nval = new mmTextCtrl(slider_, wxID_HIGHEST + row, "", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxTE_PROCESS_ENTER, mmCalcValidator(), m_currency);
    nval->Enable(enabled);
    nval->Connect(wxID_HIGHEST + row, wxEVT_COMMAND_TEXT_ENTER
                , wxCommandEventHandler(mmSplitTransactionDialog::OnTextEntered), nullptr, this);
        nval->SetMinSize(wxSize(100,-1));
    wxBitmapButton* nother = new wxBitmapButton(slider_, wxID_HIGHEST + row, mmBitmapBundle(png::GRM, mmBitmapButtonSize));
    nother->Connect(wxID_HIGHEST + row, wxEVT_BUTTON
            , wxCommandEventHandler(mmSplitTransactionDialog::OnOtherButton), nullptr, this);
    nother->Enable(enabled);
        flexGridSizer_->Add(ncbc, g_flagsExpand);
        flexGridSizer_->Add(nval, g_flagsH);
    flexGridSizer_->Add(nother, g_flagsH);

    SplitWidget sw = {ncbc, nval, nother};
    m_splits_widgets.push_back(sw);

    ncbc->SetFocus();
    slider_->FitInside();
    wxScrollWinEvent evt(wxEVT_SCROLLWIN_BOTTOM);
    slider_->GetEventHandler()->AddPendingEvent(evt);
}

void mmSplitTransactionDialog::activateNewRow()  
{
    if (row_num_ < (m_splits_widgets.size() - 1)) 
    {
        int row = row_num_ + 1;
        if (row >= m_splits.size())
        {
            Split s = {-1, -1, 0, ""};
            m_splits.push_back(s);
        }
        m_splits_widgets.at(row).category->Enable(true);
        m_splits_widgets.at(row).amount->Enable(true);
        m_splits_widgets.at(row).other->Enable(true);
        m_splits_widgets.at(row).category->SetFocus();
    } else
    {
        createNewRow(true);
        Split s = {-1, -1, 0, ""};
        m_splits.push_back(s);
    }
}

void mmSplitTransactionDialog::OnOk( wxCommandEvent& /*event*/ )
{
    for (int id=0; id<m_splits.size(); id++)
        if (!mmDoCheckRow(id))
            return;

    //Check total amount - should be positive
    totalAmount_ = 0;
    for (const auto& entry : m_splits)
        totalAmount_ += entry.SPLITTRANSAMOUNT;
    if (totalAmount_ < 0) {
        return mmErrorDialogs::MessageError(this, _("Invalid Total Amount"), _("Error"));
    }

    m_splits.erase(
        std::remove_if(
            m_splits.begin(), m_splits.end(),
            [](Split const& s) { return s.CATEGID == -1; }
        ),
        m_splits.end()
    );

    m_orig_splits = m_splits;
    EndModal(wxID_OK);
}

void mmSplitTransactionDialog::OnAddRow(wxCommandEvent& event)
{
    activateNewRow();
    FillControls();
    event.Skip();
}

void mmSplitTransactionDialog::OnRemoveRow(wxCommandEvent& event)
{
    if (m_splits.size() < 2)    // Should keep one split
        return;
    m_splits.erase(m_splits.begin() + row_num_ );
    FillControls(row_num_);
    UpdateSplitTotal();
}

void mmSplitTransactionDialog::OnFocusChange(wxChildFocusEvent& event)
{
    wxWindow* w = event.GetWindow();
    if (w && !w->GetName().empty()) {
        row_num_ = w->GetId() - wxID_HIGHEST;
    }    
    event.Skip();
}

void mmSplitTransactionDialog::OnTextEntered(wxCommandEvent& event)
{
    int row = event.GetId() - wxID_HIGHEST;
    if (m_splits_widgets.at(row).category->GetValue().empty() && m_splits_widgets.at(row).amount->GetValue().empty())
        return;

    if (mmDoCheckRow(row))
    {
        activateNewRow();
        UpdateSplitTotal();
    }
    event.Skip();
}

void mmSplitTransactionDialog::OnOtherButton(wxCommandEvent& event)
{
    int row = event.GetId() - wxID_HIGHEST;
    mmEditSplitOther dlg(this, m_currency, &m_splits.at(row));
    dlg.ShowModal();
    UpdateExtraInfo(row);

    event.Skip();
}

void mmSplitTransactionDialog::OnComboKey(wxKeyEvent& event)
{
    if (event.GetKeyCode() == WXK_RETURN)
    {
        auto cbc = static_cast<mmComboBoxCategory*>(event.GetEventObject());
        if (cbc) {
            auto category = cbc->GetValue();
            if (category.empty())
            {
                mmCategDialog dlg(this, true, -1, -1);
                dlg.ShowModal();
                for (int i=0; i<m_splits_widgets.size(); i++)
                {
                    auto cbcUpdate = m_splits_widgets.at(i).category;
                    if (cbc != cbcUpdate)
                    {
                        category = Model_Category::full_name(cbcUpdate->mmGetCategoryId(), cbcUpdate->mmGetSubcategoryId());
                        cbcUpdate->mmDoReInitialize();
                        cbcUpdate->ChangeValue(category);
                    }
                }
                category = Model_Category::full_name(dlg.getCategId(), dlg.getSubCategId());
                cbc->mmDoReInitialize();
                cbc->ChangeValue(category);
            }
        }
    }
    event.Skip();
}

void mmSplitTransactionDialog::UpdateSplitTotal()
{
    double total = 0;
    for (int i=0; i<m_splits.size(); i++)
    {
        double amount = 0.0;
        if (m_splits_widgets.at(i).amount->GetDouble(amount))
            total += amount;
    }

    wxString total_text = Model_Currency::toCurrency(total, m_currency);
    transAmount_->SetLabelText(total_text);
    Layout();
}

void mmSplitTransactionDialog::UpdateExtraInfo(int id)
{
    if (m_splits.at(id).NOTES.IsEmpty())
        m_splits_widgets.at(id).other->SetBitmap(mmBitmapBundle(png::GRM,mmBitmapButtonSize));
    else
        m_splits_widgets.at(id).other->SetBitmap(mmBitmapBundle(png::RIGHTARROW,mmBitmapButtonSize));

    m_splits_widgets.at(id).other->SetToolTip(m_splits.at(id).NOTES);   
}

bool mmSplitTransactionDialog::mmDoCheckRow(int row)
{
    if (m_splits_widgets.at(row).category->GetValue().empty() && 
        m_splits_widgets.at(row).amount->GetValue().empty() &&
        m_splits.at(row).NOTES.IsEmpty())
        return true;

    double amount = 0.0;

    // Validate category and amount
    if (!m_splits_widgets.at(row).category->mmIsValid()) {
            mmErrorDialogs::InvalidCategory(m_splits_widgets.at(row).category, true);
            return false;
    }
    if (!m_splits_widgets.at(row).amount->Calculate())
            return false;

    m_splits_widgets.at(row).amount->GetDouble(amount);

    m_splits.at(row).CATEGID = m_splits_widgets.at(row).category->mmGetCategoryId();
    m_splits.at(row).SUBCATEGID = m_splits_widgets.at(row).category->mmGetSubcategoryId();
    m_splits.at(row).SPLITTRANSAMOUNT = amount;   
    return true;
}