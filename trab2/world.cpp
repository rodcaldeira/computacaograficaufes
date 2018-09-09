#include "world.hpp"
#include <math.h>
#define PI 3.1415926535897932384626433832795

void Island::DesenhaCircle(GLfloat x, GLfloat y, GLfloat rad, GLfloat R, GLfloat G, GLfloat B) {
  glColor3f(R, G, B);

  glBegin(GL_POLYGON);
     for(double i = 0; i < 2 * PI; i += PI / 12)
       glVertex3f(cos(i) * rad, sin(i) * rad, 0.0);
  glEnd();
}

void Island::DesenhaIsland(GLfloat x, GLfloat y, GLfloat pos_z, GLfloat radius, GLfloat R, GLfloat G, GLfloat B) {
//void Player::DesenhaPlayer(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat theta1, GLfloat theta2, GLfloat theta3) {
  glPushMatrix();
    glTranslatef(x, y, 0.0);
    DesenhaCircle(x, y, radius, R, G, B);
  glPopMatrix();
}

void Island::setPosX(GLfloat x) {
  pos_x = x;
}

GLfloat Island::getPosX() {
  return pos_x;
}

void Island::setPosY(GLfloat y) {
  pos_y = y;
}

GLfloat Island::getPosY() {
  return pos_y;
}

void Island::setPosZ(GLfloat z) {
  pos_z = z;
}

GLfloat Island::getPosZ() {
  return pos_z;
}

void Island::setRadius(GLfloat r) {
  radius = r;
}

GLfloat Island::getRadius() {
  return radius;
}

void Island::setColor(GLfloat colors[]) {
  for (int i = 0; i < 3; i++) rgb[i] = colors[i];
}

GLfloat * Island::getColor() {
  return rgb;
}

void Island::setId(GLint ident) {
  id = ident;
}

GLint Island::getId() {
  return id;
}

void Sea::DesenhaCircle(GLfloat x, GLfloat y, GLfloat rad, GLfloat R, GLfloat G, GLfloat B) {
  glColor3f(R, G, B);

  glBegin(GL_POLYGON);
     for(double i = 0; i < 2 * PI; i += PI / 36)
       glVertex3f(cos(i) * rad, sin(i) * rad, 0.0);
  glEnd();
}

void Sea::DesenhaSea(GLfloat x, GLfloat y, GLfloat pos_z, GLfloat radius, GLfloat R, GLfloat G, GLfloat B) {
//void Player::DesenhaPlayer(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat theta1, GLfloat theta2, GLfloat theta3) {
  glPushMatrix();
    glTranslatef(x, y, 0.0);
    DesenhaCircle(x, y, radius, R, G, B);
  glPopMatrix();
}

void Sea::setPosX(GLfloat x) {
  pos_x = x;
}

GLfloat Sea::getPosX() {
  return pos_x;
}

void Sea::setPosY(GLfloat y) {
  pos_y = y;
}

GLfloat Sea::getPosY() {
  return pos_y;
}

void Sea::setPosZ(GLfloat z) {
  pos_z = z;
}

GLfloat Sea::getPosZ() {
  return pos_z;
}

void Sea::setRadius(GLfloat r) {
  radius = r;
}

GLfloat Sea::getRadius() {
  return radius;
}

void Sea::setColor(GLfloat colors[]) {
  for (int i = 0; i < 3; i++) rgb[i] = colors[i];
}

GLfloat * Sea::getColor() {
  return rgb;
}

void Sea::setId(GLint ident) {
  id = ident;
}

GLint Sea::getId() {
  return id;
}
