//File name: sarboleda.cpp
//author: serafin arboleda
#include <cmath>
#include <iostream>
#include "sarboleda.h"
using namespace std;
using namespace std::chrono;

Timer::Timer() {
    _start_time = _current_time = _last_delta_tick = system_clock::now();
    _delta_time = duration<float>(0);
}
void Timer::reset() {
    _start_time = system_clock::now();
}
float Timer::deltaTime() {
    _current_time = system_clock::now();
    _delta_time = _current_time - _last_delta_tick;
    _last_delta_tick = _current_time;
    return _delta_time.count();
}
float Timer::elapsedTime() {
    _current_time = system_clock::now();
    duration<float> elapse_time = _current_time - _start_time;
    return elapse_time.count();
}

Sword::Sword() {
    damage = 1;
    width = 40.0f;
    height = 20.0f;
    pos[0] = 0.0f;
    pos[1] = 0.0f;
    color[0] = 100.0f;
    color[1] = 100.0f;
    color[2] = 100.0f;
    
}

Player::Player() {
     pos[0] = 0.0f;
     pos[1] = 0.0f;
     pos[2] = 0.0f;
     rad = ((angle+90)/360.0f) * PI * 2.0;
     maxSpeed = 1;
     speed = 0;
     VecZero(dir);
     VecZero(vel);
     VecZero(acc);
     angle = 0.0;
     color[0] = color[1] = color[2] = 1.0;
     canDodge = true;
}


float dashSpeed = 20;
void messageK(){

    cout << "The letter K has been pressed" << endl;
}

/*void dodgeLeft(float angle, float velx, float vely) {
    angle = 90;
    velx = -20;
    vely = 0;
}*/

void dodgeRight(float d[3]) {
  d[0] = 270;
  d[1] = dashSpeed;
  d[2] = 0;
}

/*void dodgeUp(float angle, float velx, float vely) {
    angle = 0;
    velx = 0;
    vely = dashSpeed;
}

void dodgeDown(float angle, float velx, float vely) {
    angle = 180;
    velx = 0;
    vely = -dashSpeed;
}*/
