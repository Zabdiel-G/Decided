//
//Author: Zabdiel Garcia
//
//Source code: zgarcia.cpp
//

#include <iostream>
#include <GL/glut.h>
#include "EnemR.h"
#include <cmath>

using namespace std::chrono;

extern struct timespec timeStart, timeCurrent;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);


//using namespace std;

void messageZ()
{
    std::cout << "Zabs Feature mode has been Entered" << std::endl;
}

/*
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
*/


void rendEnemR(Asteroid* ahead)
{
    Asteroid *a = ahead;
    while (a)
    {
        //Log("draw asteroid...\n");
        glColor3fv(a->color);
        glPushMatrix();
        glTranslatef(a->pos[0], a->pos[1], a->pos[2]);
        glRotatef(a->angle, 0.0f, 0.0f, 1.0f);
        glBegin(GL_LINE_LOOP);
        //Log("%i verts\n",a->nverts);
        for (int j=0; j<a->nverts; j++)
        {
            glVertex2f(a->vert[j][0], a->vert[j][1]);
        }
        glEnd();
        //glBegin(GL_LINES);
        //  glVertex2f(0,   0);
        //  glVertex2f(a->radius, 0);
        //glEnd();
        glPopMatrix();
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_POINTS);
        glVertex2f(a->pos[0], a->pos[1]);
        glEnd();
        a = a->next;
    }
}
