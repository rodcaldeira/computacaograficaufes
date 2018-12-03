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
#include <cmath>
#include "tinyxml2/tinyxml2.h"
#include "submarine.h"
#include "torpedo.h"
#include "cursor.h"
#include "world.hpp"
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
int alturaDaJanela=500;
int larguraDaJanela=500;
int tower_number;
int animation_submerge_time;
//Key status
int keyStatus[256];
bool* mouseStates = new bool[3];
bool is_playing = true;

/*
ref para funcao str2int
https://stackoverflow.com/questions/16388510/evaluate-a-string-with-a-switch-in-c#answer-16388610
utilizada para usar o switch case com string
*/

void PrintScore(GLfloat x, GLfloat y);

constexpr unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

int camera = 3;

void drawView() {

}

void drawMiniMap() {
  

  glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glViewport(larguraDaJanela*0.75, alturaDaJanela*0.75, larguraDaJanela, alturaDaJanela);
    glOrtho(-0.5, 0.5, -0.5, 0.5, -1, 1);

    glPushMatrix();
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glColor3f(1,1,1);
      glTranslatef(0,0,-0.8);
      glutSolidCube(40);
    glPopMatrix();
  glPopMatrix();
}

void pickCam() {
  GLfloat eyex;
  GLfloat eyey;
  GLfloat eyez;

  GLfloat poix;
  GLfloat poiy;
  GLfloat poiz;

  GLfloat upvx;
  GLfloat upvy;
  GLfloat upvz;
  switch(camera) {
    case 1: // first person
      eyex = p.getPosX();
      eyey = p.getPosY();
      eyez = p.getPosZ() + p.getMaxRadius()*0.3;

      poix = p.getPosX() + 10*p.getRadius()*cos(p.getTethaCenter());
      poiy = p.getPosY() + 10*p.getRadius()*sin(p.getTethaCenter());
      poiz = p.getPosZ();

      upvx = 0.0;
      upvy = 0.0;
      upvz = 1.0;
      break;
    case 2:
      gluLookAt(p.getPosX() - 3.5*p.getRadius()*cos(p.getTethaCenter()), p.getPosY() - sin(p.getTethaCenter())*3.5*p.getRadius(), p.getPosZ() + 2*p.getRadius(),
            p.getPosX(), p.getPosY(), p.getPosZ(),
            0, 0, 1);
      break;
    case 3: // third person
      eyex = p.getPosX() - 2.5*p.getRadius()*cos(p.getTethaCenter());
      eyey = p.getPosY() - sin(p.getTethaCenter())*2.5*p.getRadius();
      eyez = p.getPosZ() + 2*p.getRadius();

      poix = p.getPosX();
      poiy = p.getPosY();
      poiz = p.getPosZ();

      upvx = 0.0;
      upvy = 0.0;
      upvz = 1.0;
      break;
  }
  gluLookAt(eyex, eyey, eyez,
            poix, poiy, poiz,
            upvx, upvy, upvz);
}

