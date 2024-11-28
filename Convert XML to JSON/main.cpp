#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;
struct Element {
    string tag;
    string value;
    vector<Element> children;
};
Element parseXML(const string& xml) {
    Element root;
    stringstream ss(xml);
    string line;
    vector<Element*> stack;
    while (getline(ss, line)) {
        size_t start = line.find('<') + 1;
        size_t end = line.find('>');
        string tag = line.substr(start, end - start);
        if (tag[0] == '/') {
            stack.pop_back();
        }
        else {
            Element element;
            element.tag = tag;
            start = line.find('>') + 1;
            end = line.find('<', start);
            if (end != string::npos) {
                element.value = line.substr(start, end - start);
            }
            if (!stack.empty()) {
                stack.back()->children.push_back(element);
                stack.push_back(&stack.back()->children.back());
            }
            else {
                root = element;
                stack.push_back(&root);
            }
        }
    }
    return root;
}
void toJSON(const Element& element, string& json, int indent = 0) {
    string indentation(indent, ' ');
    json += indentation + "\"" + element.tag + "\": ";
    if (element.children.empty()) {
        json += "\"" + element.value + "\"";
    }
    else {
        bool isArray = element.tag == "users" || element.tag == "posts" || element.tag == "followers";
        json += isArray ? "[\n" : "{\n";
        for (size_t i = 0; i < element.children.size(); ++i) {
            toJSON(element.children[i], json, indent + 2);
            if (i < element.children.size() - 1) {
                json += ",";
            }
            json += "\n";
        }
        json += indentation + (isArray ? "]" : "}");
    }
}
int main() {
    string xml =  "<users>\n" " <user>\n" " <id>1</id>\n" " <name>Ahmed Ali</name>\n" " <posts>\n" " <post>\n" " <body>\n" " Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n" " </body>\n" " <topics>\n" " <topic>\n" " economy\n" " </topic>\n" " <topic>\n" " finance\n" " </topic>\n" " </topics>\n" " </post>\n" " <post>\n" " <body>\n" " Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n" " </body>\n" " <topics>\n" " <topic>\n" " solar_energy\n" " </topic>\n" " </topics>\n" " </post>\n" " </posts>\n" " <followers>\n" " <follower>\n" " <id>2</id>\n" " </follower>\n" " <follower>\n" " <id>3</id>\n" " </follower>\n" " </followers>\n" " </user>\n" " <user>\n" " <id>2</id>\n" " <name>Yasser Ahmed</name>\n" " <posts>\n" " <post>\n" " <body>\n" " Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n" " </body>\n" " <topics>\n" " <topic>\n" " education\n" " </topic>\n" " </topics>\n" " </post>\n" " </posts>\n" " <followers>\n" " <follower>\n" " <id>1</id>\n" " </follower>\n" " </followers>\n" " </user>\n" " <user>\n" " <id>3</id>\n" " <name>Mohamed Sherif</name>\n" " <posts>\n" " <post>\n" " <body>\n" " Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n" " </body>\n" " <topics>\n" " <topic>\n" " sports\n" " </topic>\n" " </topics>\n" " </post>\n" " </posts>\n" " <followers>\n" " <follower>\n" " <id>1</id>\n" " </follower>\n" " </followers>\n" " </user>\n" "</users>";
    Element root = parseXML(xml);
    string json = "{\n";
    toJSON(root, json, 2);
    json += "\n}";
    cout << json << endl;
    return 0;
}
