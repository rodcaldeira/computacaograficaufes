/*
 * submarine.h
 *
 *  Created on: 22 de set de 2018
 *      Author: rodcaldeira
 */

#ifndef SUBMARINE_H_
#define SUBMARINE_H_
#include <GL/gl.h>
#include <GL/glu.h>

namespace Submarine
{

enum REFERENCE
{
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

class submarine
{
	GLfloat R;
	GLfloat G;
	GLfloat B;

	GLfloat pos_x;
	GLfloat pos_y;
	GLfloat pos_z;
	GLfloat radius;
	GLfloat max_radius;
	GLfloat max_height;
	GLfloat min_height;
	GLfloat direction_sub_x;
	GLfloat direction_sub_y;
	GLfloat direction_steering_x;
	GLfloat direction_steering_y;
	GLfloat tetha_center;
	GLfloat tetha_paddle;
	GLfloat tetha_heli;
	GLfloat tetha_canon;
	GLfloat tetha_canon_z;

	GLfloat velTiro;
	GLfloat vel;
	bool can_move;
	bool moving_z_axis;
	GLint submergin_status;
	GLint id;
	bool to_delete;
    GLint ticks = 0;

  private:
	void DesenhaSubmarine(GLfloat x, GLfloat y, GLfloat radius, GLfloat cR, GLfloat cG, GLfloat cB);
	void DesenhaCircle(GLfloat x, GLfloat y, GLfloat radx, GLfloat rady, GLfloat cR, GLfloat cG, GLfloat cB, GLfloat ang = 360);
	void DesenhaRect(REFERENCE ref, GLfloat base, GLfloat altura, GLfloat cR, GLfloat cG, GLfloat cB);
	void DesenhaTriangle(GLfloat x, GLfloat y, GLfloat l, GLfloat cR, GLfloat cG, GLfloat cB);

  public:
	submarine();
	virtual ~submarine();
	GLfloat getB() const;
	void setB(GLfloat b);
	GLfloat getG() const;
	void setG(GLfloat g);
	GLfloat getR() const;
	void setR(GLfloat r);
	GLfloat getDirectionSteeringX() const;
	void setDirectionSteeringX(GLfloat directionSteeringX);
	GLfloat getDirectionSteeringY() const;
	void setDirectionSteeringY(GLfloat directionSteeringY);
	GLfloat getDirectionSubX() const;
	void setDirectionSubX(GLfloat directionSubX);
	GLfloat getDirectionSubY() const;
	void setDirectionSubY(GLfloat directionSubY);
	GLint getSubmerginStatus() const;
	void setSubmerginStatus(GLint i);
	GLfloat getVelTiro() const;
	void setVelTiro(GLfloat vel_tiro);
	GLfloat getVel() const;
	void setVel(GLfloat v);
	bool getCanMove() const;
	void setCanMove(bool s);
	bool getMovingZAxis() const;
	void setMovingZAxis(bool s);
	GLint getId() const;
	void setId(GLint x);
	GLfloat getPosX() const;
	void setPosX(GLfloat posX);
	GLfloat getPosY() const;
	void setPosY(GLfloat posY);
	GLfloat getPosZ() const;
	void setPosZ(GLfloat posZ);
	GLfloat getRadius() const;
	void setRadius(GLfloat radius);
	GLfloat getMaxRadius() const;
	void setMaxRadius(GLfloat r);
	GLfloat getTethaCenter() const;
	void setTethaCenter(GLfloat tethaCenter);
	GLfloat getTethaHeli() const;
	void setTethaHeli(GLfloat tethaHeli);
	GLfloat getTethaPaddle() const;
	void setTethaPaddle(GLfloat tethaPaddle);
	bool getToDelete() const;
	void setToDelete(bool status);
	void setTethaCanon(GLfloat a);
	GLfloat getTethaCanon();
	void setTethaCanonZ(GLfloat a);
	GLfloat getTethaCanonZ();


	bool checkHit(GLfloat x, GLfloat y, GLfloat z, GLfloat r);
	void Move(GLdouble d);
	void MoveX(GLfloat dx);
	void MoveY(GLfloat dy);
	void MoveZ(GLfloat dz);
	void RotateTethaCenter(GLfloat ang);
	void RotateTethaHeli(GLfloat ang);
	void RotateTethaPaddle(GLfloat ang);
	void MoveCanon(GLfloat da);
	void MoveCanonZ(GLfloat daz);

	void updateHeli(GLdouble timeDiff);
	GLfloat getMinHeight();
	void setMinHeight(GLfloat h);
	GLfloat getMaxHeight();
	void setMaxHeight(GLfloat h);
	void submerge(int milisec);
	void incTicks();
	GLint getTicks();
	void setTicks(GLint t);

	void Desenha()
	{
		DesenhaSubmarine(pos_x, pos_y, radius, R, G, B);
	}

	void Desenha2D(GLfloat x, GLfloat y)
	{
		DesenhaCircle(x, y, max_radius/5, max_radius/5, R, G, B, 360);
	}
};

} /* namespace Submarine */

#endif /* SUBMARINE_H_ */
