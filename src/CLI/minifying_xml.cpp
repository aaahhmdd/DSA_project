#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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

int main() {
    // Variables to store input and output file paths
    string inputPath, outputPath;

    // Prompt the user for the input and output file paths
    cout << "Enter the input XML file path: ";
    getline(cin, inputPath);
    cout << "Enter the output XML file path: ";
    getline(cin, outputPath);

    // Call the minifying function
    minifyXML(inputPath, outputPath);

    return 0;
}

