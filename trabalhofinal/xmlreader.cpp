#include "xmlreader.hpp"
#include "tinyxml2/tinyxml2.h"
#include <stdlib.h>
#include <string.h>

int XMLCheckResult(int a_eResult) {
    if (a_eResult != tinyxml2::XML_SUCCESS) {
    std::printf("Error: %i\n", a_eResult);
    return a_eResult;
  }
  return 1;
}

int xmlReader(tinyxml2::XMLDocument &xml_doc, char *path) {
	tinyxml2::XMLNode * app_root = xml_doc.FirstChild();
	if (app_root == nullptr) return tinyxml2::XML_ERROR_FILE_READ_ERROR;

	tinyxml2::XMLElement * app_ele = app_root->FirstChildElement("arquivoDaArena");
	if (app_ele == nullptr) return tinyxml2::XML_ERROR_PARSING_ELEMENT;

	tinyxml2::XMLElement * tmp_xml = app_ele->FirstChildElement("caminho");
	if (tmp_xml == nullptr) return tinyxml2::XML_ERROR_PARSING_ELEMENT;


	strcpy(path, tmp_xml->GetText());
  //cout << path_to_svg << endl;
  /*
	tmp_xml = app_ele->FirstChildElement("nome");
	if (tmp_xml == nullptr) return nullptr;
	strcat(path_to_svg, tmp_xml->GetText());
	strcat(path_to_svg, ".");

	tmp_xml = app_ele->FirstChildElement("tipo");
	if (tmp_xml == nullptr) return nullptr;
	strcat(path_to_svg, tmp_xml->GetText());
*/
	return 1;

}
