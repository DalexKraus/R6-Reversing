#include "Globals.hpp"

namespace Globals
{
    /* Generic window information */
    LxWindow* lxwindow;
    HWND windowHandle;
    double dTime, time;
    unsigned int fps;

    /* A reconstruction of the game's camera */
    LxCamera* camera;

    /* The player list */
    Player_t* playerList;
    /* The index of our own player in the list */
    unsigned int playerIdx;
}