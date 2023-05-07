//
//Modified by: Zabdiel Garcia
//

#include <cstring>
#include <chrono>

#pragma once

typedef float Flt;
typedef float Vec[3];
typedef Flt Matrix[4][4];

using namespace std::chrono;

class Global 
{
    public:
        int xres, yres;
        char keys[65536];
        // bool serafinFeature;
        int pauseMenuButton;
        bool obstR;
        bool dodgePressing;
        bool swordSlash;
        bool canPressSword;
        bool keyPressed;
        bool isPaused;
        bool isLoad;
        bool isSave;
        bool upKeyPressed;
        bool downKeyPressed;
        int menuFlag;

        Global() 
        {
            menuFlag = 0;
            isLoad = false;
            isSave = false;
            upKeyPressed = false;
            downKeyPressed = false;
            pauseMenuButton = 0;
            isPaused = false;
            xres = 640;
            yres = 480;
            memset(keys, 0, 65536);
            //serafinFeature = false;
            obstR = true;
            dodgePressing = false;
            swordSlash = false;
            canPressSword = true;
            keyPressed = false;
        }
} gl;
