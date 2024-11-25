#include "xml_parser.h"
bool validateXML(const string& filePath, vector<string>& errors);
// Validates XML for consistency. Returns true if valid, false otherwise. Populates `errors` if invalid.

string prettifyXML(const string& xmlContent);
// Returns a prettified version of the XML content.

string minifyXML(const string& xmlContent);
// Returns a minified version of the XML content.

string fixXML(const string& xmlContent);
// Attempts to fix XML inconsistencies and returns the corrected XML.
