#include "graph_parser.h"
#include <sstream>
#include <iostream>
#include "graph.h"

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

Element parseXML(const std::string& xml) {
    Element root;
    std::stringstream ss(xml);
    std::string line;
    std::vector<Element*> stack;

    while (getline(ss, line)) {
        bool flag = false;
        size_t start = line.find('<') + 1;
        size_t end = line.find('>');
        if (start == std::string::npos || end == std::string::npos) {
            stack.back()->value = trim(line);
            continue;
        }
        std::string tag = line.substr(start, end - start);
        if (tag[0] == '/') {
            stack.pop_back();
        } else {
            Element element;
            element.tag = tag;
            start = line.find('>') + 1;
            end = line.find('<', start);
            if (end != std::string::npos) {
                flag = true;
                element.value = line.substr(start, end - start);
            }

            if (stack.empty()) {
                root = element;
                stack.push_back(&root);
            } else {
                stack.back()->children.push_back(element);
                if (!flag) stack.push_back(&stack.back()->children.back());
            }
        }
    }
    return root;
}

void toMap(const Element& element, std::map<std::string, std::vector<std::string>>& userFollowersMap) {
    if (element.tag == "user") {
        std::string userId;
        std::vector<std::string> followers;
        for (const auto& child : element.children) {
            if (child.tag == "id") {
                userId = child.value;
            } else if (child.tag == "followers") {
                for (const auto& follower : child.children) {
                    if (follower.tag == "follower") {
                        for (const auto& followerChild : follower.children) {
                            if (followerChild.tag == "id") {
                                followers.push_back(followerChild.value);
                            }
                        }
                    }
                }
            }
        }
        if (!userId.empty()) {
            userFollowersMap[userId] = followers;
            std::cout << std::endl;
        }
    }
    for (const auto& child : element.children) {
        toMap(child, userFollowersMap);
    }
}

void generateGraphFromMap(const std::map<std::string, std::vector<std::string>>& userFollowersMap, Graph<std::string>& graph) {
    for (const auto& userEntry : userFollowersMap) {
        const std::string& userId = userEntry.first;
        graph.AddVertex(userId);
    }
    for (const auto& userEntry : userFollowersMap) {
        const std::string& userId = userEntry.first;
        const std::vector<std::string>& followers = userEntry.second;

        for (const std::string& follower : followers) {
            graph.AddEdge(follower, userId);
        }
    }
}
