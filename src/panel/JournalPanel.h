/*******************************************************
Copyright (C) 2006 Madhan Kanagavel
Copyright (C) 2011, 2012 Stefano Giorgio
Copyright (C) 2013, 2014 Nikolay Akimov
Copyright (C) 2021-2025 Mark Whalley (mark@ipx.co.uk)
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
#include <wx/tglbtn.h>
#include <map>

#include "base/_constants.h"

#include "model/AccountModel.h"
#include "model/Journal.h"
#include "_PanelBase.h"
#include "JournalList.h"

class TrxFilterDialog;
class mmFrame;

class JournalPanel : public PanelBase
{
    friend class JournalList;

// -- static

public:
    enum FILTER_ID
    {
        FILTER_ID_DATE = 0,     // Not currently used
        FILTER_ID_DATE_RANGE,
        FILTER_ID_DATE_PICKER
    };
private:
    static const std::vector<std::pair<FILTER_ID, wxString>> FILTER_NAME;
public:
    static const wxString filter_id_name(FILTER_ID id);

public:
    enum
    {
        ICON_UNRECONCILED,
        ICON_RECONCILED,
        ICON_VOID,
        ICON_FOLLOWUP,
        ICON_DUPLICATE,
        ICON_DESC,
        ICON_ASC,
    };

private:
    wxDECLARE_EVENT_TABLE();
    enum
    {
        ID_DATE_RANGE_BUTTON = wxID_HIGHEST + 50,
        ID_DATE_RANGE_MIN,
        ID_DATE_RANGE_MAX = ID_DATE_RANGE_MIN + 99,
        ID_FILTER_ADVANCED,
        ID_FILTER_BUTTON,
        ID_START_DATE_PICKER,
        ID_END_DATE_PICKER,
        ID_DATE_RANGE_EDIT,
        ID_SCHEDULED_BUTTON,
    };

public:
    static void loadDateRanges(
        std::vector<mmDateRange2::Range>* date_range_a,
        int* date_range_m,
        bool all_ranges = false
    );
    static void mmPlayTransactionSound();

// -- state

private:
    // set by constructor or loadAccount()
    int64 m_account_group_id = -1;
        //  1..   : single account with id equal to m_account_group_id
        // -1     : all non-deleted transactions
        // -2     : deleted transactions
        // -3     : favorite accounts
        // -(4+X) : accounts of type X
    int64 m_account_id = -1;                    // applicable if m_account_group_id >= 1
    std::set<int64> m_account_id_m = {};        // applicable if m_account_group_id <= -3
    const AccountData* m_account_n = nullptr;   // non-null if m_account_group_id >= 1
    const CurrencyData* m_currency_n = nullptr; // currency of m_account_id, or base currency
    std::vector<mmDateRange2::Range> m_date_range_a = {};
    int m_date_range_m = -1;

    // set by gui
    mmDateRange2 m_date_range = mmDateRange2();
    FILTER_ID m_filter_id;
    bool m_filter_advanced;
    bool m_scheduled_enable;
    bool m_scheduled_selected;

    // calculated by filterList(); applicable if isAccount()
    double m_flow = 0.0;
    double m_balance = 0.0;
    double m_reconciled_balance = 0.0;
    double m_today_reconciled_balance = 0.0;
    bool m_show_reconciled;

    // set by showTips()
    bool m_show_tips = false;

    bool m_use_account_specific_filter;
    wxString m_info_panel_selectedbal;

    wxSharedPtr<TrxFilterDialog> w_filter_dlg;
    wxVector<wxBitmapBundle> w_image_a;
    JournalList*          w_list             = nullptr;
    mmFrame*              w_frame            = nullptr;
    wxButton*             w_range_btn        = nullptr;
    wxDatePickerCtrl*     w_start_date       = nullptr;
    wxDatePickerCtrl*     w_end_date         = nullptr;
    wxButton*             w_filter_btn       = nullptr;
    wxBitmapButton*       w_filter_reset_btn = nullptr;
    wxButton*             w_new_btn          = nullptr;
    wxButton*             w_edit_btn         = nullptr;
    wxButton*             w_dup_btn          = nullptr;
    wxButton*             w_delete_btn       = nullptr;
    wxButton*             w_enter_btn        = nullptr;
    wxButton*             w_skip_btn         = nullptr;
    wxButton*             w_restore_btn      = nullptr;
    wxBitmapButton*       w_attachment_btn   = nullptr;
    wxStaticText*         w_header_text      = nullptr;
    wxBitmapToggleButton* w_header_scheduled = nullptr;
    wxStaticText*         w_header_sortOrder = nullptr;
    wxGauge*              w_header_credit    = nullptr;
    wxStaticText*         w_header_balance   = nullptr;
    wxStaticText*         w_info_text        = nullptr;
    wxStaticText*         w_mini_text        = nullptr;

public:
    bool isAllTrans() const { return m_account_group_id == -1; }
    bool isDeletedTrans() const { return m_account_group_id == -2; }
    bool isGroup() const { return m_account_group_id <= -3; }
    bool isAccount() const { return m_account_group_id >= 1; }
    auto todayReconciledBalance() const -> double { return m_today_reconciled_balance; }
private:
    auto getPanelTitle() const -> wxString;

// -- constructor

public:
    JournalPanel(
        mmFrame* frame,
        wxWindow* parent_win,
        int64 account_group_id,
        const std::vector<int64>& group_ids = std::vector<int64>{}
    );
    ~JournalPanel();

private:
    bool create(
        wxWindow* perent_win,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxTAB_TRAVERSAL | wxNO_BORDER,
        const wxString& name = "JournalPanel"
    );
    void createControls();

// -- override

public:
    // override PanelBase
    virtual wxString buildPage() const override {
        return w_list->buildPage(m_account_n ? getPanelTitle() : "");
    }
    virtual void sortList() override {
        w_list->sortList();
    }

// -- methods

public:
    void loadAccount(int64 account_id = -1);
    void refreshList();
    void resetColumnView();
    void setSelectedTransaction(JournalKey journal_key);

private:
    void loadFilter();
    void loadFilterSettings();
    void saveFilterSettings();
    void filterList();

    void updateHeader();
    void updateFilter();
    void updateFilterTooltip();
    void updateScheduledToolTip();
    void updateExtraTransactionData(bool single, int repeat_num, bool foreign);
    void enableButtons(bool edit, bool dup, bool del, bool enter, bool skip, bool attach);
    void showTips(const wxString& tip);
    void showTips();
    void datePickProceed();
    void displaySplitCategories(JournalKey journal_key);

// -- event handlers

private:
    void onDateRangePopup(wxCommandEvent& event);
    void onDateRangeSelect(wxCommandEvent& event);
    void onStartDateChanged(wxDateEvent& event);
    void onEndDateChanged(wxDateEvent& event);
    void onFilter(wxCommandEvent& event);
    void onFilterCancel(wxCommandEvent& event);
    void onDateRangeEdit(wxCommandEvent& event);
    void onScheduled(wxCommandEvent& event);
    void onNewTrx(wxCommandEvent& event) { w_list->onNewTrx(event); }
    void onEditTrx(wxCommandEvent& event) {
        w_list->onEditTrx(event);
        w_list->SetFocus();
    }
    void onDeleteTrx(wxCommandEvent& event) { w_list->onDeleteTrx(event); }
    void onRestoreTrx(wxCommandEvent& event) { w_list->onRestoreTrx(event); }
    void onDuplicateTrx(wxCommandEvent& event) { w_list->onDuplicateTrx(event); }
    void onMoveTrx(wxCommandEvent& event) { w_list->onMoveTrx(event); }
    void onEnterSched(wxCommandEvent& event) { w_list->onEnterSched(event); }
    void onSkipSched(wxCommandEvent& event) { w_list->onSkipSched(event); }
    void onOpenAttachment(wxCommandEvent& event) {
        w_list->onOpenAttachment(event);
        w_list->SetFocus();
    }
    void onSearchTxtEntered(wxCommandEvent& event);
    void onButtonRightDown(wxMouseEvent& event);
    void onInfoPanelClick(wxMouseEvent& event, wxStaticText* infoPanel);
    void onReconcile(wxCommandEvent& event);
};
