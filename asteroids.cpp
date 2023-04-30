//
//program: asteroids.cpp
//author:  Gordon Griesel
//date:    2014 - 2021
//mod spring 2015: added constructors
//This program is a game starting point for a 3350 project.
//
//modified: Serafin, Marc, Paramveer, Zabdiel Garcia
//

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"
#include <time.h>
#include <chrono>
#include "sarboleda.h"
#include "EnemR.h"
#include "Global.h"
#define MAX_ASTEROIDS 10


//defined types
typedef float Flt;
typedef float Vec[3];
typedef Flt	Matrix[4][4];

//macros
#define rnd() (((Flt)rand())/(Flt)RAND_MAX)
#define random(a) (rand()%a)
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
						(c)[1]=(a)[1]-(b)[1]; \
						(c)[2]=(a)[2]-(b)[2]
//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define PI 3.141592653589793
#define ALPHA 1
const int MAX_BULLETS = 11;
const Flt MINIMUM_ASTEROID_SIZE = 15.0;

//-----------------------------------------------------------------------------
//Setup timers
const double dodgeCooldown = 2.0;
const double physicsRate = 1.0 / 60.0;
const double oobillion = 1.0 / 1e9;
extern struct timespec timeStart, timeCurrent;
extern struct timespec timePause;
extern double physicsCountdown;
extern double timeSpan;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);
//-----------------------------------------------------------------------------
//member's function
extern void messageZ();
extern void messageK();
extern void messageFire();
extern void messageF();
extern void rendEnemR(EnemR*);
extern void rendBullet(int, Bullet* barr);
extern void buildAsteroidFragment(EnemR *ta, EnemR *a);


using namespace std::chrono;
Sword sword;
Player player;
extern struct Ability abilities[3];
bool serafinFeatureMode = false;


class Ship {
public:
	Vec pos;
	Vec dir;
	Vec vel;
	Vec acc;
	float angle;
	float color[3];
    bool canDodge;
   
   // bool swordSlash;
public:
	Ship() {
		pos[0] = (Flt)(gl.xres/2);
		pos[1] = (Flt)(gl.yres/2);
		pos[2] = 0.0f;
		VecZero(dir);
		VecZero(vel);
		VecZero(acc);
		angle = 0.0;
		color[0] = color[1] = color[2] = 1.0;
        canDodge = true;
       
	}
};

class Game {
public:
	Ship ship;
    Player player;
	EnemR *ahead;
	Bullet *barr;
	EnemR *aarr;
    int width;
    int height;
	int nasteroids;
	int nbullets;
	struct timespec bulletTimer;
    struct timespec dodgeTimer;
	struct timespec mouseThrustTimer;
    struct timespec slashTimer;
	bool mouseThrustOn;
public:
	Game() {
		ahead = NULL;
		barr = new Bullet[MAX_BULLETS];
		nasteroids = 0;
		nbullets = 0;
		mouseThrustOn = false;
		//build 10 asteroids...
		for (int j=0; j<10; j++) {
			EnemR *a = new EnemR;
			a->nverts = 4;
			//a->radius = rnd()*80.0 + 40.0;
			a->radius = 20.0;
			Flt r2 = a->radius / 2.0;
			Flt angle = 0.0f;
			Flt inc = (PI * 2.0) / (Flt)a->nverts;
			for (int i=0; i<a->nverts; i++) {
				//a->vert[i][0] = sin(angle) * (r2 + rnd() * a->radius);
				//a->vert[i][1] = cos(angle) * (r2 + rnd() * a->radius);
				a->vert[i][0] = sin(angle) * r2 * 2;
				a->vert[i][1] = cos(angle) * r2 * 2;
				angle += inc;
			}
			a->pos[0] = (Flt)(rand() % gl.xres);
			a->pos[1] = (Flt)(rand() % gl.yres);
			//a->pos[0] = (Flt)(gl.xres);
			//a->pos[1] = (Flt)(gl.yres);
			a->pos[2] = 0.0f;
			a->angle = 0.0;
			a->rotate = rnd() * 4.0 - 2.0;
			a->color[0] = 0.0;
			a->color[1] = 1.0;
			a->color[2] = 0.0;
			a->vel[0] = (Flt)(rnd()*2.0-1.0);
			a->vel[1] = (Flt)(rnd()*2.0-1.0);
			//add to front of linked list
			a->next = ahead;
			if (ahead != NULL)
            {
				ahead->prev = a;
            }
			ahead = a;
			++nasteroids;
		}
        
	    clock_gettime(CLOCK_REALTIME, &bulletTimer);
        clock_gettime(CLOCK_REALTIME, &dodgeTimer);
        clock_gettime(CLOCK_REALTIME, &slashTimer);

	}
	~Game() {
		delete [] barr;
	}
} g;


