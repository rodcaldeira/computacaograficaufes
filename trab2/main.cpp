/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.cc
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
#include "xmlreader.hpp"
#include "player.hpp"
using namespace std;

// variaveis
string path_to_svg;
Player p;
GLint janela_x;
GLint janela_y;
GLfloat ortho_x;
GLfloat ortho_y;
GLfloat ortho_z;
GLfloat ortho_radius;
/*
ref para funcao str2int
https://stackoverflow.com/questions/16388510/evaluate-a-string-with-a-switch-in-c#answer-16388610
utilizada para usar o switch case com string
*/
constexpr unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

void display(void)
{
   /* Limpar todos os pixels  */
   glClear (GL_COLOR_BUFFER_BIT);

   /* Desenhar um polígono branco (retângulo) */
   p.Desenha();

   /* Não esperar! */
   glutSwapBuffers ();
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
  tinyxml2::XMLError e_result = xml_doc_config.LoadFile(path_to.c_str());

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
  cout << path_to_svg << endl;
// inicio da leitura do svg
	e_result = xml_svg_file.LoadFile(path_to_svg.c_str());

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
				cout << "retirar daqui valores do mar" << endl;
				break;
			case str2int("red"):
				// red em rgb: 0, 0, 1
				rgb[0] = 1.0;
				rgb[1] = 0.0;
				rgb[2] = 0.0;
				cout << "informacoes do enemy" << endl;
				break;
			case str2int("green"):
				// azul em rgb: 0, 0, 1
				rgb[0] = 0.0;
				rgb[1] = 1.0;
				rgb[2] = 0.0;
        p.setPosX(cx);
        p.setPosY(cy);
        p.setRadius(r);
        p.setColor(rgb);
        p.setId(id);
				cout << p.getPosX() << endl;
				break;
			case str2int("black"):
				// azul em rgb: 0, 0, 1
				rgb[0] = 0.0;
				rgb[1] = 0.0;
				rgb[2] = 0.0;
				cout << "info ilha" << endl;
				break;
			default:
				break;
		}


		tmp_xml = tmp_xml->NextSiblingElement("circle");
	}

	/*XMLCheckResult(e_result);
	if (xmlReader(xml_doc_config, path_to_svg) == 1) {
      cout << 1 << endl;
  }
	if (path_to_svg == nullptr) {
		cout << "error" << endl;
	} else {
		cout << path_to_svg << endl;
	}*/

	return 0;
}
