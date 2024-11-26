#include <iostream>
#include <fstream> // 34an reading file
#include <stack>
#include <string>
#include <regex>


using namespace std;

struct Tag {    // 34an store tag name w line number bta3ha
    string name;
    int line;
};

bool isXMLBalanced(ifstream& file) { // input xml file

    stack<Tag> tagStack;                        // stack mn no3 Tag 

    regex openTagPattern(R"(<([a-zA-Z0-9]+)>)");    // chat gpt function to detect open tag
    regex closeTagPattern(R"(</([a-zA-Z0-9]+)>)");  // chat gpt function to detect close tag

    string line;                                    // string 25zn fyh kol str fy file
    int lineNumber = 0;                             // rqm str dh 
    bool isBalanced = true;

    while (getline(file, line)) {                   // function to read file line by line

        lineNumber++;
        line.erase(0, line.find_first_not_of(" \t"));  // gpt function to Trim leading spaces

        
        if (line.find("<!--") == 0 || line.find("<?") == 0) 
            continue;  // lma yb2a fyh comment 3nd bdayt str skip, .find() btrg3 position  
        

        smatch match; // bt5zn result bta3t regsearch

        // Search for opening tags
        while (regex_search(line, match, openTagPattern)) {
            tagStack.push({match[1].str(), lineNumber});    // Push the tag name and line number
            line = match.suffix();                          // Update 'line' to the unmatched part of the string
        }


        // Search for closing tags
        while (regex_search(line, match, closeTagPattern)) {
            if (tagStack.empty()) {
                cout << "Error: Closing tag </" << match[1].str() 
                    << "> on line " << lineNumber << " has no matching opening tag.\n";
                isBalanced = false;
            } 
            else {
                Tag lastOpenTag = tagStack.top();
                tagStack.pop();
                if (lastOpenTag.name != match[1].str()) {
                    cout << "Error: Mismatched tag on line " << lineNumber 
                        << ". Expected </" << lastOpenTag.name << "> but found </" << match[1].str() << ">.\n";
                    isBalanced = false;
                }
            }
            line = match.suffix();  // Update 'line' to the unmatched part of the string
        }

    }

    // Final check for unmatched opening tags
    while (!tagStack.empty()) {
        Tag unmatchedTag = tagStack.top();
        tagStack.pop();
        cout << "Error: Opening tag <" << unmatchedTag.name 
            << "> on line " << unmatchedTag.line << " has no matching closing tag.\n";
        isBalanced = false;
    }

    return isBalanced;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
    string filename;
    cout << "Enter the XML file name: ";
    cin >> filename;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }

    if (isXMLBalanced(file)) {
        cout << "The XML file is balanced." << endl;
    } else {
        cout << "The XML file is not balanced." << endl;
    }

    file.close();
    return 0;
}
