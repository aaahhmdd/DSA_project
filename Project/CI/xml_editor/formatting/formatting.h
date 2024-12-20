
#ifndef FORMATTING_H
#define FORMATTING_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stack>
#include <sstream>
void writeFile(const std::string& filePath, const std::string& content);
std::string readFile(const std::string& filePath);
std::string format(const std::string& xmlContent);


#endif //FORMATTING_H
