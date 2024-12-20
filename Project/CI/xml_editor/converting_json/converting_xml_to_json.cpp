#include "converting_xml_to_json.h"

Element parseXML_json(const string& xml) {
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
// Convert an Element tree to JSON format
void json(const Element& element, string& jsonn, int indent ) {
    string indentation(indent, ' ');
    jsonn += indentation + "\"" + element.tag + "\": ";
    if (element.children.empty()) {
        jsonn += "\"" + element.value + "\"";
    }
    else {
        bool isArray = element.tag == "users" || element.tag == "posts" || element.tag == "followers";
        jsonn += isArray ? "[\n" : "{\n";
        for (size_t i = 0; i < element.children.size(); ++i) {
            json(element.children[i], jsonn, indent + 2);
            if (i < element.children.size() - 1) {
                jsonn += ",";
            }
            jsonn += "\n";
        }
        jsonn += indentation + (isArray ? "]" : "}");
    }
}

