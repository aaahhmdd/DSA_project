#include "minifyxml.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

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

// Function to minify the XML content
void minifyXML(const string& inputPath, const string& outputPath) {
    // Read the input XML file
    ifstream inputFile(inputPath);
    if (!inputFile.is_open()) {
        cout << "Error opening the input file!" << endl;
        return;
    }

    stringstream buffer;
    buffer << inputFile.rdbuf(); // Read entire file into buffer
    string xmlContent = buffer.str();

    // Process the XML content to remove unnecessary spaces and newlines
    string processedXML = processText(xmlContent);

    // Save the processed XML to the output file
    ofstream outputFile(outputPath);
    if (!outputFile.is_open()) {
        cout << "Error opening the output file!" << endl;
        return;
    }

    outputFile << processedXML; // Write the compact XML to the file
    outputFile.close();

    cout << "Processed XML saved to " << outputPath << " successfully." << endl;
}
