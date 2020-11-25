#ifndef GLOBALS_H
#define GLOBALS_H

#include "lx.hpp"
#include "Types.hpp"

namespace Globals
{
    /* Generic window information */
    extern LxWindow* lxwindow;
    extern HWND windowHandle;
    extern double dTime, time;
    extern unsigned int fps;

    /* A reconstruction of the game's camera */
    extern LxCamera* camera;

    /* The player list */
    extern Player_t* playerList;
    /* The index of our own player in the list */
    extern unsigned int playerIdx;
}

#endif
