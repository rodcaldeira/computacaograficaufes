#ifndef Enemy_HPP
#define Enemy_HPP
#include <GL/gl.h>
#include <GL/glu.h>
// dimensions
#define paddleHeight 80
#define paddleWidth 10
#define baseHeight 40
#define baseWidth 100
#define radiusWheel 30

class Enemy {
  GLfloat id;
  GLfloat pos_x;
  GLfloat pos_y;
  GLfloat pos_z;
  GLfloat radius;
  GLfloat rgb[3];


private:
  void DesenhaRect(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B);
  void DesenhaCircle(GLfloat x, GLfloat y, GLfloat rad, GLfloat R, GLfloat G, GLfloat B);
  void DesenhaEnemy(GLfloat x, GLfloat y, GLfloat pos_z, GLfloat radius, GLfloat R, GLfloat G, GLfloat B);//, GLfloat theta2, GLfloat theta3);

public:
  Enemy(GLint ident, GLfloat colors[3], GLfloat x = 0.0, GLfloat y = 0.0, GLfloat z = 0.0, GLfloat rad = 1.0) {
    id = ident;
    pos_x = x;
    pos_y = y;
    pos_z = z;
    radius = rad;
    rgb[0] = colors[0];
    rgb[1] = colors[1];
    rgb[2] = colors[2];
  }

  void Desenha() {
    DesenhaEnemy(pos_x, pos_y, pos_z, radius, rgb[0], rgb[1], rgb[2]);
  }

  void MoveX(GLfloat dx);

  void MoveY(GLfloat dy);

  void MoveZ(GLfloat dz);

  void setPosX(GLfloat x);

  GLfloat getPosX();

  void setPosY(GLfloat y);

  GLfloat getPosY();

  void setPosZ(GLfloat z);

  GLfloat getPosZ();

  void setRadius(GLfloat r);

  GLfloat getRadius();

  void setColor(GLfloat colors[]);

  GLfloat * getColor();

  void setId(GLint ident);

  GLint getId();

};
#endif /* END Enemy_hpp */
