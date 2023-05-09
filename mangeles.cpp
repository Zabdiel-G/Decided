//3350
//progam: mangeles.cpp
//Author: Marc Angeles
//2/27/2023

#include <iostream>
//#include "sarboleda.cpp"
//#include "Global.h"
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <cstring>
#include "sarboleda.h"
#include "EnemR.h"
//#include "Game.h"
using namespace std;


int gameState = 0;

void messageFire()
{
    printf("pew");    
}

//Main menu prototype
bool startMenu() {
    bool exit = false;
    int num = 0;
    while (exit != true) {
        cout << "Start Menu" << endl;
        cout << "1: Start Game 2:End Game" << endl;
        cin >> num;
        switch (num) {
            case 1:
                return true;
                break;
            case 2:
                return false;
                break;
            default:
                cout << "Please Choose A Different Number" << endl;
                break;
        }
    }
    return 0;

/*    glColor3f(100.0f, 100.0f, 100.0f);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex2f(-xres, yres);
    glVertex2f(xres, yres);
    glVertex2f(xres, -yres);
    glVertex2f(-xres, -yres);
    glEnd();
    glPopMatrix();*/


}

void Menu(float xres, float yres) {

    bool menu = true;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Initialize X11 display and window
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        cout << "Failed to open X11 display" << endl;
        return;
    }
    Window window = XCreateSimpleWindow(display, 
            DefaultRootWindow(display), 0, 0, xres, yres, 0, 0, 0);
    XSelectInput(display, window, ExposureMask | ButtonPressMask);

    // Create a graphics context for drawing text
    GC gc = XCreateGC(display, window, 0, NULL);

    // Set the font and font color
    XFontStruct *font_info = XLoadQueryFont(display, "10x20");
    XSetFont(display, gc, font_info->fid);
    XSetForeground(display, gc, 0xFFFFFF);

    // Define button properties
    int button_x = xres/2 - 25;
    int button_y = yres/2 + 50;
    int button_width = 50;
    int button_height = 25;

    // Map the window to the screen
    XMapWindow(display, window);

    while(menu) {
        XEvent event;
        XNextEvent(display, &event);
        switch (event.type) {
            case Expose:
                // Draw the text and button
                XDrawString(display, window, gc, xres/2 - 22,
                    font_info->ascent, "Decided", 7);
                XSetForeground(display, gc, 0x0000FF);
                XFillRectangle(display, window, gc, button_x, button_y,
                    button_width, button_height);
                XSetForeground(display, gc, 0xFFFFFF);
                XDrawRectangle(display, window, gc, button_x, button_y,
                    button_width, button_height);
                XDrawString(display, window, gc, button_x, button_y + 17,
                        "Start", 5);
                XSetForeground(display, gc, 0xFF0000);


                break;
            case ButtonPress:
                // Check for button click event
                if (event.xbutton.x >= button_x && event.xbutton.x 
                        <= button_x + button_width && event.xbutton.y 
                        >= button_y && event.xbutton.y 
                        <= button_y + button_height) {
                    menu = false;
                }
                break;
        }
    }

    // Clean up
    XFreeGC(display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
}

