#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
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
  string filename;
  cout << "Enter the path to the XML file: ";
      cin >> filename;
      ifstream file(filename);
      if (!file.is_open()) {
          cerr << "Error opening file!" << endl;
          return 1;
      }
      stringstream buffer;
      buffer << file.rdbuf();
      string xml = buffer.str();
      file.close();
      Element root = parseXML(xml);
      string json = "{\n";
      toJSON(root, json, 2);
      json += "\n}";
      cout << "Converted JSON:" << endl;
      cout << json << endl;
    return 0;
}