//X Windows variables
class X11_wrapper {
private:
	Display *dpy;
	Window win;
	GLXContext glc;
public:
	X11_wrapper() { }
	X11_wrapper(int w, int h) {
		GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
		//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
		XSetWindowAttributes swa;
		setup_screen_res(gl.xres, gl.yres);
		dpy = XOpenDisplay(NULL);
		if (dpy == NULL) {
			std::cout << "\n\tcannot connect to X server" << std::endl;
			exit(EXIT_FAILURE);
		}
		Window root = DefaultRootWindow(dpy);
		XWindowAttributes getWinAttr;
		XGetWindowAttributes(dpy, root, &getWinAttr);
		int fullscreen = 0;
		gl.xres = w;
		gl.yres = h;
		if (!w && !h) {
			//Go to fullscreen.
			gl.xres = getWinAttr.width;
			gl.yres = getWinAttr.height;
			//When window is fullscreen, there is no client window
			//so keystrokes are linked to the root window.
			XGrabKeyboard(dpy, root, False,
				GrabModeAsync, GrabModeAsync, CurrentTime);
			fullscreen=1;
		}
		XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
		if (vi == NULL) {
			std::cout << "\n\tno appropriate visual found\n" << std::endl;
			exit(EXIT_FAILURE);
		} 
		Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
		swa.colormap = cmap;
		swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
			PointerMotionMask | MotionNotify | ButtonPress | ButtonRelease |
			StructureNotifyMask | SubstructureNotifyMask;
		unsigned int winops = CWBorderPixel|CWColormap|CWEventMask;
		if (fullscreen) {
			winops |= CWOverrideRedirect;
			swa.override_redirect = True;
		}
		win = XCreateWindow(dpy, root, 0, 0, gl.xres, gl.yres, 0,
			vi->depth, InputOutput, vi->visual, winops, &swa);
		//win = XCreateWindow(dpy, root, 0, 0, gl.xres, gl.yres, 0,
		//vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
		set_title();
		glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
		glXMakeCurrent(dpy, win, glc);
		show_mouse_cursor(0);
	}
	~X11_wrapper() {
		XDestroyWindow(dpy, win);
		XCloseDisplay(dpy);
	}
	void set_title() {
		//Set the window title bar.
		XMapWindow(dpy, win);
		XStoreName(dpy, win, "Asteroids template");
	}
	void check_resize(XEvent *e) {
		//The ConfigureNotify is sent by the
		//server if the window is resized.
		if (e->type != ConfigureNotify)
			return;
		XConfigureEvent xce = e->xconfigure;
		if (xce.width != gl.xres || xce.height != gl.yres) {
			//Window size did change.
			reshape_window(xce.width, xce.height);
		}
	}
	void reshape_window(int width, int height) {
		//window has been resized.
		setup_screen_res(width, height);
		glViewport(0, 0, (GLint)width, (GLint)height);
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
		set_title();
	}
	void setup_screen_res(const int w, const int h) {
		gl.xres = w;
		gl.yres = h;
	}
	void swapBuffers() {
		glXSwapBuffers(dpy, win);
	}
	bool getXPending() {
		return XPending(dpy);
	}
	XEvent getXNextEvent() {
		XEvent e;
		XNextEvent(dpy, &e);
		return e;
	}
	void set_mouse_position(int x, int y) {
		XWarpPointer(dpy, None, win, 0, 0, 0, 0, x, y);
	}
	void show_mouse_cursor(const int onoff) {
		if (onoff) {
			//this removes our own blank cursor.
			XUndefineCursor(dpy, win);
			return;
		}
		//vars to make blank cursor
		Pixmap blank;
		XColor dummy;
		char data[1] = {0};
		Cursor cursor;
		//make a blank cursor
		blank = XCreateBitmapFromData (dpy, win, data, 1, 1);
		if (blank == None)
			std::cout << "error: out of memory." << std::endl;
		cursor = XCreatePixmapCursor(dpy, blank, blank, &dummy, &dummy, 0, 0);
		XFreePixmap(dpy, blank);
		//this makes the cursor. then set it using this function
		XDefineCursor(dpy, win, cursor);
		//after you do not need the cursor anymore use this function.
		//it will undo the last change done by XDefineCursor
		//(thus do only use ONCE XDefineCursor and then XUndefineCursor):
	}
} x11(gl.xres, gl.yres);
// ---> for fullscreen x11(0, 0);

