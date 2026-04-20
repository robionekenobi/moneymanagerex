/*******************************************************
 Copyright (C) 2013 - 2016, 2020, 2022 Nikolay Akimov

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

#include "model/AssetModel.h"
#include "model/PrefModel.h"
#include "model/TrxLinkModel.h"

class mmDatePicker;
class mmTextCtrl;
class TrxLinkDialog;
class mmFrame;

class AssetDialog : public wxDialog
{
    wxDECLARE_DYNAMIC_CLASS(AssetDialog);
    wxDECLARE_EVENT_TABLE();

// -- static

private:
    enum
    {
        IDC_COMBO_TYPE = wxID_HIGHEST + 1100,
        IDC_VALUE,
        IDC_CURR_VAL,
        IDC_COMPOUNDING,
        IDC_RATE,
        IDC_NOTES,
    };

// -- state

private:
    AssetData* m_asset_n = nullptr;
    const TrxLinkData* m_tl_n = nullptr;
    TrxData* m_trx_n = nullptr;
    PrefModel::COMPOUNDING_ID m_compounding = PrefModel::COMPOUNDING_ID_DAY;
    bool m_show_trx_panel = false;
    bool m_asset_rich_text = true;
    wxString m_dialog_heading = _t("New Asset");

    wxChoice*       w_type_choice        = nullptr;
    wxTextCtrl*     w_name_text          = nullptr;
    mmDatePicker*   w_date_picker        = nullptr;
    wxTextCtrl*     w_notes_text         = nullptr;
    mmTextCtrl*     w_value_text         = nullptr;
    mmTextCtrl*     m_current_value_text = nullptr;
    wxChoice*       w_change_choice      = nullptr;
    wxStaticText*   w_compounding_label  = nullptr;
    wxChoice*       w_compounding_choice = nullptr;
    wxStaticText*   w_rate_label         = nullptr;
    mmTextCtrl*     w_rate_text          = nullptr;
    wxBitmapButton* w_att_btn            = nullptr;
    wxStaticBox*    w_tl_frame           = nullptr;
    TrxLinkDialog*  w_tl_dlg             = nullptr;

public:
    auto asset_id() const -> int64 { return m_asset_n ? m_asset_n->m_id : -1; }

// -- constructor

public:
    AssetDialog() {};
    AssetDialog(
        wxWindow* parent_win,
        AssetData* asset_n,
        bool show_trx_panel = false
    );
    AssetDialog(
        wxWindow* parent_win,
        const TrxLinkData* tl_n,
        TrxData* trx_n
    );

private:
    bool create(
        wxWindow* parent_win,
        wxWindowID win_id,
        const wxString& caption,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX
    );
    void createControls();
    void dataToControls();

// -- methods

public:
    void setTransactionAccountName(const wxString& account_name);
    void setTransactionDate();

private:
    void hideTransactionPanel();
    void enableDisableRate(bool en);
    void createAssetAccount();
    double convertRate(
        int changeType,
        double xRate,
        int xCompounding,
        int yCompounding = PrefModel::COMPOUNDING_ID_DAY
    );

// -- event handlers

private:
    void onOk(                     wxCommandEvent&    event);
    void onCancel(                 wxCommandEvent&    event);
    void onAttachments(            wxCommandEvent&    event);
    void onChangeAppreciationType( wxCommandEvent&    event);
    void onChangeCompounding(      wxCommandEvent&    event);
    void onChangeFocus(            wxChildFocusEvent& event);
    void onQuit(                   wxCloseEvent&      event);
};
