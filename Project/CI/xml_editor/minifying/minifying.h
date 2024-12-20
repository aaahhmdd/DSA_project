#ifndef MINIFYING_H
#define MINIFYING_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

// Function to process text content by removing all spaces and newlines (outside of tags)
std::string processText(const std::string& s);

// Function to minify the XML content
string mini(const string& xmlContent) ;
#endif //MINIFYING_H
