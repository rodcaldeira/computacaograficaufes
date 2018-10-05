#include "player.hpp"
#include <math.h>
#include <cstddef>
#include <iostream>
#include <sstream>
#define PI 3.1415926535897932384626433832795

void Player::DesenhaRect(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B) {
  glColor3f(R, G, B);

  glBegin(GL_QUADS);
    glVertex2f(-width/2, height);
    glVertex2f(-width/2, 0.0);
    glVertex2f(width/2, 0.0);
    glVertex2f(width/2, height);
  glEnd();
}

void Player::DesenhaCircle(GLfloat x, GLfloat y, GLfloat rad, GLfloat R, GLfloat G, GLfloat B) {
  glColor3f(R, G, B);

  glBegin(GL_POLYGON);
     for(double i = 0; i < 2 * PI; i += PI / 12)
       glVertex3f(cos(i) * rad, sin(i) * rad, 0.0);
  glEnd();
}

void Player::DesenhaTriangle(GLfloat x, GLfloat y, GLfloat l, GLfloat R, GLfloat G, GLfloat B) {
  glColor3f(R, G, B);

  glBegin(GL_POLYGON);
    //for(double i = 0; i <= 2 * PI; i += PI/12)
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(cos(60) * l, sin(60) * l, 0.0);
    glVertex3f(cos(60) * l, sin(-60) * l, 0.0);
  glEnd();
}

void Player::DesenhaElipse(GLfloat cx, GLfloat cy, GLfloat radx, GLfloat rady, GLfloat R, GLfloat G, GLfloat B) {
  glColor3f(R, G, B);

  glBegin(GL_POLYGON);
    for(double i = 0; i < 2 * PI; i += PI/12)
      glVertex3f(cos(i)*radx, sin(i)*rady, 0.0);
  glEnd();
  return;
}

void Player::DesenhaPlayer(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLfloat R, GLfloat G, GLfloat B) {
//void Player::DesenhaPlayer(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat theta1, GLfloat theta2, GLfloat theta3) {
  glPushMatrix();
    glTranslatef(x, y, 0.0);
    //glRotatef(tethaSub, 0, 0, 1);

    DesenhaCircle(x, y, radius, 0.8, 0.8, 0.8);
    DesenhaElipse(x, y, radius/2.5, radius, 0.0, 0.7, 0.0);
    glPushMatrix();
      glTranslatef(radius/1.9, -radius/4, 0.0);
      DesenhaRect(radius/2, radius/3, 0.0, 0.0, 0.0);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(-radius/1.9, -radius/4, 0.0);
      DesenhaRect(radius/2, radius/3, 0.0, 0.0, 0.0);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0.0, -radius, 0.0);
      glRotatef(tethaHeli, 0, 1, 0); // rotation moving
      glRotatef(60, 1, 0, 0); // rotation for further usage (3d inclination of helipad)
      DesenhaTriangle(0.0, 0.0, radius/3, 1.0, 1.0, 0.0);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0.0, -radius, 0.0);
      glRotatef(tethaHeli, 0, 1, 0); // rotation for moving
      glRotatef(60, 1, 0, 0); // rotation for further usage (3d inclination of helipad)
      DesenhaTriangle(0.0, 0.0, -radius/3, 0.0, 1.0, 1.0);
    glPopMatrix();
    glPushMatrix();
      // desenhar leme
      glTranslatef(0.0, -radius, 0.0);
      glRotatef(tethaLeme+180, 0, 0, 1);
      DesenhaRect(radius/3, radius/12, 0.0, 0.0, 0.0);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(radius/4, 0.0, 0.0);
      DesenhaCircle(0.0, 0.0, radius/10, 1.0, 0.0, 0.0);
    glPopMatrix();
    glPushMatrix();
      // black circle
      glTranslatef(0.0, (radius - radius/6), 0.0);
      DesenhaCircle(0.0, 0.0, radius/10, 0.0, 0.0, 0.0);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(radius/(2*tan( (90-tethaLeme)*PI/180 )), 0.0, 0.0);
      glColor3f(0.0, 0.0, 0.0);
      glPointSize(3);
      glBegin(GL_POINTS);
        glVertex3f(0.0, 0.0, 0);
      glEnd();

      glRotatef(tethaLeme, 0, 0, 1);
      //DesenhaCircle(0.0, 0.0, sqrt(pow(pos_x - trajectory_x, 2) + pow(pos_y - trajectory_y, 2)), 0.9, 0.0, 0.3);
    glPopMatrix();

  glPopMatrix();
}
// frontWheel.x += carSpeed*(cos(carHeading+steeringAngle));
//     frontWheel.y += 0;
//     frontWheel.z += carSpeed*(sin(carHeading+steeringAngle));


