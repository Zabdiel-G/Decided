//3350
//progam: mangeles.cpp
//Author: Marc Angeles
//2/27/2023

#include <iostream>
//#include "Global.h"
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <cstring>

using namespace std;


int gameState = 0;

void messageFire()
{
    printf("pew");    
}

void exitGame() {
    exit(0);
}

/*bool startMenu() {
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

    glColor3f(100.0f, 100.0f, 100.0f);
    glPushMatrix();
    glBegin(GL_QUADS);
    glVertex2f(-xres, yres);
    glVertex2f(xres, yres);
    glVertex2f(xres, -yres);
    glVertex2f(-xres, -yres);
    glEnd();
    glPopMatrix();


}*/

void Menu(float xres, float yres) {

    int flag = 0;
    bool menu = true;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Initialize X11 display and window
    Display *display = XOpenDisplay(NULL);
    if (!display) {
        cout << "Failed to open X11 display" << endl;
        return;
    }
    Window window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, xres, yres, 0, 0, 0);
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
                if (event.xbutton.x >= button_x && event.xbutton.x <= button_x + button_width &&
                    event.xbutton.y >= button_y && event.xbutton.y <= button_y + button_height) {
                    flag = 1;
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

