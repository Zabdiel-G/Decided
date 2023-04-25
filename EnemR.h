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


/*
class EnemR
{
    public:
    float w, h;
    float pos[2];
    float vel[2];
    unsigned char color[3];
    void set_color(unsigned char col[3]){
        memcpy(color, col, sizeof(unsigned char) *3);
    }
    EnemR() {
        w = 20.0f;
        h = 20.0f;
        pos[0] = 0;
        pos[1] = 0;
        vel[0] = 0;
        vel[1] = 0;
    }
    EnemR(int wid, int hgt, int x, int y, float v0, float v1) {
        w = wid;
        h = hgt;
        pos[0] = x;
        pos[1] = y;
        vel[0] = v0;
        vel[1] = v1;
    }

    void drawEnemR()
    {
        glPushMatrix();
        glColor3f(0.0f, 0.0f, 1.0f);
        glTranslatef(pos[0], pos[1], 0.0f);
        glBegin(GL_QUADS);
        glVertex2f(-w + 20, -h + 20); //bottom left
        glVertex2f(-w + 20,  h + 20); //top left
        glVertex2f( w + 20,  h + 20); //top right
        glVertex2f( w + 20, -h + 20); //bottom right
        glEnd();
        glPopMatrix();

    }

    void updateEnemR(float deltaTime)
    {
        pos[0] += vel[0] * deltaTime;
        if (pos[0] > 630) 
        {
            pos[0] = -630;
        }
        else if (pos[0] < -630)
        {
            pos[0] = 630;
        }
    }


}obst[5];
*/


class Asteroid {
public:
    Vec pos;
    Vec vel;
    int nverts;
    Flt radius;
    Vec vert[8];
    float angle;
    float rotate;
    float color[3];
    struct Asteroid *prev;
    struct Asteroid *next;
public:
    Asteroid() {
        prev = NULL;
        next = NULL;
    }
};
