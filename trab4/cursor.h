/*
 * cursor.h
 *
 *  Created on: 22 de set de 2018
 *      Author: rodcaldeira
 */

#ifndef CURSOR_H_
#define CURSOR_H_
#include <GL/gl.h>
#include <GL/glu.h>

class Cursor {
	GLfloat pos_x;
	GLfloat pos_y;
  GLfloat radius;

private:
  void DesenhaCursor(GLfloat x, GLfloat y, GLfloat radius);
public:
	Cursor();
	virtual ~Cursor();
  GLfloat getPosX();
  void setPosX(GLfloat x);
  GLfloat getPosY();
  void setPosY(GLfloat y);
  GLfloat getRadius();
  void setRadius(GLfloat r);
	void Desenha() {
		DesenhaCursor(pos_x, pos_y, radius);
	}
};

#endif /* CURSOR_H_ */
