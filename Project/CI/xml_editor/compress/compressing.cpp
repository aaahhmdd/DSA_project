#include "compressing.h"



// Function to find unique XML tags and build the dictionary
void buildDictionary(const string& content, vector<Pair>& dictionary) {
    size_t pos = 0;
    int tokenIndex = 1;

    // Find all XML-like tags
    while ((pos = content.find('<', pos)) != string::npos) {
        size_t end = content.find('>', pos);
        if (end != string::npos) {
            string tag = content.substr(pos, end - pos + 1);

            // Check if the tag is already in the dictionary
            bool exists = false;
            for (const auto& entry : dictionary) {
                if (entry.pattern == tag) {
                    exists = true;
                    break;
                }
            }

            // If not in the dictionary, add it with a unique token
            if (!exists) {
                dictionary.push_back({ "T" + to_string(tokenIndex++), tag });
            }

            pos = end + 1; // Move past the current tag
        } else {
            break; // No valid tag found
        }
    }
}

// Function to replace patterns with tokens in the content
string replacePatternsWithTokens(const string& content, const vector<Pair>& dictionary) {
    string compressedContent = content;

    // Replace each pattern with its corresponding token
    for (const auto& entry : dictionary) {
        size_t pos = 0;
        while ((pos = compressedContent.find(entry.pattern, pos)) != string::npos) {
            compressedContent.replace(pos, entry.pattern.length(), entry.token);
            pos += entry.token.length(); // Move past the replaced token
        }
    }

    return compressedContent;
}

// Function to write the dictionary and compressed content to a file
void writeCompressedFile(const string& outputFile, const vector<Pair>& dictionary, const string& compressedContent) {
    ofstream output(outputFile, ios::out);

    if (!output.is_open()) {
        cerr << "Error: Unable to open file for writing." << endl;
        return;
    }

    // Write the dictionary
    for (const auto& entry : dictionary) {
        output << entry.token << "->" << entry.pattern << "\n";
    }

    output << "---\n"; // Separator between dictionary and content

    // Write the compressed content
    output << compressedContent;

    output.close();
    cout << "Compression completed. Output saved to: " << outputFile << endl;
}

// Function to compress the XML file
void compress(const string& inputFile, const string& outputFile) {
    ifstream input(inputFile, ios::in);
    if (!input.is_open()) {
        cerr << "Error: Unable to open input file." << endl;
        return;
    }

    string content, line;
    while (getline(input, line)) {
        content += line + "\n"; // Read the entire XML file
    }
    input.close();

    vector<Pair> dictionary; // Custom vector to store dictionary entries

    buildDictionary(content, dictionary); // Build the dictionary
    string compressedContent = replacePatternsWithTokens(content, dictionary); // Compress the content
    writeCompressedFile(outputFile, dictionary, compressedContent); // Write to the compressed file
}
