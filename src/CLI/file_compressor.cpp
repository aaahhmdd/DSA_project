#include <string>
using namespace std;
bool compressFile(const string& inputFilePath, const string& outputFilePath);
// Compresses the content of `inputFilePath` and writes it to `outputFilePath`. Returns true on success.

bool decompressFile(const string& inputFilePath, const string& outputFilePath);
// Decompresses the content of `inputFilePath` and writes it to `outputFilePath`. Returns true on success.
