#include "searchHandler.h"

#include "log.h"
#include "search.h"
#include "searchModel.h"
#include "searchViewEntry.h"

//libzim
#include "archive.h"
#include "item.h"

#include <memory>

SearchHandler::SearchHandler(const irect &contentRect, const std::vector<zim::Archive> &archives, int page) : ListView(contentRect, page)
{
    _archives = std::make_shared<std::vector<zim::Archive>>(archives);
}

void SearchHandler::getAndDrawSearchResults(const std::string &searchText)
{
    if(!searchText.empty())
    {
        ShowHourglassForce();
        try{
            searchAndStoreResults(searchText);
            drawSearchResults(1,15);
        } catch (const std::exception& e) {
            Log::writeInfoLog(e.what());
        }
    }
    else
    {
        Message(ICON_ERROR, "Error", "Search text empty", 1200);
    }
    HideHourglass();

}

void SearchHandler::searchAndStoreResults(const std::string &searchText)
{
    zim::Searcher searcher(*_archives);
    zim::Query query(searchText);
    query.setQuery(searchText);
    _currentSearch = std::make_unique<zim::Search>(searcher.search(query));
    //TODO use estimated search results
    //auto estimatedMatches = _currentSearch->getEstimatedMatches();

}

void SearchHandler::drawSearchResults(int rangeBegin, int rangeEnd)
{
    auto results = _currentSearch->getResults(rangeBegin,rangeEnd);
    auto pageHeight = 0;
    auto contentHeight = _contentRect.h - _footerHeight;

    deleteCurrentEntries();

    if(results.size() > 0)
    {
        for(auto it = results.begin(); it != results.end(); ++it)
        {
            /*
             TODO add to SearchEntry
            Log::writeInfoLog(std::to_string(it.getWordCount()));
            Log::writeInfoLog(std::to_string(it.getScore()));
            Log::writeInfoLog(std::to_string(it.getSize()));
            Log::writeInfoLog(std::to_string(it.getFileIndex()));
            Log::writeInfoLog(it.getSnippet());

            Log::writeInfoLog(it->getItem().getMimetype());
            Log::writeInfoLog(std::to_string(it->getItem().getSize()));
            */
            auto searchEntry = SearchEntry(*it);

            auto entrySize = TextRectHeight(_contentRect.w, it.getTitle().c_str(), 0) + 2.5 * _entryFontHeight;

            if ((pageHeight + entrySize) > contentHeight)
            {
                pageHeight = 0;
                _page++;
            }
            irect rect = iRect(_contentRect.x, _contentRect.y + pageHeight, _contentRect.w, entrySize, 0);

           _entries.emplace_back(std::make_unique<SearchViewEntry>(SearchViewEntry(_page, rect, searchEntry)));

            pageHeight = pageHeight + entrySize;
        }
        draw();
    }else{
        FillAreaRect(&_contentRect, WHITE);
        DrawTextRect(_contentRect.x, _contentRect.y, _contentRect.w, 20, "No results.", ALIGN_LEFT);
        PartialUpdate(_contentRect.x, _contentRect.y, _contentRect.w, _contentRect.h);
    }

}

void SearchHandler::deleteCurrentEntries()
{
    _entries.clear();
    _page = 1;
    _shownPage = 1;
}
