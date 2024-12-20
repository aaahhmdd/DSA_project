#ifndef REPRESENTING_H
#define REPRESENTING_H

#include <string>
#include <vector>
#include <map>
#include "Graph.h"

struct Elementt {
    std::string tag;
    std::string value;
    std::vector<Elementt> children;
};

std::string trim(const std::string& str);
Elementt parseXML(const std::string& xml);
void generateGraphFromMap(const std::map<std::string, std::vector<std::string>>& userFollowersMap, Graph<std::string>& graph);
void toMap(const Elementt& Elementt, std::map<std::string, std::vector<std::string>>& userFollowersMap);



#endif //REPRESENTING_H