void pauseMenu(int i, float xres, float yres, int flag)
{
    //i = i-1;
    float boxWidth = xres / 2.0f;
    float boxHeight = yres / 2.0f;
    float x = (xres - boxWidth) / 2.0f;
    float y = (yres - boxHeight - 30.0f) / 2.0f;
    //Draw the box
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(x, y);
    glVertex2f(x + boxWidth, y);
    glVertex2f(x + boxWidth, y + boxHeight);
    glVertex2f(x, y + boxHeight);
    glEnd();

    float buttonY[3];
    float buttonWidth = boxWidth - 20.0f;
    float buttonHeight = (boxHeight * 0.2f);
    float buttonX = ((xres - boxWidth)/2.0f) - 200;
    for (int j = 0; j < 4; j++) {
    buttonY[j] = y + boxHeight - (boxHeight/2.5) - j * (buttonHeight + 10.0f);
    }
    //Draw the buttons


    glBegin(GL_QUADS);
    for(int k = 0; k < 4; k++) {
        glColor3f(1,1,1);
        glVertex2f(buttonX, buttonY[k]);
        glVertex2f(buttonX + buttonWidth, buttonY[k]);
        glVertex2f(buttonX + buttonWidth, buttonY[k] + buttonHeight);
        glVertex2f(buttonX, buttonY[k] + buttonHeight);
    }
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(buttonX - 2.0f, buttonY[i] - 2.0f);
    glVertex2f(buttonX + buttonWidth + 2.0f, buttonY[i] - 2.0f);
    glVertex2f(buttonX + buttonWidth + 2.0f, buttonY[i] + buttonHeight + 2.0f);
    glVertex2f(buttonX - 2.0f, buttonY[i] + buttonHeight + 2.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(buttonX, buttonY[i]);
    glVertex2f(buttonX + buttonWidth, buttonY[i]);
    glVertex2f(buttonX + buttonWidth, buttonY[i] + buttonHeight);
    glVertex2f(buttonX, buttonY[i] + buttonHeight);
    glEnd();

    Rect r;
    r.bot = y + boxHeight;
    r.left = x + boxWidth/2;
    r.center = 0;
    ggprint8b(&r, 100, 0x00ffff44, "PAUSED");
    const char* buttonText[4] = {"continue", "save", "load", "quit"};
    switch (flag) {
        case 0:
            buttonText[0] = "Continue";
            buttonText[1] = "Save";
            buttonText[2] = "Load";
            buttonText[3] = "Quit";
            break;
        case 1:
            buttonText[0] = "Back";
            buttonText[1] = "Save 1";
            buttonText[2] = "Save 2";
            buttonText[3] = "Cotinue";
            break;
        case 2:
            buttonText[0] = "Back";
            buttonText[1] = "Load 1";
            buttonText[2] = "Load 2";
            buttonText[3] = "Continue";

            break;

    }
    for(int k = 0; k < 4; k++) {
        glColor3f(1,1,1);
        glVertex2f(buttonX, buttonY[k]);
        glVertex2f(buttonX + buttonWidth, buttonY[k]);
        glVertex2f(buttonX + buttonWidth, buttonY[k] + buttonHeight);
        glVertex2f(buttonX, buttonY[k] + buttonHeight);
        Rect r;
        r.bot = buttonY[k] + buttonHeight/2.0f - 8.0f;
        r.left = buttonX + buttonWidth/2.0f;
        r.center = 1;
        ggprint8b(&r, 0, 0x000000ff, buttonText[k]);
    }
}


//was meant to be here, but was not able to transfer due to needing the classes in the main file
/*void pauseMenuChoices(Player& player, EnemR* ahead) {
    extern void makeSaveFile(int, Player, EnemR*);
    extern int countSaveFile();
    extern void loadSaveFile(int, Player&, EnemR*&);
    static bool isUpPressed = false;
    static bool isDownPressed = false;


    // Check if up key is pressed
    if (gl.keys[XK_Up] && !isUpPressed) {
        gl.pauseMenuButton = (gl.pauseMenuButton + 3) % 4;
        std::cout << gl.pauseMenuButton << std::endl;
        isUpPressed = true;
    } else if (!gl.keys[XK_Up]) {
        isUpPressed = false;
    }

    // Check if down key is pressed
    if (gl.keys[XK_Down] && !isDownPressed) {
        gl.pauseMenuButton = (gl.pauseMenuButton + 1) % 4;
        std::cout << gl.pauseMenuButton << std::endl;
        isDownPressed = true;
    } else if (!gl.keys[XK_Down]) {
        isDownPressed = false;
    }


    if (gl.isPaused) {

        if (gl.keys[XK_a] ) {
            switch (gl.pauseMenuButton) {
                case 0:
                    gl.isPaused = false;
                    break;
                case 1:
                    gl.menuFlag = 1;
                    gl.isSave = true;
                    gl.isPaused = false;
                    break;
                case 2:
                    gl.menuFlag = 2;
                    gl.isLoad = true;
                    gl.isPaused = false;
                    break;
                case 3:
                    exit(0);
            }
        }
    }

    if (gl.isLoad) {
        if (gl.keys[XK_a] ) {
            switch (gl.pauseMenuButton) {
                case 0:
                    gl.menuFlag = 0;
                    gl.isPaused = true;
                    gl.isLoad = false;
                    break;
                case 1:
                    loadSaveFile(0, player, ahead);
                    break;
                case 2:
                    loadSaveFile(1, player, ahead);
                    break;
                case 3:
                    gl.isPaused = false;
                    gl.isLoad = false;
            }
        }

    }
     if (gl.isSave) {
        if (gl.keys[XK_a]) {
            switch (gl.pauseMenuButton) {
                case 0:
                    gl.menuFlag = 0;
                    gl.isPaused = true;
                    gl.isSave = false;
                    break;
                case 1:
                    makeSaveFile(0, player, ahead);
                    break;
                case 2:
                    makeSaveFile(1, player, ahead);
                    break;
                case 3:
                    gl.isPaused = false;
                    gl.isSave = false;
            }
        }
    }
}*/

