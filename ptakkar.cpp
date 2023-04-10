//Source code: ptakkar.cpp
//Paramveer Takkar

#include <GL/gl.h>
#include <math.h>

void drawSmallCircles(float x, float y, float radius, int numCircles)
{
    const float PI = 3.141592653589793;
    const int STEPS = 32;
    const float STEP_SIZE = 2.0f * PI / STEPS;
    float angle = 0.0f;
    float centerX, centerY;

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for (int i = 0; i <= numCircles; ++i)
        {
            angle = i * STEP_SIZE;
            centerX = x + cos(angle) * radius;
            centerY = y + sin(angle) * radius;
            glVertex2f(centerX, centerY);
        }
    glEnd();
}

