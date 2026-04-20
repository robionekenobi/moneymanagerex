/*******************************************************
 Copyright (C) 2006 Madhan Kanagavel
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

#pragma once

#include "base/_defs.h"
#include "util/_simple.h"
#include "model/StockModel.h"

class mmDatePicker;
class mmTextCtrl;
class mmFrame;

class StockDialog : public wxDialog
{
    wxDECLARE_DYNAMIC_CLASS(StockDialog);
    wxDECLARE_EVENT_TABLE();

// -- static

private:
    enum
    {
        ID_DPC_STOCK_PDATE = wxID_HIGHEST + 800,
        ID_TEXTCTRL_STOCKNAME,
        ID_TEXTCTRL_STOCK_SYMBOL,
        ID_TEXTCTRL_NUMBER_SHARES,
        ID_TEXTCTRL_STOCK_PP,
        ID_TEXTCTRL_STOCK_CP,
        ID_STATIC_STOCK_VALUE,
        ID_TEXTCTRL_STOCK_COMMISSION,
        ID_TEXTCTRL_STOCK_CURR_PRICE,
        ID_DIALOG_STOCKS,
        ID_DPC_CP_PDATE,
        ID_BUTTON_IMPORT,
        ID_BUTTON_DOWNLOAD
    };

// -- state

private:
    StockData* m_stock_n = nullptr;
    int64 m_account_id = -1;

    wxTextCtrl*     w_name_text            = nullptr;
    wxTextCtrl*     w_symbol_text          = nullptr;
    mmTextCtrl*     w_num_text             = nullptr;
    mmTextCtrl*     w_purchase_price_text  = nullptr;
    wxTextCtrl*     w_notes_text           = nullptr;
    mmTextCtrl*     w_history_price_text   = nullptr;
    mmDatePicker*   w_purchase_date_picker = nullptr;
    mmDatePicker*   w_history_date_picker  = nullptr;
    mmTextCtrl*     w_commission_text      = nullptr;
    mmTextCtrl*     w_current_price_text   = nullptr;
    wxBitmapButton* w_att_btn              = nullptr;
    wxListCtrl*     w_price_list           = nullptr;
    wxButton*       w_ok_btn               = nullptr;
    wxStaticText*   w_date_label           = nullptr;
    wxStaticText*   w_value_label          = nullptr;

public:
    auto stock_id() const -> int64 { return m_stock_n ? m_stock_n->m_id : -1; }

// -- constructor

public:
    StockDialog() {}
    StockDialog(
        wxWindow* parent_win,
        StockData* stock_n,
        int64 account_id,
        const wxString& name = "StockDialog"
    );

private:
    bool create(
        wxWindow* parent_win,
        wxWindowID win_id,
        const wxString& caption,
        const wxPoint& pos,
        const wxSize& size,
        long style,
        const wxString& name = "StockDialog"
    );
    void createControls();
    void dataToControls();

// methods

private:
    void updateControls();
    void showStockHistory();
    void createShareAccount(
        const AccountData* stock_account,
        const wxString& name,
        const wxString& openingDate
    );

// -- event handlers

private:
    void onQuit(                  wxCloseEvent&      event);
    void onSave(                  wxCommandEvent&    event);
    void onCancel(                wxCommandEvent&    event);
    void onAttachments(           wxCommandEvent&    event);
    void onStockPriceButton(      wxCommandEvent&    event);
    void onHistoryImportButton(   wxCommandEvent&    event);
    void onHistoryDownloadButton( wxCommandEvent&    event);
    void onHistoryAddButton(      wxCommandEvent&    event);
    void onHistoryDeleteButton(   wxCommandEvent&    event);
    void onListItemSelected(      wxListEvent&       event);
    void onFocusChange(           wxChildFocusEvent& event);
};

