/*******************************************************
 Copyright (C) 2016 Stefano Giorgio
 Copyright (C) 2025 Klaus Wich

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
#include <wx/numdlg.h>
#include <wx/textdlg.h>
#include <wx/valnum.h>

#include "base/_constants.h"
#include "util/mmPath.h"
#include "util/mmImage.h"
#include "util/mmAttachment.h"
#include "util/mmTextCtrl.h"
#include "util/mmCalcValidator.h"
#include "util/_util.h"
#include "util/_simple.h"

#include "model/AccountModel.h"
#include "model/CategoryModel.h"
#include "model/StockHistoryModel.h"

#include "AttachmentDialog.h"
#include "SplitDialog.h"
#include "TrxShareDialog.h"
#include "TrxLinkDialog.h"

IMPLEMENT_DYNAMIC_CLASS(TrxShareDialog, wxDialog)

wxBEGIN_EVENT_TABLE(TrxShareDialog, wxDialog)
    EVT_BUTTON(wxID_OK,         TrxShareDialog::onOk)
    EVT_BUTTON(wxID_CANCEL,     TrxShareDialog::onCancel)
    EVT_BUTTON(wxID_INDEX,      TrxShareDialog::onStockPriceButton)
    EVT_BUTTON(mmID_COMM_SPLIT, TrxShareDialog::onDeductibleSplit)
    EVT_CLOSE(                  TrxShareDialog::onQuit)
wxEND_EVENT_TABLE()

TrxShareDialog::TrxShareDialog()
{
}

TrxShareDialog::TrxShareDialog(wxWindow* parent_win, StockData* stock_n) :
    m_stock_n(stock_n),
    m_dialog_heading(_t("Add Share Transaction"))
{
    long style = wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX;
    create(parent_win, wxID_ANY, m_dialog_heading, wxDefaultPosition, wxSize(400, 300), style);
}

TrxShareDialog::TrxShareDialog(
    wxWindow* parent_win,
    const TrxLinkData* tl_n,
    TrxData* trx_n
) :
    m_trx_n(trx_n),
    m_tl_n(tl_n),
    m_dialog_heading(_t("Edit Share Transaction"))
{
    if (m_tl_n) {
        m_stock_n = StockModel::instance().unsafe_get_idN_data_n(
            m_tl_n->m_ref_id
        );
        if (m_tl_n->m_ref_type == StockModel::s_ref_type) {
            m_ts_n = TrxShareModel::instance().unsafe_get_trxId_data_n(
                m_tl_n->m_trx_id
            );
            if (m_ts_n->m_lot.IsEmpty())
                // FIXME: m_ts_n is changed but not saved
                m_ts_n->m_lot = m_stock_n->m_id.ToString();

            for (const auto& tp_d: TrxSplitModel::instance().find_data_a(
                TrxSplitCol::WHERE_TRANSID(OP_EQ, m_ts_n->m_id)
            )) {
                wxArrayInt64 tag_id_a;
                for (const auto& gl_d : TagLinkModel::instance().find_data_a(
                    TagLinkCol::WHERE_REFTYPE(OP_EQ, TrxSplitModel::s_ref_type.key_n()),
                    TagLinkCol::WHERE_REFID(OP_EQ, tp_d.m_id)
                )) {
                    tag_id_a.push_back(gl_d.m_tag_id);
                }
                m_local_deductible_splits.push_back({
                    tp_d.m_category_id, tp_d.m_amount, tp_d.m_notes, tag_id_a
                });
            }
        }
    }

    if (m_trx_n) {
        for (const auto& tp_d: TrxSplitModel::instance().find_data_a(
            TrxSplitCol::WHERE_TRANSID(OP_EQ, m_trx_n->m_id)
        )) {
            wxArrayInt64 tag_id_a;
            for (const auto& gl_d : TagLinkModel::instance().find_data_a(
                TagLinkCol::WHERE_REFTYPE(OP_EQ, TrxSplitModel::s_ref_type.key_n()),
                TagLinkCol::WHERE_REFID(OP_EQ, tp_d.m_id)
            )) {
                tag_id_a.push_back(gl_d.m_tag_id);
            }
            m_local_non_deductible_splits.push_back({
                tp_d.m_category_id, tp_d.m_amount, tp_d.m_notes, tag_id_a
            });
        }
    }

    this->SetFont(parent_win->GetFont());
    create(parent_win, wxID_ANY, m_dialog_heading, wxDefaultPosition, wxSize(400, 300));
    this->SetMinSize(wxSize(400, 300));
}

bool TrxShareDialog::create(
    wxWindow* parent_win,
    wxWindowID id,
    const wxString& caption,
    const wxPoint& pos,
    const wxSize& size,
    long style
) {
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create(parent_win, id, caption, pos, size, style);

    createControls();
    mmThemeAutoColour(this);
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);

    SetIcon(mmPath::getProgramIcon());

    dataToControls();

    Centre();
    return true;
}

void TrxShareDialog::createControls()
{
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(main_sizer);

    wxBoxSizer* panel_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* left_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* right_sizer = new wxBoxSizer(wxVERTICAL);

    main_sizer->Add(panel_sizer, wxSizerFlags(g_flagsV));
    panel_sizer->Add(left_sizer, 0);
    panel_sizer->Add(right_sizer, 0);

    /********************************************************************
    Stock Details Panel
    *********************************************************************/
    wxStaticBox* details_frame = new wxStaticBox(this, wxID_ANY, _t("Stock Investment Details"));
    wxStaticBoxSizer* details_frame_sizer = new wxStaticBoxSizer(details_frame, wxVERTICAL);
    left_sizer->Add(details_frame_sizer, g_flagsExpand);

    wxPanel* stock_details_panel = new wxPanel(details_frame, wxID_STATIC);
    details_frame_sizer->Add(stock_details_panel, g_flagsV);

    wxFlexGridSizer* itemFlexGridSizer6 = new wxFlexGridSizer(0, 2, 0, 0);
    stock_details_panel->SetSizer(itemFlexGridSizer6);

    itemFlexGridSizer6->Add(new wxStaticText(stock_details_panel, wxID_STATIC, _t("Company Name")), g_flagsH);

    w_name_text = new wxTextCtrl(stock_details_panel, ID_STOCKTRANS_SHARE_NAME, "");
    itemFlexGridSizer6->Add(w_name_text, g_flagsExpand);
    mmToolTip(w_name_text, _t("Enter the stock company name"));

    //Symbol
    wxStaticText* symbol = new wxStaticText(stock_details_panel, wxID_STATIC, _t("Stock Symbol"));
    itemFlexGridSizer6->Add(symbol, g_flagsH);
    symbol->SetFont(this->GetFont().Bold());

    w_symbol_text = new wxTextCtrl(stock_details_panel, ID_STOCKTRANS_SHARE_SYMBOL
        , "", wxDefaultPosition, wxSize(150, -1), 0);
    itemFlexGridSizer6->Add(w_symbol_text, g_flagsH);
    mmToolTip(w_symbol_text, _t("Enter the stock symbol. (Optional) Include exchange. eg: IBM.BE"));

    //Share Lot
    wxStaticText* lot_text = new wxStaticText(stock_details_panel, wxID_STATIC, _t("Share Lot"));
    itemFlexGridSizer6->Add(lot_text, g_flagsH);
    lot_text->SetFont(this->GetFont().Bold());

    w_lot_text = new wxTextCtrl(stock_details_panel, ID_STOCKTRANS_SHARE_LOT
        , "", wxDefaultPosition, wxSize(150, -1), 0);
    itemFlexGridSizer6->Add(w_lot_text, g_flagsH);
    mmToolTip(w_lot_text, _t("Enter the LOT that this parcel os shares belong to"));

    //Share Unit Number
    wxStaticText* number = new wxStaticText(stock_details_panel, wxID_STATIC, _t("Share Number"));
    itemFlexGridSizer6->Add(number, g_flagsH);
    number->SetFont(this->GetFont().Bold());
    w_num_text = new mmTextCtrl(stock_details_panel, ID_STOCKTRANS_SHARE_NUMBER, ""
        , wxDefaultPosition, wxSize(150, -1), wxALIGN_RIGHT | wxTE_PROCESS_ENTER, mmCalcValidator());
    w_num_text->SetAltPrecision(PrefModel::instance().getSharePrecision());
    itemFlexGridSizer6->Add(w_num_text, g_flagsH);
    mmToolTip(w_num_text, _t("Enter number of shares held"));

    w_num_text->Connect(ID_STOCKTRANS_SHARE_NUMBER, wxEVT_COMMAND_TEXT_UPDATED
        , wxCommandEventHandler(TrxShareDialog::onCalculateAmount), nullptr, this);

    //Share Price
    wxStaticText* pprice = new wxStaticText(stock_details_panel, wxID_STATIC, _t("Share Price"));
    pprice->SetFont(this->GetFont().Bold());
    w_price_text = new mmTextCtrl(stock_details_panel, ID_STOCKTRANS_SHARE_PRICE, ""
        , wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxTE_PROCESS_ENTER, mmCalcValidator());
    w_price_text->SetAltPrecision(PrefModel::instance().getSharePrecision());
    w_price_text->SetMinSize(wxSize(150, -1));
    itemFlexGridSizer6->Add(pprice, g_flagsH);
    itemFlexGridSizer6->Add(w_price_text, g_flagsH);
    mmToolTip(w_price_text, _t("Enter the current value for a single share unit"));

    w_price_text->Connect(ID_STOCKTRANS_SHARE_PRICE, wxEVT_COMMAND_TEXT_UPDATED
        , wxCommandEventHandler(TrxShareDialog::onCalculateAmount), nullptr, this);

    // Deductible Commission
    itemFlexGridSizer6->Add(new wxStaticText(stock_details_panel, wxID_STATIC, _t("Deductible Comm.")), g_flagsH);

    wxBoxSizer* commission_sizer = new wxBoxSizer(wxHORIZONTAL);
    itemFlexGridSizer6->Add(commission_sizer, wxSizerFlags(g_flagsExpand).Border(0));

    w_commission_text = new mmTextCtrl(stock_details_panel, ID_STOCKTRANS_SHARE_COMMISSION, "0"
        , wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxTE_PROCESS_ENTER, mmCalcValidator());
    w_commission_text->SetAltPrecision(PrefModel::instance().getSharePrecision());
    w_commission_text->SetMinSize(wxSize(110, -1));
    commission_sizer->Add(w_commission_text, g_flagsH);
    mmToolTip(w_commission_text, _t("Enter any commission paid"));

    w_deduct_comm_btn = new wxBitmapButton(stock_details_panel, mmID_COMM_SPLIT, mmImage::bitmapBundle(mmImage::png::NEW_TRX, mmImage::bitmapButtonSize));
    commission_sizer->Add(w_deduct_comm_btn, g_flagsH);
    mmToolTip(w_deduct_comm_btn, _t("Use Deductible Comm. split Categories"));

    w_commission_text->Connect(ID_STOCKTRANS_SHARE_COMMISSION, wxEVT_COMMAND_TEXT_UPDATED
        , wxCommandEventHandler(TrxShareDialog::onCalculateAmount), nullptr, this);

    //Notes
    itemFlexGridSizer6->Add(new wxStaticText(stock_details_panel, wxID_STATIC, _t("Notes")), g_flagsH);

    // Attachments
    w_att_btn = new wxBitmapButton(
        stock_details_panel, wxID_FILE,
        mmImage::bitmapBundle(mmImage::png::CLIP, mmImage::bitmapButtonSize)
    );
    mmToolTip(w_att_btn, _t("Manage stock attachments"));
    // TODO: m_attachments not used here
    w_att_btn->Hide();

    w_web_btn = new wxBitmapButton(
        stock_details_panel, wxID_INDEX,
        mmImage::bitmapBundle(mmImage::png::WEB, mmImage::bitmapButtonSize)
    );
    mmToolTip(w_web_btn, _t("Display the web page for the specified Stock symbol"));

    wxBoxSizer* icon_sizer = new wxBoxSizer(wxHORIZONTAL);
    icon_sizer->Add(w_att_btn, g_flagsH);
    icon_sizer->Add(w_web_btn, g_flagsH);
    itemFlexGridSizer6->Add(icon_sizer, wxSizerFlags(g_flagsH).Align(wxALIGN_RIGHT));

    w_notes_text = new wxTextCtrl(
        details_frame, wxID_STATIC,
        "",
        wxDefaultPosition, wxSize(200, 162),
        wxTE_MULTILINE
    );
    details_frame_sizer->Add(w_notes_text, g_flagsExpand);
    details_frame_sizer->AddSpacer(1);
    mmToolTip(w_notes_text, _t("Enter notes associated with this investment"));

    // Transaction Panel
    wxStaticBox* transaction_frame = new wxStaticBox(this, wxID_ANY, m_trx_n
        ? _t("Edit Transaction Details")
        : _t("Add Transaction Details")
    );
    wxStaticBoxSizer* transaction_frame_sizer = new wxStaticBoxSizer(
        transaction_frame, wxVERTICAL
    );
    right_sizer->Add(transaction_frame_sizer, g_flagsV);

    w_tl_dlg = new TrxLinkDialog(transaction_frame, m_trx_n, false, wxID_STATIC);
    w_tl_dlg->Bind(wxEVT_CHOICE, &TrxShareDialog::onCalculateAmount, this, wxID_VIEW_DETAILS);
    transaction_frame_sizer->Add(w_tl_dlg, g_flagsV);
    if (m_tl_n && m_trx_n) {
        w_tl_dlg->CheckingType(TrxLinkModel::type_checking(m_trx_n->m_to_account_id_n));
    }
    else {
        wxString acc_held = AccountModel::instance().get_id_name(m_stock_n->m_account_id_n);
        w_tl_dlg->SetTransactionNumber(m_stock_n->m_name + "_" + m_stock_n->m_symbol);
        w_tl_dlg->SetTransactionAccount(acc_held);
    }

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
    wxButton* close_button = new wxButton(button_panel, wxID_CANCEL, _t("&Cancel "));

    main_sizer->Add(button_panel, wxSizerFlags(g_flagsH).Center());
    button_panel->SetSizer(button_panel_sizer);
    button_panel_sizer->Add(ok_button, g_flagsH);
    button_panel_sizer->Add(close_button, g_flagsH);
    //cancel_button->SetFocus();
}

