//File name: sarboleda.cpp
//author: serafin arboleda
#include <cmath>
#include <iostream>
#include "sarboleda.h"
using namespace std;
using namespace std::chrono;
extern struct timespec timeStart, timeCurrent;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);

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
    // rad = ((angle+90)/360.0f) * PI * 2.0;
     maxSpeed = 1;
     speed = 0;
     VecZero(dir);
     VecZero(vel);
     VecZero(acc);
     angle = 0.0;
     color[0] = color[1] = color[2] = 1.0;
     canDodge = true;
}

Ability abilities[3] = {{2, 0}, {1, 0}, {3, 0}};

void updateAbilityCooldowns(Ability* abilityPtr, int numAbilities) 
{
    int x = 20000; 
    if (serafinFeatureMode) {
        x = 6250;
    }
    //double dummy = abilityPtr[0].timer; 
    clock_gettime(CLOCK_REALTIME, &timeCurrent);
    double elapsedTime = timeDiff(&timeStart, &timeCurrent);
    timeCopy(&timeStart, &timeCurrent);
    
    for (int i = 0; i < numAbilities; i++) {
        abilityPtr[i].timer -= abs(elapsedTime) * x;
        if (abilityPtr[i].timer < 0.0) {
            abilityPtr[i].timer = 0.0;
        }
    }
   
}
void useAbility(Ability& ability) 
{
    //double dummy = ability.timer;
    if (ability.timer == 0.0) {
        ability.timer = ability.cooldown;
      
    }   
    else {
        if (serafinFeatureMode) {
        double x = ability.cooldown - ability.timer;
        std::cout << "Cooldown: " << x << endl;
        }
    }
}
UpdateAbilityCooldownsFunc updateFuncPtr = &updateAbilityCooldowns;
UseAbilityFunc useFuncPtr = &useAbility;

