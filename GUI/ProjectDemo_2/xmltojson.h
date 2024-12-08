#ifndef XMLTOJSON_H
#define XMLTOJSON_H

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>

// Element struct definition
struct Element {
    std::string tag;
    std::string value;
    std::vector<Element> children;
};

// Function declarations
Element parseXML(const std::string& xml);
void toJSON(const Element& element, std::string& json, int indent = 0);

#endif // XMLTOJSON_H
