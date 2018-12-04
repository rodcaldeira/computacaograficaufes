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
#include "imageloader.h"
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
GLfloat biggest_island_radius = 0.1;
int alturaDaJanela=500;
int larguraDaJanela=500;
int tower_number;
int animation_submerge_time;
//Key status
int keyStatus[256];
bool* mouseStates = new bool[3];
bool is_playing = true;
double mouseX;
double mouseY;
int right_button = 0;
float camera_angle, camera_angle_z;
GLuint textureSky;
/*
ref para funcao str2int
https://stackoverflow.com/questions/16388510/evaluate-a-string-with-a-switch-in-c#answer-16388610
utilizada para usar o switch case com string
*/

void rasterChars(float x, float y, float z, const char *text, double r, double g, double b) {
  //Push to recover original attributes
  glPushAttrib(GL_ENABLE_BIT);
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  //Draw text in the x, y, z position
  glColor3f(r, g, b);
  glRasterPos3f(x, y, z);
  const char *tmpStr;
  tmpStr = text;
  while (*tmpStr) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *tmpStr);
    tmpStr++;
  }
  glPopAttrib();
}

void PrintText(float x, float y, const char *text, double r, double g, double b) {
  glMatrixMode(GL_PROJECTION);
  //Draw text considering a 2D space (disable all 3d features)
  //Push to recover original PROJECTION MATRIX
  glPushMatrix();
    glLoadIdentity();
    
    glOrtho(-world.getRadius(), world.getRadius(), -world.getRadius(), world.getRadius(), 1000.0, -1000.0);
    rasterChars(x, y, 0, text, r, g, b);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

void youWin() {
  PrintText(0.1, 0.2, "YOU WIN!", 0, 0.5, 0);
  is_playing = false;
}

void youLost() {
  PrintText(0.1, 0.2, "YOU LOST!", 1, 0, 0);
  is_playing = false;
}

constexpr unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

int camera = 3;

GLuint LoadTextureRAW(const char * filename) {

    GLuint texture;

    Image* image = loadBMP(filename);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    gluBuild2DMipmaps(GL_TEXTURE_2D,
            GL_RGB,
            image->width, image->height,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            image->pixels);

    glTexImage2D(GL_TEXTURE_2D, //Always GL_TEXTURE_2D
            0, //0 for now
            GL_RGB, //Format OpenGL uses for image
            image->width, image->height, //Width and height
            0, //The border of the image
            GL_RGB, //GL_RGB, because pixels are stored in RGB format
            GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
            //as unsigned numbers
            image->pixels); //The actual pixel data

    delete image;

    return texture;
}

void drawView() {
  return;
}

void drawCirc(GLfloat x, GLfloat y, GLfloat radius, GLfloat r, GLfloat g, GLfloat b) {
  glPushMatrix();
	glTranslatef(x, y, 0.0);
  glColor3f(r, g, b);
	glBegin(GL_POLYGON);
	for (int i = 0; i <= 360; i += 360 / 30) {
		double angle = i * M_PI / 180;
		glVertex3f(radius * cos(angle), radius * sin(angle), 0.0);
	}
	glEnd();
	glPopMatrix();
}

void drawMiniMap() {
  glPushMatrix();
    
    glViewport(larguraDaJanela*0.75, 0, larguraDaJanela*0.25, alturaDaJanela*0.25);
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_DEPTH_TEST);
    glPushMatrix();
      glOrtho(-world.getRadius(), world.getRadius(), -world.getRadius(), world.getRadius(), 1000.0, -1000.0);
      gluLookAt(world.getPosX(), world.getPosY(), 7*p.getMaxRadius(), world.getPosX(), world.getPosY(), 0.0, 0.0, 1.0, 0.0);
      
      // draw sea
      drawCirc(world.getPosX(), world.getPosY(), world.getRadius()/8, 0.0, 0.0, 1.0);  
      double f = (world.getPosX() + world.getRadius()/8)/(world.getPosX() + world.getRadius());
      // draw island
      for (Island i : islands) {
        drawCirc(i.getPosX()*f, i.getPosY()*f, i.getRadius()/8, 0.0, 0.0, 0.0);
        if (i.getAlive()) {
          drawCirc(i.getPosX()*f, i.getPosY()*f, i.getTowerPerc()/8, 1.0, 1.0, 0.0);
        }
      }
      
      for (Submarine::submarine e : enemies) drawCirc(e.getPosX()*f, e.getPosY()*f, e.getMaxRadius()/8, 1.0, 0.0, 0.0);
      drawCirc(p.getPosX()*f, p.getPosY()*f, p.getMaxRadius()/8, 0.0, 1.0, 0.0);
    //  p.Desenha();
    glPopMatrix();
  glPopMatrix();

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
}


