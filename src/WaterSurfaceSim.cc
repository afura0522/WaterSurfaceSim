/**
 * Copyright (c) 2014 The Motel On Jupiter
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

// Avoid the warning caused by the GLUT header
#pragma warning(disable:4505)

// Detect memory leak
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <algorithm>
#include <sstream>

#pragma warning(push)
#include <freeglut.h>
#pragma warning(pop)

#include "util/FPSCounter.h"
#include "stimulator/RainStimulator.h"
#include "map/WaterSurfaceMap.h"

#define UNUSED(_var) (void)_var

static const char *WindowTitle = "Water Surface Simulator";
static const int WindowPosX = 100;
static const int WindowPosY = 100;
static const int WindowWidth = 600;
static const int WindowHeight = 600;

static const int FPS = 60;
static const int MapWidth = 200;
static const int MapHeight = 200;
static const char *BitmapPath = ".\\capture.bmp";

static const float DefaultPropagation = 0.2f;
static const float DefaultAttenuation = 0.99f;

static const float RainMinForce = 0.0f;
static const float RainMaxForce = 25.0f;
static const float RainFrequency = 0.2f;

static const RectangleRainStimulator::StimulusOption RainOption(MapWidth,
                                                                MapHeight,
                                                                RainMinForce,
                                                                RainMaxForce,
                                                                RainFrequency);

static int get_time();
static int on_stimulate(int x, int y, float force);

static FPSCounter fpscounter(1000, get_time);
static WaterSurfaceMap map(MapWidth, MapHeight, DefaultPropagation,
                           DefaultAttenuation);
static RectangleRainStimulator rain(RainOption, on_stimulate);
static bool rainstimulateflag = true;

/**
 * the callback function for FPSCounter
 */
static int get_time() {
  return glutGet(GLUT_ELAPSED_TIME);
}

/**
 * the callback function for RainStimulator
 */
static int on_stimulate(int x, int y, float force) {
  map.Set(x, y, force);
  return 0;
}

/**
 * The callback function called every fixed time
 */
void on_timer(int value) {
  UNUSED(value);

  if (rainstimulateflag) {
    rain.Execute();
  }
  map.Execute();
  fpscounter.Update();

  glutPostRedisplay();
  glutTimerFunc(1000 / FPS, on_timer, 0);
}

/**
 * The callback function for the mouse drug reaction
 */
void on_mouse_input(int x, int y) {
  // must validate
  int x_validate = std::max(std::min(x, WindowWidth), 0);
  int y_validate = std::max(std::min(y, WindowHeight), 0);

  int x_on_map = x_validate * MapWidth / WindowWidth;
  int y_on_map = y_validate * MapHeight / WindowHeight;

  map.Set(x_on_map, y_on_map, 100.0f);
}

/**
 * The callback function for the keyboard input reaction
 */
void on_keyboard_input(unsigned char key, int x, int y) {
  UNUSED(x);
  UNUSED(y);

  switch (key) {
    case 'c':
      map.ClearAll();
      break;
    case 'r':
      rainstimulateflag = !rainstimulateflag;
      break;
    case 'b':
      map.OutputBitmap(BitmapPath);
      break;
  }
}

/**
 * Draw the string at the specified position
 */
void draw_string(float x, float y, std::string str) {
  glDisable (GL_TEXTURE_2D);
  glColor4f(0.7f, 0.8f, 0.8f, 1.0f);
  glRasterPos3f(x, y, -1.0f);
  const char* p = str.c_str();
  while ('\0' != *p) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
    ++p;
  }
  glEnable(GL_TEXTURE_2D);
}

/**
 * The callback function to display
 */
void on_display(void) {
  // Clear the screen
  glClear (GL_COLOR_BUFFER_BIT);
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();

  // Update the texture
  const float *texture = map.OutputTexture();
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, MapWidth, MapHeight, 0, GL_RGB,
               GL_FLOAT, texture);

  // Draw the texture to screen
  glColor3f(1.0, 1.0, 1.0);
  glBegin (GL_POLYGON);
  glTexCoord2f(0, 0);
  glVertex2f(-1.0f, 1.0f);
  glTexCoord2f(1, 0);
  glVertex2f(-1.0f, -1.0f);
  glTexCoord2f(1, 1);
  glVertex2f(1.0f, -1.0f);
  glTexCoord2f(0, 1);
  glVertex2f(1.0f, 1.0f);
  glEnd();

  // Draw the current FPS
  std::stringstream ss;
  ss.str("");
  ss << "real fps: " << fpscounter.fps() << " (ideal: " << FPS << ")";
  draw_string(-0.90f, 0.86f, ss.str());
  ss.str("");
  ss << "rain stimulus: " << (rainstimulateflag ? "on" : "off");
  draw_string(-0.90f, 0.78f, ss.str());

  // Draw the how to interfere
  ss.str("");
  ss << "c: clear all";
  draw_string(-0.10f, -0.76f, ss.str());
  ss.str("");
  ss << "r: toggle the rain simulator effectivity";
  draw_string(-0.10f, -0.84f, ss.str());
  ss.str("");
  ss << "b: save as bitmap";
  draw_string(-0.10f, -0.92f, ss.str());

  // Reflect the screen
  glutSwapBuffers();
}

/**
 * main (the entry point)
 */
int main(int argc, char** argv) {
  // Detect memory leak
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

  // Initialize the GLUT library and Create the Window
  glutInitWindowPosition(WindowPosX, WindowPosY);
  glutInitWindowSize(WindowWidth, WindowHeight);
  glutInit(&argc, argv);
  glutCreateWindow(WindowTitle);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

  // Set the callback functions
  glutDisplayFunc(on_display);
  glutTimerFunc(1000 / FPS, on_timer, 0);
  glutMotionFunc(on_mouse_input);
  glutKeyboardFunc(on_keyboard_input);

  // Clear the screen
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  // Set up the texture as screen
  glEnable (GL_TEXTURE_2D);
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  // Execute the main loop
  map.ClearAll();
  map.Set(map.width() / 2, map.height() / 2, 100.0f);
  glutMainLoop();

  // Release the texture
  glDeleteTextures(1, &texture);

  return 0;
}
