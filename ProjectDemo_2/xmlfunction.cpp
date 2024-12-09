#include "xmlfunction.h"

#include <sstream> // For std::ostringstream

bool isXMLBalanced(std::ifstream& file, std::ostringstream& log) {
    std::stack<Tag> tagStack;

    std::regex openTagPattern(R"(<([a-zA-Z0-9]+)>)");
    std::regex closeTagPattern(R"(</([a-zA-Z0-9]+)>)");

    std::string line;
    int lineNumber = 0;
    bool isBalanced = true;

    while (std::getline(file, line)) {
        lineNumber++;
        line.erase(0, line.find_first_not_of(" \t")); // Trim leading spaces

        if (line.find("<!--") == 0 || line.find("<?") == 0)
            continue; // Skip comments and declarations

        std::smatch match;

        // Search for opening tags
        while (std::regex_search(line, match, openTagPattern)) {
            tagStack.push({match[1].str(), lineNumber});
            line = match.suffix();
        }

        // Search for closing tags
        while (std::regex_search(line, match, closeTagPattern)) {
            if (tagStack.empty()) {
                log << "Error: Closing tag </" << match[1].str()
                << "> on line " << lineNumber << " has no matching opening tag.\n";
                isBalanced = false;
            } else {
                Tag lastOpenTag = tagStack.top();
                tagStack.pop();
                if (lastOpenTag.name != match[1].str()) {
                    log << "Error: Mismatched tag on line " << lineNumber
                        << ". Expected </" << lastOpenTag.name << "> but found </" << match[1].str() << ">.\n";
                    isBalanced = false;
                }
            }
            line = match.suffix();
        }
    }

    // Final check for unmatched opening tags
    while (!tagStack.empty()) {
        Tag unmatchedTag = tagStack.top();
        tagStack.pop();
        log << "Error: Opening tag <" << unmatchedTag.name
            << "> on line " << unmatchedTag.line << " has no matching closing tag.\n";
        isBalanced = false;
    }

    return isBalanced;
}
