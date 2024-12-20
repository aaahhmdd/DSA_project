#ifndef DECOMPRESSION_H
#define DECOMPRESSION_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Custom Pairde Structure to represent dictionary entries
struct Pairde {
    string token;
    string pattern;
};


// Function to parse the dictionary from the compressed file
void parseDictionary(ifstream& input, vector<Pairde>& dictionary);

// Function to replace tokens with their original patterns in the content
string replaceTokensWithPatterns(const string& content, const vector<Pairde>& dictionary);

// Function to decompress the compressed file
void decompress(const string& inputFile, const string& outputFile);

#endif // DECOMPRESSION_H




