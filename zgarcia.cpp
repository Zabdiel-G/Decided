//
//Author: Zabdiel Garcia
//
//Source code: zgarcia.cpp
//

#include <iostream>
#include <GL/glut.h>
#include "Obstacle.h"
#include <cmath>

//using namespace std;

void messageZ()
{
    std::cout << "Zabs Feature mode has been Entered" << std::endl;
}


void obstRend()
{

    //glClear(GL_COLOR_BUFFER_BIT);
    //Draw Box
    for (int i = 0; i < 5; ++i){
        glPushMatrix();
        glColor3ub(100, 200, 100);
        glTranslatef(obst[i].pos[0], obst[i].pos[1], 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(-obst[i].w, -obst[i].h);
        glVertex2f(-obst[i].w, obst[i].h);
        glVertex2f( obst[i].w, obst[i].h);
        glVertex2f( obst[i].w, -obst[i].h);
        glEnd();
        glPopMatrix();
    }

    float cx, cy, angle, radius;

    glPushMatrix();
    glColor3ub(200, 000, 000);
    glTranslatef(520, 10, 0);
    cx = 60;
    cy = 40;
    radius = 50;
    glBegin(GL_TRIANGLE_FAN);
    for(angle = 0; angle <= 360; ++angle){
        glVertex2f(cx + sin(angle)*radius, cy + cos(angle)*radius);
    }
    glEnd();
    glPopMatrix();
}


