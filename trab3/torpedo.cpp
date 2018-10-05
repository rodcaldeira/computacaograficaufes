/*
 * Torpedo.cpp
 *
 *  Created on: 22 de set de 2018
 *      Author: rodcaldeira
 */

#include "torpedo.h"
#include <math.h>
#include <iostream>

Torpedo::Torpedo(GLfloat x, GLfloat y, GLfloat tetha, GLfloat r, GLfloat t, GLfloat mx, GLfloat my) {
  pos_x = x; // setPosX(x);
  pos_y = y; //setPosY(y);
  tetha_center = tetha; //setTethaCenter(tetha);
  radius = r; //setRadius(r);
  max_radius = 2*r; //setMaxRadius(2*r);
  type = t; //setType(t);
  if (t == 1) {
    target_x = mx; //setTargetX(mx);
    target_y = my; //setTargetY(my);
    dist_target = sqrt(pow(x - mx, 2) + pow(y - my, 2)); //setDistTarget( sqrt(pow(x - mx, 2) + pow(y - my, 2)) );
  }
}

Torpedo::~Torpedo() {
  return;
}

GLfloat Torpedo::getR() {
  return R;
}

void Torpedo::setR(GLfloat cR) {
  R = cR;
}

GLfloat Torpedo::getB() {
  return B;
}

void Torpedo::setB(GLfloat cB) {
  B = cB;
}

GLfloat Torpedo::getG() {
  return G;
}

void Torpedo::setG(GLfloat cG) {
  G = cG;
}

GLfloat Torpedo::getPosX() {
  return pos_x;
}

void Torpedo::setPosX(GLfloat x) {
  pos_x = x;
}

GLfloat Torpedo::getPosY() {
  return pos_y;
}

void Torpedo::setPosY(GLfloat y) {
  pos_y = y;
}

GLfloat Torpedo::getTethaCenter() {
  return tetha_center;
}

void Torpedo::setTethaCenter(GLfloat a) {
  tetha_center = a;
}

GLfloat Torpedo::getRadius() {
  return radius;
}

void Torpedo::setRadius(GLfloat r) {
  radius = r;
}

GLfloat Torpedo::getMaxRadius() {
  return max_radius;
}

void Torpedo::setMaxRadius(GLfloat r) {
  max_radius = r;
}

GLfloat Torpedo::getType() {
  return type;
}

void Torpedo::setType(GLint t) {
  type = t;
}

GLfloat Torpedo::getTargetX() {
  return target_x;
}

void Torpedo::setTargetX(GLfloat x) {
  target_x = x;
}

GLfloat Torpedo::getTargetY() {
  return target_y;
}

void Torpedo::setTargetY(GLfloat y) {
  target_y = y;
}

GLfloat Torpedo::getDistTarget() {
  return dist_target;
}

void Torpedo::setDistTarget(GLfloat d) {
  dist_target = d;
}

void Torpedo::Move(GLdouble d) {
  if (type == 1) { // flor d'agua movimento de lancamento de projetil
    MoveX(d*cos(tetha_center));
    MoveY(d*sin(tetha_center));
    GLfloat actual_dist = sqrt( pow(pos_x - target_x, 2) + pow(pos_y - target_y, 2) );
    GLfloat half_dist = getDistTarget()/2;
    if (actual_dist < half_dist) { // subindo o Torpedo
        // r + (mr - r)p
        // r - rp + mrp
        // (1 - p)r + mrp
      setRadius( radius + (max_radius - radius)*(actual_dist/half_dist) );
    } else { // descendo o Torpedo
      actual_dist = abs(actual_dist - getDistTarget());
      setRadius( radius - (max_radius - radius)*(actual_dist/half_dist) );
    }
  } else if (type == -1) { // underwater movimento retilinio uniforme
    MoveX(d*cos(tetha_center));
    MoveY(d*sin(tetha_center));
  }
}

