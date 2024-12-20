#include "minifying.h"




// Function to process text content by removing all spaces and newlines (outside of tags)
string processText(const string& s) {
    string processedText = "";
    bool insideTag = false;

    for (char c : s) {
        if (c == '<') {
            insideTag = true;
            processedText += c;
        } else if (c == '>') {
            insideTag = false;
            processedText += c;
        } else if (!insideTag && (c != ' ' && c != '\n' && c != '\t')) {
            processedText += c;
        } else if (insideTag) {
            processedText += c;
        }
    }

    return processedText;
}

// Mini function to remove unnecessary whitespaces and line breaks in XML
string mini(const string& xmlContent) {
    string result;
    bool insideTag = false;

    for (size_t i = 0; i < xmlContent.size(); ++i) {
        if (xmlContent[i] == '<') {
            insideTag = true;
        }
        if (!isspace(xmlContent[i]) || insideTag) {
            result += xmlContent[i];
        }
        if (xmlContent[i] == '>') {
            insideTag = false;
        }
    }

    return result;
}