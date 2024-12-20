#ifndef CONVERTING_XML_TO_JSON_H
#define CONVERTING_XML_TO_JSON_H

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
using namespace std ;

// Element struct definition
struct Element {
    std::string tag;
    std::string value;
    std::vector<Element> children;
};

// Function declarations
Element parseXML_json(const std::string& xml);
void json(const Element& element, std::string& json, int indent = 0);

#endif //CONVERTING_XML_TO_JSON_H
