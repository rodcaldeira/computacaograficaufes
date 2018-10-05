/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.cpp
 * Copyright (C) 2018 Rodrigo Caldeira <rod.cdr@gmail.com>
 *
 * trabalhocg is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * trabalhocg is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstddef>
#include <iostream>
#include <sstream>
#include <list>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "tinyxml2/tinyxml2.h"
#include "submarine.h"
#include "player.hpp"
#include "world.hpp"
#include "enemy.hpp"
using namespace std;

// variaveis
string path_to_svg;
Submarine::submarine p;
Sea world;
list<Island> islands;
list<Island>::iterator isl;
list<Enemy> enemies;
list<Enemy>::iterator enmy, del_enemy;
GLint janela_x;
GLint janela_y;
int animation_submerge_time;
//Key status
int keyStatus[256];


bool collisionTest = false;
bool collision[4] = {false, false, false, false}; // 0 - north
                                                  // 1 - east
                                                  // 2 - south
                                                  // 3 - west
/*
ref para funcao str2int
https://stackoverflow.com/questions/16388510/evaluate-a-string-with-a-switch-in-c#answer-16388610
utilizada para usar o switch case com string
*/
constexpr unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

bool collisionDetection(GLfloat x1, GLfloat y1, GLfloat r1, GLfloat x2, GLfloat y2, GLfloat r2) {
  if ( sqrt(pow((x1-x2),2) + pow((y1-y2),2)) < r1+r2 ) return true;
  return false;
}

void display(void)
{
   /* Limpar todos os pixels  */
   glClear (GL_COLOR_BUFFER_BIT);

   /* Desenhar um polígono branco (retângulo) */

   world.Desenha();
   for (Island i : islands) i.Desenha();
   p.Desenha();
   for (Enemy e : enemies) e.Desenha();
   /* Não esperar! */
   glutSwapBuffers ();
}

