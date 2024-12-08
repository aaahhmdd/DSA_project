#ifndef MINIFYXML_H
#define MINIFYXML_H

#include <string>

// Function to process text content by removing all spaces and newlines (outside of tags)
std::string processText(const std::string& s);

// Function to minify the XML content
void minifyXML(const std::string& inputPath, const std::string& outputPath);

#endif // MINIFYXML_H
