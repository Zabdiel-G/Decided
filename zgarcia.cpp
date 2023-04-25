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

void rendEnemR(Asteroid* ahead)
{
    Asteroid *a = ahead;
    while (a)
    {
        //Log("draw asteroid...\n");
        //glColor3fv(a->color);
        glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
        glTranslatef(a->pos[0], a->pos[1], a->pos[2]);
        glRotatef(a->angle, 0.0f, 0.0f, 1.0f);
        //glBegin(GL_LINE_LOOP);
	glBegin(GL_QUADS);
        //Log("%i verts\n",a->nverts);
        for (int j=0; j<a->nverts; j++)
        {
            glVertex2f(a->vert[j][0], a->vert[j][1]);
        }
        glEnd();

        //glBegin(GL_LINES);
        //glVertex2f(0,   0);
        //glVertex2f(a->radius, 0);
        //glEnd();
	
        glPopMatrix();
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_POINTS);
        glVertex2f(a->pos[0], a->pos[1]);
        glEnd();
        a = a->next;
    }
}
