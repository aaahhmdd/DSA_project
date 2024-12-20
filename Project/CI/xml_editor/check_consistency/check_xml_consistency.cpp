#include "check_xml_consistency.h"
#include <iostream>

bool verify(std::ifstream& file, std::vector<Error>& errors, std::stringstream& fixedContent) {
    std::stack<Tag> tagStack;
    std::regex openTagPattern(R"(<([a-zA-Z0-9_]+)>)");
    std::regex closeTagPattern(R"(</([a-zA-Z0-9_]+)>)");

    std::string line;
    int lineNumber = 0;
    bool isBalanced = true;

    while (std::getline(file, line)) {
        lineNumber++;
        std::string originalLine = line;
        line.erase(0, line.find_first_not_of(" \t")); // Trim leading spaces

        if (line.find("<!--") == 0 || line.find("<?") == 0) {
            fixedContent << originalLine << "\n"; // Add unchanged comments or declarations
            continue;
        }

        std::smatch match;

        // Search for opening tags
        while (std::regex_search(line, match, openTagPattern)) {
            tagStack.push({match[1].str(), lineNumber});
            fixedContent << "<" << match[1].str() << ">";
            line = match.suffix();
        }

        // Search for closing tags
        while (std::regex_search(line, match, closeTagPattern)) {
            if (tagStack.empty()) {
                errors.push_back({"Closing tag </" + match[1].str() + "> has no matching opening tag.", lineNumber});
                isBalanced = false;
                // Skip writing unmatched closing tag
            } else {
                Tag lastOpenTag = tagStack.top();
                tagStack.pop();
                if (lastOpenTag.name != match[1].str()) {
                    errors.push_back({"Mismatched tag. Expected </" + lastOpenTag.name + "> but found </" + match[1].str() + ">.", lineNumber});
                    isBalanced = false;
                    fixedContent << "</" << lastOpenTag.name << ">"; // Correct the closing tag
                } else {
                    fixedContent << "</" << match[1].str() << ">"; // Add the correct closing tag
                }
            }
            line = match.suffix();
        }

        fixedContent << "\n"; // Preserve line breaks
    }

    // Final check for unmatched opening tags
    while (!tagStack.empty()) {
        Tag unmatchedTag = tagStack.top();
        tagStack.pop();
        errors.push_back({"Opening tag <" + unmatchedTag.name + "> has no matching closing tag.", unmatchedTag.line});
        fixedContent << "</" << unmatchedTag.name << ">\n"; // Add missing closing tag
        isBalanced = false;
    }

    return isBalanced;
}

void printErrors(const std::vector<Error>& errors) {
    std::cout << "Number of errors: " << errors.size() << std::endl;
    for (const auto& error : errors) {
        std::cout << "Line " << error.line << ": " << error.message << std::endl;
    }
}
