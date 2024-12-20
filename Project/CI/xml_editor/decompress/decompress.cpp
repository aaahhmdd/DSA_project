#include "decompress.h"

// Function to parse the dictionary from the compressed file
void parseDictionary(ifstream& input, vector<Pairde>& dictionary) {
    string line;
    while (getline(input, line)) {
        if (line == "---") break;  // Stop when separator is reached
        size_t arrowPos = line.find("->");
        if (arrowPos != string::npos) {
            string token = line.substr(0, arrowPos);
            string pattern = line.substr(arrowPos + 2);
            dictionary.push_back({ token, pattern });
        }
    }
}

// Function to replace tokens with their corresponding patterns
string replaceTokensWithPatterns(const string& content, const vector<Pairde>& dictionary) {
    string decompressedContent = content;
    for (const auto& entry : dictionary) {
        size_t pos = 0;
        while ((pos = decompressedContent.find(entry.token, pos)) != string::npos) {
            decompressedContent.replace(pos, entry.token.length(), entry.pattern);
            pos += entry.pattern.length(); // Move past the replaced pattern
        }
    }
    return decompressedContent;
}


// Function to decompress the content from the compressed file
void decompress(const string& inputFile, const string& outputFile) {
    ifstream input(inputFile, ios::in);
    if (!input.is_open()) {
        cerr << "Error: Unable to open input file." << endl;
        return;
    }

    ofstream output(outputFile, ios::out);
    if (!output.is_open()) {
        cerr << "Error: Unable to open output file." << endl;
        return;
    }

    vector<Pairde> dictionary;
    parseDictionary(input, dictionary);  // Read and parse the dictionary

    string line, compressedContent;
    while (getline(input, line)) {
        compressedContent += line + "\n";  // Read the compressed content
    }

    string decompressedContent = replaceTokensWithPatterns(compressedContent, dictionary);  // Decompress the content

    output << decompressedContent;  // Write the decompressed content to output file
    input.close();
    output.close();

    cout << "Decompression completed. Output saved to: " << outputFile << endl;
}