void display(void)
{
  /* Limpar todos os pixels  */
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glViewport(0, 0, larguraDaJanela, alturaDaJanela);
  glLoadIdentity();
  gluLookAt(0.0, 0.0, 150.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  //glutWireTeapot(1);
  p.Desenha();
  //
  // glViewport(width/2, 0, width/2, height/2);
  // glLoadIdentity();
  // gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  // glutWireTeapot(1);
  // p.setPosX(0.0);
  // p.setPosY(0.0);
// minimap
  glViewport(larguraDaJanela*0.75, alturaDaJanela*0.75, larguraDaJanela*0.25, alturaDaJanela*0.25);
  glLoadIdentity();
  glPushMatrix();
    glOrtho(-world.getRadius(), world.getRadius(), -world.getRadius(), world.getRadius(), 1000.0, -1000.0);
    gluLookAt(world.getPosX(), world.getPosY(), 7*p.getMaxRadius(), world.getPosX(), world.getPosY(), 0.0, 1.0, 0.0, 0.0);
    glutSolidCube(5);
    // draw circle
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 360; i+=10) {
      glVertex2d(p.getPosX() + p.getMaxRadius()/5*cos(i), p.getPosY() + p.getMaxRadius()/5*sin(i));
    }
    glEnd();
  //  p.Desenha();
  glPopMatrix();
  
  //
  // glViewport(width/2, height/2, width/2, height/2);
  // glLoadIdentity();
  // gluLookAt(0.0, -3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
  // glutWireTeapot(1);

  // glViewport(0, alturaDaJanela, larguraDaJanela, alturaDaJanela);
  // glColor3f(1.0f, 0.0f, 0.0f);

  // glMatrixMode(GL_PROJECTION);
  // glLoadIdentity();
  // glOrtho(-3.0, 3.0, -3.0, 3.0, 1.0, 50.0);
  // gluLookAt(0.0, 5.0, 0.0,
  //         0.0, 0.0, 0.0,
  //         0.0, 0.0, -1.0);

  // glMatrixMode(GL_MODELVIEW);
  // glLoadIdentity();
  // glutSolidCube(1);
  // //glMatrixMode(GL_PROJECTION);


  //  glMatrixMode(GL_MODELVIEW);
  //  glLoadIdentity();
  // //  gluLookAt(world.getPosX(), world.getPosY(), world.getPosZ()+world.getRadius()*2, 
  // //            world.getPosX(), world.getPosY(), world.getPosZ(),
  // //            0, 1, 0);
  // drawMiniMap();

  //  GLfloat light_position[] = {world.getPosX(), world.getPosY(), world.getRadius()*2, 1.0};
  //  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  //  /* Desenhar um polígono branco (retângulo) */

  //  //world.Desenha();
  // //  for (Island i : islands) i.Desenha();
  // //  for (Torpedo t : torpedos) t.Desenha();
  // //  for (Torpedo et : e_torpedos) et.Desenha(); 
  // //  p.Desenha();
  // //  for (Submarine::submarine e : enemies) e.Desenha();
  // //  if (mouseStates[GLUT_RIGHT_BUTTON]) {
  // //    mouse_info.Desenha();
  // //  }
  // //  PrintScore(180.0, 15.0);

  //  pickCam();
   /* Não esperar! */
   glutSwapBuffers ();
}

void init (void)
{
  /* inicia estados do jogador */
  p.setTethaHeli(0.0);
  p.setTethaPaddle(0.0);

  /* selecionar cor de fundo (preto) */
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
  gluPerspective(90, (GLdouble)alturaDaJanela/(GLdouble)larguraDaJanela, 0.1, 150.0);
  glMatrixMode(GL_MODELVIEW);
  
  /* inicializar sistema de viz. */
  // glShadeModel(GL_SMOOTH);

  // glEnable(GL_LIGHTING);
  // glEnable(GL_LIGHT0);
  // glEnable(GL_DEPTH_TEST);
  
}

// Text variable
static char str[2000];
void * font = GLUT_BITMAP_9_BY_15;

void PrintScore(GLfloat x, GLfloat y)
{
    //Create a string to be printed
    char *tmpStr;
    if (tower_number == 0) {
      sprintf(str, "Voce ganhou");
    } else if (!is_playing) {
      sprintf(str, "Voce perdeu");
    } else {
      sprintf(str, "Towers: %d, Score: %d", tower_number, (int)islands.size()-tower_number );
    }
    //Define the position to start printing
    glRasterPos2f(world.getPosX() + world.getRadius() - x, world.getPosY() + world.getRadius() - y);
    //Print  the first Char with a certain font
    //glutBitmapLength(font,(unsigned char*)str);
    tmpStr = str;
    //Print each of the other Char at time
    while( *tmpStr ){
        glutBitmapCharacter(font, *tmpStr);
        tmpStr++;
    }

}

