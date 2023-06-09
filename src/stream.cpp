#include "stream.h"
#include <windows.h>

void str::LoadJSONData(std::string filename)
{
  std::fstream file(filename);

  if (file.is_open())
    file >> data;

  file.close();  
}

json::iterator str::begin()
{
    return data.begin();
}

json::iterator str::end()
{
  return data.end();
}

json::iterator str::get(std::string key)
{
  if (data.contains(key))
    return data.at(key).begin();

  return json::iterator();
}

bool str::copyAndRenameFile(const std::string& sourceFile, const std::string& destinationDirectory, const std::string& newName)
{
  std::string source = "C:/Users/alexs/Downloads/" + sourceFile;

  // Check if the source file exists
  if (GetFileAttributesA(source.c_str()) == INVALID_FILE_ATTRIBUTES) {
    std::cout << "Source file does not exist." << std::endl;
    return false;
  }

  // Construct the full path for the destination file
  std::string destinationFile = destinationDirectory + "\\" + newName;

  // Copy the file
  if (!CopyFileA(source.c_str(), destinationFile.c_str(), FALSE)) {
    std::cout << "Error occurred while copying the file: " << GetLastError() << std::endl;
    return false;
  }
#if !_DEBUG
  // Delete the original file
  if (!DeleteFileA(source.c_str())) {
    std::cout << "Error occurred while deleting the original file: " << GetLastError() << std::endl;
    return false;
  }
#endif

  std::cout << "File copied and original file deleted successfully." << std::endl;
  return true;
}

void str::deleteColumnFromCSV(const std::string& filename, int columnToDelete)
{
#if _DEBUG
  std::string filepath = "C:/PersonalProjects/dailybudget/data/" + filename;
#else
  std::string filepath = "C:/PersonalProjects/dailybudget/x64/Release/data/" + filename;
#endif
  std::ifstream inputFile(filepath);
  if (!inputFile)
  {
    std::cerr << "Error opening file: " << filename << std::endl;
    return;
  }

  std::vector<std::vector<std::string>> data;
  std::string line;

  // Read and parse CSV data
  while (std::getline(inputFile, line))
  {
    std::vector<std::string> row;
    std::stringstream ss(line);
    std::string cell;

    while (std::getline(ss, cell, ','))
      row.push_back(cell);

    data.push_back(row);
  }

  inputFile.close();

  // Delete the specified column
  for (auto& row : data)
  {
    if (columnToDelete >= 0 && columnToDelete < row.size())
      row.erase(row.begin() + columnToDelete);
  }

  // Write the modified data back to the file
  std::ofstream outputFile(filepath);
  if (!outputFile)
  {
    std::cerr << "Error opening file for writing: " << filename << std::endl;
    return;
  }

  for (const auto& row : data)
  {
    for (size_t i = 0; i < row.size(); ++i)
    {
      outputFile << row[i];
      if (i != row.size() - 1)
        outputFile << ",";
    }
    outputFile << std::endl;
  }

  outputFile.close();

  std::cout << "Column " << columnToDelete << " deleted from " << filename << std::endl;
}

void str::deleteFirstColumnInCSV(const std::string& filename) {

  std::ifstream inputFile(filename);
  if (!inputFile) {
    std::cerr << "Failed to open the file: " << filename << std::endl;
    return;
  }

  std::vector<std::vector<std::string>> data;
  std::string line;

  while (std::getline(inputFile, line)) {
    std::vector<std::string> row;
    std::istringstream iss(line);
    std::string token;
    int col = 0;

    while (std::getline(iss, token, ',')) {
      if (col != 0) {  // Skip column 0
        row.push_back(token);
      }
      col++;
    }

    data.push_back(row);
  }

  inputFile.close();

  std::ofstream outputFile(filename);
  if (!outputFile) {
    std::cerr << "Failed to open the file for writing: " << filename << std::endl;
    return;
  }

  for (const auto& row : data) {
    for (size_t i = 0; i < row.size(); i++) {
      outputFile << row[i];
      if (i != row.size() - 1) {
        outputFile << ",";
      }
    }
    outputFile << std::endl;
  }

  outputFile.close();
}

void str::insertEmptyColumn(const std::string& filename) {
  std::ifstream inputFile(filename);
  if (!inputFile) {
    std::cerr << "Failed to open the file: " << filename << std::endl;
    return;
  }

  std::vector<std::vector<std::string>> data;
  std::string line;

  while (std::getline(inputFile, line)) {
    std::vector<std::string> row;
    std::istringstream iss(line);
    std::string token;

    // Insert an empty value at column 0
    row.push_back("");

    while (std::getline(iss, token, ',')) {
      row.push_back(token);
    }

    data.push_back(row);
  }

  inputFile.close();

  std::ofstream outputFile(filename);
  if (!outputFile) {
    std::cerr << "Failed to open the file for writing: " << filename << std::endl;
    return;
  }

  for (const auto& row : data) {
    for (size_t i = 0; i < row.size(); i++) {
      outputFile << row[i];
      if (i != row.size() - 1) {
        outputFile << ",";
      }
    }
    outputFile << std::endl;
  }

  outputFile.close();
}

void str::convertColumns(const std::string& filename) {
  std::ifstream inputFile(filename);
  if (!inputFile) {
    std::cerr << "Failed to open the file: " << filename << std::endl;
    return;
  }

  std::vector<std::vector<std::string>> data;
  std::string line;

  while (std::getline(inputFile, line)) {
    std::vector<std::string> row;
    std::istringstream iss(line);
    std::string token;
    int col = 0;

    while (std::getline(iss, token, ',')) {
      if (col == 0) {  // Convert column 0 to text
        row.push_back(token);
      }
      else if (col == 1) {  // Convert column 1 to number while preserving original value
        double number;
        std::istringstream(token) >> number;
        row.push_back(token);
        row.push_back(std::to_string(number));
      }
      else {
        row.push_back(token);
      }
      col++;
    }

    data.push_back(row);
  }

  inputFile.close();

  std::ofstream outputFile(filename);
  if (!outputFile) {
    std::cerr << "Failed to open the file for writing: " << filename << std::endl;
    return;
  }

  for (const auto& row : data) {
    for (size_t i = 0; i < row.size(); i++) {
      outputFile << row[i];
      if (i != row.size() - 1) {
        outputFile << ",";
      }
    }
    outputFile << std::endl;
  }

  outputFile.close();
}

void str::convertToText(const std::string& filename) {
  std::ifstream inputFile(filename);
  if (!inputFile) {
    std::cerr << "Failed to open the file: " << filename << std::endl;
    return;
  }

  std::vector<std::vector<std::string>> data;
  std::string line;

  while (std::getline(inputFile, line)) {
    std::vector<std::string> row;
    std::istringstream iss(line);
    std::string token;

    while (std::getline(iss, token, ',')) {
      if (token.empty() || token.front() == '"' || token.back() == '"') {
        row.push_back(token);
      }
      else {
        row.push_back("\"" + token + "\"");
      }
    }

    data.push_back(row);
  }

  inputFile.close();

  std::ofstream outputFile(filename);
  if (!outputFile) {
    std::cerr << "Failed to open the file for writing: " << filename << std::endl;
    return;
  }

  for (const auto& row : data) {
    for (size_t i = 0; i < row.size(); i++) {
      outputFile << row[i];
      if (i != row.size() - 1) {
        outputFile << ",";
      }
    }
    outputFile << std::endl;
  }

  outputFile.close();;
}

