/*******************************************************
 Copyright (C) 2006 Madhan Kanagavel
 Copyright (C) 2013-2022 Nikolay Akimov
 Copyright (C) 2021-2024 Mark Whalley (mark@ipx.co.uk)
 Copyright (C) 2025 George Ef (george.a.ef@gmail.com)

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

#include "mmUserColor.h"

wxColour mmUserColor::getId(const int c)
{
    switch (c)
    {
    case 1: return  mmUserColor::s_color1;
    case 2: return  mmUserColor::s_color2;
    case 3: return  mmUserColor::s_color3;
    case 4: return  mmUserColor::s_color4;
    case 5: return  mmUserColor::s_color5;
    case 6: return  mmUserColor::s_color6;
    case 7: return  mmUserColor::s_color7;
    }
    return wxNullColour;
}

const wxColor* mmUserColor::bestFontColor(const wxColour& background)
{
    // http://stackoverflow.com/a/3943023/112731
    int r = static_cast<int>(background.Red());
    int g = static_cast<int>(background.Green());
    int b = static_cast<int>(background.Blue());
    int k = (r * 299 + g * 587 + b * 114);
    /*wxLogDebug(
        "best FontColour: [%s] -> r=%d, g=%d, b=%d | k: %d",
        background.GetAsString(wxC2S_HTML_SYNTAX), r, g, b, k
    );*/
    return (k > 149000)
        ? wxBLACK
        : wxWHITE;
}
