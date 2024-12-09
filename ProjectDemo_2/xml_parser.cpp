#include "xml_parser.h"
using namespace std;
string readFile(const string& filePath) {
    ifstream file(filePath);  // Open the file
    if (!file) {
        cerr << "Error opening file!" << endl;
        return "";
    }

    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    return content;  // Return file content as a string
}
//reads the xml file and returns it as string
void writeFile(const string& filePath, const string& content) {
    ofstream file(filePath);
    if (!file) {
        cerr << "Error: Cannot open file " << filePath << endl;
        return;
    }
    file << content;
}
//returns the xml output after processing
bool validateXML(const string& xmlContent, vector<string>& errors);
// Validates XML for consistency. Returns true if valid, false otherwise. Populates `errors` if invalid.

string prettifyXML(const string& xmlContent){
    stringstream result;
    stack<string> tagStack;            // Stack to track open tags
    int indentLevel = 0;               // Tracks current indentation level
    const string indent = "    ";      // Four spaces for indentation

    size_t i = 0;
    while (i < xmlContent.size()) {
        if (xmlContent[i] == '<') { // Start of a tag
            size_t endPos = xmlContent.find('>', i);
            if (endPos == string::npos) {
                cerr << "Error: Malformed XML, missing '>'." << endl;
                break;
            }

            string tag = xmlContent.substr(i, endPos - i + 1); // Extract tag
            if (tag[1] == '/') { // Closing tag
                indentLevel--;
                result << string(indentLevel * indent.size(), ' ') << tag << "\n";
                if (!tagStack.empty() && tag.substr(2, tag.size() - 3) == tagStack.top()) {
                    tagStack.pop(); // Pop the matching opening tag
                }
            }  else { // Opening tag
                result << string(indentLevel * indent.size(), ' ') << tag << "\n";
                tagStack.push(tag.substr(1, tag.size() - 2)); // Push the tag name
                indentLevel++;
            }

            i = endPos + 1;
        } else { // Content between tags
            size_t endPos = xmlContent.find('<', i);
            string content = xmlContent.substr(i, endPos - i);
            if (!content.empty()) {
                result << string(indentLevel * indent.size(), ' ') << content << "\n";
            }
            i = endPos;
        }
    }

    if (!tagStack.empty()) {
        cerr << "Error: Mismatched tags in XML file." << endl;
    }

    return result.str();
}
// Returns a prettified version of the XML content.

string minifyXML(const string& xmlContent);
// Returns a minified version of the XML content.

string fixXML(const string& xmlContent);
// Attempts to fix XML inconsistencies and returns the corrected XML.
