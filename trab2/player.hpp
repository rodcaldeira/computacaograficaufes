#ifndef Player_HPP
#define Player_HPP
#include <GL/gl.h>
#include <GL/glu.h>
// dimensions
#define paddleHeight 80
#define paddleWidth 10
#define baseHeight 40
#define baseWidth 100
#define radiusWheel 30

class Player {
  GLfloat id;
  GLfloat pos_x;
  GLfloat pos_y;
  GLfloat pos_z;
  GLfloat radius;
  GLfloat max_radius;
  GLfloat rgb[3];
  GLint submergin_status; // 0 - ongoing // 1 - water level // -1 - submerged
  bool can_move;
  bool moving_z_axis;


private:
  void DesenhaRect(GLint height, GLint width, GLfloat R, GLfloat G, GLfloat B);
  void DesenhaCircle(GLfloat x, GLfloat y, GLfloat rad, GLfloat R, GLfloat G, GLfloat B);
  void DesenhaPlayer(GLfloat x, GLfloat y, GLfloat pos_z, GLfloat radius, GLfloat R, GLfloat G, GLfloat B);//, GLfloat theta2, GLfloat theta3);

public:
  Player() {
    pos_x = 0;
    pos_y = 0;
    pos_z = 0;
    rgb[0] = 0.0;
    rgb[1] = 1.0;
    rgb[2] = 0.0;
    submergin_status = 1;
    moving_z_axis = false;
    can_move = true;
  }

  void Desenha() {
    DesenhaPlayer(pos_x, pos_y, pos_z, radius, rgb[0], rgb[1], rgb[2]);
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

  void setMaxRadius(GLfloat r);

  GLfloat getMaxRadius();

  void setColor(GLfloat colors[]);

  GLfloat * getColor();

  void setId(GLint ident);

  GLint getId();

  void setSubmerge(GLint i);

  GLint getSubmerge();

  void setMovingZ(bool f);

  bool getMovingZ();

  void submergeTime(int milisec);

  void submerge();

};
#endif /* END Player_hpp */
