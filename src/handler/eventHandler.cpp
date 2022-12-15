//------------------------------------------------------------------
// eventHandler.cpp
//
// Author:           JuanJakobo
// Date:             12.12.2022
//
//-------------------------------------------------------------------
#include "eventHandler.h"

#include "inkview.h"

#include "searchHandler.h"
#include "log.h"
#include "config.h"

//libzim
#include "archive.h"
#include "item.h"

#include <string>
#include <fstream>
#include <experimental/filesystem>


using std::string;
namespace fs = std::experimental::filesystem;

std::unique_ptr<EventHandler> EventHandler::_eventHandlerStatic;

EventHandler::EventHandler()
{
    createConfigAndStorageFolders();
    _eventHandlerStatic = std::unique_ptr<EventHandler>(this);

    addArchivesFromStoragePath();

    //TODO all this into searchview!
    int _panelMenuBeginY = 20;
    int _panelMenuHeight = _panelMenuBeginY + ScreenHeight() / 15;
    int _mainMenuWidth = ScreenWidth() / 3;
    int _panelMenuBeginX = ScreenWidth() - _mainMenuWidth;
    int contentOffsetY = 30;

    _contentRect = iRect(0, _panelMenuHeight+contentOffsetY, ScreenWidth(), (ScreenHeight() - _panelMenuHeight - contentOffsetY), 0);
    FillAreaRect(&_contentRect, WHITE);

    _filterButton = iRect(_mainMenuWidth * 2, _panelMenuBeginY,_mainMenuWidth, _panelMenuHeight, ALIGN_CENTER);
    _searchTextView =  iRect(0, _panelMenuBeginY, ScreenWidth()-_mainMenuWidth, _panelMenuHeight, ALIGN_CENTER);
    drawSearchScreen();

    //TODO create searchHandler move
    _search = std::make_unique<SearchHandler>(_contentRect, _archives);

    FullUpdate();
}

int EventHandler::eventDistributor(const int type, const int x, const int y)
{
    if (ISPOINTEREVENT(type))
        return EventHandler::pointerHandler(type, x, y);
    else if (ISKEYEVENT(type))
        return EventHandler::keyHandler(type, x, y);

    return 1;
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
    DrawTextRect2(&_searchTextView, "Click to search");
}

void EventHandler::drawSearchButton() const
{
    DrawTextRect2(&_filterButton, "Filter");
    DrawRect(_filterButton.x - 1, _filterButton.y - 1, _filterButton.w + 2,
            _filterButton.h + 2, BLACK);
}

void EventHandler::createConfigAndStorageFolders() const
{
    if (iv_access(CONFIG_FOLDER.c_str(), W_OK) != 0)
        iv_mkdir(CONFIG_FOLDER.c_str(), 0666);
    if (iv_access(STORAGE_FOLDER.c_str(), W_OK) != 0)
        iv_mkdir(STORAGE_FOLDER.c_str(), 0666);
    if (iv_access(ARTICLE_FOLDER.c_str(), W_OK) != 0)
        iv_mkdir(ARTICLE_FOLDER.c_str(), 0666);
    if (iv_access(IMAGE_FOLDER.c_str(), W_OK) != 0)
        iv_mkdir(IMAGE_FOLDER.c_str(), 0666);
}

void EventHandler::addArchivesFromStoragePath()
{
    if (iv_access(STORAGE_FOLDER.c_str(), R_OK) == 0)
    {
        for (const auto &entry : fs::directory_iterator(STORAGE_FOLDER))
        {
            if(!is_directory(entry))
            {
                Log::writeInfoLog(entry.path());
                try{
                    _archives.emplace_back(entry.path());
                } catch (const std::exception& e) {
                    Log::writeErrorLog(e.what());
                }
            }
        }
    }
    if(_archives.size() == 0)
    {
        int dialogResult = DialogSynchro(ICON_INFORMATION, "Action", "Could not find any .ZIM files. Please place them into the Zim-Reader folder.", "Close App", NULL, NULL);
        CloseApp();
    }
}

int EventHandler::keyHandler(const int type, const int pressedKey, const int par2)
{
    if (type == EVT_KEYPRESS)
    {
        //TODO check if results are shown
        switch(pressedKey)
        {
            case 23:
                _search->firstPage();
                break;
            case 24:
                _search->prevPage();
                break;
            case 25:
                _search->nextPage();
                break;
            default:
                return 1;
        }
        return 0;
    }

    return 1;
}

int EventHandler::pointerHandler(const int type, const int x, const int y)
{
    if (type == EVT_POINTERUP)
    {
        if (IsInRect(x, y, &_searchTextView))
        {
            openSearchTextKeyboardAndSearch();
        }
        else if(IsInRect(x, y, &_filterButton))
        {
            //TODO implement filter
            ShowHourglassForce();
            std::string currentArchives;
            currentArchives += "Number of files: " + std::to_string(_archives.size()) + '\n';
            for(const auto &archive : _archives)
            {
                currentArchives += archive.getFilename() +  "\n Filesize : " + std::to_string(archive.getFilesize()) + ", Entry Count : " + std::to_string(archive.getAllEntryCount()) + ", Article Count: " + std::to_string(archive.getArticleCount()) + ", Media Count: " + std::to_string(archive.getMediaCount()) + '\n';
            }

            HideHourglass();
            int dialogResult =
                DialogSynchro(ICON_INFORMATION, "Action",  currentArchives.c_str(),
                        "Close", NULL, NULL);

        }
        else if(_search->checkIfEntryClicked(x,y))
        {
            std::string storagePath = createHtmlFromEntry(_search->getCurrentEntry().entry);
            OpenBook(storagePath.c_str(), "", 0);
        }
    }
    return 0;
}

void EventHandler::openSearchTextKeyboardAndSearch()
{
    if (_searchText.empty())
      _searchText = "";

    _searchText.resize(KEYBOARD_STRING_LENGHT);
    OpenKeyboard("Search", &_searchText[0], KEYBOARD_STRING_LENGHT - 1, KBD_NORMAL, &setSearchTextToKeyboardInputStatic);
}

void EventHandler::setSearchTextToKeyboardInputStatic(char *keyboardText)
{
  _eventHandlerStatic->setSearchTextToKeyboardInput(keyboardText);
}

void EventHandler::setSearchTextToKeyboardInput(char *keyboardText)
{
    if (!keyboardText)
        return;

    string stringKeyboardText(keyboardText);
    if (stringKeyboardText.empty())
        return;
    _searchText = stringKeyboardText;
    drawSearchResults();
}

void EventHandler::drawSearchResults() const
{
    //TODO define fonts
    FillAreaRect(&_searchTextView, WHITE);
    DrawTextRect2(&_searchTextView, _searchText.c_str());

    PartialUpdate(_searchTextView.x, _searchTextView.y, _searchTextView.w, _searchTextView.h);

    _search->getAndDrawSearchResults(_searchText);

}

std::string EventHandler::createHtmlFromEntry(const zim::Entry &entry) const
{
    //TODO check for type
    string storagePath = ARTICLE_FOLDER + '/' + entry.getTitle() + ".html";
    Log::writeInfoLog(storagePath);

    //TODO create images
    //std::string getMetadata(const std::string& name) const;
    //Item getMetadataItem(const std::string& name) const;
    std::string entryContent = entry.getItem().getData();

    std::ofstream htmlfile;
    htmlfile.open(storagePath);
    htmlfile << entryContent;
    htmlfile.close();
    Log::writeInfoLog("html created.");
    return storagePath;
}
