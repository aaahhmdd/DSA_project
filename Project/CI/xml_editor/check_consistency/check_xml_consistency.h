#ifndef CHECK_XML_CONSISTENCY_H
#define CHECK_XML_CONSISTENCY_H


#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <regex>
#include <fstream>

struct Error {
    std::string message;
    int line;
};

struct Tag {
    std::string name;
    int line;
};

// Function declarations
bool verify(std::ifstream& file, std::vector<Error>& errors, std::stringstream& fixedContent);

void printErrors(const std::vector<Error>& errors);




#endif //CHECK_XML_CONSISTENCY_H
