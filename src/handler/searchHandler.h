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

#include "archive.h"
#include "search.h"

#include <string>

class SearchHandler
{
    public:
        /**
         * Defines fonds, sets global Event Handler and starts new content
         */

        static void drawSearchResults(const std::string &_searchText);
    private:
        explicit SearchHandler();

        static zim::SearchResultSet searchForText(const std::string& text);

};
#endif
