#include "xml_parser.h"
string readFile(const string& filePath) {
    ifstream file(filePath);  // Open the file
    if (!file) {
        cerr << "Error opening file!" << endl;
        return "";
    }

    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    return content;  // Return file content as a string
}
bool validateXML(const string& xmlContent, vector<string>& errors);
// Validates XML for consistency. Returns true if valid, false otherwise. Populates `errors` if invalid.

string prettifyXML(const string& xmlContent);
// Returns a prettified version of the XML content.

string minifyXML(const string& xmlContent);
// Returns a minified version of the XML content.

string fixXML(const string& xmlContent);
// Attempts to fix XML inconsistencies and returns the corrected XML.
