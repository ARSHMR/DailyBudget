#pragma once

#include "headers.h"
#include "json-develop/single_include/nlohmann/json.hpp"


using json = nlohmann::json;

class str
{
private:
  json data;

public:
  void LoadJSONData(std::string filename);
  bool copyAndRenameFile(const std::string& sourceFile, const std::string& destinationDirectory, const std::string& newName);
  void deleteColumnFromCSV(const std::string& filename, int columnToDelete);
  void deleteFirstColumnInCSV(const std::string& inputFileName);
  void insertEmptyColumn(const std::string& filename);
  void convertColumns(const std::string& filename);
  void convertToText(const std::string& filename);


  json::iterator begin();
  json::iterator end();
  json::iterator get(std::string key);
};
