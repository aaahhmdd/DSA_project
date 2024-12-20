#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include "graph_parser.h"
#include "xmltojson.h" // Include your XML-related functions
#include "xmlfunction.h" // Include the header for isXMLBalanced
#include <fstream>
#include <sstream>
//#include "compression.h"  // Include the compression functions
#include "decompression.h" // Include this header to access the decompression functions
#include "minifyxml.h"
#include "xml_editor.h"
#include <map>
#include "graph.h"
#include "NetworkAnalysis.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->imageLabel->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}



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
        inputFilePath = QFileDialog::getOpenFileName(this, "Select Input File", "", "XML Files (*.xml);; TXT Files (*.txt) ;; All Files (*.*)");
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
        inputFilePath = QFileDialog::getOpenFileName(this, "Select Input File", "", "XML Files (*.xml);; TXT Files (*.txt) ;; All Files (*.*)");
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
        inputFilePath = QFileDialog::getOpenFileName(this, "Select Input File", "", "XML Files (*.xml);; TXT Files (*.txt) ;; All Files (*.*)");
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


void MainWindow::on_pushButton_4_clicked() {
    QString inputFilePath;

    // Step 1: Save content to temporary file or select file
    if (!ui->textEdit->toPlainText().isEmpty()) {
        inputFilePath = saveTextToTempFile(ui->textEdit->toPlainText());
        if (inputFilePath.isEmpty()) {
            QMessageBox::critical(this, "Error", "Failed to save content to a temporary file.");
            return;
        }
    } else {
        inputFilePath = QFileDialog::getOpenFileName(this, "Select Compressed File", "", "Compressed Files (*.xmlc);;All Files (*.*)");
        if (inputFilePath.isEmpty()) {
            QMessageBox::information(this, "No File Selected", "Please select a compressed file or type content in the text box.");
            return;
        }
    }

    // Step 2: Parse compressed file
    std::ifstream input(inputFilePath.toStdString(), std::ios::in);
    if (!input.is_open()) {
        QMessageBox::critical(this, "Error", "Failed to open the selected compressed file.");
        return;
    }

    std::vector<Pair> dictionary;
    parseDictionary(input, dictionary);

    std::string compressedContent;
    std::string line;
    while (std::getline(input, line)) {
        compressedContent += line + "\n";
    }
    input.close();

    // Step 3: Decompress content
    std::string decompressedContent = replaceTokensWithPatterns(compressedContent, dictionary);

    // Step 4: Display decompressed content
    ui->textEdit_2->setPlainText(QString::fromStdString(decompressedContent));

    // Step 5: Notify user
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
        inputFilePath = QFileDialog::getOpenFileName(this, "Select Input File", "", "XML Files (*.xml);; TXT Files (*.txt) ;; All Files (*.*)");
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
        inputFilePath = QFileDialog::getOpenFileName(this, "Select Input File", "", "XML Files (*.xml);; TXT Files (*.txt) ;; All Files (*.*)");
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

        QString outputFilePath = QFileDialog::getSaveFileName(this, "Save Output File", "", "XML Files (*.xml);;TXT Files (*.txt);;All Files (*.*)");

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

void MainWindow::processXML(const QString& filePath)
{
    // Step 1: Read the contents of the XML file
    std::string xmlContent = readFile(filePath.toStdString());
    if (xmlContent.empty()) {
        QMessageBox::critical(this, "Error", "Failed to read the XML file. Please check the file and try again.");
        return;
    }

    // Step 2: Parse the XML and generate the graph
    Element root = parseXML(xmlContent);
    std::map<std::string, std::vector<std::string>> userFollowersMap;
    toMap(root, userFollowersMap);

    // Step 3: Create the graph
    Graph<std::string> graph(10);
    generateGraphFromMap(userFollowersMap, graph);

    // Step 4: Visualize the graph and save the result
    graph.VisualizeGraph("graph.dot");

    // Step 5: Display the DOT content in the output field
    displayDotFile("graph.dot");
}

std::string MainWindow::readFile(const std::string& filePath)
{
    QFile file(QString::fromStdString(filePath));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Error opening file!" << std::endl;
        return "";
    }

    QString content = file.readAll();
    file.close();
    return content.toStdString();
}

void MainWindow::displayDotFile(const std::string& fileName)
{
    QFile dotFile(QString::fromStdString(fileName));
    if (dotFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString dotContent = dotFile.readAll();
        ui->textEdit_2->setPlainText(dotContent);  // Display DOT content in the output field
        dotFile.close();
    } else {
        std::cerr << "Error opening DOT file!" << std::endl;
    }
}

void MainWindow::on_pushButton_8_clicked()
{
    QString inputFilePath;

    // Step 1: Check if the input field has content
    if (!ui->textEdit->toPlainText().isEmpty()) {
        inputFilePath = saveTextToTempFile(ui->textEdit->toPlainText());
        if (inputFilePath.isEmpty()) {
            QMessageBox::critical(this, "Error", "Failed to save the content to a temporary file.");
            return; // Exit if temporary file creation fails
        }
    } else {
        // Step 1.2: Open a file dialog to select the XML file
        inputFilePath = QFileDialog::getOpenFileName(this, "Open XML File", "", "XML Files (*.xml)");
        if (inputFilePath.isEmpty()) {
            QMessageBox::information(this, "No File Selected", "Please select a file or input content.");
            return; // Exit if no file is selected
        }
    }

    // Step 2: Process the XML file and generate the graph
    processXML(inputFilePath);
}









