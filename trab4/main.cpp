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
#include "torpedo.h"
#include "cursor.h"
#include "world.hpp"
#include "enemy.hpp"
using namespace std;

// variaveis
string path_to_svg;
Submarine::submarine p;
Sea world;
Cursor mouse_info;
list<Torpedo> torpedos, e_torpedos;
list<Torpedo>::iterator tps, del_tps;
list<Island> islands;
list<Island>::iterator isl;
list<Submarine::submarine> enemies;
list<Submarine::submarine>::iterator enmy, del_enemy;
GLint janela_x;
GLint janela_y;
GLfloat tower_size;
GLfloat enemy_shot_freq;
GLfloat enemy_vel;
GLfloat enemy_vel_tiro;
int animation_submerge_time;
//Key status
int keyStatus[256];
bool* mouseStates = new bool[3];


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

bool pointInsideCircle(GLfloat px, GLfloat py, GLfloat cx, GLfloat cy, GLfloat cr) {
  if (sqrt( pow(px-cx, 2) + pow(py-cy, 2) ) < cr) return true;
  return false;
}

GLfloat calcDist2d(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) {
  return sqrt( pow(x1 - x2, 2) + pow(y1 - y2, 2) );
}

void display(void)
{
   /* Limpar todos os pixels  */
   glClear (GL_COLOR_BUFFER_BIT);

   /* Desenhar um polígono branco (retângulo) */

   world.Desenha();
   for (Island i : islands) i.Desenha();
   for (Torpedo t : torpedos) t.Desenha();
   for (Torpedo et : e_torpedos) et.Desenha(); 
   p.Desenha();
   for (Submarine::submarine e : enemies) e.Desenha();
   if (mouseStates[GLUT_RIGHT_BUTTON]) {
     mouse_info.Desenha();
   }
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
    case 'u':
    case 'U':
      keyStatus[(int)('u')] = 0;
      break;
  }
  glutPostRedisplay();
}

bool canMove(GLfloat d, const Submarine::submarine& s) {

  GLfloat r;
  GLfloat move_delta;
  GLfloat cx, cy, c_angle, s_angle;
  if (abs(s.getTethaPaddle()) < 0.02) {
    cx = s.getPosX() + d*cos(s.getTethaCenter());
    cy = s.getPosY() + d*sin(s.getTethaCenter());
  } else {
    r = s.getRadius()/tan(-s.getTethaPaddle());
    move_delta = -d/r;

    c_angle = -M_PI_2 + s.getTethaCenter();
    s_angle = M_PI_2 + s.getTethaCenter() + move_delta;

    cx = s.getPosX() + r*(cos(c_angle));
    cy = s.getPosY() + r*(sin(c_angle));

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
                           s.getMaxRadius())) {
                             std::cout << "collision with island" << std::endl;
       coll = true;
     }
  }

  // verify collision with enemies
  
    GLint id = s.getId();
    for (Submarine::submarine e : enemies) {
      if ((id != e.getId()) && (s.getSubmerginStatus() == e.getSubmerginStatus()) && collisionDetection(e.getPosX(),
                             e.getPosY(),
                             e.getRadius(),
                             cx,
                             cy,
                             s.getMaxRadius())) {
                               std::cout << "collision with submarine" << std::endl;
         coll = true;
       }
    }
  

