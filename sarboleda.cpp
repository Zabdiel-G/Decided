//File name: sarboleda.cpp
//author: serafin arboleda
#include <cmath>
#include <iostream>
#include "sarboleda.h"
#include <GL/glut.h>
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
//enemy tracking
void trackEnemyMovement(Player player, EnemR*& enemy, float xres, float yres) 
{
     EnemR *a = enemy;

     while (a) {
        a->pos[0] += a->vel[0];
        a->pos[1] += a->vel[1];
        //Check for collision with window edges
        if (a->pos[0] < 0) {
            a->pos[0] = 0;
            a->vel[0] = -a->vel[0];
        }
        else if (a->pos[0] > xres) {
            a->vel[0] = -a->vel[0];
            a->pos[0] = xres;
        }
        else if (a->pos[1] < 0) {
            a->pos[1] = 0;
            a->vel[1] = -a->vel[1];
        }
        else if (a->pos[1] > yres) {
            a->pos[1] = yres;
            a->vel[1] = -a->vel[1];
        }

        float diffX = a->pos[0] - player.pos[0];
        float diffY = a->pos[1] - player.pos[1];
        float angleE = (atan2(diffY, diffX)*180.0f)/PI;
        float dist = sqrt(diffX*diffX + diffY*diffY);

        if (dist < (50)) {
        float angleD = atan2(diffY,diffX)*180.0f/PI;

            a->vel[0] += cos(angleD)/4;
            a->vel[1] += sin(angleD)/4;
        }
        float max_speed = 2.0f;
        float speed = sqrt(a->vel[0]*a->vel[0] + a->vel[1]*a->vel[1]);
        if (speed > max_speed) {
        a->vel[0] = a->vel[0] / speed * max_speed;
        a->vel[1] = a->vel[1] / speed * max_speed;
        }

        //angle
        if (a->angle < angleE) {
            a->angle += 20;
        }
        if (a->angle > angleE) {
            a->angle -= 20;
        }
        a = a->next;

    }

}
bool directoryExists(const char* dirPath) 
{
    return access(dirPath, F_OK) != -1;
}
void makeSaveFile(int q, Player player, EnemR *enemy) 
{
    //if the Save file directory does not exist, then make it
    string directoryName = "SaveFile";
    if (!directoryExists("SaveFile")){
        system("mkdir SaveFile");
        system("echo > SaveFile/loadfile4.txt");
        ofstream fout("SaveFile/loadfile4.txt"); 
        if (fout.is_open()) {
            fout << 0 << " ";
            fout << 0 << " ";
            fout << 0 << " ";
            fout << 0 << " ";
            fout << 1 << " ";
            fout << 20 << " ";
            fout << 20 << " ";
            fout << 0 << " ";
            fout << 0 << " ";
        }
        fout.close();

    }
    //q = q+1;
    //cout << "succesfully made saveFile" << endl;
    char fileName[50];
    char command[100];
    //make the .txt file in the SaveFile directory
    sprintf(fileName, "SaveFile/loadfile%d.txt", q); 
    sprintf(command, "echo > %s", fileName);
    system(command);
    ofstream out(fileName);
    
    if (out.is_open()) {
        //player informationtonWidth;
        out << player.pos[0] << " "; 
        out << player.pos[1] << " ";
        out << player.pos[2] << " ";
        out << player.angle << " ";
        int enemyCount = 0;
        //enemy information
        EnemR *e = enemy;
        while (e != NULL) {
            enemyCount++;
            e = e->next;
        }
        out << enemyCount << " ";
        e = enemy;
        while (e != NULL) {
            out << e->pos[0] << " ";
            out << e->pos[1] << " ";
            out << e->pos[2] << " ";
            out << e->angle << " ";
            e = e->next; 
        }
    }
    out.close();


}
int countSaveFile() 
{

    int fileNum = 0;
    char line[100];
    system("touch SaveFileMeta.txt");
    if (!directoryExists("SaveFile")) {
        return 0;
    }
    const char saveMeta[] = "SaveFileMeta.txt";
    ifstream fin(saveMeta);
    system("cd SaveFile;ls -1 > ../SaveFileMeta.txt");
    while (!fin.eof()) {
        //cout << "fileNum" << fileNum << endl;
        fileNum++;
        fin >> line;
    }
    system("rm SaveFileMeta.txt");
    return fileNum;


}
void loadSaveFile(int fileNumber, Player& player, EnemR*& enemy) 
{   
    char fileName[50];
    //make the .txt file in the SaveFile directory
    sprintf(fileName, "SaveFile/loadfile%d.txt", fileNumber);
    ifstream fin(fileName);
    
    fin >> player.pos[0];
    fin >> player.pos[1];
    fin >> player.pos[2];
    fin >> player.angle;

    int numberOfEnemies = 0;
    fin >> numberOfEnemies;
    //cout << "numberOfEnemies"<< numberOfEnemies << endl;
    EnemR* currentEnemy = enemy;
    EnemR* lastEnemy = nullptr;
    for (int i = 0; i < numberOfEnemies; i++) {
        fin >> currentEnemy->pos[0];
        fin >> currentEnemy->pos[1];
        fin >> currentEnemy->pos[2];
        fin >> currentEnemy->angle;
        // move to the next enemy in the linked list
        lastEnemy = currentEnemy;
        currentEnemy = currentEnemy->next;
    }
    if (lastEnemy != nullptr) {
        lastEnemy->next = nullptr;
    }
}
void pauseMenu(int i, float xres, float yres, int flag) 
{
    //i = i-1;
    float boxWidth = xres / 2.0f;
    float boxHeight = yres / 2.0f;
    float x = (xres - boxWidth) / 2.0f;
    float y = (yres - boxHeight - 30.0f) / 2.0f;
    //Draw the box
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(x, y);
    glVertex2f(x + boxWidth, y);
    glVertex2f(x + boxWidth, y + boxHeight);
    glVertex2f(x, y + boxHeight);
    glEnd();
    
    float buttonY[3];
    float buttonWidth = boxWidth - 20.0f;
    float buttonHeight = (boxHeight * 0.2f);
    float buttonX = ((xres - boxWidth)/2.0f) - 200;
    for (int j = 0; j < 4; j++) {
    buttonY[j] = y + boxHeight - (boxHeight/2.5) - j * (buttonHeight + 10.0f);
    }
    //Draw the buttons
    

    glBegin(GL_QUADS);
    for(int k = 0; k < 4; k++) {
        glColor3f(1,1,1);
        glVertex2f(buttonX, buttonY[k]);
        glVertex2f(buttonX + buttonWidth, buttonY[k]);
        glVertex2f(buttonX + buttonWidth, buttonY[k] + buttonHeight);
        glVertex2f(buttonX, buttonY[k] + buttonHeight);
    }
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f); 
    glVertex2f(buttonX - 2.0f, buttonY[i] - 2.0f);
    glVertex2f(buttonX + buttonWidth + 2.0f, buttonY[i] - 2.0f);
    glVertex2f(buttonX + buttonWidth + 2.0f, buttonY[i] + buttonHeight + 2.0f);
    glVertex2f(buttonX - 2.0f, buttonY[i] + buttonHeight + 2.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(buttonX, buttonY[i]); 
    glVertex2f(buttonX + buttonWidth, buttonY[i]);
    glVertex2f(buttonX + buttonWidth, buttonY[i] + buttonHeight); 
    glVertex2f(buttonX, buttonY[i] + buttonHeight);
    glEnd();

    Rect r;
    r.bot = y + boxHeight;
    r.left = x + boxWidth/2;
    r.center = 0;
    ggprint8b(&r, 100, 0x00ffff44, "PAUSED"); 
    const char* buttonText[4] = {"continue", "save", "load", "quit"};
    switch (flag) {
        case 0:
            buttonText[0] = "Continue";
            buttonText[1] = "Save";
            buttonText[2] = "Load";
            buttonText[3] = "Quit";
            break;
        case 1: 
            buttonText[0] = "Back";
            buttonText[1] = "Save 1";
            buttonText[2] = "Save 2";
            buttonText[3] = "Cotinue";
            break;
        case 2:
            buttonText[0] = "Back";
            buttonText[1] = "Load 1";
            buttonText[2] = "Load 2";
            buttonText[3] = "Continue";

            break; 

    }
    for(int k = 0; k < 4; k++) {
        glColor3f(1,1,1);
        glVertex2f(buttonX, buttonY[k]);
        glVertex2f(buttonX + buttonWidth, buttonY[k]);
        glVertex2f(buttonX + buttonWidth, buttonY[k] + buttonHeight);
        glVertex2f(buttonX, buttonY[k] + buttonHeight);
        Rect r;
        r.bot = buttonY[k] + buttonHeight/2.0f - 8.0f;
        r.left = buttonX + buttonWidth/2.0f;
        r.center = 1;
        ggprint8b(&r, 0, 0x000000ff, buttonText[k]);
    }
}

