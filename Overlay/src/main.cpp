#include <dwmapi.h>
#pragma comment(lib, "Dwmapi.lib")

#include "util/fileutil.h"
#include <glm/gtc/matrix_transform.hpp>

#include "lx.hpp"
#include "LxVao.hpp"
#include "shader/LxShader.hpp"

#include "lxcrystal/Globals.hpp"
#include "lxcrystal/renderer/HeadBoxRenderer.hpp"
#include "lxcrystal/renderer/TracersRenderer.hpp"

#define WINDOW_WIDTH    1920
#define WINDOW_HEIGHT   1080

HeadBoxRenderer* headboxRenderer;
TracersRenderer* tracersRenderer;

unsigned int frameCount;
double previousTime = glfwGetTime();

void updateCamera()
{
    Player_t*       players     = Globals::playerList;
    unsigned int    playerIdx   = Globals::playerIdx;

    glm::vec3 front, headPos;
    headPos   = glm::vec3(players[playerIdx].headPos[0], players[playerIdx].headPos[2], players[playerIdx].headPos[1]);
    front     = glm::vec3(players[playerIdx].frontVector[0], players[playerIdx].frontVector[2], players[playerIdx].frontVector[1]);

    Globals::camera->setPosition(&headPos);
    Globals::camera->setFront(&front);
    Globals::camera->updateView();
}

void draw(double deltaTime)
{
    SetWindowPos(Globals::windowHandle, HWND_TOPMOST, 0, 0, Globals::lxwindow->width(), Globals::lxwindow->height(), SWP_NOMOVE | SWP_NOSIZE);

    //Update time
    Globals::dTime = deltaTime;
    Globals::time = glfwGetTime();

    //Update FPS count
    frameCount++;
    if (Globals::time - previousTime >= 1.0)
    {
        previousTime = Globals::time;
        Globals::fps = frameCount;
        frameCount = 0;
    }

    //Update the camera reconstruction
    updateCamera();
    tracersRenderer->update(Globals::dTime);
    tracersRenderer->draw(Globals::camera->getProjection());

    headboxRenderer->update(Globals::dTime);
    headboxRenderer->draw(Globals::camera->getProjection());
}

DWORD WINAPI manipulator(LPVOID lpParameter)
{
    while (true)
    {
        printf("d: ");
        scanf_s("%d\n", &Globals::playerIdx);
        if (Globals::playerIdx < 0 || Globals::playerIdx > 9)
            Globals::playerIdx = 0;
    }
}

int main()
{
    /* Allocate the player list, in which the informations for each player will be stored. */
    Globals::playerList = (Player_t*) calloc(PLAYER_COUNT, sizeof(Player_t));

    /* Print the address of the player list.
     * This is currently the only way for the cheat to communicate with the vmcore.
     * 
     * At a later stage, this print statement might be replaced with signature scanning 
     * from the vmcore side. 
     */
    printf("PL: 0x%llx\n", (long long) Globals::playerList);

    // Some test data
    // Globals::playerList[0].headPos[0] = 5;
    // Globals::playerList[0].headPos[1] = 3;
    // Globals::playerList[0].headPos[2] = -2;
    // Globals::playerList[0].frontVector[0] = 0;
    // Globals::playerList[0].frontVector[1] = -1;
    // Globals::playerList[0].frontVector[2] = 0;
    // Globals::playerList[0].health = 100;

    // Globals::playerList[0].headPos[0] = 5;
    // Globals::playerList[0].headPos[1] = -2;
    // Globals::playerList[0].headPos[2] = -4;
    // Globals::playerList[0].frontVector[0] = 0;
    // Globals::playerList[0].frontVector[1] = -1;
    // Globals::playerList[0].frontVector[2] = 0;
    // Globals::playerList[0].health = 100;

    // Globals::playerList[0].headPos[0] = -3;
    // Globals::playerList[0].headPos[1] = 5;
    // Globals::playerList[0].headPos[2] = 2;
    // Globals::playerList[0].frontVector[0] = 0;
    // Globals::playerList[0].frontVector[1] = -1;
    // Globals::playerList[0].frontVector[2] = 0;
    // Globals::playerList[0].health = 100;

    /* Create window */
    Globals::lxwindow = new LxWindow("", WINDOW_WIDTH, WINDOW_HEIGHT, false);
    Globals::lxwindow->antiAlias(true);
    Globals::lxwindow->useVsync(true);

    float fovY      = 45;
    float aspect    = (float) Globals::lxwindow->width() / (float) Globals::lxwindow->height();
    Globals::camera = new LxCamera(fovY, aspect, 0.1f, 1000.0f);

    Globals::windowHandle = GetActiveWindow();
    LONG curr_style = GetWindowLong(Globals::windowHandle, GWL_EXSTYLE);
    
    SetLayeredWindowAttributes(Globals::windowHandle, 0, 255, LWA_ALPHA);

    MARGINS Margin = { -1 };
	DwmExtendFrameIntoClientArea(Globals::windowHandle, &Margin);
    SetWindowLong(Globals::windowHandle, GWL_EXSTYLE, WS_EX_LAYERED | WS_POPUP | WS_EX_TRANSPARENT);

    /* Features */
    headboxRenderer = new HeadBoxRenderer();
    tracersRenderer = new TracersRenderer();

    // Start thread to manipulate the current player position in the player list
    CreateThread(0, 0, manipulator, 0, 0, 0);

    Globals::lxwindow->show(&draw);
    getchar();
    
    delete headboxRenderer;
    delete tracersRenderer;
    delete Globals::camera;
    delete Globals::lxwindow;
    return 0;
}