void TrxShareDialog::dataToControls()
{
    if (!m_stock_n)
        return;

    w_name_text->SetValue(m_stock_n->m_name);
    w_tl_dlg->SetTransactionAccount(m_stock_n->m_name);
    w_symbol_text->SetValue(m_stock_n->m_symbol);
    w_notes_text->SetValue(m_stock_n->m_notes);

    w_name_text->Enable(false);
    w_symbol_text->Enable(false);
    w_lot_text->Enable(false);
    w_notes_text->Enable(false);

    TrxLinkModel::DataA tl_a = TrxLinkModel::instance().find_ref_data_a(
        StockModel::s_ref_type, m_stock_n->m_id
    );

    if (tl_a.empty()) {
        // Set up the transaction as the first entry.
        int precision = m_stock_n->m_num_shares == floor(m_stock_n->m_num_shares)
            ? 0
            : PrefModel::instance().getSharePrecision();
        w_num_text->SetValue(m_stock_n->m_num_shares, precision);
        w_price_text->SetValue(
            m_stock_n->m_purchase_price,
            PrefModel::instance().getSharePrecision()
        );
        w_commission_text->SetValue(
            m_stock_n->m_commission,
            PrefModel::instance().getSharePrecision()
        );
        w_lot_text->SetValue(m_stock_n->m_id.ToString());
        w_tl_dlg->TransactionDate(m_stock_n->m_purchase_date.dateTime());
        w_tl_dlg->SetTransactionValue(
            get_amount(m_stock_n->m_num_shares, m_stock_n->m_purchase_price, m_stock_n->m_commission),
            true
        );
    }
    else {
        if (m_ts_n) {
            int precision = m_ts_n->m_number == floor(m_ts_n->m_number)
                ? 0
                : PrefModel::instance().getSharePrecision();
            w_num_text->SetValue(std::abs(m_ts_n->m_number), precision);
            w_price_text->SetValue(
                m_ts_n->m_price,
                PrefModel::instance().getSharePrecision()
            );
            w_commission_text->SetValue(
                m_ts_n->m_commission,
                PrefModel::instance().getSharePrecision()
            );
            w_lot_text->SetValue(m_ts_n->m_lot);

            if (m_tl_n) {
                TrxData* trx_n = TrxModel::instance().unsafe_get_idN_data_n(m_tl_n->m_trx_id);
                if (trx_n) {
                    w_tl_dlg->TransactionDate(
                        trx_n->m_datetime.dateTime()
                    );
                    w_tl_dlg->SetTransactionValue(
                        get_amount(std::abs(m_ts_n->m_number), m_ts_n->m_price, m_ts_n->m_commission),
                        true
                    );
                    w_tl_dlg->SetTransactionAccount(
                        AccountModel::instance().get_id_name(trx_n->m_account_id)
                    );
                    w_tl_dlg->SetTransactionStatus(trx_n->m_status.id());
                    w_tl_dlg->SetTransactionPayee(trx_n->m_payee_id_n);
                    w_tl_dlg->SetTransactionCategory(trx_n->m_category_id_n);
                    if (trx_n->is_deleted()) {
                        w_num_text->Enable(false);
                        w_price_text->Enable(false);
                        w_commission_text->Enable(false);
                        w_web_btn->Enable(false);
                    }
                }
            }
        }
        else {
            w_num_text->SetValue(0, 0);
            w_price_text->SetValue(0, PrefModel::instance().getSharePrecision());
            w_lot_text->SetValue(m_stock_n->m_id.ToString());
            w_tl_dlg->SetTransactionValue(0, true);
        }
    }

    bool has_split = !(m_local_deductible_splits.size() <= 1);
    if (has_split) {
        w_commission_text->Enable(!has_split);
        w_commission_text->SetValue(
            TrxSplitModel::instance().get_total(m_local_deductible_splits),
            PrefModel::instance().getSharePrecision()
        );
        mmToolTip(w_deduct_comm_btn, TrxSplitModel::instance().get_tooltip(
            m_local_deductible_splits, nullptr /* currency */
        ));
    }
}

