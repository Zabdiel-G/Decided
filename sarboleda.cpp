//File name: sarboleda.cpp
//author: serafin arboleda
#include <cmath>
#include <iostream>
#include "sarboleda.h"
#include "EnemR.h"
//#include "Global.h"
using namespace std;
using namespace std::chrono;
extern struct timespec timeStart, timeCurrent;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);

//taken from asteroids.cpp////////////
void normalized2d(Vec v)            
{                                   
    Flt len = v[0]*v[0] + v[1]*v[1];
    if (len == 0.0f) {              
        v[0] = 1.0;                
        v[1] = 0.0;
        return;
    }
    len = 1.0f / sqrt(len);
    v[0] *= len;
    v[1] *= len;
}
///////////////////////////////////
Sword::Sword() {
    damage = 1;
    width = 80.0f;
    height = 60.0f;
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
     dashSpeed = 50;
     VecZero(dir);
     VecZero(vel);
     VecZero(acc);
     angle = 0.0;
     color[0] = color[1] = color[2] = 1.0;
     canDodge = true;
     timestop = false;
}

//Ability Logic. Keeping track of cooldown and duration of every ability.//
Ability abilities[3] = {{2, 1, 0, 0}, {1, .5, 0, 0}, {20, 7, 0, 0}};

void updateAbilityCooldowns(Ability* abilityPtr, int numAbilities) 
{
    //x is a times constant to make one second more accurate to real time
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
    for (int i = 0; i < numAbilities; i++) {
        abilityPtr[i].durationTimer -= abs(elapsedTime) * x;
        if (abilityPtr[i].durationTimer < 0.0) {
            abilityPtr[i].durationTimer = 0.0;
        }
    }

   
}
void useAbility(Ability& ability) 
{
    //double dummy = ability.timer;
    if (ability.timer == 0.0) {
        ability.timer = ability.cooldown;
      
    }
     if (ability.durationTimer == 0.0) {
        ability.durationTimer = ability.duration;
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
//collisions
bool swordEnemyCollision(Sword sword, EnemR *enemy, bool slash) 
{
    //int hit = 0;
    if (sword.pos[0] - sword.width/2 <= enemy->pos[0] + enemy->radius &&
        sword.pos[0] + sword.width/2 >= enemy->pos[0] - enemy->radius &&
        sword.pos[1] + sword.height/2 >= enemy->pos[1] - enemy->radius &&
        sword.pos[1] - sword.height/2 <= enemy->pos[1] + enemy->radius &&
        slash == true)
        {
            //hit++;
            //cout <<hit << endl;
            //cout << "Hit detected" << endl;
            return true;
    }
    return false;
}


void playerWallCollision(Player& player, float xres, float yres)
{
    //negative x
    if (player.pos[0] < 0.0) {
        if (player.vel[0] < player.maxSpeed) {
            player.pos[0] = 0.0;
            player.vel[0] = -player.vel[0]*0.01;
        }
        else {
            player.pos[0] = 0.0;
            player.vel[0] = -player.vel[0]*0.00005;
        }
    }
    //under y
    else if (player.pos[1] < 0.0) {
        player.pos[1] = 0.0;
        player.vel[1] = -player.vel[1]*0.0005;
    }
    //positve x
    else if (player.pos[0] > (float)xres) {
        player.pos[0] = (float)xres;
        player.vel[0] = -player.vel[0]*0.00005;
    }
    //above y
    else if (player.pos[1] > (float)yres) {
        player.pos[1] = (float)yres;
        player.vel[1] = player.vel[1]*0.00005;
    }
}


//Movement of the player
void moveLeft(Player& player, float xdir, float MAX_SPEED) 
{
    player.angle = 90;
    //player.vel[1] = 0;
    player.vel[0] += xdir*0.50f;
    float speed = player.vel[0];
    if (speed < -MAX_SPEED) {
        speed = -MAX_SPEED;
        normalized2d(player.vel);
        player.vel[0] *= abs(speed);
    }
}

void moveRight(Player& player, float xdir, float MAX_SPEED) 
{
    player.angle = 270;
    //player.vel[1] = 0;
    player.vel[0] += xdir*0.50f;
    float speed = player.vel[0]*0.50;
    if (speed > MAX_SPEED) {
        speed = MAX_SPEED;
        normalized2d(player.vel);
        player.vel[0] *= abs(speed);
    }
}

void moveUp(Player& player, float ydir, float MAX_SPEED) 
{
    player.angle = 0;
    //player.vel[0] = 0;
    player.vel[1] += ydir*0.50f;
    float speed = player.vel[1]*0.50;
    if (speed > MAX_SPEED * 1.5) {
        speed = MAX_SPEED * 1.5;
        normalized2d(player.vel);
        player.vel[1] *= abs(speed);
    }
}

void moveDown(Player& player, float ydir, float MAX_SPEED) 
{
    player.angle = 180;
    //player.vel[0] = 0;
    player.vel[1] += ydir*0.50f;
    float speed = player.vel[1]*0.50;
    if (speed < -MAX_SPEED * 1.5) {
        speed = -MAX_SPEED * 1.5;
        normalized2d(player.vel);
        player.vel[1] *= abs(speed);
    }
}
void moveLeftUp(Player& player, float xdir, float ydir, float MAX_SPEED) 
{
    player.angle = 45;
    player.vel[0] += xdir*0.50f;
    player.vel[1] += ydir*0.50f;
    normalized2d(player.vel);
    float speed = sqrt(pow(player.vel[0], 2) + pow(player.vel[1], 2));
    if (speed > MAX_SPEED) {
        speed = MAX_SPEED;
        normalized2d(player.vel);
        player.vel[0] *= abs(speed);
        player.vel[1] *= abs(speed);
    }
}

void moveLeftDown(Player& player, float xdir, float ydir, float MAX_SPEED) 
{
    player.angle = 135;
    player.vel[0] += xdir*0.50f;
    player.vel[1] += ydir*0.50f;
    normalized2d(player.vel);
    float speed = sqrt(pow(player.vel[0], 2) + pow(player.vel[1], 2));
    if (speed > MAX_SPEED) {
        speed = MAX_SPEED;
        normalized2d(player.vel);
        player.vel[0] *= abs(speed);
        player.vel[1] *= abs(speed);
    }
}

void moveRightUp(Player& player, float xdir, float ydir, float MAX_SPEED) 
{
    player.angle = 315;
    player.vel[0] += xdir*0.50f;
    player.vel[1] += ydir*0.50f;
    normalized2d(player.vel);
    float speed = sqrt(pow(player.vel[0], 2) + pow(player.vel[1], 2));
    if (speed > MAX_SPEED) {
        speed = MAX_SPEED;
        normalized2d(player.vel);
        player.vel[0] *= abs(speed);
        player.vel[1] *= abs(speed);
    }
}

void moveRightDown(Player& player, float xdir, float ydir, float MAX_SPEED) 
{
    player.angle = 225;
    player.vel[0] += xdir*0.50f;
    player.vel[1] += ydir*0.50f;
    normalized2d(player.vel);
    float speed = sqrt(pow(player.vel[0], 2) + pow(player.vel[1], 2));
    if (speed > MAX_SPEED) {
        speed = MAX_SPEED;
        normalized2d(player.vel);
        player.vel[0] *= abs(speed);
        player.vel[1] *= abs(speed);
    }
}

void dashLeft(Player& player) 
{
    player.angle = 270;
    player.vel[1] = 0;
    player.vel[0] = -player.dashSpeed;

}

void dashRight(Player& player) 
{
    player.angle = 90;
    player.vel[1] = 0;
    player.vel[0] = player.dashSpeed;

}

void dashUp(Player& player) 
{
    player.angle = 0;
    player.vel[1] = player.dashSpeed;
    player.vel[0] = 0;

}

void dashDown(Player& player) 
{
    player.angle = 180;
    player.vel[1] = -player.dashSpeed;
    player.vel[0] = 0;


}

void dashLeftUp(Player& player) 
{

    player.angle = 45;
    player.vel[1] = player.dashSpeed;
    player.vel[0] = -player.dashSpeed;    
}

void dashLeftDown(Player& player) 
{
    player.angle = 135;
    player.vel[1] = -player.dashSpeed;
    player.vel[0] = -player.dashSpeed;

}

void dashRightUp(Player& player) 
{
    player.angle = 315;
    player.vel[1] = player.dashSpeed;
    player.vel[0] = player.dashSpeed;

}

void dashRightDown(Player& player) 
{
    player.angle = 225;
    player.vel[1] = -player.dashSpeed;
    player.vel[0] = player.dashSpeed;

}
