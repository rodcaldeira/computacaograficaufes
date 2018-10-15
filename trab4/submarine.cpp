/*
 * submarine.cpp
 *
 *  Created on: 22 de set de 2018
 *      Author: rodcaldeira
 */

#include "submarine.h"
#include <math.h>
#include <iostream>


namespace Submarine {

submarine::submarine() {
	setPosX(0.0);
	setPosY(0.0);
	setRadius(40.0);
	setTethaCenter(90.0);
	setTethaHeli(0.0);
	setTethaPaddle(0.0);
	setMovingZAxis(false);
	setCanMove(true);
	setSubmerginStatus(1);
	setDirectionSubX(cos(getTethaCenter()));
	setDirectionSubY(sin(getTethaCenter()));
	setDirectionSteeringX(cos(getTethaPaddle()));
	setDirectionSteeringY(sin(getTethaPaddle()));
}

GLfloat submarine::getB() const {
	return B;
}

void submarine::setB(GLfloat b) {
	B = b;
}

GLfloat submarine::getG() const {
	return G;
}

void submarine::setG(GLfloat g) {
	G = g;
}

GLfloat submarine::getR() const {
	return R;
}

void submarine::setR(GLfloat r) {
	R = r;
}

GLfloat submarine::getDirectionSteeringX() const {
	return direction_steering_x;
}

void submarine::setDirectionSteeringX(GLfloat directionSteeringX) {
	direction_steering_x = directionSteeringX;
}

GLfloat submarine::getDirectionSteeringY() const {
	return direction_steering_y;
}

void submarine::setDirectionSteeringY(GLfloat directionSteeringY) {
	direction_steering_y = directionSteeringY;
}

GLfloat submarine::getDirectionSubX() const {
	return direction_sub_x;
}

void submarine::setDirectionSubX(GLfloat directionSubX) {
	direction_sub_x = directionSubX;
}

GLfloat submarine::getDirectionSubY() const {
	return direction_sub_y;
}

void submarine::setDirectionSubY(GLfloat directionSubY) {
	direction_sub_y = directionSubY;
}

GLfloat submarine::getId() {
	return id;
}

void submarine::setId(GLfloat x) {
	id = x;
}

GLfloat submarine::getPosX() const {
	return pos_x;
}

void submarine::setPosX(GLfloat posX) {
	pos_x = posX;
}

GLfloat submarine::getPosY() const {
	return pos_y;
}

void submarine::setPosY(GLfloat posY) {
	pos_y = posY;
}

GLfloat submarine::getPosZ() const {
	return pos_z;
}

void submarine::setPosZ(GLfloat posZ) {
	pos_z = posZ;
}

GLfloat submarine::getRadius() const {
	return radius;
}

void submarine::setRadius(GLfloat radius) {
	this->radius = radius;
}

GLfloat submarine::getMaxRadius() const {
  return max_radius;
}

void submarine::setMaxRadius(GLfloat r) {
  max_radius = r;
}

GLfloat submarine::getTethaCenter() const {
	return tetha_center;
}

void submarine::setTethaCenter(GLfloat tethaCenter) {
	if (tethaCenter > 0) {
		while (tethaCenter >= 360) tethaCenter -= 360;
	} else if (tethaCenter < 0) {
		while (tethaCenter <= 360) tethaCenter += 360;
	}
	tetha_center = tethaCenter * M_PI / 180;
}

GLfloat submarine::getTethaHeli() const {
	return tetha_heli;
}

void submarine::setTethaHeli(GLfloat tethaHeli) {
	tetha_heli = tethaHeli;
}

GLfloat submarine::getTethaPaddle() const {
	return tetha_paddle;
}

void submarine::setTethaPaddle(GLfloat tethaPaddle) {
	tetha_paddle = tethaPaddle * M_PI / 180;
}

submarine::~submarine() {
	// TODO Auto-generated destructor stub
}

bool submarine::checkHit(GLfloat x, GLfloat y, GLfloat z) {
	if (
		sqrt(
			pow(pos_x - x, 2) +
			pow(pos_y - y, 2) +
			pow(pos_z - z, 2)
		)
	) return true;
	return false;
}

void submarine::Move(GLdouble d) {
  // if (d > 0) tetha_heli += 0.2;
  // else tetha_heli -= 0.2;
	tetha_heli += 0.1;
	if (abs(tetha_paddle) < 0.02) {
		//straight

		MoveX(d*cos(tetha_center));
		MoveY(d*sin(tetha_center));

	} else {
		// curve
		GLfloat r;
		GLfloat move_delta;
		r = radius/tan(-tetha_paddle);
		move_delta = -d/r;
		GLfloat cx, cy, c_angle, s_angle;
		tetha_center-=move_delta;
		c_angle = -M_PI_2 + tetha_center;
		s_angle = M_PI_2 + tetha_center + move_delta;
		cx = pos_x + r*(cos(c_angle));
		cy = pos_y + r*(sin(c_angle));

		setPosX(cx + r*cos(s_angle));
		setPosY(cy + r*sin(s_angle));
	}
}

void submarine::MoveX(GLfloat dx) {
	pos_x += dx;
}

void submarine::MoveY(GLfloat dy) {
	pos_y += dy;
}

void submarine::updateHeli() {
  tetha_heli += 0.02;
}

void submarine::RotateTethaCenter(GLfloat ang) {
	setTethaCenter(getTethaCenter() + ang * M_PI / 180);
}

void submarine::RotateTethaHeli(GLfloat ang) {
	setTethaHeli(getTethaHeli() + ang * M_PI / 180);
}

void submarine::DesenhaSubmarine(GLfloat x, GLfloat y, GLfloat radius, GLfloat cR,
		GLfloat cG, GLfloat cB) {

	glColor3f(cR, cG, cB);
	glPushMatrix();
		glTranslatef(x, y, 0.0);
		glRotatef(tetha_center * (180 / M_PI), 0.0, 0.0, 1.0);

		//DesenhaCircle(0.0, 0.0, radius, radius, 0.8, 0.8, 0.8, 360); // circle for collision
		DesenhaCircle(0.0, 0.0, radius, radius / 3, cR, cG, cB, 360); // ellipse of submarine
		glPushMatrix();
			glTranslatef(0.0, -radius / 3, 0.0);
			DesenhaRect(center, radius / 3, radius / 5, 0.0, 0.0, cB*0.5); // black side1
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0, radius/3, 0.0);
			DesenhaRect(center, radius / 3, radius / 5, 0.0, 0.0, cB*0.5); // black side2
		glPopMatrix();
		glPushMatrix();
			glTranslatef(radius - radius / 5, 0.0, 0.0);
			DesenhaCircle(0.0, 0.0, radius/12, radius/12, 0.0, 0.0, cB*0.5, 360); // underwater torpedo
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0, - radius / 6, 0.0);
			DesenhaCircle(0.0, 0.0, radius/12, radius/12, 1.0, 0.0, cB, 360); // overwater torpedo
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-radius, 0.0, 0.0);
			glRotatef(tetha_paddle * (180 / M_PI), 0, 0, 1);
			DesenhaRect(right_center, radius / 4, radius / 10, 0.0, 0.0, cB*0.5);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-radius, 0.0, 0.0);
			glRotatef(90, 0, 0, 1);
			glRotatef(tetha_heli, 0, 1, 0); // rotation moving
			glRotatef(45, 1, 0, 0); // rotation for further usage (3d inclination of helipad)
			DesenhaTriangle(0.0, 0.0, radius/2, 1.0, 1.0, cB*0.5);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-radius, 0.0, 0.0);
			glRotatef(90, 0, 0, 1);
			glRotatef(tetha_heli, 0, 1, 0); // rotation for moving
			glRotatef(45, 1, 0, 0); // rotation for further usage (3d inclination of helipad)
			DesenhaTriangle(0.0, 0.0, -radius/2, 1.0, 1.0, cB*0.5);
		glPopMatrix();
	glPopMatrix();
}


