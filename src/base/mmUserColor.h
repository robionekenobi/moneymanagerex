/*******************************************************
 Copyright (C) 2006 Madhan Kanagavel
 Copyright (C) 2013-2022 Nikolay Akimov
 Copyright (C) 2021,2024 Mark Whalley (mark@ipx.co.uk)
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

#pragma once

#include "base/_defs.h"

class mmUserColor
{
public:
    static wxColour s_color1;
    static wxColour s_color2;
    static wxColour s_color3;
    static wxColour s_color4;
    static wxColour s_color5;
    static wxColour s_color6;
    static wxColour s_color7;

public:
    static auto getId(const int c) -> wxColour;
    static auto bestFontColor(const wxColour& background) -> const wxColor*;
};