float gradToRad(float v) {
  return v * M_PI / 180;
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

  GLfloat tetha_canon_rad = gradToRad(p.getTethaCanon());
  GLfloat tetha_canon_z_rad = gradToRad(p.getTethaCanonZ());
  switch(camera) {
    case 1: // first person

      PrintText(0.1, 0.1, "Submarine command", 0, 1, 0);
      eyex = p.getPosX();
      eyey = p.getPosY();
      eyez = p.getPosZ() + p.getMaxRadius()*0.3;

      poix = p.getPosX() + 10*p.getRadius()*cos(p.getTethaCenter());
      poiy = p.getPosY() + 10*p.getRadius()*sin(p.getTethaCenter());
      poiz = p.getPosZ();

      upvx = 0.0f;
      upvy = 0.0f;
      upvz = 1.0f;
      break;
    case 2: // canon PoV
      PrintText(0.1, 0.1, "Canon PoV", 0, 1, 0);
      eyex = p.getPosX() + p.getMaxRadius()*cos(p.getTethaCenter());
      eyey = p.getPosY() + p.getMaxRadius()*sin(p.getTethaCenter());// - sin(p.getTethaCenter())*3.5*p.getRadius(); 
      //eyez = p.getPosZ() + p.getMaxRadius()/10;
      eyez = 1.5*p.getPosZ();

      // poix = world.getPosX();
      // poiy = world.getPosY();
      // poiz = world.getPosZ();
      poix = eyex + p.getRadius()*cos(p.getTethaCenter() - tetha_canon_rad);
      poiy = eyey + p.getRadius()*sin(p.getTethaCenter() - tetha_canon_rad);//p.getPosY() + 10*p.getMaxRadius()*sin(p.getTethaCanon()); 
      poiz = p.getPosZ() + p.getRadius()*cos(tetha_canon_z_rad);
      cout << p.getTethaCanon() << "º " << p.getTethaCanonZ() << "º - " << eyex << " " << eyey << " " << eyez << " " << poix << " " << poiy << " " << poiz << endl;
      upvx = 0.0f;
      upvy = 0.0f;
      upvz = 1.0f;
      break;
    case 3: // third person

      PrintText(0.1, 0.1, "Third Person", 0, 1, 0);
      glPushMatrix();
        glTranslatef(
          p.getPosX() + p.getMaxRadius()*sin(p.getTethaCenter()) + p.getRadius()*sin(p.getTethaCenter() + tetha_canon_rad),
          p.getPosY() + p.getMaxRadius()*cos(p.getTethaCenter()) - p.getRadius()*cos(p.getTethaCenter() + tetha_canon_rad),
          p.getPosZ() + p.getMaxRadius()/10 + p.getRadius()*cos(tetha_canon_z_rad));
        glutSolidSphere(30.0, 32, 10);
      glPopMatrix();
      eyex = p.getPosX() + 2.5*p.getRadius()*cos(p.getTethaCenter() + gradToRad(camera_angle));
      eyey = p.getPosY() + 2.5*p.getRadius()*sin(p.getTethaCenter() + gradToRad(camera_angle));
      eyez = p.getPosZ() + 2*p.getRadius()*cos(gradToRad(camera_angle_z));

      poix = p.getPosX();
      poiy = p.getPosY();
      poiz = p.getPosZ();

      upvx = 0.0f;
      upvy = 0.0f;
      upvz = 1.0f;
      break;
  }
  gluLookAt(eyex, eyey, eyez,
            poix, poiy, poiz,
            upvx, upvy, upvz);
}


