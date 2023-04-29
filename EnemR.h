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


class EnemR {
public:
    Vec pos;
    Vec vel;
    int nverts;
    Flt radius;
    Vec vert[8];
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