// -- methods

double TrxShareDialog::get_amount(double shares, double price, double commission) const
{
    if (w_tl_dlg->TransactionType() == TrxType::e_deposit)
        return (shares * price - commission);
    else
        return (shares * price + commission);
}

// -- event handlers

void TrxShareDialog::onQuit(wxCloseEvent& WXUNUSED(event))
{
    // CHECK: Is this cleanup needed?
    // FIXME: Avoid premature clone of attachments.
    if (!m_stock_n)
        AttachmentModel::instance().purge_ref_all(StockModel::s_ref_type, 0);
    EndModal(wxID_CANCEL);
}

void TrxShareDialog::onCancel(wxCommandEvent& WXUNUSED(event))
{
    // CHECK: Is this cleanup needed?
    // FIXME: Avoid premature clone of attachments.
    if (!m_stock_n)
        AttachmentModel::instance().purge_ref_all(StockModel::s_ref_type, 0);
    EndModal(wxID_CANCEL);
}

void TrxShareDialog::onStockPriceButton(wxCommandEvent& WXUNUSED(event))
{
    const wxString stockSymbol = w_symbol_text->GetValue().Trim();

    if (!stockSymbol.IsEmpty()) {
        const wxString& stockURL = InfoModel::instance().getString(
            "STOCKURL",
            mmex::weblink::DefStockUrl
        );
        const wxString& httpString = wxString::Format(stockURL, stockSymbol);
        wxLaunchDefaultBrowser(httpString);
    }
}