// Text variable
static char str[2000];
void * font = GLUT_BITMAP_9_BY_15;

void display(void)
{
  /* Limpar todos os pixels  */
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //cout << p.getTethaCanon() << " " << p.getTethaCanonZ() << endl;

  glViewport(0, 0, larguraDaJanela, alturaDaJanela);
  glLoadIdentity();
  
  if (tower_number == 0) {
    sprintf(str, "Voce ganhou");  
  } else if (!is_playing) {
    sprintf(str, "Voce perdeu");
  } else {
    sprintf(str, "Towers: %d, Score: %d", tower_number, (int)islands.size()-tower_number);
  }
  
  PrintText(-world.getRadius()+15, world.getRadius()-25, str, 0.7, 0.7, 0.7);

  pickCam();

  glBindTexture(GL_TEXTURE_2D, textureSky);
  world.Desenha(textureSky);
  p.Desenha();
  GLfloat light_position[] = {world.getPosX(), world.getPosY(), world.getRadius()*2, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  /* Desenhar um polígono branco (retângulo) */

  for (Island i : islands) i.Desenha();
  for (Torpedo t : torpedos) t.DesenhaTorpedo3d(t.getPosX(), t.getPosY(), t.getPosZ(), t.getRadius(), 0.5, 0.5, 0.5);
  for (Torpedo et : e_torpedos) et.DesenhaTorpedo3d(et.getPosX(), et.getPosY(), et.getPosZ(), et.getRadius(), 0.5, 0.5, 0.5); 
  p.Desenha();
  for (Submarine::submarine e : enemies) e.Desenha();
// minimap
  drawMiniMap();

   
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
  gluPerspective(90, (GLdouble)alturaDaJanela/(GLdouble)larguraDaJanela, 0.1, 1000.0);
  glMatrixMode(GL_MODELVIEW);
  
  /* inicializar sistema de viz. */
  // glShadeModel(GL_SMOOTH);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);

  textureSky = LoadTextureRAW("sky12.bmp");
  
}


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
    case 'j':
    case 'J':
      keyStatus[(int)('j')] = 0;
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
    case 'j':
    case 'J':
      keyStatus[(int)('j')] = 1;
      break;
    case '1':
      camera = 1;
      break;
    case '2':
      camera = 2;
      break;
    case '3':
      camera = 3;
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

  if (button == GLUT_LEFT_BUTTON) { // para frente
    if (state == GLUT_DOWN) {
      torpedos.push_back(Torpedo(p.getPosX(), p.getPosY(), p.getPosZ(), p.getTethaCenter()+p.getTethaCanon(), p.getTethaCanonZ(), p.getRadius()/12, p.getSubmerginStatus(), x, y));
    }
  }
  // if (button == GLUT_RIGHT_BUTTON) { // para cima
  //   if (state == GLUT_DOWN) {
  //     mouse_info.setPosX(x);
  //     mouse_info.setPosY(y);
  //     mouseX = x;
  //     mouseY = y;
  //     mouseStates[button] = true;
  //     right_button = 1;
  //   } else {
  //     mouseStates[button] = false;
  //     right_button = 0;
  //     camera_angle = 0;
  //     camera_angle_z = 0;
  //     torpedos.push_back(Torpedo(p.getPosX(), p.getPosY(), p.getPosZ(), p.getTethaCenter(), 0.0, p.getRadius()/12, 1, x, y));
  //   }
  // }

}

