/*******************************************************
Copyright (C) 2014, 2015 Nikolay Akimov
Copyright (C) 2021 Mark Whalley (mark@ipx.co.uk)
Copyright (C) 2025, 2026 Klaus Wich

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
*******************************************************/

#pragma once

#include <vector>
#include <map>
#include <rapidjson/document.h>

#include "base/_defs.h"
#include <wx/bmpbndl.h>
#include <wx/window.h>
//#include <wx/imaglist.h>
#include <wx/arrstr.h>

struct mmImage
{
public:
    // Meta data items
    enum meta {
        THEME_NAME,
        THEME_AUTHOR,
        THEME_DESCRIPTION,
        THEME_URL,
        COLOR_NAVPANEL,
        COLOR_NAVPANEL_FONT,
        COLOR_LISTPANEL,
        COLOR_LIST,
        COLOR_LISTALT0,
        COLOR_LISTALT0A,
        COLOR_LISTTOTAL,
        COLOR_LISTBORDER,
        COLOR_LISTFUTURE,
        COLOR_HTMLPANEL_BACK,
        COLOR_HTMLPANEL_FORE,
        COLOR_REPORT_ALTROW,
        COLOR_REPORT_CREDIT,
        COLOR_REPORT_DEBIT,
        COLOR_REPORT_DELTA,
        COLOR_REPORT_PERF,
        COLOR_REPORT_FORECOLOR,
        COLOR_REPORT_PALETTE,
        COLOR_TOOLBAR,
        COLOR_BUTTON,
        COLOR_HIDDEN,
        COLOR_GRM_LINENUMBER,
        COLOR_GRM_LINENUMBER_BG,
        COLOR_GRM_MARKER,
        COLOR_GRM_KEYWORD,
        COLOR_GRM_KEYATTRIBUTE,
        COLOR_GRM_STRING,
        COLOR_GRM_COMMENT,
        COLOR_GRM_IDENTIFIER,
        COLOR_GRM_UNKNOWN,
        COLOR_GRM_OPERATOR,
        COLOR_GRM_NUMBER,
        COLOR_GRM_SPECIAL,
        COLOR_TEXTCONTROL,
        COLOR_TEXTCONTROL_FONT,
        // The end
        MAX_METADATA
    };

    enum img {
        HOUSE_PNG,
        ALLTRANSACTIONS_PNG,
        SCHEDULE_PNG,
        CALENDAR_PNG,
        PIECHART_PNG,
        HELP_PNG,
        FAVOURITE_PNG,
        FILTER_PNG,
        ASSET_NORMAL_PNG,
        CUSTOMSQL_PNG,
        CUSTOMSQL_GRP_PNG,
        SAVINGS_ACC_NORMAL_PNG,
        CARD_ACC_NORMAL_PNG,
        TERMACCOUNT_NORMAL_PNG,
        STOCK_ACC_NORMAL_PNG,
        CASH_ACC_NORMAL_PNG,
        TRASH_PNG,
        LOAN_ACC_NORMAL_PNG,
        ACCOUNT_CLOSED_PNG,
        LAST_NAVTREE_PNG,
    };

    /* Images set for accounts to be user assigned */
    enum acc_img {
        ACC_ICON_MONEY = LAST_NAVTREE_PNG, //First user img
        ACC_ICON_EURO,
        ACC_ICON_FLAG,
        ACC_ICON_COINS,
        ACC_ICON_ABOUT,
        ACC_ICON_CLOCK,
        ACC_ICON_CAT,
        ACC_ICON_DOG,
        ACC_ICON_TREES,
        ACC_ICON_HOURGLASS,
        ACC_ICON_WORK,
        ACC_ICON_PAYPAL,
        ACC_ICON_WALLET,
        ACC_ICON_RUBIK,
        MAX_ACC_ICON,
    };

    enum png {
        // Toolbar
        NEW_DB,
        OPEN,
        NEW_ACC,
        HOME,
        CATEGORY,
        PAYEE,
        CURR,
        TAG,
        FILTER,
        GRM,
        OPTIONS,
        NEW_TRX,
        NEW_NEWS,
        NEWS,
        CURRATES,
        FULLSCREEN,
        PRINT,
        ABOUT,
        HELP,