void keyup(unsigned char key, int x, int y)
{
  keyStatus[(int)(key)] = 0;
  switch (key) {
    case 'w':
    case 'W':
      keyStatus[(int)('w')] = 0;
      break;
    case 'd':
    case 'D':
      keyStatus[(int)('d')] = 0;
      break;
    case 's':
    case 'S':
      keyStatus[(int)('s')] = 0;
      break;
    case 'a':
    case 'A':
      keyStatus[(int)('a')] = 0;
      break;
    case 'u':
    case 'U':
      keyStatus[(int)('u')] = 0;
      break;
  }
  glutPostRedisplay();
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

void updatePlayer(GLdouble timeDiff) {
  float dist = p.getVel() * timeDiff;
  //Treat keyPress
  if(keyStatus[(int)('w')])
  {
    cout << "move forward" << endl;
  }
  if(keyStatus[(int)('d')])
  {
    p.RotateTethaPaddle(+0.05*timeDiff);
  }
  if(keyStatus[(int)('s')])
  {
    cout << "move backward" << endl;
  }
  if(keyStatus[(int)('a')])
  {
    p.RotateTethaPaddle(-0.05*timeDiff);
  }
  if(keyStatus[(int)('u')])
  {
    cout << "submerge" << endl;
  }
  // handling idle moves and collision
  p.updateHeli(timeDiff);
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

    GLfloat paddle_inc;
    GLfloat factor;
    // move paddle random
    if (rand() % 100+1 > 60) {
      paddle_inc = (float)(rand() % 10000+1)/100;
      if (rand()%2 == 0) {
        factor = 1.0;
      } else {
        factor = -1.0;
      }
      enmy->RotateTethaPaddle(factor * paddle_inc);
    }
    enmy->setTicks(enmy->getTicks() + timeDiff);
    if (enmy->getTicks() >= 1000) {
      enmy->setTicks(0);
      if (((float)(rand() % 100+1))/100 < enemy_shot_freq) {
        
        GLfloat px = enmy->getPosX();
        GLfloat py = enmy->getPosY();
        GLfloat tetha = enmy->getTethaCenter();
        GLfloat r = enmy->getRadius()/10;
      
        e_torpedos.push_back(Torpedo(px, py, tetha, r, -1, 0.0, 0.0));
      }
    }
    enmy->updateHeli(timeDiff);
  }
}

void updateTorpedos(GLdouble timediff) {
  return;
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

  if (is_playing && (tower_number > 0)) {
    updatePlayer(timeDiference);
    updateEnemies(timeDiference);
    updateTorpedos(timeDiference);
  }
	
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

  tower_number = 0;

  double world_pos_x;
  double world_pos_y;

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
      world_pos_x = cx;
      world_pos_y = cy;
      world.setColor(rgb);
      // world.setPosX(cx);
      // world.setPosY(cy);
      world.setPosX(0);
      world.setPosY(0);
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
      new_enmy.setPosX(cx-world_pos_x);
      // new_enmy.setPosY(2 * world.getPosY() - cy);
      new_enmy.setPosY(-1*(cy-world_pos_y));
      new_enmy.setPosZ(2*r);
      new_enmy.setId(id);
      new_enmy.setRadius(r);
      new_enmy.setMaxRadius(r);
      new_enmy.setSubmerginStatus(1);
      new_enmy.setVel(enemy_vel);
      new_enmy.setVelTiro(enemy_vel_tiro);
      rgb[0] = 1.0;
      rgb[1] = 0.0;
      rgb[2] = 0.0;
      enemies.push_back(new_enmy);
      //enemies.push_back(Enemy(id, rgb, cx, cy, 0.0, r));
      break;
    case str2int("green"):
      // verde em rgb: 0, 0, 1
      rgb[0] = 0.0;
      rgb[1] = 1.0;
      rgb[2] = 0.0;
      // cy = 2 * world.getPosY() - cy;
      cy = -(cy-world_pos_y);
      p.setPosX(cx - world_pos_x);
      p.setPosY(cy);
      p.setPosZ(r*4);
      p.setRadius(r);
      p.setMaxRadius(r);
      p.setMaxHeight(r*4);
      p.setMinHeight(r/2);
      //p.setColor(rgb);
      p.setR(rgb[0]);
      p.setG(rgb[1]);
      p.setB(rgb[2]);
      p.setId(id);
      mouse_info.setRadius(r);
      break;
			case str2int("black"):
				// preto em rgb: 0, 0, 0
        tower_number++;
				rgb[0] = 0.0;
				rgb[1] = 0.0;
				rgb[2] = 0.0;
        cy = 2*world.getPosY() - cy;
        islands.push_back(Island(id, rgb, cx, cy, 0.0, r, r*(tower_size/100), p.getMaxRadius()*4));
				break;
			default:
				break;
		}
		tmp_xml = tmp_xml->NextSiblingElement("circle");
	}
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(larguraDaJanela, alturaDaJanela);
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
