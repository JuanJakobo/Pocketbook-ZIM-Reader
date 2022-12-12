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

//#include <zim/archive.h>
//#include <zim/search.h>
//#include <zim/suggestion.h>
//#include <zim/item.h>
#include <archive.h>

using std::string;

std::unique_ptr<EventHandler> EventHandler::_eventHandlerStatic;

EventHandler::EventHandler()
{
    if (iv_access(CONFIG_FOLDER.c_str(), W_OK) != 0)
        iv_mkdir(CONFIG_FOLDER.c_str(), 0777);
    if (iv_access(ARTICLE_FOLDER.c_str(), W_OK) != 0)
        iv_mkdir(ARTICLE_FOLDER.c_str(), 0777);

    std::string path = ARTICLE_FOLDER + "/archlinux_en_all_maxi_2022-04.zim";
    Log::writeInfoLog(path);
    //create an copy of the eventhandler to handle methods that require static functions
    _eventHandlerStatic = std::unique_ptr<EventHandler>(this);
    try{
        zim::Archive archive(path);
        Log::writeInfoLog(std::to_string(archive.getArticleCount()));
    } catch (const std::exception& e) {
        Log::writeInfoLog(e.what());
    }
}

