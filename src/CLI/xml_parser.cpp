#include "xml_parser.h"
string readFile(const string& filePath) {
    ifstream file(filePath);  // Open the file
    if (!file) {
        cerr << "Error opening file!" << endl;
        return "";
    }

    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    return content;  // Return file content as a string
}
bool validateXML(const string& xmlContent, vector<string>& errors);
// Validates XML for consistency. Returns true if valid, false otherwise. Populates `errors` if invalid.


// Function to process text content by removing spaces and newlines
string processText(const string& s) {
    string processedText = "";
    for (char c : s) {
        if (c != ' ' && c != '\n') {
            processedText += c;
        }
    }
    return processedText;
}

// Recursive function to traverse XML nodes and build a compact XML structure
void traverseAndProcess(XMLNode* node, XMLDocument& newXmlDoc, XMLElement* parentElement) {
    if (node->ToElement()) {
        // Create a new element in the new XML document
        XMLElement* currentElement = newXmlDoc.NewElement(node->ToElement()->Name());
        parentElement->InsertEndChild(currentElement);

        // Process text content inside the element (if it exists)
        if (node->ToElement()->GetText()) {
            string processedText = processText(node->ToElement()->GetText());
            currentElement->SetText(processedText.c_str());  // Set processed text for the current tag
        }

        // Traverse child nodes recursively
        for (XMLNode* child = node->FirstChild(); child != nullptr; child = child->NextSibling()) {
            traverseAndProcess(child, newXmlDoc, currentElement);
        }
    }
}

// Minifying function that takes input and output file paths as parameters
void minifying(const string& inputPath, const string& outputPath) {
    // Load the input XML file
    XMLDocument xmlDoc;
    XMLError eResult = xmlDoc.LoadFile(inputPath.c_str());  // Reading from the provided input path
    if (eResult != XML_SUCCESS) {
        cout << "Error loading XML file!" << endl;
        return;
    }

    // Create a new XML document for the output
    XMLDocument newXmlDoc;
    XMLDeclaration* decl = newXmlDoc.NewDeclaration();
    newXmlDoc.InsertFirstChild(decl);

    // Get the root element of the input file
    XMLNode* pRoot = xmlDoc.FirstChild();
    if (pRoot == nullptr) {
        cout << "No root element found!" << endl;
        return;
    }

    // Create a new root element in the new document
    XMLElement* newRoot = newXmlDoc.NewElement(pRoot->ToElement()->Name());
    newXmlDoc.InsertEndChild(newRoot);

    // Traverse and process the input XML document
    traverseAndProcess(pRoot, newXmlDoc, newRoot);

    // Create XMLPrinter to print the XML compactly (to string)
    XMLPrinter printer(nullptr, true);  // null means print to memory, true enables compact mode

    // Print the XML to a string (instead of a file)
    newXmlDoc.Print(&printer);

    // Save the compact XML to the output file
    ofstream outFile(outputPath);
    outFile << printer.CStr(); // Write the compact XML to the file
    outFile.close();

    cout << "Processed XML saved to " << outputPath << " successfully." << endl;
}


string fixXML(const string& xmlContent);
// Attempts to fix XML inconsistencies and returns the corrected XML.
