#ifndef EVENT_HANDLER
#define EVENT_HANDLER
//------------------------------------------------------------------
// eventHandler.h
//
// Author:           JuanJakob
// Date:             12.12.2022
// Description:      Handles all events and directs them
//-------------------------------------------------------------------


#include "inkview.h"

#include "archive.h"
#include "search.h"
#include "searchHandler.h"

#include <string>
#include <vector>
#include <memory>


constexpr int KEYBOARD_STRING_LENGHT = 90;

class EventHandler
{
    public:
        EventHandler();

        int eventDistributor(const int type, const int par1, const int par2);

    private:
        static std::unique_ptr<EventHandler> _eventHandlerStatic;
        std::vector<zim::Archive> _archives;

        std::string _searchText = "";

        irect _searchTextView;
        irect _filterButton;
        irect _contentRect;

        std::unique_ptr<SearchHandler> _search;


        void drawSearchScreen() const;
        void drawSearchTextView() const;
        void drawSearchButton() const;
        void createConfigAndStorageFolders() const;
        void addArchivesFromStoragePath();

        int keyHandler(const int type, const int par1, const int par2);
        int pointerHandler(const int type, const int par1, const int par2);

        void openSearchTextKeyboardAndSearch();
        static void setSearchTextToKeyboardInputStatic(char *keyboardText);
        void setSearchTextToKeyboardInput(char *keyboardText);

        void drawSearchResults() const;

        std::string createHtmlFromEntry(const zim::Entry &entry) const;

};
#endif