void Torpedo::MoveX(GLfloat dx) {
	pos_x += dx;
}

void Torpedo::MoveY(GLfloat dy) {
	pos_y += dy;
}

void Torpedo::DesenhaTorpedo(GLfloat x, GLfloat y, GLfloat radius, GLfloat cR,
		GLfloat cG, GLfloat cB) {
      if(type == -1) { // rect
        glPushMatrix();
          glTranslatef(x, y, 0.0);
          glRotatef(tetha_center, 0, 0, 1);
          DesenhaRect(center, radius, 2*radius, 0.0, 0.0, 0.0);
        glPopMatrix();
      } else { // circle
        glPushMatrix();
          glTranslatef(x, y, 0.0);
          glRotatef(tetha_center, 0, 0, 1);
          DesenhaCircle(0.0, 0.0, radius, radius, 0.0, 0.0, 0.0, 360);
        glPopMatrix();
      }
	}

void Torpedo::DesenhaCircle(GLfloat x, GLfloat y, GLfloat radx, GLfloat rady,
		GLfloat cR, GLfloat cG, GLfloat cB, GLfloat ang) {
	glColor3f(cR, cG, cB);

	glPushMatrix();
	glTranslatef(x, y, 0.0);
	glBegin(GL_POLYGON);
	for (int i = 0; i <= ang; i += 360 / 30) {
		double angle = i * M_PI / 180;
		glVertex3f(radx * cos(angle), rady * sin(angle), 0.0);
	}
	glEnd();
	glPopMatrix();

}

void Torpedo::DesenhaRect(REFERENCE ref, GLfloat base, GLfloat altura,
		GLfloat cR, GLfloat cG, GLfloat cB) {
	glColor3f(cR, cG, cB);
	glPushMatrix();
	glBegin(GL_POLYGON);
	switch (ref) {
	case center:
		glVertex3d(-base / 2, altura / 2, 0.0);
		glVertex3d(base / 2, altura / 2, 0.0);
		glVertex3d(base / 2, -altura / 2, 0.0);
		glVertex3d(-base / 2, -altura / 2, 0.0);
		break;
	case top_center:
		glVertex3d(-base / 2, 0.0, 0.0);
		glVertex3d(base / 2, 0.0, 0.0);
		glVertex3d(base / 2, -altura, 0.0);
		glVertex3d(-base / 2, -altura, 0.0);
		break;
	case right_center:
		glVertex3d(0.0, altura / 2, 0.0);
		glVertex3d(0.0, -altura / 2, 0.0);
		glVertex3d(-base, -altura / 2, 0.0);
		glVertex3d(-base, altura / 2, 0.0);
		break;
	case bottom_center:
		glVertex3d(-base / 2, 0.0, 0.0);
		glVertex3d(-base / 2, altura, 0.0);
		glVertex3d(base / 2, altura, 0.0);
		glVertex3d(base / 2, 0.0, 0.0);
		break;
	case left_center:
		glVertex3d(0.0, altura / 2, 0.0);
		glVertex3d(base, altura / 2, 0.0);
		glVertex3d(base, -altura / 2, 0.0);
		glVertex3d(0.0, -altura / 2, 0.0);
		break;
	case top_right_corner:
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(-base, 0.0, 0.0);
		glVertex3d(-base, -altura, 0.0);
		glVertex3d(0.0, -altura, 0.0);
		break;
	case bottom_right_corner:
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, altura, 0.0);
		glVertex3d(-base, altura, 0.0);
		glVertex3d(-base, 0.0, 0.0);
		break;
	case bottom_left_corner:
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, altura, 0.0);
		glVertex3d(base, altura, 0.0);
		glVertex3d(base, 0.0, 0.0);
		break;
	case top_left_corner:
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(base, 0.0, 0.0);
		glVertex3d(base, -altura, 0.0);
		glVertex3d(0.0, -altura, 0.0);
		break;
	}
	glEnd();
	glPopMatrix();
}