void TrxShareDialog::onOk(wxCommandEvent& WXUNUSED(event))
{
    double num_shares = 0;
    if (!w_num_text->checkValue(num_shares))
        return;

    double share_price = 0;
    if (!w_price_text->checkValue(share_price))
        return;

    double commission = 0;
    w_commission_text->GetDouble(commission);

    // allow for loyalty shares. These are "Free"
    bool loyalty_shares = (share_price == 0) && (num_shares > 0);

    if (w_tl_dlg->ValidCheckingAccountEntry()) {
        // addition or removal shares
        if (num_shares > 0 &&
            w_tl_dlg->TransactionType() == TrxType::e_deposit
        ) {
            // we need to subtract the number of shares for a sale
            num_shares = num_shares * -1;
        }

        int64 trx_id = w_tl_dlg->SaveChecking();
        if (trx_id < 0)
            return;

        // The PURCHASEDATE field in STOCK table holds the earliest purchase date of the stock.
        // NUMSHARES, PURCHASEPRICE and COMMISSION fields in the Stocks table are used as
        // a summary and allows Stock history to work in its current form.
        // The Shares table now maintains share_num, share_price, and commission on the
        // date of purchase, together with a record in the checking account table.
        if (!m_tl_n) {
             TrxLinkModel::instance().save_stock_record(
                trx_id, m_stock_n->m_id,
                w_tl_dlg->CheckingType()
            );
        }
        TrxShareModel::instance().update_trxID(trx_id,
            num_shares, share_price, commission, w_lot_text->GetValue(),
            m_local_deductible_splits
        );

        StockModel::instance().update_data_position(m_stock_n);
        if (!loyalty_shares) {
            StockHistoryModel::instance().save_record(
                m_stock_n->m_symbol,
                mmDate(w_tl_dlg->TransactionDate()),
                share_price,
                UpdateType(UpdateType::e_manual)
            );
        }
    }
    else {
        mmErrorDialogs::MessageWarning(this, _t("Invalid Transaction"), m_dialog_heading);
        return;
    }

    EndModal(wxID_OK);
}