void loadMenu(int i, float xres, float yres)
{
    //i = i-1;
    float boxWidth = xres / 2.0f;
    float boxHeight = yres / 2.0f;
    float x = (xres - boxWidth) / 2.0f;
    float y = (yres - boxHeight - 30.0f) / 2.0f;
    //Draw the box
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(x, y);
    glVertex2f(x + boxWidth, y);
    glVertex2f(x + boxWidth, y + boxHeight);
    glVertex2f(x, y + boxHeight);
    glEnd();

    float buttonY[3];
    float buttonWidth = boxWidth - 20.0f;
    float buttonHeight = (boxHeight * 0.2f);
    float buttonX = x + (boxWidth - buttonWidth)/2.0f;
    for (int j = 0; j < 4; j++) {
    buttonY[j] = y + boxHeight - (boxHeight/2.5) - j * (buttonHeight + 10.0f);
    }
    //Draw the buttons


    glBegin(GL_QUADS);
    for(int k = 0; k < 4; k++) {
        glColor3f(1,1,1);
        glVertex2f(buttonX, buttonY[k]);
        glVertex2f(buttonX + buttonWidth, buttonY[k]);
        glVertex2f(buttonX + buttonWidth, buttonY[k] + buttonHeight);
        glVertex2f(buttonX, buttonY[k] + buttonHeight);
    }
    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(buttonX - 2.0f, buttonY[i] - 2.0f);
    glVertex2f(buttonX + buttonWidth + 2.0f, buttonY[i] - 2.0f);
    glVertex2f(buttonX + buttonWidth + 2.0f, buttonY[i] + buttonHeight + 2.0f);
    glVertex2f(buttonX - 2.0f, buttonY[i] + buttonHeight + 2.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(buttonX, buttonY[i]);
    glVertex2f(buttonX + buttonWidth, buttonY[i]);
    glVertex2f(buttonX + buttonWidth, buttonY[i] + buttonHeight);
    glVertex2f(buttonX, buttonY[i] + buttonHeight);
    glEnd();

    Rect r;
    r.bot = y + boxHeight;
    r.left = x + boxWidth/2;
    r.center = 0;
    ggprint8b(&r, 100, 0x00ffff44, "Choose Save File");
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
    if (sword.pos[0] - sword.width/2 <= enemy->pos[0] + enemy->radius &&
        sword.pos[0] + sword.width/2 >= enemy->pos[0] - enemy->radius &&
        sword.pos[1] + sword.height/2 >= enemy->pos[1] - enemy->radius &&
        sword.pos[1] - sword.height/2 <= enemy->pos[1] + enemy->radius &&
        slash == true)
        {
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
//to do
//enemy to wall collision

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
