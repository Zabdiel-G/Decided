//File name: sarboleda.cpp
//author: serafin arboleda
#include <iostream>
using namespace std;

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