/*bool canDodge() {
    struct timespec currentTime;
    clock_gettime(CLOCK_REALTIME, &currentTime); // get current system time

    double timeSinceLastDodge = (currentTime.tv_sec - lastDodgeTime) + (double)(currentTime.tv_nsec - lastDodgeTime) / 1000000000.0; // calculate time since last dodge in seconds

    return timeSinceLastDodge >= 2.0; // return true if enough time has passed since last dodge
}*/

//function prototypes

void init_opengl(void);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics();
void render();
void playerPhysics();
//bool canDodge();
void dodgeCdTracker();
//extern void updateAbilityCooldowns(*Ability, int);
//extern void useAbility(&Ability);

//==========================================================================
// M A I N
//==========================================================================


int main()
{
    g.player.pos[0] = (Flt)(gl.xres/2);
    g.player.pos[1] = (Flt)(gl.yres/2);
    g.player.pos[2] = 0.0f;
    //extern bool startMenu();
    //if (startMenu()) {
	logOpen();
	init_opengl();
	srand(time(NULL));
	clock_gettime(CLOCK_REALTIME, &timePause);
	clock_gettime(CLOCK_REALTIME, &timeStart);
	x11.set_mouse_position(100,100);
    int done=0;
	while (!done) {
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.check_resize(&e);
			check_mouse(&e);
			done = check_keys(&e);
		}
		clock_gettime(CLOCK_REALTIME, &timeCurrent);
		timeSpan = timeDiff(&timeStart, &timeCurrent);
		timeCopy(&timeStart, &timeCurrent);
		physicsCountdown += timeSpan;
		while (physicsCountdown >= physicsRate) {
            //std::cout << count << std::endl;
            //count++;
            updateAbilityCooldowns(abilities, 3);
            playerPhysics();
			physics();
			physicsCountdown -= physicsRate;
		}
		render();
        if(gl.obstR){
            rendEnemR(g.ahead);
            rendBullet(g.nasteroids, g.barr);

        }
		x11.swapBuffers();
	}
	cleanup_fonts();
	logClose();
    //}
	return 0;
}

void init_opengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, gl.xres, gl.yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
	//
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//
	//Clear the screen to black
	glClearColor(0.0, 0.0, 0.0, 1.0);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();
}


void createNewAsteroid(Game *g) //new mode param
{
    if (g->nasteroids < MAX_ASTEROIDS) {
        EnemR *a = &g->aarr[g->nasteroids];
        a->pos[0] = rnd()*g->width;
        a->pos[1] = rnd()*g->height;
        a->vel[0] = rnd()*2.0-1.0;
        a->vel[1] = rnd()*2.0-1.0;
        a->color[0] = 0.8f;
        a->color[1] = 0.8f;
        a->color[2] = 0.8f;
        a->color[3] = 1.8f;
        //a->radius = rnd()*80.0 + 40.0;
        a->radius = 20.0;
        ++g->nasteroids;
    }
}


void normalize2d(Vec v)
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

