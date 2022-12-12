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

#include <string>
#include <memory>

//TODO constexpr=
const std::string CONFIG_FOLDER = "/mnt/ext1/system/config/ZIM-Reader";
const std::string ARTICLE_FOLDER = "/mnt/ext1/ZIM-Reader";

class EventHandler
{
    public:
        /**
         * Defines fonds, sets global Event Handler and starts new content
         */
        EventHandler();

    private:
        static std::unique_ptr<EventHandler> _eventHandlerStatic;
};
#endif
