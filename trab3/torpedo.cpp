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
  to_delete = false;
  if (t == 1) {
    target_x = mx; //setTargetX(mx);
    target_y = my; //setTargetY(my);
    tetha_center = atan((my-y)/(mx-x))*180/M_PI;

    if ((my > y) && (mx > x)) { // 1o quad
      tetha_center = atan((my-y)/(mx-x))*180/M_PI;
      std::cout << "1o quad " << tetha_center << std::endl;
    } else if ((my > y) && (mx < x)) { // 2o quad
      tetha_center = atan((my-y)/(mx-x))*180/M_PI + 180;
      std::cout << "2o quad " << tetha_center << std::endl;
    } else if ((my < y) && (mx < x)) { // 3o quad
      tetha_center = atan((my-y)/(mx-x))*180/M_PI + 180;
      std::cout << "3o quad " << tetha_center << std::endl;
    } else { // 4o quad
      tetha_center = atan((my-y)/(mx-x))*180/M_PI + 360;
      std::cout << "4o quad " << tetha_center << std::endl;
    }
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

bool Torpedo::getToDelete() {
  return to_delete;
}

void Torpedo::setToDelete(bool status) {
  to_delete = status;
}

GLfloat Torpedo::getDistTarget() {
  return dist_target;
}

void Torpedo::setDistTarget(GLfloat d) {
  dist_target = d;
}
void Torpedo::MoveX(GLfloat dx) {
	pos_x += dx;
}

void Torpedo::MoveY(GLfloat dy) {
	pos_y += dy;
}

void Torpedo::Move(GLdouble d, GLfloat vel_torpedo) {
  //d *= 10;
  if (type == 1) { // flor d'agua movimento de lancamento de projetil
    MoveX(vel_torpedo*d*cos(tetha_center*M_PI/180));
    MoveY(vel_torpedo*d*sin(tetha_center*M_PI/180));

    GLfloat actual_dist = sqrt( pow(pos_x - target_x, 2) + pow(pos_y - target_y, 2) );
    GLfloat half_dist = getDistTarget()/2;
    if (dist_target - actual_dist < half_dist) { // subindo o Torpedo
      if (radius < max_radius) {
        radius = max_radius/2 + max_radius * (1 - actual_dist/dist_target);
      }
    } else { // descendo o Torpedo
      if (radius > max_radius/2) {
        radius = max_radius/2 + max_radius*(actual_dist/dist_target);
        if (abs(pos_x - target_x) < 0.1 && (abs(pos_y - target_x))) to_delete = true;
      }
    }
  } else if (type == -1) { // underwater movimento retilinio uniforme

    MoveX(vel_torpedo*d*cos(tetha_center));
    MoveY(vel_torpedo*d*sin(tetha_center));
  }
}

void Torpedo::DesenhaTorpedo(GLfloat x, GLfloat y, GLfloat radius, GLfloat cR,
		GLfloat cG, GLfloat cB) {
      if(type == -1) { // rect
        glPushMatrix();
          glTranslatef(x, y, 0.0);
          glRotatef(tetha_center*180/M_PI, 0, 0, 1);
          DesenhaRect(center, 6*radius, radius, 0.0, 0.0, 0.0);
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
