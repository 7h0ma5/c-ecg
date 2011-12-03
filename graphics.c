#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif
#include "string.h"
#include "audio.h"
#include "graphics.h"

#define WIDTH 600
#define HEIGHT 300

int freq_buffer[WIDTH];

void display() {
  glClear(GL_COLOR_BUFFER_BIT);

  glBegin(GL_LINES);
  glColor4f(1.0, 0.0, 0.0, 1.0);
  for (int i = 1; i < WIDTH; i++) {
    glVertex2f(i-1, freq_buffer[i-1]);
    glVertex2f(i, freq_buffer[i]);
  }
  glEnd();

  glutSwapBuffers();
}

void tick(int time) {
  glutTimerFunc(10, tick, time+1);
  freq_buffer[time%WIDTH] = audio_read();
  if (time % 5 == 0) glutPostRedisplay();
}

void graphics_loop() {
  glutMainLoop();
}

void graphics_init(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
  glutCreateWindow("ecg");

  glutDisplayFunc(display);
  glutTimerFunc(10, tick, 0);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, WIDTH, HEIGHT, 0, 0, 1);
  glMatrixMode(GL_MODELVIEW);
  glDisable(GL_DEPTH_TEST);

  memset(freq_buffer, 0, sizeof(int)*WIDTH);
}
