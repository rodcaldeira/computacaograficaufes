#ifndef World_HPP
#define World_HPP
#include <GL/gl.h>
#include <GL/glu.h>
// dimensions

class Island {
  GLfloat id;
  GLfloat pos_x;
  GLfloat pos_y;
  GLfloat pos_z;
  GLfloat radius;
  GLfloat rgb[3];
  GLfloat tower_perc;
  bool alive;

private:
  void DesenhaCircle(GLfloat x, GLfloat y, GLfloat rad, GLfloat R, GLfloat G, GLfloat B);
  void DesenhaIsland(GLfloat x, GLfloat y, GLfloat pos_z, GLfloat radius, GLfloat tower_radius, GLfloat R, GLfloat G, GLfloat B);//, GLfloat theta2, GLfloat theta3);

public:
  Island(GLint ident, GLfloat colors[3], GLfloat x = 0.0, GLfloat y = 0.0, GLfloat z = 0.0, GLfloat rad = 1.0, GLfloat t_perc = 0.0) {
    id = ident;
    pos_x = x;
    pos_y = y;
    pos_z = z;
    radius = rad;
    rgb[0] = colors[0];
    rgb[1] = colors[1];
    rgb[2] = colors[2];
    tower_perc = t_perc;
    alive = true;
  }

  void Desenha() {
    DesenhaIsland(pos_x, pos_y, pos_z, radius, tower_perc, rgb[0], rgb[1], rgb[2]);
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

  void setTowerPerc(GLfloat s);

  GLfloat getTowerPerc();

  void setAlive(bool status);

  bool getAlive();

};

class Sea {
  GLfloat id;
  GLfloat pos_x;
  GLfloat pos_y;
  GLfloat pos_z;
  GLfloat radius;
  GLfloat rgb[3];


private:
  void DesenhaCircle(GLfloat x, GLfloat y, GLfloat rad, GLfloat R, GLfloat G, GLfloat B);
  void DesenhaSea(GLfloat x, GLfloat y, GLfloat pos_z, GLfloat radius, GLfloat R, GLfloat G, GLfloat B);//, GLfloat theta2, GLfloat theta3);

public:
  Sea() {
    pos_x = 0;
    pos_y = 0;
    pos_z = 0;
    rgb[0] = 0.0;
    rgb[1] = 0.0;
    rgb[2] = 1.0;
  }

  void Desenha() {
    DesenhaSea(pos_x, pos_y, pos_z, radius, rgb[0], rgb[1], rgb[2]);
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

#endif /* END World_hpp */
