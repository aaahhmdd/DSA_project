#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include "xmltojson.h" // Include your XML-related functions
#include "xmlfunction.h" // Include the header for isXMLBalanced
#include <fstream>
#include <sstream>
//#include "compression.h"  // Include the compression functions
#include "decompression.h" // Include this header to access the decompression functions
#include "minifyxml.h"
#include "xml_editor.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*void MainWindow::on_pushButton_clicked()
{
    /*
    QString filter = "All Files (*.*) ;; XML File (*.xml)";
    QString file_name = QFileDialog::getOpenFileName(this,"title","C://",filter);
    QFile file(file_name) ; // one arg -> the location of the file
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this,"title","file not open");
    }
    QTextStream in(&file);
    QString text = in.readAll() ;
    file.close();


    // Open file dialog to select an XML file
    QString filter = "XML File (*.xml) ;; All Files (*.*)";
    QString file_name = QFileDialog::getOpenFileName(this, "Select XML File", "C://", filter);

    // Check if a file was selected
    if (file_name.isEmpty()) {
        QMessageBox::warning(this, "No File Selected", "Please select a valid XML file.");
        return;
    }

    // Open the file using std::ifstream for compatibility with isXMLBalanced
    std::ifstream file(file_name.toStdString());
    if (!file.is_open()) {
        QMessageBox::critical(this, "Error", "Failed to open the selected file.");
        return;
    }

    // Call the isXMLBalanced function and capture the result
    bool balanced = isXMLBalanced(file);

    // Close the file after processing
    file.close();

    // Show the result in a message box
    if (balanced) {
        QMessageBox::information(this, "XML Validation", "The XML file is balanced.");
    } else {
        QMessageBox::warning(this, "XML Validation", "The XML file is not balanced.");
    }
}
    */


QString MainWindow::saveTextToTempFile(const QString& text) {
    QString tempFilePath = QDir::tempPath() + "/temp_input.xml";
    QFile tempFile(tempFilePath);

    if (tempFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&tempFile);
        out << text;
        tempFile.close();
        return tempFilePath; // Return the path of the temporary file
    } else {
        QMessageBox::critical(this, "Error", "Unable to create temporary file!");
        return QString();
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString inputFilePath;

    // Step 1: Check if textEdit has content
    if (!ui->textEdit->toPlainText().isEmpty()) {
        inputFilePath = saveTextToTempFile(ui->textEdit->toPlainText());
        if (inputFilePath.isEmpty()) {
            QMessageBox::critical(this, "Error", "Failed to save the content to a temporary file.");
            return; // Exit if temporary file creation fails
        }
    } else {
        // Step 1.2: Open file dialog to select the input file
        inputFilePath = QFileDialog::getOpenFileName(this, "Select Input File", "", "XML Files (*.xml);;All Files (*.*)");
        if (inputFilePath.isEmpty()) {
            QMessageBox::information(this, "No File Selected", "Please select a file or type content in the text box.");
            return;
        }
    }

    // Step 2: Open the file for reading
    std::ifstream file(inputFilePath.toStdString());
    if (!file.is_open()) {
        QMessageBox::critical(this, "Error", "Failed to open the selected file.");
        return;
    }

    // Step 3: Prepare log collector
    std::ostringstream log;

    // Step 4: Call the modified isXMLBalanced function
    bool balanced = isXMLBalanced(file, log);

    file.close();

    // Step 5: Prepare result message and log
    QString resultMessage = balanced ? "The XML file is balanced.\n" : "The XML file is not balanced.\n";
    resultMessage += "\nDetails:\n" + QString::fromStdString(log.str());

    // Step 6: Display result and log in the textEdit_OutputXML field
    ui->textEdit_2->setPlainText(resultMessage);

    // Step 7: Notify the user
    QMessageBox::information(this, "XML Validation Result", "The validation result has been displayed.");
}




void MainWindow::on_pushButton_2_clicked()
{
    QString inputFilePath;

    // Step 1: Check if textEdit has content
    if (!ui->textEdit->toPlainText().isEmpty()) {
        inputFilePath = saveTextToTempFile(ui->textEdit->toPlainText());
        if (inputFilePath.isEmpty()) {
            QMessageBox::critical(this, "Error", "Failed to save the content to a temporary file.");
            return; // Exit if temporary file creation fails
        }
    } else {
        // Step 1.2: Open file dialog to select the input file
        inputFilePath = QFileDialog::getOpenFileName(this, "Select Input File", "", "XML Files (*.xml);;All Files (*.*)");
        if (inputFilePath.isEmpty()) {
            QMessageBox::information(this, "No File Selected", "Please select a file or type content in the text box.");
            return;
        }
    }

    // Step 2: Read the XML file content
    std::ifstream file(inputFilePath.toStdString());
    if (!file.is_open()) {
        QMessageBox::critical(this, "Error", "Failed to open the selected XML file.");
        return;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string xmlContent = buffer.str();
    file.close();

    // Step 3: Convert the XML content to JSON
    std::string jsonContent;
    try {
        Element root = parseXML(xmlContent); // Parse the XML content into a tree structure
        jsonContent = "{\n";
        toJSON(root, jsonContent, 2); // Convert the XML tree to JSON
        jsonContent += "\n}";
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to convert XML to JSON: %1").arg(e.what()));
        return;
    }

    // Step 4: Display the JSON content in the read-only text field
    ui->textEdit_2->setPlainText(QString::fromStdString(jsonContent));

    // Step 5: Notify the user
    QMessageBox::information(this, "Operation Completed", "JSON content is displayed in the text field.");
}



void MainWindow::on_pushButton_3_clicked()
{
    QString inputFilePath;

    // Step 1: Check if textEdit has content
    if (!ui->textEdit->toPlainText().isEmpty()) {
        inputFilePath = saveTextToTempFile(ui->textEdit->toPlainText());
        if (inputFilePath.isEmpty()) {
            QMessageBox::critical(this, "Error", "Failed to save the content to a temporary file.");
            return; // Exit if temporary file creation fails
        }
    } else {
        // Step 1.2: Open file dialog to select the input file
        inputFilePath = QFileDialog::getOpenFileName(this, "Select Input File", "", "XML Files (*.xml);;All Files (*.*)");
        if (inputFilePath.isEmpty()) {
            QMessageBox::information(this, "No File Selected", "Please select a file or type content in the text box.");
            return;
        }
    }

    // Step 2: Read the XML file content
    std::ifstream file(inputFilePath.toStdString());
    if (!file.is_open()) {
        QMessageBox::critical(this, "Error", "Failed to open the selected XML file.");
        return;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string xmlContent = buffer.str();
    file.close();

    // Step 3: Build the dictionary and compress the content
    std::vector<Pair> dictionary;
    std::string compressedContent;
    try {
        // Build dictionary and replace patterns with tokens
        buildDictionary(xmlContent, dictionary);
        compressedContent = replacePatternsWithTokens(xmlContent, dictionary);
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to compress XML: %1").arg(e.what()));
        return;
    }

    // Step 4: Display the compressed content in the read-only text field
    ui->textEdit_2->setPlainText(QString::fromStdString(compressedContent));

    // Step 5: Notify the user
    QMessageBox::information(this, "Operation Completed", "Compressed content is displayed in the text field.");
}



void MainWindow::on_pushButton_4_clicked()
{
    QString inputFilePath;

    // Step 1: Check if textEdit has content
    if (!ui->textEdit->toPlainText().isEmpty()) {
        inputFilePath = saveTextToTempFile(ui->textEdit->toPlainText());
        if (inputFilePath.isEmpty()) {
            QMessageBox::critical(this, "Error", "Failed to save the content to a temporary file.");
            return; // Exit if temporary file creation fails
        }
    } else {
        // Step 1.2: Open file dialog to select the input file
        inputFilePath = QFileDialog::getOpenFileName(this, "Select Compressed File", "", "Compressed Files (*.xmlc);;All Files (*.*)");
        if (inputFilePath.isEmpty()) {
            QMessageBox::information(this, "No File Selected", "Please select a compressed file or type content in the text box.");
            return;
        }
    }

    // Step 2: Open the compressed file and parse the dictionary
    std::ifstream input(inputFilePath.toStdString(), std::ios::in);
    if (!input.is_open()) {
        QMessageBox::critical(this, "Error", "Failed to open the selected compressed file.");
        return;
    }

    std::vector<Pair> dictionary;
    try {
        parseDictionary(input, dictionary); // Parse dictionary from the compressed file
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to parse the dictionary: %1").arg(e.what()));
        return;
    }

    // Step 3: Read the compressed content
    std::string compressedContent;
    std::string line;
    while (std::getline(input, line)) {
        compressedContent += line + "\n"; // Read the entire compressed content
    }
    input.close();

    // Step 4: Replace tokens with their original patterns
    std::string decompressedContent;
    try {
        decompressedContent = replaceTokensWithPatterns(compressedContent, dictionary); // Decompress the content
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Failed to decompress content: %1").arg(e.what()));
        return;
    }

    // Step 5: Display the decompressed content in the read-only text field
    ui->textEdit_2->setPlainText(QString::fromStdString(decompressedContent));

    // Step 6: Notify the user
    QMessageBox::information(this, "Operation Completed", "Decompressed content is displayed in the text field.");
}



void MainWindow::on_pushButton_5_clicked()
{
    QString inputFilePath; // Declare inputFilePath at the beginning of the function
    QString outputFilePath; // Declare outputFilePath for saving the minified XML

    // Step 1: Check if QTextEdit_InputXML has content
    if (!ui->textEdit->toPlainText().isEmpty()) {
        // Save the content to a temporary file
        inputFilePath = saveTextToTempFile(ui->textEdit->toPlainText());
        if (inputFilePath.isEmpty()) {
            QMessageBox::critical(this, "Error", "Failed to save the content to a temporary file.");
            return; // Exit if temporary file creation fails
        }
    } else {
        // Step 1.2: Open a file dialog to select the input file
        inputFilePath = QFileDialog::getOpenFileName(this, "Select Input File", "", "XML Files (*.xml);;All Files (*.*)");
        if (inputFilePath.isEmpty()) {
            QMessageBox::information(this, "No File Selected", "Please select a file or type content in the text box.");
            return; // Exit if no file is selected
        }
    }

    // Step 2: Open a file dialog to specify the output file path
    outputFilePath = QFileDialog::getSaveFileName(this, "Save Minified XML File", "C://output_minified.xml", "XML Files (*.xml);;All Files (*.*)");
    if (outputFilePath.isEmpty()) {
        QMessageBox::information(this, "No File Selected", "Please select a location to save the minified file.");
        return; // Exit if no file is selected
    }

    // Step 3: Minify the XML content and save to the output file
    try {
        // Convert file paths from QString to std::string
        std::string inputPath = inputFilePath.toStdString();
        std::string outputPath = outputFilePath.toStdString();

        // Call the minifyXML function
        minifyXML(inputPath, outputPath);

        // Step 4: Read the minified XML content from the output file
        std::ifstream outFile(outputPath);
        if (!outFile.is_open()) {
            QMessageBox::critical(this, "Error", "Failed to read the minified file.");
            return;
        }

        std::stringstream buffer;
        buffer << outFile.rdbuf(); // Read the entire file into buffer
        std::string minifiedXML = buffer.str();

        // Step 5: Display the minified XML content in the QTextEdit
        ui->textEdit_2->setPlainText(QString::fromStdString(minifiedXML));

        // Step 6: Inform the user that the operation is complete
        QMessageBox::information(this, "Operation Completed", "The XML file has been successfully minified and the output is displayed.");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("An error occurred during the minification process: %1").arg(e.what()));
        return;
    }
}



void MainWindow::on_pushButton_6_clicked()
{
    QString inputFilePath;

    // Step 1: Check if textEdit has content
    if (!ui->textEdit->toPlainText().isEmpty()) {
        inputFilePath = saveTextToTempFile(ui->textEdit->toPlainText());
        if (inputFilePath.isEmpty()) {
            QMessageBox::critical(this, "Error", "Failed to save the content to a temporary file.");
            return; // Exit if temporary file creation fails
        }
    } else {
        // Step 1.2: Open a file dialog to select the input file
        inputFilePath = QFileDialog::getOpenFileName(this, "Select Input File", "", "XML Files (*.xml);;All Files (*.*)");
        if (inputFilePath.isEmpty()) {
            QMessageBox::information(this, "No File Selected", "Please select a file or type content in the text box.");
            return; // Exit if no file is selected
        }
    }

    // Step 2: Process the input file or text content
    std::string xmlContent = readFile(inputFilePath.toStdString());
    if (xmlContent.empty()) {
        QMessageBox::critical(this, "Error", "Failed to read the XML file. Please check the file and try again.");
        return;
    }

    // Step 3: Prettify the XML content
    std::string prettifiedXML = prettifyXML(xmlContent);

    // Step 4: Display the prettified XML content in the read-only text field
    ui->textEdit_2->setPlainText(QString::fromStdString(prettifiedXML));

    // Step 5: Inform the user
    QMessageBox::information(this, "Operation Completed", "The XML file has been successfully prettified and the output is displayed in the text field.");
}




void MainWindow::on_pushButton_7_clicked()
{

        QString outputFilePath = QFileDialog::getSaveFileName(this, "Save Output File", "", "XML Files (*.xml);;All Files (*.*)");

        if (outputFilePath.isEmpty()) {
            QMessageBox::information(this, "No File Selected", "Please select a location to save the output.");
            return;
        }

        QFile outputFile(outputFilePath);
        if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&outputFile);
            out << ui->textEdit_2->toPlainText(); // Save the content from the text field
            outputFile.close();

            QMessageBox::information(this, "Success", "Output saved to the selected file.");
        } else {
            QMessageBox::critical(this, "Error", "Unable to save output file!");
        }


}

