
#include "searchHandler.h"
//move storage and other paths to config!
#include "eventHandler.h"
#include "log.h"
#include "archive.h"
#include "search.h"


void SearchHandler::drawSearchResults(const std::string &_searchText)
{
    //test if is empty?
    if(!_searchText.empty())
    {
        Log::writeInfoLog(_searchText);
        auto textHeight = ScreenHeight() / 50;
        auto startscreenFont = OpenFont("LiberationMono", textHeight, FONT_BOLD);
        SetFont(startscreenFont, BLACK);
        try{
            auto results = searchForText(_searchText);
            auto heigth = 200;
            if(results.size() > 0)
            {
                for(auto entry: results) {
                    DrawTextRect(10, heigth, ScreenWidth(), heigth+20, entry.getTitle().c_str(), ALIGN_LEFT);
                    //PartialUpdate(10, heigth, ScreenWidth(),heigth+20);
                    //auto x = entry.getItem();
                    heigth += 30;
                }
                FullUpdate();
            }else{
                DrawTextRect(10, heigth, ScreenWidth(), 20, "not found", ALIGN_LEFT);
                PartialUpdate(10, heigth, ScreenWidth(),20);
            }
        } catch (const std::exception& e) {
            Log::writeInfoLog(e.what());
        }
        CloseFont(startscreenFont);
    }
    else
    {
        Message(ICON_ERROR, "Error", "Search text empty", 1200);
    }
}

zim::SearchResultSet SearchHandler::searchForText(const std::string& text)
{
    std::string path = STORAGE_FOLDER + "archlinux_en_all_maxi_2022-04.zim";
    zim::Archive archive(path);
    Log::writeInfoLog(path);
    //zim::Searcher searcher(_archives);
    zim::Searcher searcher(archive);
    zim::Query query;
    query.setQuery(text);
    zim::Search search = searcher.search(query);
    return search.getResults(1,50);
}
