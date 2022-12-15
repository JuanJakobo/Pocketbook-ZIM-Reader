#ifndef SEARCH_HANDLER
#define SEARCH_HANDLER
//------------------------------------------------------------------
// eventHandler.h
//
// Author:           JuanJakob
// Date:             12.12.2022
// Description:      Handles all events and directs them
//-------------------------------------------------------------------


#include "inkview.h"

#include "listView.h"

#include "archive.h"
#include "search.h"
#include "searchModel.h"
#include "searchViewEntry.h"

#include <memory>
#include <string>
#include <future>

class SearchHandler final : public ListView
{
    public:
        explicit SearchHandler(const irect &contentRect, const std::vector<zim::Archive> &archives, int page = 1);

        void getAndDrawSearchResults(const std::string &searchText);

        SearchEntry& getCurrentEntry() { return getEntry(_selectedEntry); };


    private:
        std::shared_ptr<std::vector<zim::Archive>> _archives;
        std::unique_ptr<zim::Search> _currentSearch;

        SearchEntry& getEntry(int entryID) {return std::dynamic_pointer_cast<SearchViewEntry>(_entries.at(entryID))->getMfEntry(); };

        void searchAndStoreResults(const std::string &searchText);
        void drawSearchResults(int rangeBegin, int rangeEnd);
        void deleteCurrentEntries();

};
#endif
