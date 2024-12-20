#include "decompression.h"

void parseDictionary(ifstream& input, vector<Pair>& dictionary) {
    string line;
    while (getline(input, line)) {
        if (line == "---") break;

        size_t arrowPos = line.find("->");
        if (arrowPos != string::npos) {
            string token = line.substr(0, arrowPos);
            string pattern = line.substr(arrowPos + 2);
            dictionary.push_back({ token, pattern });
            cout << "Parsed: " << token << " -> " << pattern << endl; // Debugging output
        }
    }
}

// Function to replace tokens with their original patterns in the content
string replaceTokensWithPatterns(const string& content, const vector<Pair>& dictionary) {
    string decompressedContent = content;

    // Replace each token with its corresponding pattern
    for (const auto& entry : dictionary) {
        size_t pos = 0;
        while ((pos = decompressedContent.find(entry.token, pos)) != string::npos) {
            decompressedContent.replace(pos, entry.token.length(), entry.pattern);
            pos += entry.pattern.length(); // Move past the replaced pattern
        }
    }

    return decompressedContent;
}

// Function to decompress the compressed file
void decompressFile(const string& inputFile, const string& outputFile) {
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

    vector<Pair> dictionary; // Custom vector to store dictionary entries

    parseDictionary(input, dictionary); // Parse the dictionary

    string line, compressedContent;
    while (getline(input, line)) {
        compressedContent += line + "\n"; // Read the compressed content
    }

    string decompressedContent = replaceTokensWithPatterns(compressedContent, dictionary); // Decompress the content
    output << decompressedContent; // Write the decompressed content to the output file

    input.close();
    output.close();

    cout << "Decompression completed. Output saved to: " << outputFile << endl;
}
