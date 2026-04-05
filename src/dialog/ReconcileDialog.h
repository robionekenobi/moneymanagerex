/*******************************************************
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

#include "util/mmCalcPopup.h"
#include "util/_simple.h"
#include "model/AccountModel.h"
#include "model/TrxModel.h"
#include "panel/JournalPanel.h"

class ReconcileDialog: public wxDialog
{
private:
    enum
    {
        SETTING_SHOW_STATE_COL,
        SETTING_SHOW_NUMBER_COL,
        SETTING_INCLUDE_VOID,
        SETTING_INCLUDE_DUPLICATED,
        SETTING_size
    };

private:
    wxDECLARE_DYNAMIC_CLASS(ReconcileDialog);
    enum
    {
        ID_CHECK_SHOW_STATE_COL = wxID_HIGHEST + 1,
        ID_CHECK_SHOW_NUMBER_COL,
        ID_CHECK_INCLUDE_VOID,
        ID_CHECK_INCLUDE_DUPLICATED,
        ID_BUTTON
    };

private:
    const AccountData*  m_account_n;
    const CurrencyData* m_currency_n;
    double              m_reconciledBalance;
    double              m_hiddenDuplicatedBalance;
    bool                m_ignore;
    bool                m_settings[SETTING_size];
    int                 m_colwidth[2]; // Store width for hidable columns
    std::vector<int64>  m_itemDataMap;

    wxVector<wxBitmapBundle> w_images;
    JournalPanel*      w_journal;
    mmTextCtrl*        w_amount_text;
    wxStaticText*      w_previous_bal_text;
    wxStaticText*      w_cleared_bal_text;
    wxStaticText*      w_ending_bal_text;
    wxStaticText*      w_diff_label;
    wxStaticText*      w_diff_text;
    wxListCtrl*        w_left_list;
    wxListCtrl*        w_right_list;
    wxButton*          w_cancel_btn;
    wxButton*          w_reconcile_btn;
    wxButton*          w_later_btn;
    wxBitmapButton*    w_calc_btn;
    wxButton*          w_edit_btn;
    mmCalcPopup*       w_calc_popup;

public:
    ReconcileDialog();
    ReconcileDialog(
        wxWindow* parent_win,
        const AccountData* account_n,
        JournalPanel* journal_panel_n
    );
    ~ReconcileDialog();

private:
    void CreateControls();
    void UpdateAll();
    void FillControls(bool init = false);

    void OnCalculator(wxCommandEvent& event);
    void OnAmountChanged(wxCommandEvent& event);

    void OnToggle(wxCommandEvent& event);
    void OnClose(wxCommandEvent& event);
    void OnEdit(wxCommandEvent& event);
    void OnNew(wxCommandEvent& event);
    void OnSettings(wxCommandEvent& event);

    void OnMenuSelected(wxCommandEvent& event);
    void OnMenuItemChecked(wxCommandEvent& event);

    void OnLeftItemLeftClick(wxMouseEvent& event);
    void OnLeftItemRightClick(wxMouseEvent& event);
    void OnRightItemLeftClick(wxMouseEvent& event);
    void OnRightItemRightClick(wxMouseEvent& event);

    void OnListKeyDown(wxKeyEvent& event);
    void OnListItemSelection(wxListEvent& event);
    void updateButtonState();

    void OnLeftFocus(wxFocusEvent& event);
    void OnRightFocus(wxFocusEvent& event);
    void handleListFocus(wxListCtrl* list);

    void OnLeftFocusKill(wxFocusEvent& event);
    void OnRightFocusKill(wxFocusEvent& event);
    void handleListFocusKill(wxListCtrl* list);

    void setListItemData(const TrxData* trx, wxListCtrl* list, long item);
    void processRightClick(wxListCtrl* list, long item);
    void processLeftClick(wxListCtrl* list, wxPoint pt);
    void addTransaction2List(const TrxData* trx);
    long getListIndexByDate(const TrxData* trx, wxListCtrl* list);
    void moveItemData(wxListCtrl* list, int row1, int row2);
    void resetListSelections(wxListCtrl* list);
    void newTransaction();
    void editTransaction(wxListCtrl* list, long item);

    void OnSize(wxSizeEvent& event);
    void resizeColumns();
    bool isListItemChecked(wxListCtrl* list, long item);

    void applyColumnSettings();
    void showHideColumn(bool show, int col, int cs);
};
