#include "xmltojson.h"


// Parse XML into a tree structure of Elements
Element parseXML(const std::string& xml) {
    Element root;
    std::stringstream ss(xml);
    std::string line;
    std::vector<Element*> stack;

    while (std::getline(ss, line)) {
        size_t start = line.find('<') + 1;
        size_t end = line.find('>');
        std::string tag = line.substr(start, end - start);

        if (tag[0] == '/') {
            stack.pop_back(); // Closing tag, remove the last element from the stack
        } else {
            Element element;
            element.tag = tag;

            start = line.find('>') + 1;
            end = line.find('<', start);
            if (end != std::string::npos) {
                element.value = line.substr(start, end - start);
            }

            if (!stack.empty()) {
                stack.back()->children.push_back(element);
                stack.push_back(&stack.back()->children.back());
            } else {
                root = element;
                stack.push_back(&root);
            }
        }
    }

    return root;
}

// Convert an Element tree to JSON format
void toJSON(const Element& element, std::string& json, int indent) {
    std::string indentation(indent, ' ');
    json += indentation + "\"" + element.tag + "\": ";

    if (element.children.empty()) {
        json += "\"" + element.value + "\"";
    } else {
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
