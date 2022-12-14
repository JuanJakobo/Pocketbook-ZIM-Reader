//------------------------------------------------------------------
// eventHandler.cpp
//
// Author:           JuanJakobo
// Date:             12.12.2022
//
//-------------------------------------------------------------------

#include "eventHandler.h"
#include "inkview.h"
#include "archive.h"
#include "search.h"
#include "item.h"
#include "searchHandler.h"
#include "suggestion.h"

#include "log.h"
#include <string>
#include <fstream>


using std::string;

std::unique_ptr<EventHandler> EventHandler::_eventHandlerStatic;

EventHandler::EventHandler()
{
    Log::writeInfoLog("testx");
    createConfigAndStorageFolders();
    _eventHandlerStatic = std::unique_ptr<EventHandler>(this);

    try{
        addArchivesFromPath();
    } catch (const std::exception& e) {
        Log::writeInfoLog(e.what());
    }
    _searchTextView = iRect(0, 20, ScreenWidth()/2, 150, ALIGN_LEFT);
    _searchButton =  iRect(ScreenWidth()/3*2, 20, ScreenWidth()/4, 150, ALIGN_CENTER);
    drawSearchScreen();

    FullUpdate();
}

void EventHandler::drawSearchScreen() const
{
    auto textHeight = ScreenHeight() / 50;
    auto startscreenFont = OpenFont("LiberationMono", textHeight, FONT_BOLD);
    SetFont(startscreenFont, BLACK);

    drawSearchTextView();
    drawSearchButton();
    CloseFont(startscreenFont);
}

void EventHandler::drawSearchTextView() const
{
    DrawRect(_searchTextView.x - 1, _searchTextView.y - 1, _searchTextView.w + 2,
            _searchTextView.h + 2, BLACK);
    DrawTextRect2(&_searchTextView, "Text");
}

void EventHandler::drawSearchButton() const
{
    DrawTextRect2(&_searchButton, "Search");
    DrawRect(_searchButton.x - 1, _searchButton.y - 1, _searchButton.w + 2,
            _searchButton.h + 2, BLACK);
}

void EventHandler::createConfigAndStorageFolders() const
{
    if (iv_access(CONFIG_FOLDER.c_str(), W_OK) != 0)
        iv_mkdir(CONFIG_FOLDER.c_str(), 0666);
    if (iv_access(ARTICLE_FOLDER.c_str(), W_OK) != 0)
        iv_mkdir(ARTICLE_FOLDER.c_str(), 0666);
    if (iv_access(ARTICLE_FOLDER.c_str(), W_OK) != 0)
        iv_mkdir(ARTICLE_FOLDER.c_str(), 0666);
    if (iv_access(IMAGE_FOLDER.c_str(), W_OK) != 0)
        iv_mkdir(IMAGE_FOLDER.c_str(), 0666);
}

int EventHandler::eventDistributor(const int type, const int x, const int y)
{
    if (ISPOINTEREVENT(type))
        return EventHandler::pointerHandler(type, x, y);

    return 1;
}

int EventHandler::pointerHandler(const int type, const int x, const int y)
{
    if (type == EVT_POINTERUP)
    {
        if (IsInRect(x, y, &_searchTextView))
        {
            openSearchDialogAndUpdateSearchTextView();
        }
        else if(IsInRect(x, y, &_searchButton))
        {
            std::thread a (SearchHandler::drawSearchResults, _searchText);//, _archives);
            a.join();
        }
    }
    return 0;
}

void EventHandler::addArchivesFromPath()
{
    //pushback contra emplace back
    std::string path = STORAGE_FOLDER + "archlinux_en_all_maxi_2022-04.zim";
    //_archives.emplace_back(path);
    _archives.push_back(zim::Archive(path));
    path = STORAGE_FOLDER + "wikipedia_en_basketball_maxi_2022-11.zim";
    _archives.emplace_back(zim::Archive(path));
    Log::writeInfoLog("loaded");
    Log::writeInfoLog(std::to_string(_archives.at(0).getAllEntryCount()));
}

void EventHandler::openSearchDialogAndUpdateSearchTextView()
{
    std::string _keyboardInput = "";
    if (!_searchText.empty())
      _keyboardInput = _searchText;
    _keyboardInput.resize(KEYBOARD_STRING_LENGHT);
    //Look at openkeyboard method
    //TODO fails on second attempt
    OpenKeyboard("Search", &_keyboardInput[0], KEYBOARD_STRING_LENGHT - 1, KBD_NORMAL, &keyboardHandlerStatic);
}

void EventHandler::keyboardHandlerStatic(char *keyboardText)
{
  _eventHandlerStatic->keyboardHandler(keyboardText);
}

void EventHandler::keyboardHandler(char *keyboardText)
{
    if (!keyboardText)
        return;

    string skeyboardText(keyboardText);
    if (skeyboardText.empty())
        return;
    _searchText = skeyboardText;
    FillAreaRect(&_searchTextView, WHITE);
    DrawTextRect2(&_searchTextView, _searchText.c_str());
    PartialUpdate(_searchTextView.x, _searchTextView.y, _searchTextView.w, _searchTextView.h);
}




void EventHandler::createHtml()
{
    /*
    string title_path = title_folder + entry.getTitle() + ".html";
    Log::writeInfoLog(title_path);

    Log::writeInfoLog("creating");
    std::ofstream htmlfile;
    htmlfile.open(title_path);
    htmlfile << x.getData();
    htmlfile.close();
    //OpenBook(title_path.c_str(), "", 0);
    */
}

void EventHandler::searchForImagesInHtml()
{
    /*
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
    */
}