void check_mouse(XEvent *e)
{
	//Did the mouse move?
	//Was a mouse button clicked?
	static int savex = 0;
	static int savey = 0;
	//
	//static int ct=0;
	//std::cout << "m" << std::endl << std::flush;
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		/*if (e->xbutton.button==1) {
			//Left button is down
			//a little time between each bullet
			struct timespec bt;
			clock_gettime(CLOCK_REALTIME, &bt);
			double ts = timeDiff(&g.bulletTimer, &bt);
			if (ts > 0.1) {
				timeCopy(&g.bulletTimer, &bt);
				//shoot a bullet...
				if (g.nbullets < MAX_BULLETS) {
					Bullet *b = &g.barr[g.nbullets];
					timeCopy(&b->time, &bt);
					b->pos[0] = g.ship.pos[0];
					b->pos[1] = g.ship.pos[1];
					b->vel[0] = g.ship.vel[0];
					b->vel[1] = g.ship.vel[1];
					//convert ship angle to radians
					Flt rad = ((g.ship.angle+90.0) / 360.0f) * PI * 2.0;
					//convert angle to a vector
					Flt xdir = cos(rad);
					Flt ydir = sin(rad);
					b->pos[0] += xdir*20.0f;
					b->pos[1] += ydir*20.0f;
					b->vel[0] += xdir*6.0f + rnd()*0.1;
					b->vel[1] += ydir*6.0f + rnd()*0.1;
					b->color[0] = 1.0f;
					b->color[1] = 1.0f;
					b->color[2] = 1.0f;
					++g.nbullets;
				}
			}
		}
		if (e->xbutton.button==3) {
			//Right button is down
		}*/
	}
	//keys[XK_Up] = 0;
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
/*		int xdiff = savex - e->xbutton.x;
		int ydiff = savey - e->xbutton.y;
		if (++ct < 10)
			return;		
		//std::cout << "savex: " << savex << std::endl << std::flush;
		//std::cout << "e->xbutton.x: " << e->xbutton.x << std::endl <<
		//std::flush;
		if (xdiff > 0) {
			//std::cout << "xdiff: " << xdiff << std::endl << std::flush;
			g.ship.angle += 0.05f * (float)xdiff;
			if (g.ship.angle >= 360.0f)
				g.ship.angle -= 360.0f;
		}
		else if (xdiff < 0) {
			//std::cout << "xdiff: " << xdiff << std::endl << std::flush;
			g.ship.angle += 0.05f * (float)xdiff;
			if (g.ship.angle < 0.0f)
				g.ship.angle += 360.0f;
		}
		if (ydiff > 0) {
			//apply thrust
			//convert ship angle to radians
			Flt rad = ((g.ship.angle+90.0) / 360.0f) * PI * 2.0;
			//convert angle to a vector
			Flt xdir = cos(rad);
			Flt ydir = sin(rad);
			g.ship.vel[0] += xdir * (float)ydiff * 0.01f;
			g.ship.vel[1] += ydir * (float)ydiff * 0.01f;
			Flt speed = sqrt(g.ship.vel[0]*g.ship.vel[0]+
												g.ship.vel[1]*g.ship.vel[1]);
			if (speed > 10.0f) {
				speed = 10.0f;
				normalize2d(g.ship.vel);
				g.ship.vel[0] *= speed;
				g.ship.vel[1] *= speed;
			}
			g.mouseThrustOn = true;
			clock_gettime(CLOCK_REALTIME, &g.mouseThrustTimer);
		}
		x11.set_mouse_position(100,100);
		savex = 100;
		savey = 100;*/
	}
}

int check_keys(XEvent *e)
{
	static int shift=0;
	if (e->type != KeyRelease && e->type != KeyPress) {
		//not a keyboard event
		return 0;
	}
	int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
	//Log("key: %i\n", key);
	if (e->type == KeyRelease) {
		gl.keys[key] = 0;
		if (key == XK_Shift_L || key == XK_Shift_R)
			shift = 0;
		return 0;
	}

	if (e->type == KeyPress) {
		//std::cout << "press" << std::endl;
		gl.keys[key]=1;
		if (key == XK_Shift_L || key == XK_Shift_R) {
			shift = 1;
            return 0;
        }

	        if (e->type == KeyPress) {  //params added code
            gl.keys[key] = 1;
            if (key == XK_Escape) {
                return 1;
            }
            if (key == XK_space) {
               createNewAsteroid(&g);
            }
           /* if (key == XK_Left) {
                g.ship.angle += 4.0;
                if (g.ship.angle >= 360.0f)
                    g.ship.angle -= 360.0f;
            }
            if (key == XK_Right) {
                g.ship.angle -= 4.0;
                if(g.ship.angle < 0.0f)
                   g.ship.angle += 360.0;
            }*/
            if (key == XK_Up) {
                g.mouseThrustOn = true;
                clock_gettime(CLOCK_REALTIME, &g.mouseThrustTimer);
            }
        }
        //return 0; //param code end

	}
	(void)shift;
	switch (key) {
		case XK_Escape:
			return 1;
        case XK_k:
	        break;
	    case XK_f: //param's feature
	    //messageF();
            break;
		case XK_s:
        serafinFeatureMode = !serafinFeatureMode;
        std::cout << "Serafin feature mode activate" << std::endl; 
			break;
		case XK_Down:
			break;
		case XK_equal:
			break;
		case XK_minus:
			break;
        case XK_z: // ZAB'S FEATURE
            gl.obstR = !gl.obstR;
            messageZ();
            break;
                                                                        
	}
	return 0;
}

