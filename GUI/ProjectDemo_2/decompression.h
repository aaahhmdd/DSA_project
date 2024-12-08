#ifndef DECOMPRESSION_H
#define DECOMPRESSION_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Custom Pair Structure to represent dictionary entries
struct Pair {
    string token;
    string pattern;
};

// Function declarations
void buildDictionary(const std::string& content, std::vector<Pair>& dictionary);
std::string replacePatternsWithTokens(const std::string& content, const std::vector<Pair>& dictionary);
void writeCompressedFile(const std::string& outputFile, const std::vector<Pair>& dictionary, const std::string& compressedContent);
void compressXML(const std::string& inputFile, const std::string& outputFile);

// Function to parse the dictionary from the compressed file
void parseDictionary(ifstream& input, vector<Pair>& dictionary);

// Function to replace tokens with their original patterns in the content
string replaceTokensWithPatterns(const string& content, const vector<Pair>& dictionary);

// Function to decompress the compressed file
void decompressFile(const string& inputFile, const string& outputFile);

#endif // DECOMPRESSION_H




