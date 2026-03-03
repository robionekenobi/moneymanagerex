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

class mmDatePickerCtrl;
class mmTextCtrl;
class TrxLinkDialog;
class mmGUIFrame;

class AssetDialog : public wxDialog
{
    wxDECLARE_DYNAMIC_CLASS(AssetDialog);
    wxDECLARE_EVENT_TABLE();

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

private:
    AssetData* m_asset_n = nullptr;
    AssetData m_asset_d;
    const TrxLinkData* m_transfer_entry = nullptr;
    TrxData* m_checking_entry = nullptr;
    PrefModel::COMPOUNDING_ID m_compounding = PrefModel::COMPOUNDING_ID_DAY;
    wxString m_dialog_heading = _t("New Asset");
    bool m_hidden_trans_entry = true;
    bool m_asset_rich_text = true;

    wxChoice*         w_assetType            = nullptr;
    wxTextCtrl*       w_assetName            = nullptr;
    mmDatePickerCtrl* w_dpc                  = nullptr;
    wxTextCtrl*       w_notes                = nullptr;
    mmTextCtrl*       w_value                = nullptr;
    mmTextCtrl*       w_curr_val             = nullptr;
    wxChoice*         w_valueChange          = nullptr;
    wxStaticText*     w_compoundingLabel     = nullptr;
    wxChoice*         w_compoundingChoice    = nullptr;
    wxStaticText*     w_valueChangeRateLabel = nullptr;
    mmTextCtrl*       w_valueChangeRate      = nullptr;
    wxBitmapButton*   w_attachments          = nullptr;
    wxStaticBox*      w_transaction_frame    = nullptr;
    TrxLinkDialog* w_transaction_panel = nullptr;

public:
    AssetDialog() {};
    AssetDialog(
        wxWindow *parent,
        AssetData* asset,
        bool trans_data = false
    );
    AssetDialog(
        wxWindow *parent,
        const TrxLinkData* transfer_entry,
        TrxData* checking_entry
    );

    void SetTransactionAccountName(const wxString& account_name);
    void SetTransactionDate();
    int64 asset_id() const { return m_asset_n ? m_asset_n->id() : -1; }

private:
    bool Create(
        wxWindow* parent,
        wxWindowID id,
        const wxString& caption,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX
    );
    void CreateControls();

    void OnOk(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);
    void OnAttachments(wxCommandEvent& event);
    void enableDisableRate(bool en);
    double convertRate(
        int changeType,
        double xRate,
        int xCompounding,
        int yCompounding = PrefModel::COMPOUNDING_ID_DAY
    );
    void OnChangeAppreciationType(wxCommandEvent& event);
    void OnChangeCompounding(wxCommandEvent& event);
    void dataToControls();
    void changeFocus(wxChildFocusEvent& event);
    void OnQuit(wxCloseEvent& event);
    void CreateAssetAccount();
    void HideTransactionPanel();
};