void deleteAsteroid(Game *g, EnemR *node)
{
	//Remove a node from doubly-linked list
	//Must look at 4 special cases below.
	if (node->prev == NULL) {
		if (node->next == NULL) {
			//only 1 item in list.
			g->ahead = NULL;
		} else {
			//at beginning of list.
			node->next->prev = NULL;
			g->ahead = node->next;
		}
	} else {
		if (node->next == NULL) {
			//at end of list.
			node->prev->next = NULL;
		} else {
			//in middle of list.
			node->prev->next = node->next;
			node->next->prev = node->prev;
		}
	}
	delete node;
	node = NULL;
}
/*void dodgeCdTracker() {
    g.ship.canDodge = false;
    dodgeTimer.reset();
    if (dodgeTimer.elapsedTime() > 2.0f) {
        std::cout << "2 sec" << std::endl;
        g.ship.canDodge = true;
    }



}*/


void physics()
{
  if(g.player.timestop == false) {
    Flt d0,d1,dist;
    //
    //
    //Update bullet positions
    //if (!serafinFeatureMode) {
    struct timespec bt;
    clock_gettime(CLOCK_REALTIME, &bt);
    int i = 0;
    while (i < g.nbullets) {
        Bullet *b = &g.barr[i];
        //How long has bullet been alive?
        double ts = timeDiff(&b->time, &bt);
        if (ts > 5.0) {
            //time to delete the bullet.
            memcpy(&g.barr[i], &g.barr[g.nbullets-1],
                sizeof(Bullet));
            g.nbullets--;
            //do not increment i.
            continue;
        }
        //move the bullet
        b->pos[0] += b->vel[0];
        b->pos[1] += b->vel[1];
        //Check for collision with window edges
        if (b->pos[0] < 0.0) {
            b->pos[0] += (float)gl.xres;
        }
        else if (b->pos[0] > (float)gl.xres) {
            b->pos[0] -= (float)gl.xres;
        }
        else if (b->pos[1] < 0.0) {
            b->pos[1] += (float)gl.yres;
        }
        else if (b->pos[1] > (float)gl.yres) {
            b->pos[1] -= (float)gl.yres;
        }
        ++i;
    }
     //Update asteroid positions
    EnemR *a = g.ahead;
    while (a) {
        a->pos[0] += a->vel[0];
        a->pos[1] += a->vel[1];
        //Check for collision with window edges
        if (a->pos[0] < -100.0) {
            a->pos[0] += (float)gl.xres+200;
        }
        else if (a->pos[0] > (float)gl.xres+100) {
            a->pos[0] -= (float)gl.xres+200;
        }
        else if (a->pos[1] < -100.0) {
            a->pos[1] += (float)gl.yres+200;
        }
        else if (a->pos[1] > (float)gl.yres+100) {
            a->pos[1] -= (float)gl.yres+200;
        }
        a->angle += a->rotate;
        a = a->next;
    }
    //
    //Asteroid collision with bullets?
    //If collision detected:
    //     1. delete the bullet
    //     2. break the asteroid into pieces
    //        if asteroid small, delete it
    a = g.ahead;
    while (a) {
        //is there a bullet within its radius?
        int i=0;
        while (i < g.nbullets) {
            Bullet *b = &g.barr[i];
            d0 = b->pos[0] - a->pos[0];
            d1 = b->pos[1] - a->pos[1];
            dist = (d0*d0 + d1*d1);
            if (dist < (a->radius*a->radius)) {
                //std::cout << "asteroid hit." << std::endl;
                //this asteroid is hit.
                if (a->radius > MINIMUM_ASTEROID_SIZE) {
                    //break it into pieces.
                    EnemR *ta = a;
                    buildAsteroidFragment(ta, a);
                    int r = rand()%10+5;
                    for (int k=0; k<r; k++) {
                         //get the next asteroid position in the array
                        EnemR *ta = new EnemR;
                        buildAsteroidFragment(ta, a);
                        //add to front of asteroid linked list
                        ta->next = g.ahead;
                        if (g.ahead != NULL)
                            g.ahead->prev = ta;
                        g.ahead = ta;
                        g.nasteroids++;
                    }
                } else {
                    a->color[0] = 1.0;
                    a->color[1] = 0.1;
                    a->color[2] = 0.1;
                    //asteroid is too small to break up
                    //delete the asteroid and bullet
                    EnemR *savea = a->next;
                    deleteAsteroid(&g, a);
                    a = savea;
                    g.nasteroids--;
                }
                //delete the bullet...
                memcpy(&g.barr[i], &g.barr[g.nbullets-1], sizeof(Bullet));
                g.nbullets--;
                if (a == NULL)
                    break;
            }
            i++;
        }
        if (a == NULL)
            break;
        a = a->next;
    }
  }
    //}
}
void playerPhysics()
{

    //Update ship position
    g.player.pos[0] += g.player.vel[0];
    g.player.pos[1] += g.player.vel[1];
    //Check for collision with window edges
    if (g.player.pos[0] < 0.0) {
        if (g.player.vel[0] < player.maxSpeed ) {
            g.player.pos[0] = 0.0;
            g.player.vel[0] = -g.player.vel[0]*.01;
        }
        else {
            g.player.pos[0] = 0.0;
            g.player.vel[0] = -g.player.vel[0]*0.00005;
        }

    }
    else if (g.player.pos[0] > (float)gl.xres) {
        //g.ship.pos[0] -= (float)gl.xres;
        g.player.pos[0] = (float)gl.xres;
        g.player.vel[0] = -g.player.vel[0]*0.00005;
    }
    else if (g.player.pos[1] < 0.0) {
        g.player.pos[1] = 0.0;
        g.player.vel[1] = -g.player.vel[1]*0.00005;
    }
    else if (g.player.pos[1] > (float)gl.yres) {
        g.player.pos[1] = (float)gl.yres;
        g.player.vel[1] = -g.player.vel[1] *0.00005;
    }

    //---------------------------------------------------
    //check keys pressed now
    //movement change by Serafin.
    //Fix: changing the asteroid-style movement into a
    //more traditional top-down
    Flt rad = ((g.player.angle+90.0) / 360.0f) * PI * 2.0;
    //convert angle to a vector
    Flt xdir = cos(rad);
    Flt ydir = sin(rad);
    float MAX_SPEED = 1;
    extern void moveLeft(Player&, float,  float);
    extern void moveRight(Player&, float,  float);
    extern void moveUp(Player&, float,  float);
    extern void moveDown(Player&, float,  float);
    extern void moveLeftUp(Player&, float, float, float);
    extern void moveLeftDown(Player&, float, float, float);
    extern void moveRightUp(Player&, float, float, float);
    extern void moveRightDown(Player&, float, float, float);
    extern void dashRight(Player&);
    extern void dashLeft(Player&);
    extern void dashUp(Player&);
    extern void dashDown(Player&);
    extern void dashLeftDown(Player&);
    extern void dashLeftUp(Player&);
    extern void dashRightDown(Player&);
    extern void dashRightUp(Player&);

    if (gl.keys[XK_Left]) {
       moveLeft(g.player, xdir,  MAX_SPEED);

    }
    if (gl.keys[XK_Right]) {
        moveRight(g.player, xdir,  MAX_SPEED);

    }
    if (gl.keys[XK_Up]) {
        moveUp(g.player, ydir,  MAX_SPEED);

    }

    if (gl.keys[XK_Down]) {
        moveDown(g.player, ydir,  MAX_SPEED);
    }

    if (gl.keys[XK_Left] && gl.keys[XK_Up]) {
        moveLeftUp(g.player, xdir, ydir, MAX_SPEED);
    }

    if (gl.keys[XK_Left] && gl.keys[XK_Down]) {
        moveLeftDown(g.player, xdir, ydir, MAX_SPEED);
    }

     if (gl.keys[XK_Right] && gl.keys[XK_Up]) {
        moveRightUp(g.player, xdir, ydir, MAX_SPEED);
    }

    if (gl.keys[XK_Right] && gl.keys[XK_Down]) {
        moveRightDown(g.player, xdir, ydir, MAX_SPEED);
    }

    //if nothing is happening then slow the ship down

    if (!gl.keys[XK_Left] && !gl.keys[XK_Right] && !gl.keys[XK_Up]
            && !gl.keys[XK_Down]) {
        while (g.player.vel[0] > 0 || g.player.vel[0] < 0
                    ||g.player.vel[1] > 0 || g.player.vel[1] < 0) {
                g.player.vel[1] *= .5;
                g.player.vel[0] *= .5;
        }
    }
    if(abilities[2].durationTimer == 0) {
        g.player.timestop = false;
    }

    if (gl.keys[XK_q] && abilities[2].timer == 0) {
        useAbility(abilities[2]);
        g.player.timestop = true;

    }

    if (!gl.keys[XK_Shift_L]) {
        gl.dodgePressing = false;
    }
    if (gl.keys[XK_Shift_L] && gl.dodgePressing == false
            && abilities[0].timer == 0) {

        gl.dodgePressing = true;
        useAbility(abilities[0]);

        if (gl.keys[XK_Right] && !gl.keys[XK_Up] && !gl.keys[XK_Down]) {

            useAbility(abilities[0]);
             dashRight(g.player);
        }

        if (gl.keys[XK_Right] && gl.keys[XK_Up]) {

            useAbility(abilities[0]);
            dashRightUp(g.player);
        }

        if (gl.keys[XK_Right] && gl.keys[XK_Down]) {

            useAbility(abilities[0]);
            dashRightDown(g.player);
        }
        if (gl.keys[XK_Left] && !gl.keys[XK_Up] && !gl.keys[XK_Down]) {

            useAbility(abilities[0]);
            dashLeft(g.player);
        }
        if (gl.keys[XK_Up] && !gl.keys[XK_Right] && !gl.keys[XK_Left]) {

            useAbility(abilities[0]);
            dashUp(g.player);
        }
        if (gl.keys[XK_Left] && gl.keys[XK_Up]) {

            useAbility(abilities[0]);
            dashLeftUp(g.player);
        }

        if (gl.keys[XK_Left] && gl.keys[XK_Down]) {

            useAbility(abilities[0]);
            dashLeftDown(g.player);
        }

        if (gl.keys[XK_Down] && !gl.keys[XK_Up] && !gl.keys[XK_Down]) {

            useAbility(abilities[0]);
            dashDown(g.player);
        }
     }
    if (serafinFeatureMode) {
        std::cout << "Dodge" <<  abilities[0].timer << std::endl;
        std::cout << "Attack" <<  abilities[1].timer << std::endl;
        std::cout << "Timestop" <<  abilities[2].timer << std::endl;
    }
    if (!gl.keys[XK_x]) {
        gl.swordSlash = false;
        gl.canPressSword = true;
        //double st = timeDiff(&
    }

    if (gl.keys[XK_x] && gl.canPressSword == true && gl.keyPressed == false) {
        gl.swordSlash = true;
        gl.canPressSword = false;
        gl.keyPressed = true;
    }
    else if (!gl.keys[XK_x]) {
        gl.keyPressed  = false;
    }

    if (gl.keys[XK_space]) {
        //a little time between each bullet
        struct timespec bt;
        //messageFire();
        clock_gettime(CLOCK_REALTIME, &bt);
        double ts = timeDiff(&g.bulletTimer, &bt);
        if (ts > 0.1) {
            timeCopy(&g.bulletTimer, &bt);
            if (g.nbullets < MAX_BULLETS) {
                //shoot a bullet...
                //Bullet *b = new Bullet;
                Bullet *b = &g.barr[g.nbullets];
                timeCopy(&b->time, &bt);

                b->pos[0] = g.ship.pos[0];
                b->pos[1] = g.ship.pos[1];
                b->vel[0] = g.ship.vel[0];
                b->vel[1] = g.ship.vel[1];
                //convert ship angle to radians
                Flt rad = ((g.ship.angle+90.0) / 360.0f) * PI * 2.0;
                //convert angle to a vector
                Flt xdir = cos(rad);
                Flt ydir = sin(rad);
                b->pos[0] += xdir*20.0f;
                b->pos[1] += ydir*20.0f;
                b->vel[0] += xdir*6.0f + rnd()*0.1;
                b->vel[1] += ydir*6.0f + rnd()*0.1;
                b->color[0] = 1.0f;
                b->color[1] = 1.0f;
                b->color[2] = 1.0f;
                g.nbullets++;
            }
        }
    }
    if (g.mouseThrustOn) {
        //should thrust be turned off
        struct timespec mtt;
        clock_gettime(CLOCK_REALTIME, &mtt);
        double tdif = timeDiff(&mtt, &g.mouseThrustTimer);
        //std::cout << "tdif: " << tdif << std::endl;
        if (tdif < -0.3)
            g.mouseThrustOn = false;
    }
}


