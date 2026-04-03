/*******************************************************
 Copyright (C) 2008 Troels K
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
// Thanks to geralds for SetLexerXml() - http://wxforum.shadonet.com/viewtopic.php?t=7155

#include "mmMiniEditor.h"
#include "util/mmImage.h"
#include "mmex.h"

wxIMPLEMENT_DYNAMIC_CLASS(mmMiniEditor, wxStyledTextCtrl);

wxBEGIN_EVENT_TABLE(mmMiniEditor, wxStyledTextCtrl)
    EVT_STC_MARGINCLICK(wxID_ANY, mmMiniEditor::OnMarginClick)
wxEND_EVENT_TABLE()

mmMiniEditor::mmMiniEditor(wxWindow* parent, wxWindowID id)
    : wxStyledTextCtrl(parent, id)
{
    int font_size = this->GetFont().GetPointSize() - 1;
    m_font = wxFont(font_size, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    this->SetFont(m_font);
}

void mmMiniEditor::SetCommonLexarProperties()
{
    StyleClearAll();
    // set spaces and indention
    SetTabWidth(2);
    SetUseTabs(false);
    SetTabIndents(true);
    SetBackSpaceUnIndents(true);
    SetViewWhiteSpace(wxSTC_WS_VISIBLEALWAYS);

    for (int i = 0; i < wxSTC_STYLE_MAX; ++i) {
        StyleSetFont(i, m_font);
    }

    SetWrapMode(wxSTC_WRAP_WORD);

    StyleSetForeground(
        wxSTC_STYLE_LINENUMBER,
        mmImage::themeMetaColour(mmImage::COLOR_GRM_LINENUMBER)
    );
    StyleSetBackground(
        wxSTC_STYLE_LINENUMBER,
        mmImage::themeMetaColour(mmImage::COLOR_GRM_LINENUMBER_BG)
    );

    SetMarginWidth(0, 32);
    SetMarginType(0, wxSTC_MARGIN_NUMBER);

    SetMarginType(2, wxSTC_MARGIN_SYMBOL);
    SetMarginMask(2, wxSTC_MASK_FOLDERS);
    SetMarginWidth(2, 16);
    SetMarginSensitive(2, true);

    // set visibility
    SetVisiblePolicy(wxSTC_VISIBLE_STRICT | wxSTC_VISIBLE_SLOP, 1);
    SetXCaretPolicy(wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1);
    SetYCaretPolicy(wxSTC_CARET_EVEN | wxSTC_VISIBLE_STRICT | wxSTC_CARET_SLOP, 1);

    SetProperty("fold", "1");

    // markers
    MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN,     wxSTC_MARK_BOXMINUS, mmImage::themeMetaColour(mmImage::COLOR_GRM_MARKER));
    MarkerDefine(wxSTC_MARKNUM_FOLDER,         wxSTC_MARK_BOXPLUS, mmImage::themeMetaColour(mmImage::COLOR_GRM_MARKER));
    MarkerDefine(wxSTC_MARKNUM_FOLDERSUB,      wxSTC_MARK_VLINE, mmImage::themeMetaColour(mmImage::COLOR_GRM_MARKER));
    MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL,     wxSTC_MARK_LCORNER, mmImage::themeMetaColour(mmImage::COLOR_GRM_MARKER));
    MarkerDefine(wxSTC_MARKNUM_FOLDEREND,      wxSTC_MARK_BOXPLUSCONNECTED, mmImage::themeMetaColour(mmImage::COLOR_GRM_MARKER));
    MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID,  wxSTC_MARK_BOXMINUSCONNECTED, mmImage::themeMetaColour(mmImage::COLOR_GRM_MARKER));
    MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL,  wxSTC_MARK_TCORNER, mmImage::themeMetaColour(mmImage::COLOR_GRM_MARKER));

    SetFoldFlags(wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED);

    StyleSetForeground(
        wxSTC_STYLE_INDENTGUIDE,
        mmImage::themeMetaColour(mmImage::COLOR_GRM_LINENUMBER)
    );
}


void mmMiniEditor::SetLexerSql()
{
    const wxString sqlwords = "asc by delete desc from group having insert into order select set update values where left inner join as and or on union";

    SetLexer(wxSTC_LEX_SQL);

    SetCommonLexarProperties();

    SetKeyWords(0, sqlwords);

    StyleSetForeground(wxSTC_SQL_COMMENT, mmImage::themeMetaColour(mmImage::COLOR_GRM_COMMENT));
    StyleSetForeground(wxSTC_SQL_COMMENTLINE, mmImage::themeMetaColour(mmImage::COLOR_GRM_COMMENT));
    StyleSetForeground(wxSTC_SQL_COMMENTDOC, mmImage::themeMetaColour(mmImage::COLOR_GRM_COMMENT));
    StyleSetForeground(wxSTC_SQL_NUMBER, mmImage::themeMetaColour(mmImage::COLOR_GRM_NUMBER));
    StyleSetForeground(wxSTC_SQL_WORD, mmImage::themeMetaColour(mmImage::COLOR_GRM_KEYWORD));
    StyleSetForeground(wxSTC_SQL_STRING, mmImage::themeMetaColour(mmImage::COLOR_GRM_STRING));
    StyleSetForeground(wxSTC_SQL_OPERATOR, mmImage::themeMetaColour(mmImage::COLOR_GRM_OPERATOR));
    StyleSetForeground(wxSTC_SQL_IDENTIFIER, mmImage::themeMetaColour(mmImage::COLOR_GRM_IDENTIFIER));

    //StyleSetBold(wxSTC_SQL_WORD, true);
}

void mmMiniEditor::SetLexerHtml()
{

    SetLexer(wxSTC_LEX_HTML);

    SetCommonLexarProperties();

    SetProperty("fold.html", "1");
    SetProperty("fold.html.preprocessor", "1");
    SetProperty("fold.compact", "1");
    SetProperty("fold.comment", "1");

    // JavaScript
    SetKeyWords(1, "var let const function if else for while return document window "
                   "alert console log true false null undefined new try catch"
    );

    SetKeyWords(5, "TMPL_VAR TMPL_LOOP TMPL_IF __FIRST__ __LAST_ __COUNT__ __COUNTER__ __TOTAL__ __ODD__ __EVEN__ __INNER__ ");

    // Syntax-Hervorhebung aktivieren
    StyleSetForeground(wxSTC_H_TAG, mmImage::themeMetaColour(mmImage::COLOR_GRM_KEYWORD));
    StyleSetForeground(wxSTC_H_TAGUNKNOWN, mmImage::themeMetaColour(mmImage::COLOR_GRM_UNKNOWN));
    StyleSetForeground(wxSTC_H_ATTRIBUTE, mmImage::themeMetaColour(mmImage::COLOR_GRM_KEYATTRIBUTE));
    StyleSetForeground(wxSTC_H_ATTRIBUTEUNKNOWN, mmImage::themeMetaColour(mmImage::COLOR_GRM_UNKNOWN));
    StyleSetForeground(wxSTC_H_NUMBER, mmImage::themeMetaColour(mmImage::COLOR_GRM_NUMBER));
    StyleSetForeground(wxSTC_H_DOUBLESTRING, mmImage::themeMetaColour(mmImage::COLOR_GRM_STRING));
    StyleSetForeground(wxSTC_H_SINGLESTRING, mmImage::themeMetaColour(mmImage::COLOR_GRM_STRING));
    StyleSetForeground(wxSTC_H_COMMENT, mmImage::themeMetaColour(mmImage::COLOR_GRM_COMMENT));
    StyleSetForeground(wxSTC_H_ENTITY, mmImage::themeMetaColour(mmImage::COLOR_GRM_IDENTIFIER));

    // JavaScript Styles
    StyleSetForeground(wxSTC_HJ_DEFAULT, mmImage::themeMetaColour(mmImage::COLOR_GRM_UNKNOWN));
    StyleSetForeground(wxSTC_HJ_COMMENT, mmImage::themeMetaColour(mmImage::COLOR_GRM_COMMENT));
    StyleSetForeground(wxSTC_HJ_COMMENTLINE, mmImage::themeMetaColour(mmImage::COLOR_GRM_COMMENT));
    StyleSetForeground(wxSTC_HJ_COMMENTDOC, mmImage::themeMetaColour(mmImage::COLOR_GRM_COMMENT));
    StyleSetForeground(wxSTC_HJ_NUMBER, mmImage::themeMetaColour(mmImage::COLOR_GRM_NUMBER));
    StyleSetForeground(wxSTC_HJ_WORD, mmImage::themeMetaColour(mmImage::COLOR_GRM_IDENTIFIER));
    StyleSetForeground(wxSTC_HJ_KEYWORD, mmImage::themeMetaColour(mmImage::COLOR_GRM_KEYWORD));
    StyleSetForeground(wxSTC_HJ_DOUBLESTRING, mmImage::themeMetaColour(mmImage::COLOR_GRM_STRING));
    StyleSetForeground(wxSTC_HJ_SINGLESTRING, mmImage::themeMetaColour(mmImage::COLOR_GRM_STRING));
    StyleSetForeground(wxSTC_HJ_SYMBOLS, mmImage::themeMetaColour(mmImage::COLOR_GRM_OPERATOR));
    StyleSetForeground(wxSTC_HJA_REGEX, mmImage::themeMetaColour(mmImage::COLOR_GRM_SPECIAL));
}

void mmMiniEditor::SetLexerLua()
{
    wxString luawords =
        "function end if then do else for in return break while local repeat elseif and or not false true nil require";
    wxString luawords2 =
        "wxamcl.echo wxamcl.echowin wxamcl.gag wxamcl.color wxamcl.colorwin wxamcl.colorline wxamcl.colorword wxamcl.colorall wxamcl.scroll \
        wxamcl.getlinenumber wxamcl.execute wxamcl.wait wxamcl.waitfor wxamcl.loadprofile \
        wxamcl.createwindow wxamcl.hidewindow wxamcl.showwindow wxamcl.clearwindow \
        wxamcl.action.new wxamcl.action.get wxamcl.action.delete wxamcl.action.enable wxamcl.action.getpattern wxamcl.action.setpattern \
        wxamcl.action.getact wxamcl.action.setact wxamcl.action.getpriority wxamcl.action.setpriority wxamcl.action.getcolmatch wxamcl.action.setcolmatch \
        wxamcl.action.enableall wxamcl.action.disableall wxamcl.action.getall wxamcl.action.delgroup wxamcl.action.enablegroup \
        wxamcl.alias.new wxamcl.alias.get wxamcl.alias.delete wxamcl.alias.enable wxamcl.alias.getall wxamcl.alias.getaction\
        wxamcl.alias.setaction wxamcl.alias.delgroup wxamcl.alias.enablegroup \
        wxamcl.hk.new wxamcl.hk.delgroup wxamcl.hk.delete wxamcl.hk.enable \
        wxamcl.var.new wxamcl.var.set wxamcl.var.get wxamcl.var.delete wxamcl.var.enable \
        wxamcl.gauge.update wxamcl.gauge.new wxamcl.gauge.setsize wxamcl.gauge.setcolor wxamcl.gauge.createwindow wxamcl.gauge.setlabel \
        wxamcl.timer.new wxamcl.timer.delete wxamcl.timer.start wxamcl.timer.stop wxamcl.timer.enable \
        wxamcl.mxp.enable wxamcl.mxp.echo wxamcl.mxp.echowin \
        wxamcl.list.new wxamcl.list.additem wxamcl.list.clear wxamcl.list.delete wxamcl.list.contains wxamcl.list.delitem wxamcl.list.delitemat \
        wxamcl.gmcp.send \
        wxamcl.db.open wxamcl.db.execute wxamcl.db.columns wxamcl.db.results wxamcl.db.close wxamcl.db.insert \
        wxamcl.draw.text wxamcl.draw.line wxamcl.draw.circle wxamcl.draw.square wxamcl.draw.winsize";
    wxString amcwords =
        "clearwin capstart capend setvar gag help loadprofile func script bscript gagwin connect pwd refreshwin raw capturewin capturenb log htmllog resume test";

    SetLexer(wxSTC_LEX_LUA);

    SetCommonLexarProperties();

    SetProperty("fold.compact", "1");
    SetProperty("fold.comment", "1");

    SetKeyWords(0, luawords);
    SetKeyWords(1, luawords2);
    SetKeyWords(2, amcwords);

    StyleSetForeground(wxSTC_LUA_COMMENTLINE, mmImage::themeMetaColour(mmImage::COLOR_GRM_COMMENT));
    StyleSetForeground(wxSTC_LUA_COMMENT, mmImage::themeMetaColour(mmImage::COLOR_GRM_COMMENT));
    StyleSetForeground(wxSTC_LUA_COMMENTDOC, mmImage::themeMetaColour(mmImage::COLOR_GRM_COMMENT));
    StyleSetForeground(wxSTC_LUA_PREPROCESSOR, mmImage::themeMetaColour(mmImage::COLOR_GRM_KEYWORD));
    StyleSetForeground(wxSTC_LUA_WORD, mmImage::themeMetaColour(mmImage::COLOR_GRM_KEYWORD));
    StyleSetForeground(wxSTC_LUA_WORD2, mmImage::themeMetaColour(mmImage::COLOR_GRM_KEYWORD));
    StyleSetForeground(wxSTC_LUA_STRING, mmImage::themeMetaColour(mmImage::COLOR_GRM_STRING));
    StyleSetForeground(wxSTC_LUA_CHARACTER, mmImage::themeMetaColour(mmImage::COLOR_GRM_STRING));
    StyleSetForeground(wxSTC_LUA_LITERALSTRING, mmImage::themeMetaColour(mmImage::COLOR_GRM_STRING));
    StyleSetForeground(wxSTC_LUA_NUMBER, mmImage::themeMetaColour(mmImage::COLOR_GRM_NUMBER));
    StyleSetForeground(wxSTC_LUA_OPERATOR, mmImage::themeMetaColour(mmImage::COLOR_GRM_OPERATOR));
    StyleSetForeground(wxSTC_LUA_IDENTIFIER, mmImage::themeMetaColour(mmImage::COLOR_GRM_IDENTIFIER));
    StyleSetForeground(wxSTC_LUA_OPERATOR, mmImage::themeMetaColour(mmImage::COLOR_GRM_OPERATOR));

    SetCurrentPos(0);
}

void mmMiniEditor::OnMarginClick(wxStyledTextEvent &event)
{
    wxLogDebug("MMEditor-Event: >%d<", event.GetMargin());
    if (event.GetMargin() > 0) {
        int lineClick = LineFromPosition(event.GetPosition());
        int levelClick = GetFoldLevel(lineClick);
        if ((levelClick & wxSTC_FOLDLEVELHEADERFLAG) > 0) {
            ToggleFold(lineClick);
        }
    }
}
