//
//Modified by: Zabdiel Garcia
//

#include <cstring>

#ifndef Obstacle_H
#define Obstacle_H

class Obstacle
{
    public:
    float w, h;
    float pos[2];
    float vel[2];
    unsigned char color[3];
    void set_color(unsigned char col[3]){
        memcpy(color, col, sizeof(unsigned char) *3);
    }
    Obstacle() {
        w = 50.0f;
        h = 15.0f;
        pos[0] = 0;
        pos[1] = 0;
        vel[0] = 0;
        vel[1] = 0;
    }
    Obstacle(int wid, int hgt, int x, int y, float v0, float v1) {
        w = wid;
        h = hgt;
        pos[0] = x;
        pos[1] = y;
        vel[0] = v0;
        vel[1] = v1;
    }
}obst[5];

#endif
