//------------------------------------------------------------------
// listViewEntry.cpp
//
// Author:           JuanJakobo
// Date:             04.08.2020
//
//-------------------------------------------------------------------

#include "searchViewEntry.h"
#include "searchModel.h"

#include <string>

SearchViewEntry::SearchViewEntry(int page, const irect &position, const SearchEntry &entry) : ListViewEntry(page, position) , _entry(entry)
{
}

void SearchViewEntry::draw(const ifont *entryFont, const ifont *entryFontBold, int fontHeight) const
{
    SetFont(entryFontBold, BLACK);
    int heightOfTitle = TextRectHeight(_position.w, _entry.entry.getTitle().c_str(), 0);
    DrawTextRect(_position.x, _position.y, _position.w, heightOfTitle, _entry.entry.getTitle().c_str(), ALIGN_LEFT);

    int line = (_position.y + _position.h) - 1;
    DrawLine(0, line, ScreenWidth(), line, BLACK);
}
