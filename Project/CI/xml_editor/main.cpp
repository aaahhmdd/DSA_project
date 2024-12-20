#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "network analysis/NetworkAnalysis.h"  // Include the NetworkAnalysis header for user analysis functions
#include "formatting/formatting.h"
#include "check_consistency/check_xml_consistency.h"
#include "converting_json/converting_xml_to_json.h"
#include "compress/compressing.h"
#include "decompress/decompress.h"
#include "representing xml/representing.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Usage: xml_editor <command> -i input_file.xml [-f] [-o output_file]" << endl;
        return 1;
    }

    string command = argv[1];

    // Initialize the NetworkAnalysis object
    NetworkAnalysis networkAnalysis;

    // Command handling
    if (command == "verify") {
        string inputFilePath;
        string outputFilePath;
        bool fixErrors = false;

        for (int i = 2; i < argc; i++) {
            string arg = argv[i];
            if (arg == "-i" && i + 1 < argc) {
                inputFilePath = argv[++i];
            } else if (arg == "-f") {
                fixErrors = true;
            } else if (arg == "-o" && i + 1 < argc) {
                outputFilePath = argv[++i];
            } else {
                cerr << "Error: Invalid argument " << arg << endl;
                return 1;
            }
        }

        if (inputFilePath.empty()) {
            cerr << "Error: Input file not specified." << endl;
            return 1;
        }

        ifstream inputFile(inputFilePath);
        if (!inputFile.is_open()) {
            cerr << "Error: Could not open input file " << inputFilePath << endl;
            return 1;
        }

        vector<Error> errors;
        stringstream fixedContent;

        bool isValid = verify(inputFile, errors, fixedContent);
        inputFile.close();

        if (isValid) {
            cout << "The XML file is valid." << endl;
        } else {
            cout << "The XML file is invalid." << endl;
            printErrors(errors);
        }

        if (fixErrors && !outputFilePath.empty()) {
            ofstream outputFile(outputFilePath);
            if (!outputFile.is_open()) {
                cerr << "Error: Could not write to output file " << outputFilePath << endl;
                return 1;
            }
            outputFile << fixedContent.str();
            outputFile.close();
            cout << "Fixed XML has been written to " << outputFilePath << endl;
        } else if (fixErrors) {
            cerr << "Error: Output file not specified for fixing." << endl;
        }

    } else if (command == "json") {
        string inputFilePath;
        string outputFilePath;

        for (int i = 2; i < argc; i++) {
            string arg = argv[i];
            if (arg == "-i" && i + 1 < argc) {
                inputFilePath = argv[++i];
            } else if (arg == "-o" && i + 1 < argc) {
                outputFilePath = argv[++i];
            } else {
                cerr << "Error: Invalid argument " << arg << endl;
                return 1;
            }
        }

        if (inputFilePath.empty()) {
            cerr << "Error: Input file not specified." << endl;
            return 1;
        }

        if (outputFilePath.empty()) {
            cerr << "Error: Output file not specified." << endl;
            return 1;
        }

        string xmlContent = readFile(inputFilePath);
        if (xmlContent.empty()) {
            return 1;
        }

        Element root = parseXML_json(xmlContent);
        string jsonn;
        json(root, jsonn, 0);

        ofstream outputFile(outputFilePath);
        if (!outputFile.is_open()) {
            cerr << "Error: Could not open output file for writing." << endl;
            return 1;
        }
        outputFile << jsonn;
        outputFile.close();
        cout << "JSON has been written to " << outputFilePath << endl;

    } else if (command == "compress") {
        string inputFilePath;
        string outputFilePath;

        for (int i = 2; i < argc; i++) {
            string arg = argv[i];
            if (arg == "-i" && i + 1 < argc) {
                inputFilePath = argv[++i];
            } else if (arg == "-o" && i + 1 < argc) {
                outputFilePath = argv[++i];
            } else {
                cerr << "Error: Invalid argument " << arg << endl;
                return 1;
            }
        }

        if (inputFilePath.empty()) {
            cerr << "Error: Input file not specified." << endl;
            return 1;
        }

        if (outputFilePath.empty()) {
            cerr << "Error: Output file not specified." << endl;
            return 1;
        }

        compress(inputFilePath, outputFilePath);

    } else if (command == "mini") {
        string inputFilePath;
        string outputFilePath;

        for (int i = 2; i < argc; i++) {
            string arg = argv[i];
            if (arg == "-i" && i + 1 < argc) {
                inputFilePath = argv[++i];
            } else if (arg == "-o" && i + 1 < argc) {
                outputFilePath = argv[++i];
            } else {
                cerr << "Error: Invalid argument " << arg << endl;
                return 1;
            }
        }

        if (inputFilePath.empty()) {
            cerr << "Error: Input file not specified." << endl;
            return 1;
        }

        if (outputFilePath.empty()) {
            cerr << "Error: Output file not specified." << endl;
            return 1;
        }

        string xmlContent = readFile(inputFilePath);
        if (xmlContent.empty()) {
            return 1;
        }

        string formattedContent = format(xmlContent);
        writeFile(outputFilePath, formattedContent);
        cout << "Mini XML has been written to " << outputFilePath << endl;

    } else if (command == "decompress") {
        string inputFilePath;
        string outputFilePath;

        for (int i = 2; i < argc; i++) {
            string arg = argv[i];
            if (arg == "-i" && i + 1 < argc) {
                inputFilePath = argv[++i];
            } else if (arg == "-o" && i + 1 < argc) {
                outputFilePath = argv[++i];
            } else {
                cerr << "Error: Invalid argument " << arg << endl;
                return 1;
            }
        }

        if (inputFilePath.empty()) {
            cerr << "Error: Input file not specified." << endl;
            return 1;
        }

        if (outputFilePath.empty()) {
            cerr << "Error: Output file not specified." << endl;
            return 1;
        }

        decompress(inputFilePath, outputFilePath);  // Decompression function called

    } else if (command == "draw") {
        string inputFilePath;
        string outputFilePath;

        for (int i = 2; i < argc; i++) {
            string arg = argv[i];
            if (arg == "-i" && i + 1 < argc) {
                inputFilePath = argv[++i];
            } else if (arg == "-o" && i + 1 < argc) {
                outputFilePath = argv[++i];
            } else {
                cerr << "Error: Invalid argument " << arg << endl;
                return 1;
            }
        }

        if (inputFilePath.empty()) {
            cerr << "Error: Input file not specified." << endl;
            return 1;
        }

        if (outputFilePath.empty()) {
            cerr << "Error: Output file not specified." << endl;
            return 1;
        }

        string xmlContent = readFile(inputFilePath);
        if (xmlContent.empty()) {
            cerr << "Error reading XML file." << endl;
            return 1;
        }

        Elementt root = parseXML(xmlContent);
        map<string, vector<string>> userFollowersMap;
        toMap(root, userFollowersMap);

        Graph<string> graph(10);
        generateGraphFromMap(userFollowersMap, graph);
        graph.VisualizeGraph(outputFilePath);

        cout << "Graph has been written to " << outputFilePath << endl;

    } else if (command == "mutual") {
        string inputFilePath;
        vector<int> user_ids;

        for (int i = 2; i < argc; i++) {
            string arg = argv[i];
            if (arg == "-i" && i + 1 < argc) {
                inputFilePath = argv[++i];
            } else if (arg == "-ids" && i + 1 < argc) {
                string ids = argv[++i];
                stringstream ss(ids);
                string id;
                while (getline(ss, id, ',')) {
                    user_ids.push_back(stoi(id));
                }
            } else {
                cerr << "Error: Invalid argument " << arg << endl;
                return 1;
            }
        }

        if (inputFilePath.empty()) {
            cerr << "Error: Input file not specified." << endl;
            return 1;
        }

        if (user_ids.empty()) {
            cerr << "Error: User IDs not specified." << endl;
            return 1;
        }

        networkAnalysis.parse_xml(inputFilePath);
        networkAnalysis.mutual_followers(user_ids);

    } else if (command == "suggest") {
        string inputFilePath;
        int user_id;

        for (int i = 2; i < argc; i++) {
            string arg = argv[i];
            if (arg == "-i" && i + 1 < argc) {
                inputFilePath = argv[++i];
            } else if (arg == "-id" && i + 1 < argc) {
                user_id = stoi(argv[++i]);
            } else {
                cerr << "Error: Invalid argument " << arg << endl;
                return 1;
            }
        }

        if (inputFilePath.empty()) {
            cerr << "Error: Input file not specified." << endl;
            return 1;
        }

        networkAnalysis.parse_xml(inputFilePath);
        networkAnalysis.suggest_users_to_follow(user_id);

    } else if (command == "most_active") {
        string inputFilePath;

        for (int i = 2; i < argc; i++) {
            string arg = argv[i];
            if (arg == "-i" && i + 1 < argc) {
                inputFilePath = argv[++i];
            } else {
                cerr << "Error: Invalid argument " << arg << endl;
                return 1;
            }
        }

        if (inputFilePath.empty()) {
            cerr << "Error: Input file not specified." << endl;
            return 1;
        }

        networkAnalysis.parse_xml(inputFilePath);
        networkAnalysis.most_active_user();

    } else if (command == "most_influencer") {
        string inputFilePath;

        for (int i = 2; i < argc; i++) {
            string arg = argv[i];
            if (arg == "-i" && i + 1 < argc) {
                inputFilePath = argv[++i];
            } else {
                cerr << "Error: Invalid argument " << arg << endl;
                return 1;
            }
        }

        if (inputFilePath.empty()) {
            cerr << "Error: Input file not specified." << endl;
            return 1;
        }

        networkAnalysis.parse_xml(inputFilePath);
        networkAnalysis.display_analysis();

    } else if (command == "format") {

        std::string inputFilePath;
        std::string outputFilePath;
        bool fixErrors = false;

        // Parse command-line arguments
        for (int i = 2; i < argc; i++) {
            std::string arg = argv[i];
            if (arg == "-i" && i + 1 < argc) {
                inputFilePath = argv[++i];
            } else if (arg == "-f") {
                fixErrors = true;
            } else if (arg == "-o" && i + 1 < argc) {
                outputFilePath = argv[++i];
            } else {
                std::cerr << "Error: Invalid argument " << arg << std::endl;
                std::cerr << "Usage: xml_editor format -i input_file.xml [-o output_file.xml]" << std::endl;
                return 1;
            }
        }

        if (inputFilePath.empty()) {
            std::cerr << "Error: Input file not specified." << std::endl;
            return 1;
        }

        // Read and format the XML content
        string xmlContent = readFile(inputFilePath);
        if (xmlContent.empty()) {
            return 1;  // If there's an error reading the file, exit early
        }

        string formattedContent = format(xmlContent);

        // Write the formatted content to the output file
        if (!outputFilePath.empty()) {
            writeFile(outputFilePath, formattedContent);
            std::cout << "Formatted XML has been written to " << outputFilePath << std::endl;
        }
        } else if (command == "search") {
            string inputFilePath;
            string word;

            for (int i = 2; i < argc; i++) {
                string arg = argv[i];
                if (arg == "-i" && i + 1 < argc) {
                    inputFilePath = argv[++i];
                } else if (arg == "-w" && i + 1 < argc) {
                    word = argv[++i];
                } else {
                    cerr << "Error: Invalid argument " << arg << endl;
                    return 1;
                }
            }

            if (inputFilePath.empty()) {
                cerr << "Error: Input file not specified." << endl;
                return 1;
            }

            networkAnalysis.parse_xml(inputFilePath);
            networkAnalysis.search_posts_by_word(word);

        } else {
            cerr << "Error: Unsupported command. Use 'verify', 'json', 'compress', 'mini', 'decompress', 'draw', 'most_influencer', 'most_active', 'mutual', 'suggest', or 'search'." << endl;
            return 1;
        }

        return 0;
    }









