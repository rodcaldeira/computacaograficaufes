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

void Island::DesenhaIsland(GLfloat x, GLfloat y, GLfloat z,
                           GLfloat radius, GLfloat tower_radius,
                           GLfloat height, 
                           GLfloat R, GLfloat G, GLfloat B) {
//void Player::DesenhaPlayer(GLfloat x, GLfloat y, GLfloat thetaWheel, GLfloat theta1, GLfloat theta2, GLfloat theta3) {
  glPushMatrix();
    glTranslatef(x, y, z);
    DesenhaCylinder(x, y, z, radius, height, R, G, B);
    DesenhaCircle(x, y, radius, R, G, B);
    if (alive) {
      DesenhaSphere(x, y, z, tower_radius, 1.0, 1.0, 0.0);
      //DesenhaCircle(x, y, tower_radius, 1.0, 1.0, 0.0);
    }
  glPopMatrix();
}

void Island::DesenhaCylinder(GLfloat x, GLfloat y, GLfloat z, GLfloat rad, GLfloat h, GLfloat R, GLfloat G, GLfloat B) {
  glPushMatrix();
    GLfloat materialEmisison[] = {0.1, 0.1, 0.1, 1.0};
    GLfloat materialColor[] = {0.0, 0.0, 0.0, 1.0};
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
    GLUquadricObj *quadric=gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluQuadricOrientation(quadric,GLU_OUTSIDE);
    gluCylinder(quadric, radius, radius, height, 32, 1);
    glTranslatef(0.0, 0.0, height);
    gluDisk(quadric, 0.0, radius, 32, 10);
    gluDeleteQuadric(quadric);
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialColorYellow);
    glutSolidSphere(tower_perc, 32, 10);
  glPopMatrix();
}

void Island::DesenhaSphere(GLfloat x, GLfloat y, GLfloat z, GLfloat tower_radius, GLfloat R, GLfloat G, GLfloat B) {
  glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(1.0, 1.0, 0.0);
    glutSolidSphere(tower_radius, 32, 32);
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

void Island::setTowerPerc(GLfloat s) {
  tower_perc = s;
}

GLfloat Island::getTowerPerc() {
  return tower_perc;
}
  
void Island::setAlive(bool status) {
  alive = status;
}

bool Island::getAlive() {
  return alive;
}

void Sea::DesenhaCircle(GLfloat x, GLfloat y, GLfloat rad, GLfloat R, GLfloat G, GLfloat B) {
  glColor3f(R, G, B);

  glBegin(GL_POLYGON);
     for(double i = 0; i < 2 * PI; i += PI / 36)
       glVertex3f(cos(i) * rad, sin(i) * rad, 0.0);
  glEnd();
}

void Sea::DesenhaCylinder(GLfloat x, GLfloat y, GLfloat z, GLfloat rad, GLfloat h, GLfloat R, GLfloat G, GLfloat B, GLuint texture) {
  glPushMatrix();
    GLfloat materialEmisison[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat materialColor[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat mat_color_blue_alpha[] = {0.0, 0.0, 1.0, 1.0};
    //GLfloat materialColorAMB[] = {1.0, 1.0, 0.0, 1.0};
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess[] = { 128 };
    //glMaterialfv()
    glMaterialfv(GL_BACK, GL_EMISSION, materialEmisison);
    glMaterialfv(GL_BACK, GL_AMBIENT, materialColor);
    glMaterialfv(GL_BACK, GL_DIFFUSE, materialColor);
    glMaterialfv(GL_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_BACK, GL_SHININESS, mat_shininess);
    glColor3f(0.0, 0.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    GLUquadricObj *quadric=gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluQuadricOrientation(quadric,GLU_INSIDE);
    gluCylinder(quadric, radius, radius, height, 32, 1);
    glDisable(GL_TEXTURE_2D);
    //draw the midcap
    gluQuadricOrientation(quadric,GLU_INSIDE);
    gluDisk( quadric, 0.0, radius, 32, 1);
    glTranslatef( 0,0,height/2 );


    //draw the second cap
    glMaterialfv(GL_FRONT, GL_EMISSION, materialEmisison);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_color_blue_alpha);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_color_blue_alpha);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    gluQuadricOrientation(quadric,GLU_OUTSIDE);
    glMaterialfv(GL_BACK, GL_AMBIENT, mat_color_blue_alpha );
    gluDisk( quadric, 0.0, radius, 32, 1);
    glPopMatrix();
    
    
    //gluDisk(quadric, 0.0, radius, 32, 10);
    
    gluDeleteQuadric(quadric);
  glPopMatrix();
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
