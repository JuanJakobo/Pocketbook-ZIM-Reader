//------------------------------------------------------------------
// eventHandler.cpp
//
// Author:           JuanJakobo
// Date:             12.12.2022
//
//-------------------------------------------------------------------

#include "eventHandler.h"
#include "inkview.h"

#include "log.h"
#include <string>
#include <fstream>

//#include <zim/archive.h>
//#include <zim/search.h>
//#include <zim/suggestion.h>
//#include <zim/item.h>
#include <archive.h>
#include <search.h>
#include <item.h>
#include <suggestion.h>

using std::string;

std::unique_ptr<EventHandler> EventHandler::_eventHandlerStatic;

EventHandler::EventHandler()
{
    if (iv_access(CONFIG_FOLDER.c_str(), W_OK) != 0)
        iv_mkdir(CONFIG_FOLDER.c_str(), 0777);
    if (iv_access(ARTICLE_FOLDER.c_str(), W_OK) != 0)
        iv_mkdir(ARTICLE_FOLDER.c_str(), 0777);

    //std::string path = ARTICLE_FOLDER + "/archlinux_en_all_maxi_2022-04.zim";
    std::string path = ARTICLE_FOLDER + "/wikipedia_en_basketball_maxi_2022-11.zim";
    Log::writeInfoLog(path);
    //create an copy of the eventhandler to handle methods that require static functions
    _eventHandlerStatic = std::unique_ptr<EventHandler>(this);
    try{
        zim::Archive archive(path);
        Log::writeInfoLog(std::to_string(archive.getArticleCount()));
        // Create a searcher, something to search on an archive
        zim::Searcher searcher(archive);

        // We need a query to specify what to search for
        zim::Query query;
        query.setQuery("A/Basketball");
        // Create a search for the specified query
        zim::Search search = searcher.search(query);

        // Now we can get some result from the search.
        // 20 results starting from offset 10 (from 10 to 30)
        //zim::SearchResultSet results = search.getResults(1,50);
        zim::SearchResultSet results = search.getResults(1,1);

        // SearchResultSet is iterable
        // TODO or use getPath later
        string title_folder = ARTICLE_FOLDER + "/A/";
        if (iv_access(title_folder.c_str(), W_OK) != 0)
            iv_mkdir(title_folder.c_str(), 0666);

        string image_folder = ARTICLE_FOLDER + "/I/";
        if (iv_access(title_folder.c_str(), W_OK) != 0)
            iv_mkdir(title_folder.c_str(), 0666);

        for(auto entry: results) {
            Log::writeInfoLog(entry.getPath() + " title " + entry.getTitle());
            auto x = entry.getItem();
            string title_path = title_folder + entry.getTitle() + ".html";
            Log::writeInfoLog(title_path);

            Log::writeInfoLog("creating");
            std::ofstream htmlfile;
            htmlfile.open(title_path);
            htmlfile << x.getData();
            htmlfile.close();

            auto z = archive.getEntryByPath("I/Basketball_Positions.png.webp");
            Log::writeInfoLog(z.getTitle() + " pa " + z.getPath());
            auto imagePath = image_folder + z.getTitle();
            std::ofstream img(imagePath, std::ofstream::out);
            if (img.is_open())
            {
                //TODO check internet getting
                img << z.getItem().getData();
                img.close();
            }
            //OpenBook(title_path.c_str(), "", 0);

        }

        /*
        // Create a searcher, something to search on an archive
        zim::SuggestionSearcher ssearcher(archive);

        // Create a search for the specified query
        zim::SuggestionSearch ssearch = ssearcher.suggest("Dell");

        // Now we can get some result from the search.
        // 20 results starting from offset 10 (from 10 to 30)
        zim::SuggestionResultSet sresults = ssearch.getResults(10, 20);

            Log::writeInfoLog("suggest");
        // SearchResultSet is iterable
        for(auto entry: sresults) {
            Log::writeInfoLog(entry.getPath() + " title " + entry.getTitle());
        }
        */
    } catch (const std::exception& e) {
        Log::writeInfoLog(e.what());
    }
}

