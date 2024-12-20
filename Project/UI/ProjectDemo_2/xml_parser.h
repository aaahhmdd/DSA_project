#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stack>
#include <sstream>
void writeFile(const std::string& filePath, const std::string& content);
std::string readFile(const std::string& filePath);
bool validateXML(const std::string& xmlContent, std::vector<std::string>& errors);
std::string prettifyXML(const std::string& xmlContent);
std::string minifyXML(const std::string& xmlContent);
std::string fixXML(const std::string& xmlContent);

#endif // XML_PARSER_H
