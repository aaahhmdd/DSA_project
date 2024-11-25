#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <string>
#include <vector>
using namespace std;
bool validateXML(const string& filePath, vector<string>& errors);
string prettifyXML(const string& xmlContent);
string minifyXML(const string& xmlContent);
string fixXML(const string& xmlContent);

#endif // XML_PARSER_H
