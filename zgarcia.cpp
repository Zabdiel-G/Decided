//
//Author: Zabdiel Garcia
//
//Source code: zgarcia.cpp
//

#include <iostream>
#include <GL/glut.h>
#include "EnemR.h"
#include <cmath>
extern struct timespec timeStart, timeCurrent;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);


//using namespace std;

void messageZ()
{
    std::cout << "Zabs Feature mode has been Entered" << std::endl;
}


void obstRend()
{
    int xres = 640;
    int yres = 480;

    //glClear(GL_COLOR_BUFFER_BIT);
    //Draw Box
    for (int i = 0; i < 5; ++i)
    {
        glPushMatrix();
        glColor3f(1.0f, 0.0f, 0.0f);
        //glTranslatef(obst[i].pos[0], obst[i].pos[1], 0.0f);
        glTranslatef(xres/2, yres/2, 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(-obst[i].w, -obst[i].h);
        glVertex2f(-obst[i].w, obst[i].h);
        glVertex2f( obst[i].w, obst[i].h);
        glVertex2f( obst[i].w, -obst[i].h);
        glEnd();
        glPopMatrix();
    }
}

/*void rangerRend()
{
    
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 1.0f);
    glTranslatef(ranger.pos[0], ranger.pos[1], 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-ranger.w + 20, -ranger.h + 20); //bottom left
    glVertex2f(-ranger.w + 20, ranger.h + 20);  //top left
    glVertex2f( ranger.w + 20, ranger.h + 20);       //top right
    glVertex2f( ranger.w + 20, -ranger.h + 20);      //bottom right
    glEnd();
    glPopMatrix();
    
    glClear(GL_COLOR_BUFFER_BIT);
    ranger.drawEnemR()
}*/

void physEnem()
{
    clock_gettime(CLOCK_REALTIME, &timeCurrent);
    double elapsedTime = timeDiff(&timeStart, &timeCurrent);
    timeCopy(&timeStart, &timeCurrent);

    EnemR ranger(20, 20, 0, 0, 100, 0);

    ranger.drawEnemR();
    ranger.updateEnemR(elapsedTime);

}