        // Navigation
        NAV_HOME,
        ALLTRANSACTIONS,
        NAV_FILTER,
        NAV_GRM,
        NAV_HELP,
        FAVOURITE,
        SAVINGS_NORMAL,
        CC_NORMAL,
        CASH_NORMAL,
        LOAN_ACC_NORMAL,
        TERM_NORMAL,
        STOCKS_NORMAL,
        ASSET_NORMAL,
        ACCOUNT_CLOSED,
        RECURRING,
        TRASH,
        BUDGET,
        PIE_CHART,

        // Status
        UNRECONCILED,
        RECONCILED,
        DUPLICATE_STAT,
        FOLLOW_UP,
        VOID_STAT,
        PROFIT,
        LOSS,
        LED_OFF,
        LED_GREEN,
        LED_YELLOW,
        LED_RED,
        RUN,
        RUN_AUTO,

        // Assets
        PROPERTY,
        CAR,
        HOUSEHOLD_OBJ,
        ART,
        JEWELLERY,
        CASH,
        OTHER,

        // Controls
        CALCULATOR,
        CLEAR,
        CLIP,
        DOWNARROW,
        UPARROW,
        LEFTARROW,
        RIGHTARROW,
        IMPORT,
        MORE_OPTIONS,
        RELOCATION,
        SAVE,
        TRANSFILTER,
        TRANSFILTER_ACTIVE,
        TRXNUM,
        UPDATE,
        WEB,

        // Settings
        GENERAL,
        VIEW,
        ATTACHMENTS,
        NETWORK,
        OTHERS,

        // Extra Account Icons
        ACC_MONEY,
        ACC_EURO,
        ACC_FLAG,
        ACC_COINS,
        ACC_ABOUT,
        ACC_CLOCK,
        ACC_CAT,
        ACC_DOG,
        ACC_TREES,
        ACC_HOURGLASS,
        ACC_WORK,
        ACC_PAYPAL,
        ACC_WALLET,
        ACC_RUBIK,

        // Other
        EMPTY,

        // The end
        MAX_PNG
    };

// -- constants

public:
    static const int bitmapButtonSize;

private:
    static constexpr int numSizes = 4;
    static const std::vector<std::pair<int, int>> sizes;
    static const std::map<std::string, std::pair<int, bool>> iconName2enum;

// -- static state

private:
    static bool darkFound, darkMode;
    static rapidjson::Document metaData_doc;
    static wxSharedPtr<wxBitmapBundle> programIconBundles[mmImage::numSizes][mmImage::MAX_PNG];
    static wxSharedPtr<wxArrayString> filesInVFS;

// -- static methods

private:
    static auto metaDataTrans() -> const std::map<int, std::tuple<wxString, wxString, bool>>;
    static int  getIconSizeIdx(const int iconSize);
    static bool processThemes(wxString themeDir, wxString myTheme, bool metaPhase);
    static bool checkThemeContents(wxArrayString* filesinTheme);
    static void reverttoDefaultTheme();

public:
    static void loadTheme();
    static void closeTheme();
    static auto themeMetaString(int ref) -> const wxString;
    static long themeMetaLong(int ref);
    static auto themeMetaColour(int ref) -> const wxColour;
    static void themeMetaColour(wxWindow* object, int ref, bool foreground = false);
    static void themeMetaColour(wxWindow* object, wxColour c, bool foreground = false);
    static auto themeMetaColour_a(int ref) -> const std::vector<wxColour>;
    static auto bitmapBundle(const int ref, const int defSize = -1) -> const wxBitmapBundle;
    static auto navtree_images(const int size) -> const std::map<int, wxBitmapBundle>;
    static auto acc_images(int size) -> const std::map<int, wxBitmapBundle>;
};


class NavTreeIconImages
{
public:
    NavTreeIconImages();
    static NavTreeIconImages& instance();
    wxVector<wxBitmapBundle> getList(const int size = 0);
    wxImageList* getImageList(const int size = 0);
    int getListSize();

    std::map<int, wxString>& getIndexMap();
    void initIndexMap();
    int getImgIndex(wxString imgName);
    bool isListChanged();
    void setChanged();

private:
    std::map<int, wxString> m_indexMap;
    std::map<wxString, int> m_indexReverseMap;
    bool m_changed;
    int m_size;
};

inline std::map<int, wxString>& NavTreeIconImages::getIndexMap() { return m_indexMap; };
inline int NavTreeIconImages::getListSize(){ return m_size; };
