/*******************************************************
 Copyright (C) 2014 Gabriele-V
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

#include "AttachmentDialog.h"

#include "base/_defs.h"
#include <wx/mimetype.h>

#include "base/_constants.h"
#include "base/mmPlatform.h"
#include "util/mmPath.h"
#include "util/mmImage.h"
#include "util/mmAttachment.h"
#include "util/_util.h"
#include "util/_simple.h"

#include "model/InfoModel.h"
#include "model/AssetModel.h"
#include "model/StockModel.h"
#include "model/PayeeModel.h"
#include "model/AttachmentModel.h"

wxIMPLEMENT_DYNAMIC_CLASS(AttachmentDialog, wxDialog);

wxBEGIN_EVENT_TABLE( AttachmentDialog, wxDialog )
    EVT_BUTTON(wxID_CANCEL,                  AttachmentDialog::onCancel)
    EVT_BUTTON(wxID_OK,                      AttachmentDialog::onOk)
    EVT_BUTTON(wxID_APPLY,                   AttachmentDialog::onMagicButton)
    EVT_DATAVIEW_SELECTION_CHANGED(wxID_ANY, AttachmentDialog::onListItemSelected)
    EVT_DATAVIEW_ITEM_CONTEXT_MENU(wxID_ANY, AttachmentDialog::onItemRightClick)
    EVT_DATAVIEW_ITEM_ACTIVATED(wxID_ANY,    AttachmentDialog::onListItemActivated)
    EVT_MENU_RANGE(
        MENU_NEW_ATTACHMENT,
        MENU_DELETE_ATTACHMENT,              AttachmentDialog::onMenuSelected)
wxEND_EVENT_TABLE()

// -- constructor

AttachmentDialog::AttachmentDialog(
    wxWindow* parent_win,
    RefTypeN ref_type,
    int64 ref_id,
    const wxString& name
) :
    m_ref_type(ref_type),
    m_ref_id(ref_id)
{
    if (m_debug)
        m_col_name_m[ATTACHMENT_ID] = "#";
    m_col_name_m[ATTACHMENT_DESCRIPTION] = _t("Description");
    m_col_name_m[ATTACHMENT_FILENAME] = _t("File");

    create(parent_win, name);
    mmThemeAutoColour(this);

    const wxString folder = mmAttachment::getFolder();

    if (folder == wxEmptyString) {
        wxString msgStr = wxString() << _t("Attachment folder not defined.") << "\n"
            << _tu("Please set it in Tools → Settings… → Attachments") << "\n";
        wxMessageBox(msgStr, _t("Attachment folder not defined"), wxICON_ERROR);
    }
    else if (!wxDirExists(folder)) {
        wxString msgStr = wxString() << _t("Unable to find attachments folder:") << "\n"
            << "'" << folder << "'" << "\n"
            << "\n"
            << _t("Please verify that above path is correct") << "\n";
        wxMessageBox(msgStr, _t("Attachments folder not found."), wxICON_ERROR);
    }
}

void AttachmentDialog::create(wxWindow* parent_win, const wxString& name)
{
    SetExtraStyle(GetExtraStyle() | wxWS_EX_BLOCK_EVENTS);
    long style = wxCAPTION | wxCLOSE_BOX | wxRESIZE_BORDER;

    wxString windowTitle;
    // Note: m_ref_id is equal to 0 for new references without id.
    if (m_ref_id > 0) {
        wxString ref_name;
        switch (m_ref_type.id_n())
        {
        case RefTypeN::e_stock:
            ref_name = StockModel::instance().get_id_name(m_ref_id);
            break;
        case RefTypeN::e_asset:
            ref_name = AssetModel::instance().get_id_name(m_ref_id);
            break;
        case RefTypeN::e_account:
            ref_name = AccountModel::instance().get_id_name(m_ref_id);
            break;
        case RefTypeN::e_payee:
            ref_name = PayeeModel::instance().get_id_name(m_ref_id);
            break;
        case RefTypeN::e_trx:
        case RefTypeN::e_sched:
        default:
            ref_name = "";
        }       
        if (ref_name.IsEmpty())
            windowTitle = wxString::Format(_t("Attachment Manager | %1$s | %2$lld"),
                wxGetTranslation(m_ref_type.name_n()), m_ref_id
            );
        else
            windowTitle = wxString::Format(_t("Attachment Manager | %1$s | %2$s"),
                wxGetTranslation(m_ref_type.name_n()), ref_name
            );
    }
    else {
        windowTitle = wxString::Format(_t("Attachment Manager | New %s"),
            wxGetTranslation(m_ref_type.name_n())
        );
    }

    if (!wxDialog::Create(
        parent_win, wxID_ANY, windowTitle,
        wxDefaultPosition, wxDefaultSize, style, name
    ))
        return;

    createControls();
    fillControls();

    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    // CHECK: called twice
    fillControls();
    SetIcon(mmPath::getProgramIcon());
    Centre();
}

void AttachmentDialog::createControls()
{
    wxBoxSizer* mainBoxSizer = new wxBoxSizer(wxVERTICAL);

    w_list = new wxDataViewListCtrl(this, wxID_ANY,
        wxDefaultPosition, wxSize(460, 500)/*, wxDV_HORIZ_RULES*/
    );

    if (m_debug)
        w_list->AppendTextColumn(m_col_name_m[ATTACHMENT_ID], wxDATAVIEW_CELL_INERT, 30);
    w_list->AppendTextColumn(m_col_name_m[ATTACHMENT_DESCRIPTION], wxDATAVIEW_CELL_INERT, 150);
    w_list->AppendTextColumn(m_col_name_m[ATTACHMENT_FILENAME], wxDATAVIEW_CELL_INERT, 300);

    w_list->DragAcceptFiles(true);
    w_list->Connect(wxEVT_DROP_FILES,
        wxDropFilesEventHandler(AttachmentDialog::onDropFiles),
        nullptr, this
    );
    mainBoxSizer->Add(w_list, wxSizerFlags(g_flagsExpand).Border(wxALL, 10));

    wxPanel* buttons_panel = new wxPanel(this, wxID_ANY);
    mainBoxSizer->Add(buttons_panel, wxSizerFlags(g_flagsV).Center());
    wxStdDialogButtonSizer* buttons_sizer = new wxStdDialogButtonSizer;
    buttons_panel->SetSizer(buttons_sizer);

    buttons_sizer->Add(
        new wxButton(buttons_panel, wxID_OK, _t("&OK ")),
        g_flagsH
    );
    buttons_sizer->Add(
        new wxButton(buttons_panel, wxID_CANCEL, wxGetTranslation(g_CancelLabel)),
        g_flagsH
    );

    wxBitmapButton* magic_btn = new wxBitmapButton(buttons_panel, wxID_APPLY,
        mmImage::bitmapBundle(mmImage::png::MORE_OPTIONS, mmImage::bitmapButtonSize)
    );
    mmToolTip(magic_btn, _t("Other tools"));
    buttons_sizer->Add(magic_btn, g_flagsH);

    Center();
    this->SetSizer(mainBoxSizer);
}

