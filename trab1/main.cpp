// trabalho  1
// para referencia futuras em caso de xml mais complexos
// https://shilohjames.wordpress.com/2014/04/27/tinyxml2-tutorial/

#include <cstddef>
#include <iostream>
#include <sstream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "tinyxml2/tinyxml2.h"

using namespace std;

GLint circle_radius;
GLfloat circle_RGB[3];
GLfloat circle_R;
// variavel cor do background
GLfloat bg_RGB[3];
GLfloat bg_R;
// window size 0 - width; 1 - height;
GLint wdw_dim[2];

bool click_on_circle = false; // flag para verificar se foi clicado dentro do circulo
// pos do x quando foi feito o click - serve para calcular o gx e gy
GLfloat pos_x_on_click = 0.0;
GLfloat pos_y_on_click = 0.0;

const char *title;
bool* mouseStates = new bool[3];

class Circle {
  private:
    int radius;
    int pos[2]; // 0 - x; 1 - y
  public:
    Circle(int x, int y, int r) {
      radius = r;
      pos[0] = x;
      pos[1] = y;
    }
};



void mouse(int button, int state, int x, int y) {
    int x_size_window = glutGet(GLUT_WINDOW_WIDTH); // get window width
    int y_size_window = glutGet(GLUT_WINDOW_HEIGHT); // get window height
    y = y_size_window - y;

    double x_in_ortho, y_in_ortho;
    x_in_ortho = 1.0 / (double) x_size_window; // valor da unidade em relacao a x
    y_in_ortho = 1.0 / (double) y_size_window; // valor da unidade em relacao a y

    if (button == GLUT_LEFT_BUTTON) {  // verifica se foi o botao esquerdo o clicado
        if (state == GLUT_DOWN) { // se o botão foi apertado
            mouseStates[button] = true; // seta estado do botao como true (sem finalidade)
            pos_x_on_click = x_in_ortho * x;
            pos_y_on_click = y_in_ortho * y;
            cout << "left button pressed down " << pos_x_on_click << "," << pos_y_on_click << endl;
        } else { // soltou o botao esquerdo
            mouseStates[button] = false;
            cout << "left button released" << endl;
        }

    } else if (button == GLUT_RIGHT_BUTTON) { // click do botao direito
        cout << "right button pressed or released" << endl;
    }
}


int XMLCheckResult(int a_eResult)
{
    if (a_eResult != tinyxml2::XML_SUCCESS) {
    std::printf("Error: %i\n", a_eResult);
    return a_eResult;
  }
  return 1;
}

int xmlReader(tinyxml2::XMLDocument &xml_doc) {

  tinyxml2::XMLError e_result;
  // leitura de janela
  tinyxml2::XMLNode * app_root = xml_doc.FirstChild();
  if (app_root == nullptr) return tinyxml2::XML_ERROR_FILE_READ_ERROR;

  tinyxml2::XMLElement * app_ele = app_root->FirstChildElement("janela");
  if (app_ele == nullptr) return tinyxml2::XML_ERROR_PARSING_ELEMENT;
  int wdw_width, wdw_height;
  e_result = app_ele->QueryIntAttribute("largura", &wdw_dim[0]);
  XMLCheckResult(e_result);
  e_result = app_ele->QueryIntAttribute("altura", &wdw_dim[1]);
  XMLCheckResult(e_result);

  tinyxml2::XMLElement * tmp_xml = app_ele->FirstChildElement("fundo");
  if (tmp_xml == nullptr) return tinyxml2::XML_ERROR_PARSING_ELEMENT;
  double bg_color[3]; // 0 - red; 1 - green; 2 - blue
  e_result = tmp_xml->QueryDoubleAttribute("corR", &bg_color[0]);
  XMLCheckResult(e_result);
  e_result = tmp_xml->QueryDoubleAttribute("corG", &bg_color[1]);
  XMLCheckResult(e_result);
  e_result = tmp_xml->QueryDoubleAttribute("corB", &bg_color[2]);
  XMLCheckResult(e_result);

  int i;
  for (i = 0; i < 3; i++) bg_RGB[i] = bg_color[i];

  tmp_xml = app_ele->FirstChildElement("titulo");
  if (tmp_xml == nullptr) return tinyxml2::XML_ERROR_PARSING_ELEMENT;

  title = tmp_xml->GetText();

  // final da leitura da janela

  // leitura de circulo
  app_ele = app_root->FirstChildElement("circulo");
  if (app_ele == nullptr) return tinyxml2::XML_ERROR_PARSING_ELEMENT;
  e_result = app_ele->QueryIntAttribute("raio", &circle_radius);
  XMLCheckResult(e_result);

  double circle_color[3];
  e_result = app_ele->QueryDoubleAttribute("corR", &circle_color[0]);
  XMLCheckResult(e_result);
  e_result = app_ele->QueryDoubleAttribute("corG", &circle_color[1]);
  XMLCheckResult(e_result);
  e_result = app_ele->QueryDoubleAttribute("corB", &circle_color[2]);
  XMLCheckResult(e_result);

  for (i = 0; i < 3; i++) circle_RGB[i] = circle_color[i];
  // final da leitura do circulo
  return 1;
}

void init(void) {

    glClearColor(bg_RGB[0], bg_RGB[1], bg_RGB[2], 0.0);

    /* inicializar sistema de vizualização. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}
void display(void)
{
   /* Limpar todos os pixels  */
   glClear (GL_COLOR_BUFFER_BIT);

   /* Desenhar um polígono branco (retângulo) */
   glColor3f (1.0, 1.0, 1.0);
   glBegin(GL_POLYGON);
      glVertex3f (0.25, 0.25, 0.0);
      glVertex3f (0.75, 0.25, 0.0);
      glVertex3f (0.75, 0.75, 0.0);
      glVertex3f (0.25, 0.75, 0.0);
   glEnd();

   /* Não esperar! */
   glutSwapBuffers ();
}

void Idle(void) {
	glutPostRedisplay();
}
int main(int argc, char** argv) {
  char *path_to;

  if (argc < 2) {
    cout << "need config.xml path to" << endl;
    return 0;
  } else {
    path_to = argv[1];
    strcat(path_to, "config.xml"); // concatena a string para path do config.xml
  }

  tinyxml2::XMLDocument xml_doc;
  tinyxml2::XMLError e_result = xml_doc.LoadFile(path_to);
  XMLCheckResult(e_result);
  xmlReader(xml_doc);

  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize (wdw_dim[0], wdw_dim[1]);
  glutInitWindowPosition (100, 100);
  glutCreateWindow (title);
  init ();
  glutDisplayFunc(display);
  //glutReshapeFunc(reshape);
  glutIdleFunc(Idle); // pq nao funciona sem esse callback?
  glutMouseFunc(mouse);
  glutMainLoop();
  return 0;
}
