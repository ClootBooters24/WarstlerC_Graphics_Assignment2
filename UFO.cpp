//---------------------------------------
// Program: building3.cpp
// Purpose: Demonstrate use of mouse callbacks to draw lines.
// Author:  John Gauch
// Date:    Spring 2012
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

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
#define LINE_COUNT 1000
#define MAX_COUNT 100

// Global variables 
float Px[MAX_COUNT], Py[MAX_COUNT];
int xangle = 10;
int yangle = 15;
int mode = 1;
int count = 0;
float point[LINE_COUNT][4];

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
// Function to draw a UFO
//---------------------------------------
void UFO(float x, float y, float z)
{
   // Draw UFO body
   glColor3f(0.0, 0.0, 1.0);
   glPushMatrix();
   glTranslatef(x, y, z);
   glutSolidSphere(5, 20, 20);
   glPopMatrix();

   // Draw UFO top
   glColor3f(1.0, 0.0, 0.0);
   glPushMatrix();
   glTranslatef(x, y, z);
   glRotatef(90, 1, 0, 0);
   glutSolidCone(5, 10, 20, 20);
   glPopMatrix();

   mode = 2;
}

//---------------------------------------
// Function to draw a zero thickness wall
//---------------------------------------
void wall(float x1, float y1, float x2, float y2)
{
   float z1 = 0;
   float z2 = 10;

   // Draw wall polygon
   glColor3f(1.0, 0.0, 0.0);
   glBegin(GL_POLYGON);
   glVertex3f(x1, y1, z1);
   glVertex3f(x2, y2, z1);
   glVertex3f(x2, y2, z2);
   glVertex3f(x1, y1, z2);
   glEnd();

   // Draw wall outline
   glColor3f(1.0, 1.0, 1.0);
   glBegin(GL_LINE_LOOP);
   glVertex3f(x1, y1, z1);
   glVertex3f(x2, y2, z1);
   glVertex3f(x2, y2, z2);
   glVertex3f(x1, y1, z2);
   glEnd();
}

//---------------------------------------
// Keyboard callback for OpenGL
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   // Update angles
   if (key == 'u')
      UFO(x,y,0);
   
   // Redraw objects
   glutPostRedisplay();
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

   // Handle mouse down
   if (state == GLUT_DOWN)
   {
      if (count < MAX_COUNT) count++; 
      Px[count] = MIN_X_VIEW + (x - MIN_X_SCREEN) * x_scale;
      Py[count] = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * y_scale;
      glutPostRedisplay();
   }
}

//---------------------------------------
// Motion callback for OpenGL
//---------------------------------------
void motion(int x, int y)
{
   // Calculate scale factors
   float x_scale = (MAX_X_VIEW - MIN_X_VIEW) / 
      (float)(MAX_X_SCREEN - MIN_X_SCREEN);
   float y_scale = (MIN_Y_VIEW - MAX_Y_VIEW) / 
      (float)(MAX_Y_SCREEN - MIN_Y_SCREEN);

   // Handle mouse motion
   point[count][2] = MIN_X_VIEW + (x - MIN_X_SCREEN) * x_scale;
   point[count][3] = MAX_Y_VIEW + (y - MIN_Y_SCREEN) * y_scale;
   glutPostRedisplay();
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // Draw control points
   glColor3f(1.0, 1.0, 1.0);
   glPointSize(3);
   glBegin(GL_POINTS);
   for (int i=0; i<=count; i++)
      glVertex2f(Px[i], Py[i]);
   glEnd();

   // Connect control points
   glLineWidth(2);
   glBegin(GL_LINE_STRIP);
   for (int i=0; i<=count; i++)
      glVertex2f(Px[i], Py[i]);
   glEnd();

   if(mode == 2) {
      for (int i=0; i<=count; i++) {
         UFO(point[i][2], point[i][3], 0);
      }
   }

   glFlush();
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
   glutMotionFunc(motion);
   init();
   printf("Keyboard commands:\n");
   printf("   'u' - fly the UFO across the lines\n");
   printf("Mouse operations:\n");
   printf("   'mouse click' - draw line\n");
   glutMainLoop();
   return 0;
}
