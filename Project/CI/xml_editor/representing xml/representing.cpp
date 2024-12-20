#include "representing.h"
#include <sstream>
#include <iostream>

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

Elementt parseXML(const std::string& xml) {
    Elementt root;
    std::stringstream ss(xml);
    std::string line;
    std::vector<Elementt*> stack;

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
            Elementt Elementt;
            Elementt.tag = tag;
            start = line.find('>') + 1;
            end = line.find('<', start);
            if (end != std::string::npos) {
                flag = true;
                Elementt.value = line.substr(start, end - start);
            }

            if (stack.empty()) {
                root = Elementt;
                stack.push_back(&root);
            } else {
                stack.back()->children.push_back(Elementt);
                if (!flag) stack.push_back(&stack.back()->children.back());
            }
        }
    }
    return root;
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

void toMap(const Elementt& Elementt, std::map<std::string, std::vector<std::string>>& userFollowersMap) {
    if (Elementt.tag == "user") {
        std::string userId;
        std::vector<std::string> followers;
        for (const auto& child : Elementt.children) {
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
        }
    }
    for (const auto& child : Elementt.children) {
        toMap(child, userFollowersMap);
    }
}