void render()
{
    //draw thrust
    //Flt rad = ((g.ship.angle+90.0) / 360.0f) * PI * 2.0;
    //convert angle to a vector
    //Flt xdir = cos(rad);
    //Flt ydir = sin(rad);
    Rect placeholder;
    placeholder.bot = 0;
    placeholder.left = 0;
    placeholder.center = 0;
    
  	Rect r;
	glClear(GL_COLOR_BUFFER_BIT);
	//
	r.bot = gl.yres - 20;
	r.left = 10;
	r.center = 0;
	ggprint8b(&r, 16, 0x00ff0000, "3350 - Asteroids");
	ggprint8b(&r, 16, 0x00ffff00, "n bullets: %i", g.nbullets);
	ggprint8b(&r, 16, 0x00ffff00, "n asteroids: %i", g.nasteroids);
    int roundedDodge = std::round(abilities[0].timer);
    int roundedTimestop = std::round(abilities[2].timer);
    int roundedTimestopDur = std::round(abilities[2].durationTimer);
    ggprint8b(&r, 16, 0x00ffff00, "Cooldown: %i", roundedDodge);
    ggprint8b(&r, 16, 0x00ffff00, "Timestop Cooldown: %i", roundedTimestop);
    ggprint8b(&r, 16, 0x00ffff00, "Timestop Duration: %i", roundedTimestopDur);
    if (serafinFeatureMode) {
       ggprint8b(&r, 16, 0x00ffff00, "Serafin's Feature Mode");

    }
	//-------------------------------------------------------------------------
	//Draw the ship
	glColor3fv(g.player.color);
	glPushMatrix();
	glTranslatef(g.player.pos[0], g.player.pos[1], g.player.pos[2]);
	//float angle = atan2(ship.dir[1], ship.dir[0]);
	glRotatef(g.player.angle, 0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
            glVertex2f(-20.0f, -20.0f);
            glVertex2f(-20.0f,  20.0f);
            glVertex2f( 20.0f,  20.0f);
            glVertex2f( 20.0f, -20.0f);
        glEnd();
        glPopMatrix();
    //Sword 
    struct timespec swordTime;
    bool timerInitiliazed = false;
    if (!timerInitiliazed) {
       timerInitiliazed = true;
       clock_gettime(CLOCK_REALTIME, &swordTime);
    }

    if(gl.swordSlash == true) {

        if (!timerInitiliazed) {
            timerInitiliazed = true;
            clock_gettime(CLOCK_REALTIME, &swordTime);
        }
        glPushMatrix();
        glColor3f(100.0f, 100.0f, 100.0f);
        glTranslatef(g.player.pos[0]+ (g.player.xdir*25.0f), g.player.pos[1]+ (g.player.ydir*25.0f), g.player.pos[2]);
        glRotatef(g.player.angle, 0.0f, 0.0f, 1.0f);
        glBegin(GL_QUADS);
            glVertex2f(-sword.width, -sword.height);
            glVertex2f(-sword.width,  sword.height);
            glVertex2f( sword.width,  sword.height);
            glVertex2f( sword.width, -sword.height);
        glEnd();
        glPopMatrix();

        double elapsed = timeDiff(&swordTime, &timeCurrent);
        int roundedDodge = std::round(elapsed);

        ggprint8b(&placeholder, 16, 0xff000000, "Cooldown: %i", roundedDodge);

       // std::cout << elapsed << std::endl;
       

        if (elapsed > -0.04) {
            gl.canPressSword = false;
            glColor3f(100.0f, 100.0f, 100.0f);
        } else {
            gl.canPressSword = true;
            timerInitiliazed = false;
            gl.swordSlash = false;
        }
    }
}



