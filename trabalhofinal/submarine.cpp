/*
 * submarine.cpp
 *
 *  Created on: 22 de set de 2018
 *      Author: rodcaldeira
 */

#include "submarine.h"
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>
#include <iostream>


namespace Submarine {

submarine::submarine() {
	setPosX(0.0);
	setPosY(0.0);
	setPosZ(1.0);
	setRadius(40.0);
	setTethaCenter(90.0);
	setTethaHeli(0.0);
	setTethaPaddle(0.0);
	setTethaCanon(0.0);
	setTethaCanonZ(0.0);
	setMovingZAxis(false);
	setCanMove(true);
	setToDelete(false);
	setSubmerginStatus(1);
	setDirectionSubX(cos(getTethaCenter()));
	setDirectionSubY(sin(getTethaCenter()));
	setDirectionSteeringX(cos(getTethaPaddle()));
	setDirectionSteeringY(sin(getTethaPaddle()));
}

void submarine::setTethaCanon(GLfloat a) {
	tetha_canon = a;
}

GLfloat submarine::getTethaCanon() {
	return tetha_canon;
}

void submarine::setTethaCanonZ(GLfloat a) {
	tetha_canon_z = a;
}

GLfloat submarine::getTethaCanonZ() {
	return tetha_canon_z;
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

GLint submarine::getId() const {
	return id;
}

void submarine::setId(GLint x) {
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

bool submarine::getToDelete() const {
	return to_delete;
}

void submarine::setToDelete(bool status) {
	to_delete = status;
}

submarine::~submarine() {
	// TODO Auto-generated destructor stub
}

bool submarine::checkHit(GLfloat x, GLfloat y, GLfloat z, GLfloat r) {
	if (sqrt(pow(pos_x - x, 2) + pow(pos_y - y, 2) + pow(pos_z - z, 2)) < radius + r) return true;
	return false;
}

void submarine::Move(GLdouble d) {
  // if (d > 0) tetha_heli += 0.2;
  // else tetha_heli -= 0.2;
	tetha_heli += 0.1*fabs(d);
	if (fabs(tetha_paddle) < 0.02) {
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

void submarine::MoveCanon(GLfloat da) {
	if (da > 0) {
		if (tetha_canon <= 30.0) {
			tetha_canon += da;
		} else {
			tetha_canon = 30.0;
		}
	} else {
		if (tetha_canon >= -30.0) {
			tetha_canon -= da;
		} else {
			tetha_canon = -30.0;
		}
	}
}

void submarine::MoveCanonZ(GLfloat daz) {
	if (daz > 0) {
		if (tetha_canon_z <= 30.0) {
			tetha_canon_z += daz;
		} else {
			tetha_canon_z = 30.0;
		}
	} else {
		if (tetha_canon_z >= -30.0) {
			tetha_canon_z -= daz;
		} else {
			tetha_canon_z = -30.0;
		}
	}
}

void submarine::incTicks() {
	ticks++;
}

GLint submarine::getTicks() {
	return ticks;
}

void submarine::setTicks(GLint t) {
	ticks = t;
}

void submarine::MoveX(GLfloat dx) {
	pos_x += dx;
}

void submarine::MoveY(GLfloat dy) {
	pos_y += dy;
}

void submarine::updateHeli(GLdouble timeDiff) {
  tetha_heli += 0.02*timeDiff;
}

void submarine::RotateTethaCenter(GLfloat ang) {
	setTethaCenter(getTethaCenter() + ang * M_PI / 180);
}

void submarine::RotateTethaHeli(GLfloat ang) {
	setTethaHeli(getTethaHeli() + ang * M_PI / 180);
}

void submarine::DesenhaSubmarine(GLfloat x, GLfloat y, GLfloat radius, GLfloat cR,
		GLfloat cG, GLfloat cB) {
	GLfloat materialEmisison[] = {0.1, 0.1, 0.1, 1.0};
	GLfloat materialColor[] = {cR, cG, cB, 1.0};
	//GLfloat materialColorAMB[] = {1.0, 1.0, 0.0, 1.0};
	GLfloat mat_specular[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat mat_shininess[] = { 128 };
	glMaterialfv(GL_FRONT, GL_EMISSION, materialEmisison);
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glColor3f(cR, cG, cB);
	glPushMatrix();
		glTranslatef(x, y, pos_z);
		glRotatef(tetha_center * (180 / M_PI), 0.0, 0.0, 1.0);

		//DesenhaCircle(0.0, 0.0, radius, radius, 0.8, 0.8, 0.8, 360); // circle for collision
		glPushMatrix();
			glScalef(1.0, 0.3, 0.3);
			glutSolidSphere(radius, 20, 10);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(radius, 0.0, 0.0);
			glColor3f(0.0, 0.0, 0.0);
			glRotatef(tetha_canon, 0, 0, 1);
			glRotatef(90, 0.0, 1.0, 0.0);
			glRotatef(tetha_canon_z, 0, 1, 0);
			// std::cout << tetha_canon << std::endl;
			GLUquadricObj *quadric=gluNewQuadric();
			gluQuadricNormals(quadric, GLU_SMOOTH);
			gluQuadricOrientation(quadric,GLU_OUTSIDE);
			gluCylinder(quadric, radius/30, radius/30, radius/5, 32, 1);
			gluDeleteQuadric(quadric);
		glPopMatrix();
		
		glPushMatrix();
			glTranslatef(-radius, 0.0, 0.0);
			glRotatef(tetha_paddle * (180 / M_PI), 0, 0, 1);
			glRotatef(90, 0, 0, 1);
			glScalef(0.02, 1.0, 0.3);
			glPushMatrix();
				glTranslatef(0, radius/3, 0);
				glutSolidCube(radius/2);
			glPopMatrix();
		glPopMatrix();

		glPushMatrix();
			
			glTranslatef(-radius, 0.0, 0.0);
			glRotatef(-90, 0, 0, 1);
			glRotatef(tetha_heli, 0, 1, 0); // rotation moving
			glRotatef(45, 1, 0, 0); // rotation for further usage (3d inclination of helipad)
			glScalef(1.0, 0.05, 0.4);
			glPushMatrix();
				glTranslatef(radius/5, 0.0, 0.0);
				glutSolidCube(radius/3);
			glPopMatrix();
		glPopMatrix();
		glPushMatrix();
			
			glTranslatef(-radius, 0.0, 0.0);
			glRotatef(-90, 0, 0, 1);
			glRotatef(tetha_heli, 0, 1, 0); // rotation for moving
			glRotatef(-45, 1, 0, 0); // rotation for further usage (3d inclination of helipad)
			glScalef(1.0, 0.05, 0.4);
			glPushMatrix();
				glTranslatef(-radius/5, 0.0, 0.0);
				glutSolidCube(radius/3);
			glPopMatrix();
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
	if (fabs(tetha_paddle + ang * M_PI / 180) <= M_PI_4) {
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

GLfloat submarine::getMinHeight() {
	return min_height;
}
void submarine::setMinHeight(GLfloat h) {
	min_height = h;
}
GLfloat submarine::getMaxHeight() {
	return max_height;
}
void submarine::setMaxHeight(GLfloat h) {
	max_height = h;
}

void submarine::MoveZ(GLfloat dz) {
	pos_z += dz;
}

void submarine::submerge(int milisec) {
	if (milisec > 0) {
		if (pos_z >= max_height) pos_z = 80.0f;
	} else if (milisec < 0) {
		if (pos_z <= min_height) pos_z = 10.0f;
	}
	//MoveZ((vel/5)*milisec);
	MoveZ(milisec*(vel/5));
	
	// if (!getMovingZAxis()) {
	// 	return;
	// } else {
	// 	double sec = (float) milisec / 1000;
	// 	GLfloat maxRadius = getMaxRadius();
	// 	GLfloat a = maxRadius / 2;
	// 	if (submergin_status == 1) { // on water level
	// 		setRadius((-1)*a*sec + maxRadius);
	// 		setR(0.0);
	// 		setG(1.0);
	// 		setB(1.0);
	// 		if (getRadius() <= getMaxRadius()/2) {
	// 			setSubmerginStatus(-1);
	// 			setMovingZAxis(false);
	// 		}
	// 	} else if (submergin_status == -1) {
	// 		setRadius(a*sec + maxRadius/2);
    //   if (getRadius() >= getMaxRadius()) {
	// 			setR(0.0);
	// 			setG(1.0);
	// 			setB(0.0);
	// 			setSubmerginStatus(1);
	// 			setMovingZAxis(false);
	// 		}
	// 	}
	// }

}

} /* namespace submarine */
