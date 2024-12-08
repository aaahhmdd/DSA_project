    #include "xml_editor.h"
    using namespace std;
    int main(int argc, char* argv[]) {
    string inputFilePath = "input.xml";
    string outputFilePath = "output.xml";

    // Read the XML file
    string xmlContent = readFile(inputFilePath);

    if (xmlContent.empty()) {
        return 1;
    }

    // Prettify the XML content
    string prettifiedXML = prettifyXML(xmlContent);

    // Write the prettified XML to an output file
    writeFile(outputFilePath, prettifiedXML);

    cout << "Prettified XML written to: " << outputFilePath << endl;

    return 0;
        return 0;
    }
