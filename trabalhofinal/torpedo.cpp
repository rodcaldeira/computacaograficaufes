/*
 * Torpedo.cpp
 *
 *  Created on: 22 de set de 2018
 *      Author: rodcaldeira
 */

#include "torpedo.h"
#include <math.h>
#include <iostream>

Torpedo::Torpedo(GLfloat x, GLfloat y, GLfloat z, GLfloat tetha, GLfloat tetha_z, GLfloat r, GLfloat t, GLfloat mx, GLfloat my) {
  tetha_center = tetha; //setTethaCenter(tetha);
  tetha_center_z = tetha_z;
  radius = r; //setRadius(r);
  max_radius = 2*r; //setMaxRadius(2*r);
  type = t; //setType(t);
  to_delete = false;
  pos_z = z;
  R = 0.0;
  G = 0.0;
  B = 0.0;
  if (t == 1) {
    pos_x = x + (radius - radius/5)*cos(tetha); // setPosX(x);
    pos_y = y + (radius - radius/5)*sin(tetha); //setPosY(y);
    target_x = mx; //setTargetX(mx);
    target_y = my; //setTargetY(my);
    tetha_center = atan((my-y)/(mx-x))*180/M_PI;

    if ((my > y) && (mx > x)) { // 1o quad
      tetha_center = atan((my-y)/(mx-x))*180/M_PI;
    } else if ((my > y) && (mx < x)) { // 2o quad
      tetha_center = atan((my-y)/(mx-x))*180/M_PI + 180;
    } else if ((my < y) && (mx < x)) { // 3o quad
      tetha_center = atan((my-y)/(mx-x))*180/M_PI + 180;
    } else { // 4o quad
      tetha_center = atan((my-y)/(mx-x))*180/M_PI + 360;
    }
    dist_target = sqrt(pow(x - mx, 2) + pow(y - my, 2)); //setDistTarget( sqrt(pow(x - mx, 2) + pow(y - my, 2)) );
  } else {
    pos_x = x - (radius / 6)*cos(tetha); // setPosX(x);
    pos_y = y - (radius / 6)*sin(tetha); //setPosY(y);
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

GLfloat Torpedo::getPosZ() {
  return pos_z;
}

void Torpedo::setPosZ(GLfloat z) {
  pos_z = z;
}

void Torpedo::setTethaCenterZ(GLfloat a) {
  while (a > 360) a-=360;
  while (a < -360) a+=360;

  tetha_center_z = a;
}

GLfloat Torpedo::getTethaCenterZ() {
  return tetha_center_z;
}

GLfloat Torpedo::getTethaCenter() {
  return tetha_center;
}

void Torpedo::setTethaCenter(GLfloat a) {
  while (a > 360) a-=360;
  while (a < -360) a+=360;
  
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

void Torpedo::MoveZ(GLfloat dz) {
  pos_z += dz;
}
   
void Torpedo::shot(GLdouble d, GLfloat vel_torpedo) {
  if (type == 1) { // lancamento de projetil
    if (pos_z <= radius*12*4) {
      MoveZ(vel_torpedo*d);
    } else {
      MoveX(vel_torpedo*d*cos(tetha_center*M_PI/180));
      MoveY(vel_torpedo*d*sin(tetha_center*M_PI/180));
      MoveZ(vel_torpedo*vel_torpedo*vel_torpedo*d*cos(tetha_center_z));
      std::cout << tetha_center << std::endl;
      GLfloat actual_dist = sqrt( pow(pos_x - target_x, 2) + pow(pos_y - target_y, 2) );
      // std::cout << actual_dist/dist_target << std::endl;
      tetha_center_z = (1-(actual_dist/dist_target))*180.0;
    }
  } else if (type == -1) { // retilinio
    MoveX(vel_torpedo*d*cos((tetha_center)*M_PI/180));
    MoveY(vel_torpedo*d*sin((tetha_center)*M_PI/180));
    MoveZ(vel_torpedo*d*sin(tetha_center_z));
  }
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
        if (abs(pos_x - target_x) <= 1 && (abs(pos_y - target_y) <= 1)) to_delete = true;
      }
    }
  } else if (type == -1) { // underwater movimento retilinio uniforme

    MoveX(vel_torpedo*d*cos(tetha_center));
    MoveY(vel_torpedo*d*sin(tetha_center));
    MoveZ(vel_torpedo*d*sin(tetha_center_z));
  }
}

void Torpedo::DesenhaTorpedo3d(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLfloat cR, GLfloat cG, GLfloat cB) {
  glPushMatrix();
    // glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    GLfloat materialEmisison[] = {1, 1, 1, 1.0};
    GLfloat materialColor[] = {1.0, 0.0, 0.0, 1.0};
    GLfloat materialColorYellow[] = {1.0, 1.0, 0.0, 1.0};
    //GLfloat materialColorAMB[] = {1.0, 1.0, 0.0, 1.0};
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess[] = { 128 };
    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmisison);
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glColor3f(0.0, 0.0, 0.0);
    glTranslatef(x, y, z);
    if (type == -1) {
      glRotatef(90, 1, 0, 0);
      //glRotatef(tetha_center,0, 0, 1);
      std::cout << tetha_center << std::endl;
    }
    GLUquadricObj *quadric=gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluQuadricOrientation(quadric,GLU_OUTSIDE);

    glRotatef(tetha_center, 0, 0, 1);
    glRotatef(tetha_center_z, 0, 1, 0);
    gluCylinder(quadric, radius, radius, radius*10, 32, 1);
    glTranslatef(0.0, 0.0, radius*10);
    gluDeleteQuadric(quadric);
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
    glutSolidSphere(radius, 32, 10);
    glPopMatrix();
}

void Torpedo::DesenhaTorpedo(GLfloat x, GLfloat y, GLfloat radius, GLfloat cR,
		GLfloat cG, GLfloat cB) {
      if(type == -1) { // rect
        glPushMatrix();
          glTranslatef(x, y, 0.0);
          glRotatef(tetha_center*180/M_PI, 0, 0, 1);
          DesenhaRect(center, 6*radius, radius, cR, cG, cB);
          /* std::cout << "desenhou torpedo" << std::endl; */
        glPopMatrix();
      } else { // circle
        glPushMatrix();
          glTranslatef(x, y, 0.0);
          glRotatef(tetha_center, 0, 0, 1);
          DesenhaCircle(0.0, 0.0, radius, radius, cR, cG, cB, 360);
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
