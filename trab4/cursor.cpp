/*
 * Cursor.cpp
 *
 *  Created on: 22 de set de 2018
 *      Author: rodcaldeira
 */

#include "cursor.h"
#include <math.h>
#include <iostream>

Cursor::Cursor() {
  return;
}

Cursor::~Cursor() {
  return;
}

GLfloat Cursor::getPosX() {
  return pos_x;
}

void Cursor::setPosX(GLfloat x) {
  pos_x = x;
}

GLfloat Cursor::getPosY() {
  return pos_y;
}

void Cursor::setPosY(GLfloat y) {
  pos_y = y;
}

GLfloat Cursor::getRadius() {
  return radius;
}

void Cursor::setRadius(GLfloat r) {
  radius = r;
}

void Cursor::DesenhaCursor(GLfloat x, GLfloat y, GLfloat radius) {
  glColor3f(1.0,1.0,1.0);
  glPushMatrix();
    glTranslatef(x, y, 0.0);
    glBegin(GL_LINES);
      glVertex3f(-radius/5,0.0,0.0);
      glVertex3f(radius/5,0.0,0.0);
    glEnd();
    glBegin(GL_LINES);
      glVertex3f(0.0, -radius/5, 0.0);
      glVertex3f(0.0, radius/5, 0.0);
    glEnd();
    glPushAttrib(GL_ENABLE_BIT);
    //# glPushAttrib is done to return everything to normal after drawing

      glLineStipple(1, 0xAAAA); 
      glEnable(GL_LINE_STIPPLE);
      glBegin(GL_LINES);
        for (float i = 0; i <= 2*M_PI; i += M_PI_4/5) {
          glVertex3f(radius*cos(i), radius*sin(i), 0.0);
        }
      glEnd();

    glPopAttrib();
  glPopMatrix();
}