/* bool canMove(GLfloat d) {

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
       coll = true;
     }
  }

  // verify collision with enemies
  if (p.getSubmerginStatus() == 1) {
    for (Submarine::submarine e : enemies) {
      if (collisionDetection(e.getPosX(),
                             e.getPosY(),
                             e.getRadius(),
                             cx,
                             cy,
                             p.getMaxRadius())) {
         coll = true;
       }
    }
  } */

  // verify if leaves the world
  if (sqrt(pow((cx-world.getPosX()),2) + pow((cy-world.getPosY()),2)) >= world.getRadius() - s.getMaxRadius()) {
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

void mouse(int button, int state, int x, int y) {
  GLint dx = world.getPosX() - world.getRadius();
  GLint dy = world.getPosY() - world.getRadius();
  y = glutGet(GLUT_WINDOW_HEIGHT) - y;
  x += dx;
  y += dy;
  if (p.getSubmerginStatus() == -1) {
    if (button == GLUT_LEFT_BUTTON) { // para frente
      if (state == GLUT_DOWN) {
        torpedos.push_back(Torpedo(p.getPosX(), p.getPosY(), p.getTethaCenter(), p.getRadius()/12, p.getSubmerginStatus(), x, y));
      }
    }
    if (button == GLUT_RIGHT_BUTTON) { // para cima
      if (state == GLUT_DOWN) {
        mouse_info.setPosX(x);
        mouse_info.setPosY(y);
        mouseStates[button] = true;
      } else {
        mouseStates[button] = false;
        torpedos.push_back(Torpedo(p.getPosX(), p.getPosY(), p.getTethaCenter(), p.getRadius()/12, 1, x, y));
      }
    }
  }
}

void dragAndDrop(int x, int y) {
  GLint dx = world.getPosX() - world.getRadius();
  GLint dy = world.getPosY() - world.getRadius();
  y = glutGet(GLUT_WINDOW_HEIGHT) - y;
  x += dx;
  y += dy;

  mouse_info.setPosX(x);
  mouse_info.setPosY(y);
  glutPostRedisplay();
}

void updateTorpedos(GLdouble timeDiff) {
  for (tps = torpedos.begin(); tps != torpedos.end(); ++tps) {
    tps->Move(timeDiff, p.getVelTiro());
    if (sqrt(pow((tps->getPosX()-world.getPosX()),2) + pow((tps->getPosY()-world.getPosY()),2)) >= world.getRadius() - tps->getRadius()) {
      tps->setToDelete(true);
    }
    if (tps->getType() == -1) { //retilinio
      GLfloat px = tps->getPosX();
      GLfloat py = tps->getPosY();
      for (Island i : islands) {
        if (pointInsideCircle(px, py, i.getPosX(), i.getPosY(), i.getRadius())) tps->setToDelete(true);
      }
      for (enmy = enemies.begin(); enmy != enemies.end(); ++enmy) {
        if (pointInsideCircle(px, py, enmy->getPosX(), enmy->getPosY(), enmy->getRadius())) {
          tps->setToDelete(true);
          enmy->setToDelete(true);
        }
      }
      /* for (Submarine::submarine e : enemies) {
        if (pointInsideCircle(px, py, e.getPosX(), e.getPosY(), e.getRadius())) tps->setToDelete(true);
      } */
    } else { // superior

    }
  }
  del_tps = torpedos.begin();
  tps = torpedos.begin();
  while (tps != torpedos.end()) {
    if (tps->getToDelete()) {
      del_tps = tps;
      tps++;
      torpedos.erase(del_tps);

    } else {
      tps++;
    }
  }
  
  //std::cout << e_torpedos.size() << std::endl;
  for (tps = e_torpedos.begin(); tps != e_torpedos.end(); ++tps) {
    tps->Move(timeDiff, enemy_vel_tiro);  
    if (sqrt(pow((tps->getPosX()-world.getPosX()),2) + pow((tps->getPosY()-world.getPosY()),2)) >= world.getRadius() - tps->getRadius()) {
      tps->setToDelete(true);
    }
    if (tps->getType() == -1) { //retilinio
      GLfloat px = tps->getPosX();
      GLfloat py = tps->getPosY();
      for (Island i : islands) {
        if (pointInsideCircle(px, py, i.getPosX(), i.getPosY(), i.getRadius())) {
          tps->setToDelete(true);
        }
      }
      if (pointInsideCircle(px, py, p.getPosX(), p.getPosY(), p.getRadius())) {
        tps->setToDelete(true);
      }
    }
  }
  del_tps = e_torpedos.begin();
  tps = e_torpedos.begin();
  while (tps != e_torpedos.end()) {
    if (tps->getToDelete()) {
      del_tps = tps;
      tps++;
      e_torpedos.erase(del_tps);
    } else {
      tps++;
    }
  }
}

void updatePlayer(GLdouble timeDiff) {
  float dist = p.getVel() * timeDiff;
  //Treat keyPress
  if(keyStatus[(int)('w')])
  {
    collision[0] = canMove(dist, p);
    if (!collision[0]) p.Move(dist);
  }
  if(keyStatus[(int)('d')])
  {
    p.RotateTethaPaddle(+0.05);
  }
  if(keyStatus[(int)('s')])
  {
    collision[2] = canMove(-dist, p);
    if (!collision[2]) p.Move(-dist); //p.MoveY(-0.05);
  }
  if(keyStatus[(int)('a')])
  {
    p.RotateTethaPaddle(-0.05);
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
        for (Submarine::submarine e : enemies) {
          if (collisionDetection(e.getPosX(),
                                 e.getPosY(),
                                 e.getRadius(),
                                 p.getPosX(),
                                 p.getPosY(),
                                 p.getMaxRadius())) {
            is_under_enemy = true;
          }
        }
        if (!is_under_enemy) {
          animation_submerge_time = glutGet(GLUT_ELAPSED_TIME);
          p.setMovingZAxis(true);
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

void updateEnemies(GLdouble timeDiff) {
  del_enemy = enemies.begin();
  enmy = enemies.begin();
  while (enmy != enemies.end()) {
    if (enmy->getToDelete()) {
      del_enemy = enmy;
      enmy++;
      enemies.erase(del_enemy);
    } else {
      enmy++;
    }
  }
  for (enmy = enemies.begin(); enmy != enemies.end(); ++enmy) {

    if (enmy->getSubmerginStatus() == 1) {
      animation_submerge_time = glutGet(GLUT_ELAPSED_TIME);
      enmy->setSubmerginStatus(-1);
      enmy->setMovingZAxis(true);
      enmy->submerge(glutGet(GLUT_ELAPSED_TIME) - animation_submerge_time);
    }

    GLfloat paddle_inc = (float)(rand() % 100+1)/1000;
    GLfloat factor;
    if (rand()%2 == 0) {
      factor = 1.0;
    } else {
      factor = -1.0;
    }
    //std::cout << factor << std::endl;
    enmy->RotateTethaPaddle(factor * paddle_inc);

    if (canMove(enmy->getVel()*timeDiff, *enmy) == false) {
      //enmy->Move(enmy->getVel()*timeDiff);
    } else {
      enmy->setVel(-1*enmy->getVel());
    }

    if (rand() % 100000+1 < enemy_shot_freq*100) {
    
      GLfloat px = enmy->getPosX();
      GLfloat py = enmy->getPosY();
      GLfloat tetha = enmy->getTethaCenter();
      GLfloat r = enmy->getRadius()/10;
    
      e_torpedos.push_back(Torpedo(px, py, tetha, r, -1, 0.0, 0.0));
    }

    enmy->updateHeli();
  }
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
  updateEnemies(timeDiference);
  updateTorpedos(timeDiference);

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
  // leitura dados do inimigo
  app_ele = app_root->FirstChildElement("inimigo");
  app_ele->QueryDoubleAttribute("freqTiro", &tmp);
  enemy_shot_freq = tmp;
  app_ele->QueryDoubleAttribute("vel", &tmp);
  enemy_vel = tmp;
  app_ele->QueryDoubleAttribute("velTiro", &tmp);
  enemy_vel_tiro = tmp;

  // leitura dados da torre
  app_ele = app_root->FirstChildElement("torre");
  app_ele->QueryDoubleAttribute("tamanho", &tmp);
  tower_size = tmp;

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

    Submarine::submarine new_enmy;
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
        // enemies.push_back(Submarine::submarine());
        // enmy = enemies.end();
        new_enmy.setR(1.0);
        new_enmy.setG(0.0);
        new_enmy.setB(0.0);
        new_enmy.setPosX(cx);
        new_enmy.setPosY(2*world.getPosY() - cy);
        new_enmy.setPosZ(0.0);
        new_enmy.setId(id);
        new_enmy.setRadius(r);
        new_enmy.setMaxRadius(r);
        new_enmy.setSubmerginStatus(1);
        new_enmy.setVel(enemy_vel);
        new_enmy.setVelTiro(enemy_vel_tiro);
				rgb[0] = 1.0;
				rgb[1] = 0.0;
				rgb[2] = 0.0;
        cy = 2*world.getPosY() - cy;
        enemies.push_back(new_enmy);
        //enemies.push_back(Enemy(id, rgb, cx, cy, 0.0, r));
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
        mouse_info.setRadius(r);
				break;
			case str2int("black"):
				// preto em rgb: 0, 0, 0
				rgb[0] = 0.0;
				rgb[1] = 0.0;
				rgb[2] = 0.0;
        cy = 2*world.getPosY() - cy;
        islands.push_back(Island(id, rgb, cx, cy, 0.0, r, r*(tower_size/100)));
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
  glutMouseFunc(mouse);
  glutMotionFunc(dragAndDrop);
  glutMainLoop();


	return 0;
}
