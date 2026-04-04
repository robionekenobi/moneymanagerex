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

#pragma once

#include "base/_defs.h"
#include "util/mmDatePicker.h"
#include "util/mmTextCtrl.h"
#include "util/mmColorButton.h"
#include "util/_simple.h"
#include "FieldValueDialog.h"

class TrxUpdateDialog : public wxDialog
{
    wxDECLARE_DYNAMIC_CLASS(TrxUpdateDialog);
    wxDECLARE_EVENT_TABLE();

private:
    enum
    {
        /* Transaction Dialog */
        ID_PAYEE = wxID_HIGHEST + 997,
        ID_TRANS_TYPE,
        ID_TRANS_ACC,
        ID_BTN_CUSTOMFIELDS,
        ID_CUSTOMFIELDS,
    };

private:
    std::vector<int64> m_trx_id_a;
    const CurrencyData* m_currency_n = nullptr;
    bool m_hasTransfers = false, m_hasNonTransfers = false, m_hasSplits = false;

    wxSharedPtr<FieldValueDialog> m_custom_fields;
    wxCheckBox*         w_payee_cb             = nullptr;
    mmComboBoxPayee*    w_payee_text           = nullptr;
    wxCheckBox*         w_date_cb              = nullptr;
    mmDatePicker*       w_date_picker          = nullptr;
    wxCheckBox*         w_time_cb              = nullptr;
    wxTimePickerCtrl*   w_time_picker          = nullptr;
    wxCheckBox*         w_type_cb              = nullptr;
    wxChoice*           w_type_choice          = nullptr;
    wxCheckBox*         w_status_cb            = nullptr;
    wxChoice*           w_status_choice        = nullptr;
    wxCheckBox*         m_transferAcc_checkbox = nullptr;
    mmComboBoxAccount*  w_account_text         = nullptr;
    wxCheckBox*         w_cat_cb               = nullptr;
    mmComboBoxCategory* w_cat_text             = nullptr;
    mmColorButton*      w_color_btn            = nullptr;
    wxCheckBox*         w_color_cb             = nullptr;
    wxCheckBox*         w_tag_cb               = nullptr;
    wxCheckBox*         tag_append_checkbox_   = nullptr;
    mmTagTextCtrl*      w_tag_text             = nullptr;
    wxCheckBox*         w_amount_cb            = nullptr;
    mmTextCtrl*         w_amount_text          = nullptr;
    wxCheckBox*         w_notes_cb             = nullptr;
    wxTextCtrl*         w_notes_text           = nullptr;
    wxCheckBox*         m_append_checkbox      = nullptr;

public:
    TrxUpdateDialog();
    TrxUpdateDialog(wxWindow* parent, std::vector<int64>& trx_id_a);
    ~TrxUpdateDialog();

private:
    bool Create(wxWindow* parent,
        wxWindowID id = wxID_ANY,
        const wxString& caption = _n("Multi Transactions Update"),
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxSize(500, 300),
        long style = wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX
    );
    void CreateControls();
    void OnOk(wxCommandEvent& event);
    void OnCheckboxClick(wxCommandEvent& event);
    void OnComboKey(wxKeyEvent& event);
    void onFocusChange(wxChildFocusEvent& event);
    void SetPayeeTransferControls();
    void OnTransTypeChanged(wxCommandEvent&);
    void OnMoreFields(wxCommandEvent& event);
};