void Player::setTrajectoryX(GLfloat dx) {
  trajectory_x = dx;
}

GLfloat Player::getTrajectoryX() {
  return trajectory_x;
}

void Player::setTrajectoryY(GLfloat dy) {
  trajectory_y = dy;
}

GLfloat Player::getTrajectoryY() {
  return trajectory_y;
}

void Player::Move(GLint dir) {
  double r = sqrt( pow(pos_x - trajectory_x, 2) + pow(pos_y - trajectory_y, 2));
  double delta = vel/r;
  delta = delta*(PI/180);
  tethaSub += dir*asin(delta);

  if (abs(tethaSub - tethaLeme) == 0.0) {
    MoveY(pos_y + vel);
  }
  std::cout << "r " << r <<
            " vel " << vel <<
            " delta " << delta <<
            " asin(delta) " << asin(delta) <<
            " acos(delta) " << acos(delta) <<
            " tethaSub " << tethaSub << std::endl;

  MoveX(trajectory_x + r*cos(tethaSub));
  MoveY(trajectory_y + r*sin(tethaSub));
  // double cat_opo = abs(pos_y - trajectory_y);
  // double cat_adj = abs(pos_x - trajectory_x);
  if (dir == 1) { // move forward
    std::cout << "move clockwise" << std::endl;
  } else if (dir == -1) { // move backwards
    std::cout << "move counter-clockwise" << std::endl;
  }
}

void Player::MoveX(GLfloat dx) {
  tethaHeli += 0.04;
  pos_x = dx;
  //pos_x += dx;
}

void Player::MoveY(GLfloat dy) {
  tethaHeli += 0.04;
  pos_y = dy;
//  pos_y += dy;
}

void Player::MoveZ(GLfloat dz) {
  pos_z += dz;
}

void Player::setPosX(GLfloat x) {
  pos_x = x;
}

void Player::setVelTiro(GLfloat v) {
  velTiro = v;
}

GLfloat Player::getVelTiro() {
  return velTiro;
}

void Player::setVel(GLfloat v) {
  vel = v;
}

GLfloat Player::getVel() {
  return vel;
}

void Player::MoveLeme(GLfloat a) {

  if (abs(tethaLeme + a) <= 45) {

    tethaLeme += a;
  }
  std::cout << "radius:" << radius << " "
            << "pos_x " << pos_x << " "
            << "pos_y " << pos_y << " "
            << "tSub rad " << tethaSub*PI/180 << " "
            << "sinr(tSub)" << sin(tethaSub*PI/180) << " "
            << "cosr(tSub)" << cos(tethaSub*PI/180) << std::endl;
  double alfaSub = tethaSub*PI/180;
  double alfaLeme = (tethaSub + tethaLeme)*PI/180;
  double x_leme = sin(tethaSub*PI/180)*radius+pos_x;
  double y_leme = cos(tethaSub*PI/180)*radius+pos_y;

  double m_sub = tan(alfaSub);
  double m_leme = tan(alfaLeme);

  trajectory_x = (y_leme - pos_y - m_leme*x_leme + m_sub*pos_x) / (m_sub - m_leme);
  trajectory_y = m_leme * (trajectory_x - x_leme) + y_leme;
  std::cout << pos_x << " " << pos_y << std::endl;
  std::cout << x_leme << " " << y_leme << std::endl;
  std::cout << trajectory_x << " " << trajectory_y << std::endl;
  //trajectory_x = radius/(2*tan( (90-tethaLeme)*PI/180 ));
    // if (tethaLeme == 0.0) {
    //   trajectory_x = 10000.0;
    // } else {
    //
    //   trajectory_x = radius/(2*tan( (90-tethaLeme)*PI/180 ));
    // }
  //std::cout << tethaLeme << " " << trajectory_x << " " << radius/(2*tan( (90-tethaLeme)*PI/180 )) << std::endl;
}

