#include "player.hpp"
#include <math.h>
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
       glVertex3f(cos(i) * rad + x, sin(i) * rad + y, 0.0);
  glEnd();
}

void Player::DesenhaPlayer(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLfloat R, GLfloat G, GLfloat B) {
//void Player::DesenhaPlayer(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat theta1, GLfloat theta2, GLfloat theta3) {
  glPushMatrix();
    glTranslatef(x, y, 0.0);
    DesenhaCircle(x, y, radius, R, G, B);
  glPopMatrix();
}

void Player::MoveX(GLfloat dx) {
  pos_x += dx;
}

void Player::MoveY(GLfloat dy) {
  pos_y += dy;
}

void Player::MoveZ(GLfloat dz) {
  pos_z += dz;
}

void Player::setPosX(GLfloat x) {
  pos_x = x;
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
