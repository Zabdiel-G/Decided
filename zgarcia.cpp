//
//Author: Zabdiel Garcia
//
//Source code: zgarcia.cpp
//

#include <iostream>
#include <GL/glut.h>
#include "EnemR.h"
//#include "Global.h"
#include <unistd.h>
#include <cmath>

using namespace std::chrono;

extern struct timespec timeStart, timeCurrent;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);

#define PI 3.141592653589793
#define rnd() (((Flt)rand())/(Flt)RAND_MAX)

//enemImage img("./images/eyeball.png");

void messageZ()
{
    std::cout << "Zabs Feature mode has been Entered" << std::endl;
}

//Enemy Image
enemImage::~enemImage() { delete [] data; }

enemImage::enemImage(const char *fname) {
    if (fname[0] == '\0')
            return;
    int ppmFlag = 0;
        char name[40];
    strcpy(name, fname);
    int slen = strlen(name);
    char ppmname[80];
    if (strncmp(name+(slen-4), ".ppm", 4) == 0)
            ppmFlag = 1;
        if (ppmFlag) {
            strcpy(ppmname, name);
        } else {
            name[slen-4] = '\0';
            sprintf(ppmname,"%s.ppm", name);
        char ts[100];
        sprintf(ts, "convert %s %s", fname, ppmname);
        system(ts);
    }
    FILE *fpi = fopen(ppmname, "r");
    if (fpi) {
        char line[200];
        fgets(line, 200, fpi);
        fgets(line, 200, fpi);
        //skip comments and blank lines
        while (line[0] == '#' || strlen(line) < 2)
            fgets(line, 200, fpi);
        sscanf(line, "%i %i", &width, &height);
        fgets(line, 200, fpi);
        //get pixel data
        int n = width * height * 3;
        data = new unsigned char[n];
        for (int i=0; i<n; i++)
            data[i] = fgetc(fpi);
        fclose(fpi);
    } else {
        printf("ERROR opening image: %s\n",ppmname);
        exit(0);
    }
    if (!ppmFlag)
        unlink(ppmname);
}

//ENEMY FRAGMENTS...............................................................
void buildAsteroidFragment(EnemR *ta, EnemR *a)
{
    //build ta from a
    ta->nverts = 4;
    ta->radius = a->radius / 2.0;
    Flt r2 = ta->radius / 2.0;
    Flt angle = 0.0f;
    Flt inc = (PI * 2.0) / (Flt)ta->nverts;
    for (int i=0; i<ta->nverts; i++) {
        ta->vert[i][0] = sin(angle) * (r2 + rnd() * ta->radius);
        ta->vert[i][1] = cos(angle) * (r2 + rnd() * ta->radius);
        angle += inc;
    }
    ta->pos[0] = a->pos[0] + rnd()*10.0-5.0;
    ta->pos[1] = a->pos[1] + rnd()*10.0-5.0;
    ta->pos[2] = 0.0f;
    ta->angle = 0.0;
    ta->rotate = a->rotate + (rnd() * 4.0 - 2.0);
    ta->color[0] = 1.0;
    ta->color[1] = 0.0;
    ta->color[2] = 0.0;
    ta->vel[0] = a->vel[0] + (rnd()*2.0-1.0);
    ta->vel[1] = a->vel[1] + (rnd()*2.0-1.0);
    //std::cout << "frag" << std::endl;
}


//RENDER FOR THE RANGED ENEMY..................................................
void rendEnemR(EnemR* ahead, GLuint eyeballText)
{
    EnemR *a = ahead;
    while (a)
    {
        glColor3fv(a->color);
        glPushMatrix();
        glTranslatef(a->pos[0], a->pos[1], a->pos[2]);
        glRotatef(a->angle, 0.0f, 0.0f, 2.0f);
        glBindTexture(GL_TEXTURE_2D, eyeballText);
        glBegin(GL_TRIANGLE_FAN);
	    //glBegin(GL_QUADS);
        //Log("%i verts\n",a->nverts);

        //for (int j=0; j<a->nverts; j++)
        for (int j=0; j<a->nverts; j++)
        {
            glTexCoord2f(a->tcoord[j][0],a->tcoord[j][1]);
            glVertex2f(a->vert[j][0], a->vert[j][1]);
        }
        glEnd();

        //glBegin(GL_LINES);
        //glVertex2f(0,   0);
        //glVertex2f(a->radius, 0);
        //glEnd();
	
        glPopMatrix();
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_POINTS);
        glVertex2f(a->pos[0], a->pos[1]);
        glEnd();
        a = a->next;
    }
}

//RENDER FOR THE BULLET.........................................................
void rendBullet(int nbullets, Bullet* barr)
{
    for (int i=0; i <nbullets; i++) {
        Bullet *b = &barr[i];
        //Log("draw bullet...\n");
        glColor3fv(b->color);
        glBegin(GL_POINTS);
        glVertex2f(b->pos[0],      b->pos[1]);
        glVertex2f(b->pos[0]-1.0f, b->pos[1]);
        glVertex2f(b->pos[0]+1.0f, b->pos[1]);
        glVertex2f(b->pos[0],      b->pos[1]-1.0f);
        glVertex2f(b->pos[0],      b->pos[1]+1.0f);
        glColor3f(0.8, 0.8, 0.8);
        glVertex2f(b->pos[0]-1.0f, b->pos[1]-1.0f);
        glVertex2f(b->pos[0]-1.0f, b->pos[1]+1.0f);
        glVertex2f(b->pos[0]+1.0f, b->pos[1]-1.0f);
        glVertex2f(b->pos[0]+1.0f, b->pos[1]+1.0f);
        glEnd();
    }

}