void Player::setTethaLeme(GLfloat t) {
  tethaLeme = t;
}

GLfloat Player::getTethaLeme() {
  return tethaLeme;
}

GLfloat Player::getPosX() {
  return pos_x;
}

void Player::setPosY(GLfloat y) {
  pos_y = y;
}

GLfloat Player::getPosY() {
  return pos_y;
}

void Player::setPosZ(GLfloat z) {
  pos_z = z;
}

GLfloat Player::getPosZ() {
  return pos_z;
}

void Player::setRadius(GLfloat r) {
  radius = r;
}

GLfloat Player::getRadius() {
  return radius;
}

void Player::setMaxRadius(GLfloat r) {
  max_radius = r;
}

GLfloat Player::getMaxRadius() {
  return max_radius;
}

void Player::setColor(GLfloat colors[]) {
  for (int i = 0; i < 3; i++) rgb[i] = colors[i];
}

GLfloat * Player::getColor() {
  return rgb;
}

void Player::setId(GLint ident) {
  id = ident;
}

GLint Player::getId() {
  return id;
}

void Player::setSubmerge(GLint i) {
  submergin_status = i;
}

GLint Player::getSubmerge() {
  return submergin_status;
}

void Player::setMovingZ(bool flag) {
  moving_z_axis = flag;
}

bool Player::getMovingZ() {
  return moving_z_axis;
}

void Player::setTethaHeli(GLfloat t) {
  tethaHeli = t;
}

GLfloat Player::getTethaHeli() {
  return tethaHeli;
}

void Player::submergeTime(int milisec) {
  if (!getMovingZ()) {
    return;
  } else {
    double sec = (float) milisec / 1000;
    GLfloat maxRadius = getMaxRadius();
    GLfloat a = maxRadius/2;
    if (submergin_status == 1) {// on water level
      setRadius((-1)*a*sec + maxRadius);
      GLfloat colors[3] = {0.0, 1.0, 1.0};
      setColor(colors);
      if (getRadius() <= getMaxRadius()/2) {
        setSubmerge(-1);
        setMovingZ(false);
      }
    } else if (submergin_status == -1){
      setRadius(a*sec + maxRadius/2);
      if (getRadius() >= getMaxRadius()) {
        GLfloat colors[3] = {0.0, 1.0, 0.0};
        setColor(colors);
        setSubmerge(1);
        setMovingZ(false);
      }
    }
  }
}


void Player::updatePlayer() {
  setTethaHeli(getTethaHeli()+0.01);
}

// NOT USING ELIPSED_TIME FOR FURTHER REFERENCE
void Player::submerge() {
  if (!getMovingZ()) {
    return;
  } else {
    if (submergin_status == 1) { // on water level
    // stuff to go under
      if (getRadius() > getMaxRadius()/2) {
        setRadius(getRadius() - 0.003);
        GLfloat colors[3] = {0.0, 1.0, 1.0};
        setColor(colors);
        return;
      } else {
        setSubmerge(-1);
        setMovingZ(false);
      }
    } else if (submergin_status == -1) { // under water level
      // do stuff to go up
      if (getRadius() < getMaxRadius()) {
        setRadius(getRadius() + 0.003);
        return;
      } else {
        setSubmerge(1);
        GLfloat colors[3] = {0.0, 1.0, 0.0};
        setColor(colors);
        setMovingZ(false);
      }
    }
  }
}
