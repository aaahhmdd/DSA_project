#ifndef GRAPH_PARSER_H
#define GRAPH_PARSER_H

#include <string>
#include <vector>
#include <map>
#include "graph.h"

struct Element {
    std::string tag;
    std::string value;
    std::vector<Element> children;
};

std::string trim(const std::string& str);
Element parseXML(const std::string& xml);
void toMap(const Element& element, std::map<std::string, std::vector<std::string>>& userFollowersMap);
void generateGraphFromMap(const std::map<std::string, std::vector<std::string>>& userFollowersMap, Graph<std::string>& graph);

#endif // GRAPH_PARSER_H