void submarine::DesenhaCircle(GLfloat x, GLfloat y, GLfloat radx, GLfloat rady,
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

void submarine::DesenhaRect(REFERENCE ref, GLfloat base, GLfloat altura,
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

void submarine::DesenhaTriangle(GLfloat x, GLfloat y, GLfloat l, GLfloat cR,
		GLfloat cG, GLfloat cB) {
	glColor3f(cR, cG, cB);
	glPushMatrix();
	glBegin(GL_POLYGON);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(cos(60) * l, sin(60) * l, 0.0);
		glVertex3f(cos(60) * l, sin(-60) * l, 0.0);
//	glVertex3f(x, y, 0.0);
//	glVertex3f(x + l * cos(M_PI / 6), y + l * sin(M_PI / 6), 0.0);
//	glVertex3f(x + l * cos(-M_PI / 6), y + l * sin(-M_PI / 6), 0.0);
	glEnd();
	glPopMatrix();
}

void submarine::RotateTethaPaddle(GLfloat ang) {
	if (abs(tetha_paddle + ang * M_PI / 180) <= M_PI_4) {
		tetha_paddle += ang * M_PI / 180;
		setDirectionSubX(cos(tetha_paddle));
		setDirectionSubY(sin(tetha_paddle));
	}
}

GLfloat submarine::getVelTiro() const {
	return velTiro;
}

void submarine::setVelTiro(GLfloat vel_tiro) {
	velTiro = vel_tiro;
}

GLfloat submarine::getVel() const {
	return vel;
}

void submarine::setVel(GLfloat v) {
	vel = v;
}

bool submarine::getCanMove() const {
	return can_move;
}

void submarine::setCanMove(bool s) {
	can_move = s;
}

bool submarine::getMovingZAxis() const {
	return moving_z_axis;
}

void submarine::setMovingZAxis(bool s) {
	moving_z_axis = s;
}

GLint submarine::getSubmerginStatus() const {
	return submergin_status;
}

void submarine::setSubmerginStatus(GLint i) {
	submergin_status = i;
}

void submarine::submerge(int milisec) {
	if (!getMovingZAxis()) {
		return;
	} else {
		double sec = (float) milisec / 1000;
		GLfloat maxRadius = getMaxRadius();
		GLfloat a = maxRadius / 2;
		if (submergin_status == 1) { // on water level
			setRadius((-1)*a*sec + maxRadius);
			setR(0.0);
			setG(1.0);
			setB(1.0);
			if (getRadius() <= getMaxRadius()/2) {
				setSubmerginStatus(-1);
				setMovingZAxis(false);
			}
		} else if (submergin_status == -1) {
			setRadius(a*sec + maxRadius/2);
      if (getRadius() >= getMaxRadius()) {
				setR(0.0);
				setG(1.0);
				setB(0.0);
				setSubmerginStatus(1);
				setMovingZAxis(false);
			}
		}
	}

}

} /* namespace submarine */