void dragAndDrop(int x, int y) {
  GLint dx = world.getPosX() - world.getRadius();
  GLint dy = world.getPosY() - world.getRadius();
  y = glutGet(GLUT_WINDOW_HEIGHT) - y;

  float perc = 0.0;
  float w2 = (float)larguraDaJanela/2;
  float h2 = (float)alturaDaJanela/2;
  int tmp_x = x;
  int tmp_y = y;
  
  if (tmp_x < w2) {
    if (tmp_x < 0) tmp_x = 0.0;
    perc = w2 - tmp_x;
    p.setTethaCanon(-30.0 * perc/w2);
  } else {
    if (tmp_x > larguraDaJanela) tmp_x = larguraDaJanela;
    perc = tmp_x - w2;
    p.setTethaCanon(30.0 * perc/w2);
  }
  if (tmp_y < h2) {
    if (tmp_y < 0) tmp_y = 0.0;
    perc = h2 - tmp_y;
    p.setTethaCanonZ(-30.0 * perc/h2);
  } else {
    if (tmp_y > alturaDaJanela) tmp_y = alturaDaJanela;
    perc = tmp_y - h2;
    p.setTethaCanonZ(30.0 * perc/h2);
  }
  //cout << p.getTethaCanon() << endl;
  x += dx;
  y += dy;
  
  mouseX = tmp_x;
  mouseY = tmp_y;
  mouse_info.setPosX(x);
  mouse_info.setPosY(y);
  if (right_button == 1) {
    camera_angle = (mouseX * 180/w2);
    camera_angle_z = (mouseY * 180/h2);
  }
  glutPostRedisplay();
}


GLfloat distanciaGeral(GLfloat x, GLfloat y, GLfloat z, GLfloat xo, GLfloat yo, GLfloat zo ){
  int dist= 0;

  dist = sqrt((x-xo)*(x-xo) + (y-yo)*(y-yo)+ (z-zo)*(z-zo));

  return dist;

}
GLfloat colisaoPlayerIlha=0;


GLfloat colisaoHeroiIlhas(GLfloat xsub, GLfloat ysub, GLfloat zsub){

  for(Island i : islands){


    if(distanciaGeral(xsub,ysub,0, i.getPosX(), i.getPosY(), 0) < p.getMaxRadius()+ i.getRadius()){
      colisaoPlayerIlha ++;
    }

  }
  return colisaoPlayerIlha;
}


GLfloat colisaoPlayerSub=0;
GLfloat colisaoHeroiInimigos(GLfloat xsub, GLfloat ysub, GLfloat zsub){

  for(Submarine::submarine e : enemies){


    if(distanciaGeral(xsub,ysub,zsub, e.getPosX(), e.getPosY(), e.getPosZ()) < (p.getMaxRadius()+ e.getRadius())){
      colisaoPlayerSub ++;
    }

  }
  return colisaoPlayerSub;
}

GLfloat colisaoLimite=0;
GLfloat colisaoHeroiLimite(GLfloat xsub, GLfloat ysub, GLfloat zsub){
    if(distanciaGeral(xsub,ysub,0, 0 , 0, 0) >  world.getRadius() - p.getMaxRadius()){
      colisaoLimite ++;
    }
  return colisaoLimite;
}


