//
//Modified by: Zabdiel Garcia
//

#include <cstring>
#include <chrono>
#include <iostream>
#include <GL/glut.h>
#include <unistd.h>

#pragma once

typedef float Flt;
typedef float Vec[3];
typedef Flt Matrix[4][4];

using namespace std::chrono;


class EnemR 
{
    public:
        Vec pos;
        Vec vel;
        int nverts;
        Flt radius;
        Vec vert[8];
        Vec tcoord[8];
        float angle;
        float rotate;
        float color[3];
        struct EnemR *prev;
        struct EnemR *next;
    public:
        EnemR() {
            prev = NULL;
            next = NULL;
        }
};

class Bullet 
{
    public:
        Vec pos;
        Vec vel;
        float color[3];
        struct timespec time;
    public:
        Bullet() { }
};

class enemImage 
{
    public:
        int width, height;
        unsigned char *data;
        ~enemImage();
        enemImage (const char *fname);
};

