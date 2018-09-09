#include "enemy.hpp"
#include <math.h>
#define PI 3.1415926535897932384626433832795

void Enemy::DesenhaRect(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B) {
  glColor3f(R, G, B);

  glBegin(GL_QUADS);
    glVertex2f(-width/2, height);
    glVertex2f(-width/2, 0.0);
    glVertex2f(width/2, 0.0);
    glVertex2f(width/2, height);
  glEnd();
}

void Enemy::DesenhaCircle(GLfloat x, GLfloat y, GLfloat rad, GLfloat R, GLfloat G, GLfloat B) {
  glColor3f(R, G, B);

  glBegin(GL_POLYGON);
     for(double i = 0; i < 2 * PI; i += PI / 12)
       glVertex3f(cos(i) * rad + x, sin(i) * rad + y, 0.0);
  glEnd();
}

void Enemy::DesenhaEnemy(GLfloat x, GLfloat y, GLfloat pos_z, GLfloat radius, GLfloat R, GLfloat G, GLfloat B) {
//void Enemy::DesenhaEnemy(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat theta1, GLfloat theta2, GLfloat theta3) {
  glPushMatrix();
    glTranslatef(x, y, 0.0);
    DesenhaCircle(x, y, radius, R, G, B);
  glPopMatrix();
}

void Enemy::MoveX(GLfloat dx) {
  pos_x += dx;
}

void Enemy::MoveY(GLfloat dy) {
  pos_y += dy;
}

void Enemy::MoveZ(GLfloat dz) {
  pos_z += dz;
}

void Enemy::setPosX(GLfloat x) {
  pos_x = x;
}

GLfloat Enemy::getPosX() {
  return pos_x;
}

void Enemy::setPosY(GLfloat y) {
  pos_y = y;
}

GLfloat Enemy::getPosY() {
  return pos_y;
}

void Enemy::setPosZ(GLfloat z) {
  pos_z = z;
}

GLfloat Enemy::getPosZ() {
  return pos_z;
}

void Enemy::setRadius(GLfloat r) {
  radius = r;
}

GLfloat Enemy::getRadius() {
  return radius;
}

void Enemy::setColor(GLfloat colors[]) {
  for (int i = 0; i < 3; i++) rgb[i] = colors[i];
}

GLfloat * Enemy::getColor() {
  return rgb;
}

void Enemy::setId(GLint ident) {
  id = ident;
}

GLint Enemy::getId() {
  return id;
}
