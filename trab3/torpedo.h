/*
 * torpedo.h
 *
 *  Created on: 22 de set de 2018
 *      Author: rodcaldeira
 */

#ifndef TORPEDO_H_
#define TORPEDO_H_
#include <GL/gl.h>
#include <GL/glu.h>

enum REFERENCE {
	center = 1,
	top_center,
	right_center,
	bottom_center,
	left_center,
	top_right_corner,
	bottom_right_corner,
	bottom_left_corner,
	top_left_corner
};

class Torpedo {
	GLfloat R;
	GLfloat G;
	GLfloat B;

	GLfloat pos_x;
	GLfloat pos_y;
  GLfloat tetha_center;
  GLfloat radius;
  GLfloat max_radius;
  GLint type; // -1 submerso; 1 flor d'agua
  GLfloat target_x;
  GLfloat target_y;
  GLfloat dist_target;

private:
  void DesenhaTorpedo(GLfloat x, GLfloat y, GLfloat r, GLfloat cR, GLfloat cG, GLfloat cB);
	void DesenhaCircle(GLfloat x, GLfloat y, GLfloat radx, GLfloat rady, GLfloat cR, GLfloat cG, GLfloat cB, GLfloat ang = 360);
	void DesenhaRect(REFERENCE ref, GLfloat base, GLfloat altura, GLfloat cR, GLfloat cG, GLfloat cB);
public:
	Torpedo(GLfloat x, GLfloat y, GLfloat tetha, GLfloat r, GLfloat t, GLfloat mx, GLfloat my);
	virtual ~Torpedo();
  GLfloat getR();
  void setR(GLfloat cR);
  GLfloat getB();
  void setB(GLfloat cB);
  GLfloat getG();
  void setG(GLfloat cG);
  GLfloat getPosX();
  void setPosX(GLfloat x);
  GLfloat getPosY();
  void setPosY(GLfloat y);
  GLfloat getTethaCenter();
  void setTethaCenter(GLfloat a);
  GLfloat getRadius();
  void setRadius(GLfloat r);
  GLfloat getMaxRadius();
  void setMaxRadius(GLfloat r);
  GLfloat getType();
  void setType(GLint t);
  GLfloat getTargetX();
  void setTargetX(GLfloat x);
  GLfloat getTargetY();
  void setTargetY(GLfloat y);
  GLfloat getDistTarget();
  void setDistTarget(GLfloat d);

  void Move(GLdouble d);
  void MoveX(GLfloat dx);
  void MoveY(GLfloat dy);




	void Desenha() {
		DesenhaTorpedo(pos_x, pos_y, radius, R, G, B);
	}
};

#endif /* TORPEDO_H_ */