void init (void)
{
  /* inicia estados do jogador */
  p.setTethaHeli(0.0);
  p.setTethaPaddle(0.0);

  /* selecionar cor de fundo (preto) */
  glClearColor (0.5, 0.5, 0.5, 0.0);

  /* inicializar sistema de viz. */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(world.getPosX() - world.getRadius(),
          world.getPosX() + world.getRadius(),
          world.getPosY() - world.getRadius(),
          world.getPosY() + world.getRadius(),
          -100.0,
          100.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void keyup(unsigned char key, int x, int y)
{
  keyStatus[(int)(key)] = 0;
  for (int i = 0; i < 4; i++) collision[i] = true;
  switch (key) {
    case 'w':
    case 'W':
      collision[0] = false;
      keyStatus[(int)('w')] = 0;
      break;
    case 'd':
    case 'D':
      collision[1] = false;
      keyStatus[(int)('d')] = 0;
      break;
    case 's':
    case 'S':
      collision[2] = false;
      keyStatus[(int)('s')] = 0;
      break;
    case 'a':
    case 'A':
      collision[3] = false;
      keyStatus[(int)('a')] = 0;
      break;
  }
  glutPostRedisplay();
}

bool canMove(GLfloat d) {

  GLfloat r;
  GLfloat move_delta;
  GLfloat cx, cy, c_angle, s_angle;
  if (abs(p.getTethaPaddle()) < 0.02) {
    cx = p.getPosX() + d*cos(p.getTethaCenter());
    cy = p.getPosY() + d*sin(p.getTethaCenter());
  } else {
    r = p.getRadius()/tan(-p.getTethaPaddle());
    move_delta = -d/r;

    c_angle = -M_PI_2 + p.getTethaCenter();
    s_angle = M_PI_2 + p.getTethaCenter() + move_delta;

    cx = p.getPosX() + r*(cos(c_angle));
    cy = p.getPosY() + r*(sin(c_angle));

    cx += r*cos(s_angle);
    cy += r*sin(s_angle);
  }
  bool coll = false;
  // verify collision with island
  for (Island i : islands) {
    if (collisionDetection(i.getPosX(),
                           i.getPosY(),
                           i.getRadius(),
                           cx,
                           cy,
                           p.getMaxRadius())) {
    //   cout << "player will collide with island at " << i.getPosX() << " " << i.getPosY() << endl;
       coll = true;
     }
  }

  // verify collision with enemies
  if (p.getSubmerginStatus() == 1) {
    for (Enemy e : enemies) {
      if (collisionDetection(e.getPosX(),
                             e.getPosY(),
                             e.getRadius(),
                             cx,
                             cy,
                             p.getMaxRadius())) {
        // cout << "player will collide with enemie at " << e.getPosX() << " " << e.getPosY() << endl;
         coll = true;
       }
    }
  }

  // verify if leaves the world
  if (sqrt(pow((cx-world.getPosX()),2) + pow((cy-world.getPosY()),2)) >= world.getRadius() - p.getMaxRadius()) {
    //cout << "player trying to leave the world" << endl;
    coll = true;
  }
  return coll;
}


void keyPress(unsigned char key, int x, int y)
{
  switch (key)
  {
    case 'w':
    case 'W':
      keyStatus[(int)('w')] = 1;
      break;
    case 'd':
    case 'D':
      keyStatus[(int)('d')] = 1;
      break;
    case 's':
    case 'S':
      keyStatus[(int)('s')] = 1;
      break;
    case 'a':
    case 'A':
      keyStatus[(int)('a')] = 1;
      break;
    case 'u':
    case 'U':
      keyStatus[(int)('u')] = 1;
      break;
      case 27 :
           exit(0);
  }
  glutPostRedisplay();
}

void updatePlayer(GLdouble timeDiff) {
  float dist = 0.1 * timeDiff;
  //Treat keyPress
  if(keyStatus[(int)('w')])
  {
    collision[0] = canMove(dist);
    if (!collision[0]) p.Move(dist);
    //if (!collision[0]) p.Move(1);//p.MoveY(0.05);
  }
  if(keyStatus[(int)('d')])
  {
    //collision[1] = canMove(dist);
    p.RotateTethaPaddle(+0.05);
    //if (!collision[1]) p.MoveX(0.05);
  }
  if(keyStatus[(int)('s')])
  {
    collision[2] = canMove(-dist);
    if (!collision[2]) p.Move(-dist); //p.MoveY(-0.05);
  }
  if(keyStatus[(int)('a')])
  {
    //collision[3] = canMove(dist);
    p.RotateTethaPaddle(-0.05);
    //if (!collision[3]) p.MoveX(-0.05);
  }
  if(keyStatus[(int)('u')])
  {
    if (p.getMovingZAxis()) {
      //cout << "esta movendo" << endl;
    } else {
      if (p.getSubmerginStatus() == 1) {
        animation_submerge_time = glutGet(GLUT_ELAPSED_TIME);
        p.setMovingZAxis(true);
        //cout << "comecando a mover em " << animation_submerge_time << endl;
      } else if (p.getSubmerginStatus() == -1) {
        bool is_under_enemy = false;
        for (Enemy e : enemies) {
          if (collisionDetection(e.getPosX(),
                                 e.getPosY(),
                                 e.getRadius(),
                                 p.getPosX(),
                                 p.getPosY(),
                                 p.getMaxRadius())) {
            is_under_enemy = true;
            //cout << "UNDER ENEMY" << endl;
          }
        }
        if (!is_under_enemy) {
          animation_submerge_time = glutGet(GLUT_ELAPSED_TIME);
          p.setMovingZAxis(true);
          //cout << "comecando a mover em " << animation_submerge_time << endl;
        }
      }
    }
  }
  // handling idle moves and collision

  if(p.getMovingZAxis()) {
    p.submerge(glutGet(GLUT_ELAPSED_TIME) - animation_submerge_time);
  }

  p.updateHeli();
}

void idle(void)
{
  static GLdouble previousTime = 0;
	GLdouble currentTime;
	GLdouble timeDiference;

	// Elapsed time from the initiation of the game
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	timeDiference = currentTime - previousTime; // elapsed time from the previous frame
	previousTime = currentTime;

	updatePlayer(timeDiference);

  glutPostRedisplay();
}

int main(int argc, char ** argv) {
	string path_to;
	if (argc < 2) {
		cout << "need config.xml path to" << endl;
		return 0;
	} else {
		path_to = argv[1];
		path_to += "config.xml";
	}
	tinyxml2::XMLDocument xml_doc_config;
	tinyxml2::XMLDocument xml_svg_file;
//	tinyxml2::XMLError e_result = xml_doc_config.LoadFile(path_to);
  //tinyxml2::XMLError e_result =
  xml_doc_config.LoadFile(path_to.c_str());

  tinyxml2::XMLNode * app_root = xml_doc_config.FirstChild();
	if (app_root == nullptr) return tinyxml2::XML_ERROR_FILE_READ_ERROR;

	tinyxml2::XMLElement * app_ele = app_root->FirstChildElement("arquivoDaArena");
	if (app_ele == nullptr) return tinyxml2::XML_ERROR_PARSING_ELEMENT;

	tinyxml2::XMLElement * tmp_xml = app_ele->FirstChildElement("caminho");
	if (tmp_xml == nullptr) return tinyxml2::XML_ERROR_PARSING_ELEMENT;

	path_to_svg = tmp_xml->GetText(); // >> path_to_svg;

  tmp_xml = app_ele->FirstChildElement("nome");
	if (tmp_xml == nullptr) return tinyxml2::XML_ERROR_PARSING_ELEMENT;
	path_to_svg += tmp_xml->GetText();
	path_to_svg += ".";

	tmp_xml = app_ele->FirstChildElement("tipo");
	if (tmp_xml == nullptr) return tinyxml2::XML_ERROR_PARSING_ELEMENT;
	path_to_svg += tmp_xml->GetText();

  app_ele = app_root->FirstChildElement("jogador");
  double tmp;
  app_ele->QueryDoubleAttribute("velTiro", &tmp);
  p.setVelTiro(tmp);
  app_ele->QueryDoubleAttribute("vel", &tmp);
  p.setVel(tmp);
  //cout << path_to_svg << endl;
// inicio da leitura do svg
	//e_result =
  xml_svg_file.LoadFile(path_to_svg.c_str());

	app_root = xml_svg_file.FirstChildElement("svg");
	tmp_xml = app_root->FirstChildElement("circle");

	const char * color;
	GLfloat rgb[3];

	while (tmp_xml != nullptr) {
	  color = tmp_xml->Attribute("fill");
    double cx, cy, r;
    int id;
    tmp_xml->QueryIntAttribute("id", &id);
    tmp_xml->QueryDoubleAttribute("cx", &cx);
    tmp_xml->QueryDoubleAttribute("cy", &cy);
    tmp_xml->QueryDoubleAttribute("r", &r);
		switch (str2int(color)) {
			case str2int("blue"):
				// azul em rgb: 0, 0, 1
				rgb[0] = 0.0;
				rgb[1] = 0.0;
				rgb[2] = 1.0;
        world.setColor(rgb);
        world.setPosX(cx);
        world.setPosY(cy);
        world.setRadius(r);
        world.setId(id);
				break;
			case str2int("red"):
				// red em rgb: 1, 0, 0
				rgb[0] = 1.0;
				rgb[1] = 0.0;
				rgb[2] = 0.0;
        cy = 2*world.getPosY() - cy;
        enemies.push_back(Enemy(id, rgb, cx, cy, 0.0, r));
				break;
			case str2int("green"):
				// verde em rgb: 0, 0, 1
				rgb[0] = 0.0;
				rgb[1] = 1.0;
				rgb[2] = 0.0;
        cy = 2*world.getPosY() - cy;
        p.setPosX(cx);
        p.setPosY(cy);
        p.setRadius(r);
        p.setMaxRadius(r);
        //p.setColor(rgb);
        p.setR(rgb[0]);
        p.setG(rgb[1]);
        p.setB(rgb[2]);
        p.setId(id);
				break;
			case str2int("black"):
				// preto em rgb: 0, 0, 0
				rgb[0] = 0.0;
				rgb[1] = 0.0;
				rgb[2] = 0.0;
        cy = 2*world.getPosY() - cy;
        islands.push_back(Island(id, rgb, cx, cy, 0.0, r));
				break;
			default:
				break;
		}
		tmp_xml = tmp_xml->NextSiblingElement("circle");
	}

  int tamanhoDaJanela = ceil(world.getRadius()*2);
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (tamanhoDaJanela, tamanhoDaJanela);
  glutInitWindowPosition (100, 100);
  glutCreateWindow ("trabalhocg");
  init ();
  glutKeyboardFunc(keyPress);
  glutKeyboardUpFunc(keyup);
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutMainLoop();


	return 0;
}
