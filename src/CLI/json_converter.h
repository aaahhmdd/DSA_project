#ifndef JSON_CONVERTER_H
#define JSON_CONVERTER_H

#include <string>

std::string convertXMLtoJSON(const std::string& xmlContent);
std::string convertJSONtoXML(const std::string& jsonContent);

#endif // JSON_CONVERTER_H
