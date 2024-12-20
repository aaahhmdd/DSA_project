#ifndef XMLFUNCTION_H
#define XMLFUNCTION_H

#include "tag.h"
#include <fstream>
#include <stack>
#include <regex>
#include <iostream>

// Function declaration
bool isXMLBalanced(std::ifstream& file, std::ostringstream& log);

#endif // XMLFUNCTION_H