void AttachmentDialog::fillControls()
{    
    w_list->DeleteAllItems();

    for (const auto& att_d : AttachmentModel::instance().find_ref_data_a(
        m_ref_type, m_ref_id
    )) {
        wxVector<wxVariant> data;
        if (m_debug)
            data.push_back(wxVariant(wxString::Format("%lld", att_d.m_id)));
        data.push_back(wxVariant(att_d.m_description));
        data.push_back(wxVariant(
            att_d.m_ref_type_n.key_n() + mmAttachment::s_path_sep + att_d.m_filename
        ));
        w_list->AppendItem(data, static_cast<wxUIntPtr>(att_d.m_id.GetValue()));

        // Set m_attachment_id to first id in results
        if (m_attachment_id == -1) {
            m_attachment_id = att_d.m_id;
        }
    }
}

// -- methods

void AttachmentDialog::addAttachment(wxString file)
{
    if (file.empty()) {
        file = wxFileSelector(
            _t("Import attachment:"),
            wxEmptyString, wxEmptyString, wxEmptyString,
            "All Files |*.*",
            wxFD_FILE_MUST_EXIST
        );
        if (file.empty())
            return;
    }

    const wxString file_name = wxFileName(file).GetName();
    const wxString file_ext = wxFileName(file).GetExt().MakeLower();

    mmDialogComboBoxAutocomplete dlg(this,
        _t("Enter a description for the new attachment:") +
            wxString::Format("\n(%s)", file),
        _t("Attachment Manager: Add Attachment"),
        file_name,
        AttachmentModel::instance().find_all_desc_a()
    );

    if (dlg.ShowModal() != wxID_OK)
        return;

    const wxString desc = dlg.getText();

    const wxString folder = mmAttachment::getFolder();
    int last_num = AttachmentModel::instance().find_ref_last_num(
        m_ref_type, m_ref_id
    );

    wxString new_file_name = wxString::Format("%s_%lld_Attach%i",
        m_ref_type.key_n(), m_ref_id, last_num + 1
    );
    if (!file_ext.empty())
        new_file_name += "." + file_ext;

    if (mmAttachment::copyFile(
        file,
        folder + m_ref_type.key_n() + mmAttachment::s_path_sep + new_file_name
    )) {
        AttachmentData new_att_d = AttachmentData();
        new_att_d.m_ref_type_n  = m_ref_type;
        new_att_d.m_ref_id      = m_ref_id;
        new_att_d.m_description = desc;
        new_att_d.m_filename    = new_file_name;
        AttachmentModel::instance().add_data_n(new_att_d);
        m_attachment_id = new_att_d.m_id;

        if (m_ref_type == TrxModel::s_ref_type)
            TrxModel::instance().save_timestamp(m_ref_id);
    }

    fillControls();
}