void updatePlayer(GLdouble timeDiff) {
  float dist = p.getVel() * timeDiff;
  //Treat keyPress
  if(keyStatus[(int)('w')])
  {
    //cout << colisaoPlayerIlha << endl;
    p.updateHeli(80);
    p.Move(dist);
    if((colisaoHeroiIlhas(p.getPosX(),p.getPosY(),p.getPosZ())>0) || (colisaoHeroiLimite(p.getPosX(),p.getPosY(),p.getPosZ())>0) || (colisaoHeroiInimigos(p.getPosX(),p.getPosY(),p.getPosZ()))>0){
      p.Move(-dist);
      colisaoPlayerIlha = 0;
      colisaoLimite=0;
      colisaoPlayerSub=0;
    }
    }
  if(keyStatus[(int)('d')])
  {
    p.RotateTethaPaddle(+0.05*timeDiff);
  }
  if(keyStatus[(int)('s')])
  {
    //cout << "move backward" << endl;
    p.Move(-dist);
    p.updateHeli(-80);

    if((colisaoHeroiIlhas(p.getPosX(),p.getPosY(),p.getPosZ())>0) || (colisaoHeroiLimite(p.getPosX(),p.getPosY(),p.getPosZ())>0)|| (colisaoHeroiInimigos(p.getPosX(),p.getPosY(),p.getPosZ()))>0){
      p.Move(+dist);
      colisaoPlayerIlha = 0;
      colisaoLimite=0;
      colisaoPlayerSub=0;
    }
  }
  if(keyStatus[(int)('a')])
  {
    p.RotateTethaPaddle(-0.05*timeDiff);
  }
  if(keyStatus[(int)('u')])
  {
    p.submerge(timeDiff*1);
    if(colisaoHeroiInimigos(p.getPosX(),p.getPosY(),p.getPosZ())>0){
      p.submerge(timeDiff*(-1));
      colisaoPlayerSub=0;

    }
  }
  if(keyStatus[(int)('j')])
  {
    p.submerge(timeDiff*(-1));
    if(colisaoHeroiInimigos(p.getPosX(),p.getPosY(),p.getPosZ())>0){
      p.submerge(timeDiff*(1));
      colisaoPlayerSub=0;

    }
  }
  // handling idle moves and collision
  p.updateHeli(timeDiff);
}
// void updatePlayer(GLdouble timeDiff) {
//   float dist = p.getVel() * timeDiff;
//   //Treat keyPress
//   if(keyStatus[(int)('w')])
//   {
//     p.Move(dist);
//   }
//   if(keyStatus[(int)('d')])
//   {
//     p.RotateTethaPaddle(+0.05*timeDiff);
//   }
//   if(keyStatus[(int)('s')])
//   {
//     p.Move(-dist);
//   }
//   if(keyStatus[(int)('a')])
//   {
//     p.RotateTethaPaddle(-0.05*timeDiff);
//   }
//   if(keyStatus[(int)('u')])
//   {
//     p.submerge(timeDiff*1);
//   }
//   if(keyStatus[(int)('j')])
//   {
//     p.submerge(timeDiff*(-1));
//   }
//   // handling idle moves and collision
//   p.updateHeli(timeDiff);
// }

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
        GLfloat pz = enmy->getPosZ();
        GLfloat tetha = enmy->getTethaCenter();
        GLfloat r = enmy->getRadius()/10;
      
        e_torpedos.push_back(Torpedo(px, py, pz, tetha, 0.0, r, -1, 0.0, 0.0));
      }
    }
    enmy->updateHeli(timeDiff);
  }
}

void updateTorpedos(GLdouble timediff) {
  for (tps = torpedos.begin(); tps != torpedos.end(); ++tps) {
    tps->shot(timediff, p.getVelTiro());
    if (sqrt(pow((tps->getPosX()-world.getPosX()),2) + pow((tps->getPosY()-world.getPosY()),2)) >= world.getRadius() - tps->getRadius()) {
      tps->setToDelete(true);
    }
    cout << tps->getPosX() << " " << tps->getPosY() << endl;
  }
  cout << torpedos.size() << endl;
  for (Torpedo tps : e_torpedos) tps.shot(timediff, enemy_vel_tiro);
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
        //cy = 2*world.getPosY() - cy;
        if (r > biggest_island_radius) biggest_island_radius = r;
        cy = -(cy-world_pos_y);
        cx = (cx - world_pos_x);
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
  glutPassiveMotionFunc(dragAndDrop);
  glutMotionFunc(dragAndDrop);
  glutMainLoop();


	return 0;
}
