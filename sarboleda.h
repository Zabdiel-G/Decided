//file name: sarboleda.h
#include <chrono>
#pragma once
typedef float Flt;
typedef float Vec[3];
typedef Flt Matrix[4][4];
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b) ((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
                        (c)[1]=(a)[1]-(b)[1]; \
                        (c)[2]=(a)[2]-(b)[2]
#define PI 3.141592653589793

using namespace std::chrono;


class Timer {
public:
    Timer();
    void reset();
    float deltaTime();
    float elapsedTime();
    system_clock::time_point _start_time, _current_time, _last_delta_tick;
    duration<float> _delta_time;
};

class Sword {
public:
    int damage;
    float width;
    float height;
    float pos[2];
    float color[3];
    struct timespec time;
    Sword(); 
};
class Player {
public:
    Vec pos;
    Vec dir;
    Vec vel;
    Vec acc;
    Flt rad;
    Flt xdir;
    Flt ydir;
    float maxSpeed;
    float speed;
    float angle;
    float color[3];
    bool canDodge;
    Player();
 };