void AttachmentDialog::openAttachment()
{
    const AttachmentData* att_n = AttachmentModel::instance().get_idN_data_n(
        m_attachment_id
    );
    if (!att_n)
        return;

    wxString file = AttachmentModel::instance().get_data_file(*att_n);
    mmAttachment::openFile(file);
}

void AttachmentDialog::editAttachment()
{
    AttachmentData *att_n = AttachmentModel::instance().unsafe_get_idN_data_n(
        m_attachment_id
    );
    if (!att_n)
        return;

    mmDialogComboBoxAutocomplete dlg(this,
        _t("Enter a new description for the attachment:"),
        _t("Attachment Manager: Edit Attachment"),
        att_n->m_description,
        AttachmentModel::instance().find_all_desc_a()
    );
    if (dlg.ShowModal() != wxID_OK)
        return;

    const wxString desc = dlg.getText();
    if (desc == att_n->m_description)
        return;

    att_n->m_description = desc;
    AttachmentModel::instance().unsafe_update_data_n(att_n);

    if (att_n->m_ref_type_n == TrxModel::s_ref_type)
        TrxModel::instance().save_timestamp(att_n->m_ref_id);

    fillControls();
}

void AttachmentDialog::deleteAttachment()
{
    const AttachmentData* att_n = AttachmentModel::instance().get_idN_data_n(
        m_attachment_id
    );
    if (!att_n)
        return;

    if (wxMessageBox(
        _t("Do you want to delete this attachment?"),
        _t("Confirm Attachment Deletion"),
        wxYES_NO | wxNO_DEFAULT | wxICON_ERROR
    ) != wxYES)
        return;

    const wxString file = AttachmentModel::instance().get_data_file(*att_n);
    if (mmAttachment::deleteFile(file)) {
        if (att_n->m_ref_type_n == TrxModel::s_ref_type)
            TrxModel::instance().save_timestamp(att_n->m_ref_id);
        AttachmentModel::instance().purge_id(m_attachment_id);
    }
    m_attachment_id = -1;
    fillControls();
}

