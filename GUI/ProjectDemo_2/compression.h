#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <string>
#include <vector>

// Custom Pair Structure to represent dictionary entries
struct Pair {
    std::string token;
    std::string pattern;
};

// Function declarations
void buildDictionary(const std::string& content, std::vector<Pair>& dictionary);
std::string replacePatternsWithTokens(const std::string& content, const std::vector<Pair>& dictionary);
void writeCompressedFile(const std::string& outputFile, const std::vector<Pair>& dictionary, const std::string& compressedContent);
void compressXML(const std::string& inputFile, const std::string& outputFile);

#endif // COMPRESSION_H
