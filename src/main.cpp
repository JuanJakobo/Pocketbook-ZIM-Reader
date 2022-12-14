//------------------------------------------------------------------
// main.h
//
// Author:           JuanJakobo
// Date:             14.06.2020
// Description:      sets the inkview main handler
//-------------------------------------------------------------------

#include "inkview.h"
#include "eventHandler.h"
#include "log.h"

#include <memory>

std::unique_ptr<EventHandler> events = nullptr;

/**
* Handles events and redirects it
*
* @param type event type
* @param par1 first argument of the event
* @param par2 second argument of the event
* @return int returns if the event was handled
*/
int Inkview_handler(int type, int par1, int par2)
{
    switch (type)
    {
        case EVT_INIT:
            {
                events = std::make_unique<EventHandler>();
                return 1;
                break;
            }
        case EVT_EXIT:
        case EVT_HIDE:
            {
                //TODO
                return 1;
                break;
            }
        default:
            return events->eventDistributor(type, par1, par2);
            break;
    }
    return 0;
}

void drawStartscreen()
{
    auto textHeight = ScreenHeight() / 30;
    auto startscreenFont = OpenFont("LiberationMono", textHeight, FONT_BOLD);
    SetFont(startscreenFont, BLACK);
    DrawTextRect(0, (ScreenHeight() / 3) * 2, ScreenWidth(), textHeight, "ZIM Reader", ALIGN_CENTER);
    CloseFont(startscreenFont);
    FullUpdate();
}

int main()
{
    OpenScreen();
    SetOrientation(0);
    SetPanelType(0);

    drawStartscreen();
    InkViewMain(Inkview_handler);
    return 0;
}


