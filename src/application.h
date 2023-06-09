#pragma once

#include "headers.h"
#include "budgetamount.h"
#include "months.h"
#include "spent.h"

class application
{
private:
  std::map<std::string, int> budgetForToday;
  int daysRemaining = 0;
  BudgetAmount budget;
  spent Spent;
  months month;
  int negativeLabels = 0;
  int totalBudgetAmount = 0;
  int totalSpentAmount = 0;

  void OpenFile(std::string filename);
  void pause();
  void OpenFileExcel(std::string filename);
  void OpenFileWord(const char* filePath);
  void openFileInNotepad(const char* filename);
  void openWebsite(const std::string& url);
  void UpdateBudget();
  void CalculateTotalBudgetAmount();
  void CalculateTotalSpentAmount();
  void AlexReport(std::ofstream &file);
  void AllieReport(std::ofstream &file);
  void RUNAlexReport();
  //Generates Allie's report based on the day of the month.
  void RUNAllieReport();

public:
  
  //Init Application.
  void Init();
  //Run Application. Init must be ran first.
  void RUN();
};

struct AppMath
{
  static int GetDayMultiplier(int frequency = 0);
  static float BudgetOutput(int BudgetToday, int frequency = 0);

};
