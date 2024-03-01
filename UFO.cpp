//---------------------------------------
// Program: building3.cpp
// Purpose: Demonstrate use of mouse callbacks to draw lines.
// Author:  John Gauch
// Date:    Spring 2012
//---------------------------------------
#include <math.h>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
using namespace std::chrono;


// Global constants
#define MIN_X_VIEW -50
#define MAX_X_VIEW 50
#define MIN_Y_VIEW -50
#define MAX_Y_VIEW 50
#define MIN_Z_VIEW -50
#define MAX_Z_VIEW 50
#define MIN_X_SCREEN 0
#define MAX_X_SCREEN 500
#define MIN_Y_SCREEN 0
#define MAX_Y_SCREEN 500
#define LINE_COUNT 100000
#define MAX_COUNT 10000

// Global variables 
float Px[MAX_COUNT], Py[MAX_COUNT];
int count = 0;
float coords[MAX_COUNT][MAX_COUNT];
int node = 0;
bool drawn = false;
bool ufoStart = false;

void ufo() {
   static int currentNode = 0;
   static float progress = 0.0;

   if (currentNode >= count) {
      currentNode = 0;
      progress = 0.0;
      return;
   }

   progress += 0.01;
   if (progress > 1.0) {
      progress = 0.0;
      currentNode++;
   }

   //Get and print current coordinates
   float currentX = coords[currentNode][0] + (coords[currentNode + 1][0] - coords[currentNode][0]) * progress;
   float currentY = coords[currentNode][1] + (coords[currentNode + 1][1] - coords[currentNode][1]) * progress;
   printf("Current position: (%f, %f)\n", currentX, currentY);

   //Get direction and angle of the line
   float dx = coords[currentNode + 1][0] - coords[currentNode][0];
   float dy = coords[currentNode + 1][1] - coords[currentNode][1];
   float angle = atan2(dy, dx);

   float size = 10.0;
   float halfSize = size / 2.0;

   glPushMatrix();
   glLoadIdentity();
   glTranslatef(currentX, currentY, 0.0);
   glRotatef(angle * 180 / M_PI, 0, 0, 1); // Rotate to align with the line segment

   //Draw triangle
   glBegin(GL_POLYGON);
   glColor3f(1.0, 0.0, 0.0);
   glVertex2f(0.0, halfSize); 
   glVertex2f(-size, -halfSize); 
   glVertex2f(-size, halfSize); 
   glEnd();

   glPopMatrix();
   glutPostRedisplay();
}

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(MIN_X_VIEW, MAX_X_VIEW, 
      MIN_Y_VIEW, MAX_Y_VIEW, 
      MIN_Z_VIEW, MAX_Z_VIEW);
   glEnable(GL_DEPTH_TEST);
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // Update angles
   if(key == 'u') {
      ufoStart = true;
      ufo();
      glutPostRedisplay();
   }
}

//---------------------------------------
// Mouse callback for OpenGL
//---------------------------------------
void mouse(int button, int state, int x, int y)
{
      // Calculate scale factors
   float x_scale = (MAX_X_VIEW - MIN_X_VIEW) / 
      (float)(MAX_X_SCREEN - MIN_X_SCREEN);
   float y_scale = (MIN_Y_VIEW - MAX_Y_VIEW) / 
      (float)(MAX_Y_SCREEN - MIN_Y_SCREEN);

   // Initialize Px and Py arrays with the first click coordinates
   if (count == 0) {
      Px[0] = MIN_X_VIEW + (x - MIN_X_SCREEN) * x_scale;
      Py[0] = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * y_scale;
      coords[0][0] = Px[0], coords[0][1] = Py[0];
   }

   // Handle mouse down
   if (state == GLUT_DOWN)
   {
      // Increment count and store coordinates
      if (count < MAX_COUNT) count++; 
      Px[count] = MIN_X_VIEW + (x - MIN_X_SCREEN) * x_scale;
      Py[count] = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * y_scale;
      coords[count][0] = Px[count], coords[count][1] = Py[count];
      glutPostRedisplay();
   }
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // Connect control points
   glLineWidth(2);
   glBegin(GL_LINE_STRIP);
   for (int i=0; i<=count; i++)
      glVertex2f(Px[i], Py[i]);
   glEnd();

   if (ufoStart) {
      ufo();
   }
   
   glutSwapBuffers(); // Use double buffering for smooth rendering
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitWindowSize(MAX_Y_SCREEN, MAX_X_SCREEN);
   glutInitWindowPosition(MAX_Y_SCREEN/2, MAX_X_SCREEN/2);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
   glutCreateWindow("UFO");
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   init();
   printf("Keyboard commands:\n");
   printf("   'u' - fly the UFO across the lines\n");
   printf("Mouse operations:\n");
   printf("   'mouse click' - draw line\n");
   glutMainLoop();
   return 0;
}