void TrxShareDialog::onCalculateAmount(wxCommandEvent& WXUNUSED(event))
{
    double share_num = 0;
    if (!w_num_text->GetValue().empty()) {
        w_num_text->GetDouble(share_num);
    }

    double share_price = 0;
    if (!w_price_text->GetValue().empty()) {
        w_price_text->GetDouble(share_price);
    }

    double share_commission = 0;
    if (w_commission_text && !w_commission_text->GetValue().empty()) {
        w_commission_text->GetDouble(share_commission);
    }

    if (share_num > 0) {
        w_tl_dlg->SetTransactionValue(get_amount(share_num, share_price, share_commission));
    }
}

void TrxShareDialog::onDeductibleSplit(wxCommandEvent&)
{
    if (m_local_deductible_splits.empty()) {
        double commission = 0;
        w_commission_text->GetDouble(commission);

        const CategoryData* cat_n = CategoryModel::instance().get_key_data_n(
            _("Investment"), int64(-1L)
        );
        if (!cat_n) {
            CategoryData new_cat_d = CategoryData();
            new_cat_d.m_name = _("Investment");
            CategoryModel::instance().add_data_n(new_cat_d);
            cat_n = CategoryModel::instance().get_idN_data_n(new_cat_d.m_id);
        }
        m_local_deductible_splits.push_back({
            cat_n->m_id, commission, "", wxArrayInt64()
        });
    }

    SplitDialog dlg(this, m_local_deductible_splits, m_stock_n->m_account_id_n);

    if (dlg.ShowModal() == wxID_OK) {
        m_local_deductible_splits = dlg.mmGetResult();

        if (m_local_deductible_splits.size() == 1) {
            // TODO other informations
            w_commission_text->SetValue(
                m_local_deductible_splits[0].m_amount,
                PrefModel::instance().getSharePrecision()
            );

            m_local_deductible_splits.clear();
        }

        if (m_local_deductible_splits.empty()) {
            w_commission_text->Enable(true);
            mmToolTip(w_deduct_comm_btn, _t("Use Deductible Comm. split Categories"));
        }
        else {
            w_commission_text->SetValue(
                TrxSplitModel::instance().get_total(m_local_deductible_splits),
                PrefModel::instance().getSharePrecision()
            );
            w_commission_text->Enable(false);
            mmToolTip(w_deduct_comm_btn, TrxSplitModel::instance().get_tooltip(
                m_local_deductible_splits, nullptr /* currency */
            ));
        }
    }
}
