//------------------------------------------------------------------
// minifluxViewEntry.h
//
// Author:           JuanJakobo
// Date:             04.08.2020
// Description:      An SearchViewEntry that handles an item of a listview
//-------------------------------------------------------------------

#ifndef MINIFLUXVIEWENTRY
#define MINIFLUXVIEWENTRY

#include "listViewEntry.h"
#include "searchModel.h"

class SearchViewEntry : public ListViewEntry
{
public:
    /**
        * Creates an SearchViewEntry
        *
        * @param Page site of the listView the Entry is shown
        * @param Rect area of the screen the item is positioned
        * @param entry entry that shall be drawn
        */
    SearchViewEntry(int page, const irect &position, const SearchEntry &entry);

    /**
        * draws the SearchViewEntry to the screen
        *
        * @param entryFont font for the entry itself
        * @param entryFontBold bold font for the header
        * @param fontHeight height of the font
        */
    void draw(const ifont *entryFont, const ifont *entryFontBold, int fontHeight) const override;

    SearchEntry &getMfEntry() { return _entry; };

private:
    SearchEntry _entry;
};
#endif