// -- event handlers

void AttachmentDialog::onOk(wxCommandEvent& WXUNUSED(event))
{
    EndModal(wxID_OK);
}

void AttachmentDialog::onCancel(wxCommandEvent& WXUNUSED(event))
{
    EndModal(wxID_CANCEL);
}

void AttachmentDialog::onDropFiles(wxDropFilesEvent& event)
{
    if (event.GetNumberOfFiles() <= 0)
        return;

    wxString* file_a = event.GetFiles();
    for (int i = 0; i < event.GetNumberOfFiles(); i++) {
        wxString file = file_a[i];
        if (wxFileExists(file))
            addAttachment(file);
    }
}

void AttachmentDialog::onListItemSelected(wxDataViewEvent& event)
{
    wxDataViewItem item = event.GetItem();
    int selected_index = w_list->ItemToRow(item);

    if (selected_index >= 0)
        m_attachment_id = static_cast<int64>(w_list->GetItemData(item));
}

void AttachmentDialog::onListItemActivated(wxDataViewEvent& WXUNUSED(event))
{
    const AttachmentData* att_n = AttachmentModel::instance().get_idN_data_n(
        m_attachment_id
    );
    if (!att_n)
        return;

    const wxString file = AttachmentModel::instance().get_data_file(*att_n);
    mmAttachment::openFile(file);
}

void AttachmentDialog::onMenuSelected(wxCommandEvent& event)
{
    switch(event.GetId())
    {
    case MENU_NEW_ATTACHMENT:    addAttachment();    break;
    case MENU_OPEN_ATTACHMENT:   openAttachment();   break;
    case MENU_EDIT_ATTACHMENT:   editAttachment();   break;
    case MENU_DELETE_ATTACHMENT: deleteAttachment(); break;
    default:                                         break;
    }
}

void AttachmentDialog::onMagicButton(wxCommandEvent& WXUNUSED(event))
{
    wxDataViewEvent evt;
    onItemRightClick(evt);
}

void AttachmentDialog::onItemRightClick(wxDataViewEvent& event)
{
    wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, wxID_ANY);
    evt.SetEventObject(this);

    const AttachmentData* att_n = AttachmentModel::instance().get_idN_data_n(
        m_attachment_id
    );

    wxMenu* mainMenu = new wxMenu;
    if (att_n)
        mainMenu->SetTitle(att_n->m_description);
    mainMenu->Append(new wxMenuItem(mainMenu, MENU_NEW_ATTACHMENT, _t("&Add ")));
    mainMenu->AppendSeparator();
    mainMenu->Append(new wxMenuItem(mainMenu, MENU_OPEN_ATTACHMENT, _t("&Open ")));
    mainMenu->Append(new wxMenuItem(mainMenu, MENU_EDIT_ATTACHMENT, _t("&Edit ")));
    if (!att_n)
        mainMenu->Enable(MENU_EDIT_ATTACHMENT, false);
    mainMenu->Append(new wxMenuItem(mainMenu, MENU_DELETE_ATTACHMENT, _t("&Remove ")));
    
    // Disable buttons
    const wxString folder = mmAttachment::getFolder();
    if (folder == wxEmptyString || !wxDirExists(folder))
        mainMenu->Enable(MENU_NEW_ATTACHMENT, false);

    if (!att_n) {
        mainMenu->Enable(MENU_OPEN_ATTACHMENT, false);
        mainMenu->Enable(MENU_EDIT_ATTACHMENT, false);
        mainMenu->Enable(MENU_DELETE_ATTACHMENT, false);
    }

    PopupMenu(mainMenu);
    delete mainMenu;
    event.Skip();
}
