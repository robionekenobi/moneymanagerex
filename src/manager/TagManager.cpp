/*******************************************************
 Copyright (C) 2006 Madhan Kanagavel
 Copyright (C) 2015 Gabriele-V
 Copyright (C) 2022  Mark Whalley (mark@ipx.co.uk)

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

#include "model/InfoModel.h"
#include "model/TagLinkModel.h"
#include "model/TagModel.h"
#include "model/TrxModel.h"
#include "model/TrxSplitModel.h"

#include "TagManager.h"

wxIMPLEMENT_DYNAMIC_CLASS(TagManager, wxDialog);

wxBEGIN_EVENT_TABLE(TagManager, wxDialog)
    EVT_BUTTON(wxID_ADD,             TagManager::OnAdd)
    EVT_BUTTON(wxID_EDIT,            TagManager::OnEdit)
    EVT_BUTTON(wxID_REMOVE,          TagManager::OnDelete)
    EVT_TEXT(wxID_FIND,              TagManager::OnTextChanged)
    EVT_LISTBOX(wxID_VIEW_LIST,      TagManager::OnListSelChanged)
    EVT_CHECKLISTBOX(wxID_VIEW_LIST, TagManager::OnCheckboxSelChanged)
    EVT_BUTTON(wxID_OK,              TagManager::OnOk)
    EVT_BUTTON(wxID_CANCEL,          TagManager::OnCancel)
wxEND_EVENT_TABLE()

TagManager::TagManager() :
    m_is_selection(false)
{
}

TagManager::TagManager(
    wxWindow* parent_win,
    bool is_selection,
    const wxArrayString& selected_tag_a
) :
    m_is_selection(is_selection),
    m_selected_tag_a(selected_tag_a)
{
    this->SetFont(parent_win->GetFont());
    create(parent_win);
    if (m_is_selection) {
        for (const auto& tag : selected_tag_a) {
            int index = getTagIndex(tag);
            if (index != wxNOT_FOUND)
                dynamic_cast<wxCheckListBox*>(w_tag_list)->Check(index);
        }
    }

    SetMinSize(wxSize(300, 420));
    Fit();
}

TagManager::~TagManager()
{
    InfoModel::instance().saveSize("TAG_DIALOG_SIZE", GetSize());
}

bool TagManager::create(
    wxWindow* parent_win,
    wxWindowID win_id,
    const wxString& caption,
    const wxString& name,
    const wxPoint& pos,
    const wxSize& size,
    long style
) {
    SetExtraStyle(GetExtraStyle() | wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create(parent_win, win_id, caption, pos, size, style, name);

    SetEvtHandlerEnabled(false);

    createControls();
    mmThemeAutoColour(this);
    SetEvtHandlerEnabled(true);

    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    this->SetInitialSize();
    SetIcon(mmPath::getProgramIcon());

    Fit();
    mmSetSize(this);
    Centre();
    return true;
}

void TagManager::createControls()
{
    wxBoxSizer* boxSizer = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer);

    for (const auto& tag_d : TagModel::instance().find_data_a(
        TableClause::ORDERBY(TagCol::NAME_TAGNAME)
    ))
        m_tag_a.Add(tag_d.m_name);

    if (!m_is_selection)
        w_tag_list = new wxListBox(this,
            wxID_VIEW_LIST,
            wxDefaultPosition, wxDefaultSize,
            m_tag_a,
            wxLB_EXTENDED | wxLB_SORT
        );
    else
        w_tag_list = new wxCheckListBox(this,
            wxID_VIEW_LIST,
            wxDefaultPosition, wxDefaultSize,
            m_tag_a,
            wxLB_EXTENDED | wxLB_SORT
        );

    boxSizer->Add(w_tag_list, g_flagsExpand);

    wxPanel* searchPanel = new wxPanel(this, wxID_ANY);
    boxSizer->Add(searchPanel, wxSizerFlags(g_flagsExpand).Proportion(0));
    wxBoxSizer* search_sizer = new wxBoxSizer(wxHORIZONTAL);
    searchPanel->SetSizer(search_sizer);

    w_search_ctrl = new wxSearchCtrl(searchPanel, wxID_FIND);
    search_sizer->Add(new wxStaticText(searchPanel, wxID_STATIC, _t("Search")), g_flagsH);
    search_sizer->Add(w_search_ctrl, g_flagsExpand);

    wxPanel* buttonsPanel = new wxPanel(this, wxID_ANY);
    boxSizer->Add(buttonsPanel, wxSizerFlags(g_flagsV).Center());
    wxBoxSizer* buttonsSizer = new wxBoxSizer(wxVERTICAL);
    buttonsPanel->SetSizer(buttonsSizer);

    wxStdDialogButtonSizer* editButtonSizer = new wxStdDialogButtonSizer;
    buttonsSizer->Add(editButtonSizer, wxSizerFlags(g_flagsV).Border(wxALL, 0).Center());

    w_add_btn = new wxButton(buttonsPanel, wxID_ADD, _t("&Add "));
    editButtonSizer->Add(w_add_btn, g_flagsH);
    mmToolTip(w_add_btn, _t("Add a new tag"));

    w_edit_btn = new wxButton(buttonsPanel, wxID_EDIT, _t("&Edit "));
    editButtonSizer->Add(w_edit_btn, g_flagsH);
    w_edit_btn->Enable(false);
    mmToolTip(w_edit_btn, _t("Edit the name of an existing tag"));

    w_delete_btn = new wxButton(buttonsPanel, wxID_REMOVE, _t("&Delete "));
    editButtonSizer->Add(w_delete_btn, g_flagsH);
    w_delete_btn->Enable(false);
    mmToolTip(w_delete_btn, _t("Delete an existing tag. The tag is unable to be used by existing transactions."));

    //--------------------------
    wxStdDialogButtonSizer* dlgButtonSizer = new wxStdDialogButtonSizer();
    boxSizer->Add(dlgButtonSizer, wxSizerFlags(g_flagsV).Centre());

    wxButton* itemButton24 = new wxButton(this, wxID_OK,
        (m_is_selection ? _t("Select") : _t("&OK "))
    );
    dlgButtonSizer->Add(itemButton24, g_flagsH);

    wxButton* itemButton25 = new wxButton(this, wxID_CANCEL,
        wxGetTranslation(m_is_selection ? g_CancelLabel : g_CloseLabel)
    );
    dlgButtonSizer->Add(itemButton25, g_flagsH);
}

int TagManager::getTagIndex(const wxString& tag)
{
    // FindString() without the second argument (bCase) fails, at least on macOS
    // Temporary fix: set bCase to true (although this is not wanted)
    // FIXME: implement a proper fix, or wait for a fix in wxWidgets
    return w_tag_list->FindString(tag, true);
}

void TagManager::fillControls()
{
    Freeze();
    wxArrayString filteredList;
    for (const auto& tag : m_tag_a)
        if (tag.Lower().Matches(m_mask_s + "*"))
            filteredList.Add(tag);

    w_tag_list->Set(filteredList);

    if (m_is_selection) {
        // reselect previously selected items
        for (const auto& tag : m_selected_tag_a) {
            int index = getTagIndex(tag);
            if (index != wxNOT_FOUND)
                dynamic_cast<wxCheckListBox*>(w_tag_list)->Check(index);
        }
    }

    w_edit_btn->Disable();
    w_delete_btn->Disable();
    Thaw();
}

bool TagManager::validateName(const wxString& name)
{
    if (name == "&" || name == "|") {
        wxString errMsg = _t("Invalid tag name");
        errMsg << "\n\n" << _t("Tag names may not be the '&' or '|' characters because these are reserved for filter operators");
        wxMessageBox(errMsg, _t("Tag Manager: Invalid Name"), wxOK | wxICON_ERROR);
        return false;
    }
    else if (name.Find(' ') != wxNOT_FOUND) {
        wxString errMsg = _t("Name contains tag delimiter.");
        errMsg << "\n\n" << _t("Tag names may not contain the space (' ') character");
        wxMessageBox(errMsg, _t("Tag Manager: Invalid Name"), wxOK | wxICON_ERROR);
        return false;
    }
    return true;
    
}

void TagManager::OnOk(wxCommandEvent& WXUNUSED(event))
{
    EndModal(wxID_OK);
}

void TagManager::OnCancel(wxCommandEvent& WXUNUSED(event))
{
    EndModal(wxID_CANCEL);
}

void TagManager::OnAdd(wxCommandEvent& WXUNUSED(event))
{
    wxString prompt_msg = _t("Enter the name for the new tag:");
    wxString text = wxGetTextFromUser(prompt_msg, _t("Add Tag"), "");

    while (!validateName(text))
        text = wxGetTextFromUser(prompt_msg, _t("Add Tag"), text);

    if (text.IsEmpty())
        return;

    if (TagModel::instance().find_count(
        TagCol::WHERE_TAGNAME(OP_EQ, text)
    ) > 0) {
        wxMessageBox(
            _t("A tag with this name already exists"),
            _t("Tag Manager: Adding Error"),
            wxOK | wxICON_ERROR
        );
        return;
    }

    TagData new_tag_d = TagData();
    new_tag_d.m_name = text;
    TagModel::instance().add_data_n(new_tag_d);

    m_refresh_requested = true;
    m_tag_a.Add(text);
    fillControls();
    setSelectedString(text);
}

void TagManager::OnEdit(wxCommandEvent& WXUNUSED(event))
{
    wxArrayInt selections;
    wxString old_name;
    
    w_tag_list->GetSelections(selections);
    if (selections.IsEmpty())
        return;
    old_name = w_tag_list->GetString(selections[0]); 

    const wxString msg = wxString::Format(_t("Enter a new name for '%s'"), old_name);
    wxString text = wxGetTextFromUser(msg, _t("Edit Tag"), old_name);

    while (!validateName(text))
        text = wxGetTextFromUser(msg, _t("Edit Tag"), text);

    if (text.IsEmpty() || old_name == text)
        return;

    const TagData* tag_n = TagModel::instance().get_name_data_n(text);
    if (tag_n) {
        wxString errMsg = _t("A tag with this name already exists");
        wxMessageBox(errMsg, _t("Tag Manager: Editing Error"), wxOK | wxICON_ERROR);
        return;
    }

    tag_n = TagModel::instance().get_name_data_n(old_name);
    TagData tag_d = *tag_n;
    tag_d.m_name = text;
    TagModel::instance().save_data_n(tag_d);

    m_tag_a.Remove(old_name);
    m_tag_a.Add(text);
    int index = m_selected_tag_a.Index(old_name);
    if (index != wxNOT_FOUND) {
        m_selected_tag_a.RemoveAt(index);
        m_selected_tag_a.Add(text);
    }
    m_refresh_requested = true;
    fillControls();
    setSelectedString(text);
}

void TagManager::OnDelete(wxCommandEvent& WXUNUSED(event))
{
    wxArrayInt index_a;
    w_tag_list->GetSelections(index_a);
    wxArrayString tag_name_a;
    for (const auto& index : index_a)
        tag_name_a.Add(w_tag_list->GetString(index));

    if (tag_name_a.IsEmpty())
        return;

    TagModel::instance().db_savepoint();
    TagLinkModel::instance().db_savepoint();
    TrxModel::instance().db_savepoint();
    TrxSplitModel::instance().db_savepoint();

    for (const auto& tag_name : tag_name_a) {
        const TagData* tag_n = TagModel::instance().get_name_data_n(tag_name);
        if (TagModel::instance().find_id_isUsed(tag_n->m_id, true)) {
            wxMessageBox(
                wxString::Format(_t("Tag '%s' in use"), tag_n->m_name),
                _t("Tag Manager: Delete Error"),
                wxOK | wxICON_ERROR
            );
            continue;
        }

        // Tag may be used in deleted tranasctions.
        std::set<int64> trx_id_m;
        for (const auto& gl_d : TagLinkModel::instance().find_data_a(
            TagLinkCol::WHERE_TAGID(OP_EQ, tag_n->m_id)
        )) {
            // gl_d points to a deleted transaction or its splits.
            if (gl_d.m_ref_type == TrxModel::s_ref_type) {
                trx_id_m.insert(gl_d.m_ref_id);
            }
            else if (gl_d.m_ref_type == TrxSplitModel::s_ref_type) {
                const TrxSplitData* tp_n = TrxSplitModel::instance().get_idN_data_n(
                    gl_d.m_ref_id
                );
                trx_id_m.insert(tp_n->m_trx_id);
            }
        }

        if (!trx_id_m.empty()) {
            wxMessageDialog msgDlg(this,
                wxString::Format(
                    _t("Deleted transactions exist which use tag '%s'."),
                    tag_n->m_name
                ) + "\n\n" +
                    _t("Deleting the tag will also automatically purge the associated deleted transactions.") + "\n\n" +
                    _t("Do you want to continue?"),
                _t("Confirm Tag Deletion"),
                wxYES_NO | wxNO_DEFAULT | wxICON_WARNING
            );
            if (msgDlg.ShowModal() != wxID_YES)
                continue;
        }

        // Purge all deleted transactions which use this tag.
        // This also deletes the associated tag links.
        for (int64 trx_id : trx_id_m) {
            TrxModel::instance().purge_id(trx_id);
        }

        TagModel::instance().purge_id(tag_n->m_id);

        m_tag_a.Remove(tag_name);
        int index = m_selected_tag_a.Index(tag_name);
        if (index != wxNOT_FOUND)
            m_selected_tag_a.RemoveAt(index);
    }

    TrxSplitModel::instance().db_release_savepoint();
    TrxModel::instance().db_release_savepoint();
    TagLinkModel::instance().db_release_savepoint();
    TagModel::instance().db_release_savepoint();

    m_refresh_requested = true;
    fillControls();
    int new_index = std::min(
        index_a[0],
        static_cast<int>(w_tag_list->GetCount()) - 1
    );
    if (new_index >= 0)
        setSelectedItem(new_index);
}

void TagManager::OnTextChanged(wxCommandEvent& event)
{
    m_mask_s = event.GetString();
    if (!m_mask_s.IsEmpty())
        m_mask_s = m_mask_s.Lower().Prepend("*");
    fillControls();
    w_search_ctrl->SetFocus();
    w_search_ctrl->SetInsertionPointEnd();
}

void TagManager::OnListSelChanged(wxCommandEvent& WXUNUSED(event))
{
    w_edit_btn->Enable(false);
    w_delete_btn->Enable(false);

    wxArrayInt tag_i_a;
    wxArrayString tag_name_a;

    w_tag_list->GetSelections(tag_i_a);
    for (const auto& tag_i : tag_i_a)
        tag_name_a.Add(w_tag_list->GetString(tag_i));

    int count = tag_i_a.GetCount();

    // Can only edit one tag at a time
    if (count == 1) {
        w_edit_btn->Enable();
    }
    // Can delete multiple tags at once as long as all are unused
    if (count > 0) {
        bool is_used = false;
        for (const auto& tag_name : tag_name_a) {
            const TagData* tag_n = TagModel::instance().get_name_data_n(tag_name);
            is_used = is_used || TagModel::instance().find_id_isUsed(tag_n->m_id, true);
        }
        w_delete_btn->Enable(!is_used);
    }    
}

void TagManager::OnCheckboxSelChanged(wxCommandEvent& event)
{
    if (dynamic_cast<wxCheckListBox*>(w_tag_list)->IsChecked(event.GetSelection()))
        m_selected_tag_a.Add(event.GetString());
    else {
        int index = m_selected_tag_a.Index(event.GetString());
        if (index != wxNOT_FOUND)
            m_selected_tag_a.RemoveAt(index);
    }
}

void TagManager::setSelectedItem(int index)
{
    w_tag_list->EnsureVisible(index);
    w_tag_list->SetSelection(index);

    wxCommandEvent evt;
    OnListSelChanged(evt);
}

void TagManager::setSelectedString(const wxString& tagname)
{
    int index = getTagIndex(tagname);
    if (index != wxNOT_FOUND)
        setSelectedItem(index);
}
