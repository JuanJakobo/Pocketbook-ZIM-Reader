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

//TODO constexpr=
const std::string CONFIG_FOLDER = "/mnt/ext1/system/config/ZIM-Reader";
const std::string STORAGE_FOLDER = "/mnt/ext1/ZIM-Reader/";
//Doppelt
const std::string ARTICLE_FOLDER = "/mnt/ext1/ZIM-Reader/A/";
const std::string IMAGE_FOLDER = "/mnt/ext1/ZIM-Reader/I/";

constexpr int KEYBOARD_STRING_LENGHT = 90;

class EventHandler
{
    public:
        /**
         * Defines fonds, sets global Event Handler and starts new content
         */
        EventHandler();
        /**
         * Handles events and redirects them
         *
         * @param type event type
         * @param par1 first argument of the event
         * @param par2 second argument of the event
         * @return int returns if the event was handled
         */
        int eventDistributor(const int type, const int par1, const int par2);

    private:
        static std::unique_ptr<EventHandler> _eventHandlerStatic;
        std::vector<zim::Archive> _archives;
        std::string _searchText;
        irect _searchTextView;
        irect _searchButton;


        void drawSearchScreen() const;
        void drawSearchTextView() const;
        void drawSearchButton() const;

        /**
         * Handles pointer Events
         *
         * @param type event type
         * @param par1 first argument of the event
         * @param par2 second argument of the event
         * @return int returns if the event was handled
         */
        int pointerHandler(const int type, const int par1, const int par2);

        /**
         * Handles key Events
         *
         * @param type event type
         * @param par1 first argument of the event (is the key)
         * @param par2 second argument of the event
         * @return int returns if the event was handled
         */
        int keyHandler(const int type, const int par1, const int par2);

        void createConfigAndStorageFolders() const;
        void addArchivesFromPath();
        void openSearchDialogAndUpdateSearchTextView();
        static void keyboardHandlerStatic(char *keyboardText);
        void keyboardHandler(char *keyboardText);
        void createHtml();
        void searchForImagesInHtml();

};
#endif