void MainWindow::on_pushButton_11_clicked()
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
        inputFilePath = QFileDialog::getOpenFileName(this, "Select Input File", "", "XML Files (*.xml);; TXT Files (*.txt) ;; All Files (*.*)");
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

    // Step 3: Initialize NetworkAnalysis object and parse the XML
    NetworkAnalysis network;
    network.parse_xml(inputFilePath.toStdString());

    // Step 4: Get the most active user result
    std::string activeUserResult = network.most_active_user();

    // Step 5: Set the result in textEdit_2
    ui->textEdit_2->setPlainText(QString::fromStdString(activeUserResult));

    // Step 6: Inform the user
    QMessageBox::information(this, "Operation Completed", "The most active user has been identified and the results are displayed in the text field.");
}




void MainWindow::on_pushButton_9_clicked()
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
        inputFilePath = QFileDialog::getOpenFileName(this, "Select Input File", "", "XML Files (*.xml);; TXT Files (*.txt) ;; All Files (*.*)");
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

    // Step 3: Initialize NetworkAnalysis object and parse the XML
    NetworkAnalysis network;
    network.parse_xml(inputFilePath.toStdString());

    // Step 4: Get the analysis result
    std::string analysisResult = network.display_analysis();

    // Step 5: Set the result in textEdit_2
    ui->textEdit_2->setPlainText(QString::fromStdString(analysisResult));

    // Step 6: Inform the user
    QMessageBox::information(this, "Operation Completed", "The analysis has been completed and the results are displayed in the text field.");
}


void MainWindow::on_pushButton_10_clicked()
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
        inputFilePath = QFileDialog::getOpenFileName(this, "Select Input File", "", "XML Files (*.xml);; TXT Files (*.txt) ;; All Files (*.*)");
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

    // Step 3: Initialize NetworkAnalysis object and parse the XML
    NetworkAnalysis network;
    network.parse_xml(inputFilePath.toStdString());

    // Step 4: Call mutual_followers with IDs 1, 2, and 3 and get the result
    std::vector<int> user_ids = {1, 2, 3};
    std::string mutualFollowersResult = network.mutual_followers(user_ids);

    // Step 5: Set the result in textEdit_2
    ui->textEdit_2->setPlainText(QString::fromStdString(mutualFollowersResult));

    // Step 6: Inform the user
    QMessageBox::information(this, "Operation Completed", "The mutual followers have been displayed in the text field.");
}



void MainWindow::on_pushButton_12_clicked()
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
        inputFilePath = QFileDialog::getOpenFileName(this, "Select Input File", "", "XML Files (*.xml);; TXT Files (*.txt) ;; All Files (*.*)");
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

    // Step 3: Initialize NetworkAnalysis object and parse the XML
    NetworkAnalysis network;
    network.parse_xml(inputFilePath.toStdString());

    // Step 4: Call suggest_users_to_follow with ID 1 and get the result
    int user_id = 1;
    std::string suggestedUsers = network.suggest_users_to_follow(user_id);

    // Step 5: Set the result in textEdit_2
    ui->textEdit_2->setPlainText(QString::fromStdString(suggestedUsers));

    // Step 6: Inform the user
    QMessageBox::information(this, "Operation Completed", "Suggested users to follow have been displayed in the text field.");
}

void MainWindow::on_pushButton_13_clicked()
{
    QString inputFilePath;

    // Step 0: Check if textEdit has content
    if (!ui->textEdit->toPlainText().isEmpty()) {
        inputFilePath = saveTextToTempFile(ui->textEdit->toPlainText());
        if (inputFilePath.isEmpty()) {
            QMessageBox::critical(this, "Error", "Failed to save the content to a temporary file.");
            return; // Exit if temporary file creation fails
        }
    } else {
        // Step 0.2: Open a file dialog to select the input file
        inputFilePath = QFileDialog::getOpenFileName(this, "Select Input File", "", "XML Files (*.xml);; TXT Files (*.txt) ;; All Files (*.*)");
        if (inputFilePath.isEmpty()) {
            QMessageBox::information(this, "No File Selected", "Please select a file or type content in the text box.");
            return; // Exit if no file is selected
        }
    }

    // Step 1: Process the input file or text content
    std::string xmlContent = readFile(inputFilePath.toStdString());
    if (xmlContent.empty()) {
        QMessageBox::critical(this, "Error", "Failed to read the XML file. Please check the file and try again.");
        return;
    }

    // Step 2: Get the word entered by the user
    QString word = ui->lineEdit->text(); // Assuming you have a QLineEdit widget for input
    if (word.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a word to search.");
        return; // Exit if the input field is empty
    }

    // Step 3: Initialize NetworkAnalysis object and parse the XML (if needed)
    NetworkAnalysis network;

    network.parse_xml(inputFilePath.toStdString());

    // Step 4: Call search_posts_by_word with the input word
    std::string searchWord = word.toStdString();
    std::string searchResult = network.search_posts_by_word(searchWord);

    // Step 5: Display the result in textEdit_2
    ui->textEdit_2->setPlainText(QString::fromStdString(searchResult));

    // Step 6: Inform the user
    QMessageBox::information(this, "Search Completed", "The search results have been displayed in the text field.");
}



void MainWindow::on_pushButton_15_clicked()
{


        // Step 1: Load the selected image
        QPixmap pixmap(":/src/E:/Senior-1/Fall 24/Data Structure/project/v0.2/resources/Graph.jpeg");
        if (pixmap.isNull()) {
            QMessageBox::critical(this, "Error", "Failed to load the image.");
            return;
        }

        // Step 2: Display the image in the QLabel
        ui->imageLabel->setPixmap(pixmap);
        ui->imageLabel->setScaledContents(true);
        ui->imageLabel->setVisible(true);
        QMessageBox::information(this, "Image Displayed", "The selected image has been displayed successfully.");


}


void MainWindow::on_pushButton_14_clicked()
{
    ui->imageLabel->setVisible(false);
}

