/*******************************************************
 Copyright (C) 2016 Stefano Giorgio

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

#include "model/StockModel.h"
#include "model/TrxLinkModel.h"
#include "model/TrxShareModel.h"

class mmDatePicker;
class mmTextCtrl;
class TrxLinkDialog;

class TrxShareDialog : public wxDialog
{
    wxDECLARE_DYNAMIC_CLASS(TrxShareDialog);
    wxDECLARE_EVENT_TABLE();

// -- static

private:
    enum
    {
        ID_STOCKTRANS_DATEPICKER_CHANGE = wxID_HIGHEST + 820,
        ID_STOCKTRANS_SHARE_NAME,
        ID_STOCKTRANS_SHARE_SYMBOL,
        ID_STOCKTRANS_SHARE_NUMBER,
        ID_STOCKTRANS_SHARE_PRICE,
        ID_STOCKTRANS_SHARE_LOT,
        ID_STOCKTRANS_SHARE_COMMISSION,
        mmID_COMM_SPLIT,
    };

// -- state

private:
    TrxData* m_trx_n = nullptr;
    const TrxLinkData* m_tl_n = nullptr;
    TrxShareData* m_ts_n = nullptr;
    StockData* m_stock_n = nullptr;
    wxString m_dialog_heading;
    std::vector<Split> m_local_deductible_splits;
    std::vector<Split> m_local_non_deductible_splits;

    TrxLinkDialog*  w_tl_dlg          = nullptr;
    wxTextCtrl*     w_name_text       = nullptr;
    mmTextCtrl*     w_num_text        = nullptr;
    wxTextCtrl*     w_symbol_text     = nullptr;
    mmTextCtrl*     w_price_text      = nullptr;
    wxTextCtrl*     w_lot_text        = nullptr;
    mmTextCtrl*     w_commission_text = nullptr;
    wxBitmapButton* w_deduct_comm_btn = nullptr;
    wxTextCtrl*     w_notes_text      = nullptr;
    wxBitmapButton* w_att_btn         = nullptr;
    wxBitmapButton* w_web_btn         = nullptr;

public:
    auto stock_id() const -> int64 { return m_stock_n ? m_stock_n->m_id : -1; }

// -- constructor

public:
    TrxShareDialog();
    TrxShareDialog(
        wxWindow* parent_win,
        StockData* stock_n
    );
    TrxShareDialog(
        wxWindow* parent_win,
        const TrxLinkData* tl_n,
        TrxData* trx_n
    );

private:
    bool create(
        wxWindow* parent_win,
        wxWindowID win_id = wxID_ANY,
        const wxString& caption = _t("Edit Share Transaction"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX
    );
    void createControls();
    void dataToControls();

// -- methods

private:
    auto get_amount(double shares, double price, double commission) const -> double;

// -- event handlers

private:
    void onQuit(             wxCloseEvent&   WXUNUSED(event));
    void onOk(               wxCommandEvent& WXUNUSED(event));
    void onCancel(           wxCommandEvent& WXUNUSED(event));
    void onStockPriceButton( wxCommandEvent& event);
    void onCalculateAmount(  wxCommandEvent& event);
    void onDeductibleSplit(  wxCommandEvent& event);
};
