#include "application.h"
#include "stream.h"
#include "headers.h"
#include "spent.h"

#define TESTING 0
#define LEFTW 20
#define RIGHTW 43
#define PRECISION 2

/******************************************************************************
 \brief Init application and get ready to run.
 *****************************************************************************/
void application::Init()
{
#if !_DEBUG
  std::string URL = "https://www.everydollar.com/app/budget";
  openWebsite(URL);
  pause();
#endif

#if TESTING
  month.currentDay = 1;
#endif

  //Init Month:
  month.Init();

  if (month.currentDay == 1)
  {
    std::cout << "Update budget\n";
    OpenFile("data/budget.json");
    pause();
  }
  else
    UpdateBudget();

  daysRemaining = month.DaysRemainingInMonth();

  int monthNum = month.GetINTMonth();

  std::string monthNumString = "";
  if (monthNum < 10)
    monthNumString = "0" + std::to_string(monthNum);
  else
    monthNumString = std::to_string(monthNum);
  


#if 1
  str initStream;
  std::string filename = monthNumString + "-2023-EveryDollar-Transactions.csv";
#if _DEBUG
  initStream.copyAndRenameFile(filename, "data", "Transactions.csv");
#else
  initStream.copyAndRenameFile(filename, "x64/Release/data", "Transactions.csv");
#endif

#endif

  this->budget.LoadBudgetAmount();
  this->Spent.LoadAmountSpent();

  for (auto budget : budget.budgetAmount)
  {
    for (auto spent : Spent.spent)
    {
      if (budget.first == spent.first)
      {
        int remaining = 0;

        if (spent.second > 0)
          remaining = budget.second - spent.second;
        else
          remaining = budget.second + spent.second;

        budgetForToday.emplace(budget.first, remaining);
      }
    }
  }
}

/******************************************************************************
 \brief Prompts user to update budget labels.
 *****************************************************************************/
void application::UpdateBudget()
{
  int response;

  std::cout << "Did you update any budget items? ['y','n']:";
  response = std::getchar();

  char res = static_cast<char>(response);

  if (res == 'n' || res == 'N') return;
  else
  {
    std::fstream file("data/budget.json");
    json data;

    if (!file.is_open()) return;

    file >> data;

    bool status = true;

    while (status)
    {
      std::string category;
      float amount = 0.0f;

      std::cin.ignore();

      std::cout << "Enter Label name: ";
      std::getline(std::cin, category);

      std::cout << "Enter amount ($): ";
      std::cin >> amount;

      if (!data.contains(category))
      {
        std::cout << "Label was not found in budget.\n";

        continue;
      }

      data.at(category) = amount;

      std::cout << "Update another label? ['y','n']:";
      std::cin.ignore();

      response = std::getchar();
      res = static_cast<char>(response);

      if (res == 'N' || res == 'n')
        status = false;
    }

    file.close();

    std::ofstream outFile("data/budget.json");

    if (!outFile.is_open()) return;

    outFile << std::setw(2) << data;

    outFile.close();
  }
}

/******************************************************************************
 \brief Runs the application.
 *****************************************************************************/
void application::RUN()
{
  CalculateTotalBudgetAmount();
  CalculateTotalSpentAmount();

  RUNAlexReport();
  RUNAllieReport();
}

void application::CalculateTotalBudgetAmount()
{
  for (auto item: budget.budgetAmount)
  {
    totalBudgetAmount += item.second;
  }

  totalBudgetAmount /= 2;
}

void application::CalculateTotalSpentAmount()
{
  for (auto item : Spent.spent)
  {
    if (item.second < 0)
    {
      totalSpentAmount += item.second;
    }
  }
}

void application::AllieReport(std::ofstream &file)
{
  file << "Date: " << month.GetINTMonth() << "/" << month.currentDay << std::endl;
  file << "USER: Allie                                 Time Period: WEEKLY\n";
  file << "WHAT WE CAN SPEND TODAY:            Days Remaining In Month: " << std::right << std::setw(2) << month.DaysRemainingInMonth() << std::endl;
  file << "---------------------------------------------------------------\n";
  file << std::left << std::setw(LEFTW) << "Label:" << std::right << std::setw(RIGHTW) << "Amount ($):" << std::endl;

  for (auto entry : budgetForToday)
  {
    int num = entry.second;
    std::string name = entry.first;

    float output = AppMath::BudgetOutput(num, 1);

    if (output < 0)
      ++negativeLabels;

    if (output > 1000.0f)
      file << std::left << std::setw(LEFTW) << entry.first << std::right << std::setw(RIGHTW - 7) << std::fixed << std::setprecision(PRECISION) << "$" << output << std::endl;
    else if (output < 10.0f && name != "Entertainment - Books")
    {
      if (output < 0.0f)
        file << std::left << std::setw(LEFTW) << entry.first << std::right << std::setw(RIGHTW - 5) << std::fixed << std::setprecision(PRECISION) << "$" << output << std::endl;
      else if(output < -10.0f)
        file << std::left << std::setw(LEFTW) << entry.first << std::right << std::setw(RIGHTW - 6) << std::fixed << std::setprecision(PRECISION) << "$" << output << std::endl;
      else if(output < - 100.0f)
        file << std::left << std::setw(LEFTW) << entry.first << std::right << std::setw(RIGHTW - 7) << std::fixed << std::setprecision(PRECISION) << "$" << output << std::endl;
      else
        file << std::left << std::setw(LEFTW) << entry.first << std::right << std::setw(RIGHTW - 4) << std::fixed << std::setprecision(PRECISION) << "$" << output << std::endl;
    }
    else if (name == "Entertainment - Books" || output < 100.0f)
      file << std::left << std::setw(LEFTW) << entry.first << std::right << std::setw(RIGHTW - 5) << std::fixed << std::setprecision(PRECISION) << "$" << output << std::endl;
    else
      file << std::left << std::setw(LEFTW) << entry.first << std::right << std::setw(RIGHTW - 6) << std::fixed << std::setprecision(PRECISION) << "$" << output << std::endl;
  }

  file << "---------------------------------------------------------------\n";
  file << "Financial Data as of day: " << month.currentDay << std::endl;
  file << "\n";
  file << "Labels over budget (GOAL: 5):" << std::right << std::setw(RIGHTW - 41) << negativeLabels << std::endl;
  file << "Total Amount Budgeted: $" << std::fixed << std::setprecision(PRECISION) << totalBudgetAmount / 100.0f << std::endl;
  file << "Total Amount Spent---: $" << std::fixed << std::setprecision(PRECISION) << totalSpentAmount * -1 / 100.0f << std::endl;

  int netIncome = totalBudgetAmount + totalSpentAmount;
  file << "Remaining Budget-----: $" << std::fixed << std::setprecision(PRECISION) << netIncome / 100.00f << std::endl;

  file << "---------------------------------------------------------------\n";

  float budgetToday = AppMath::BudgetOutput(totalBudgetAmount, 1);
  float spentToday = totalSpentAmount * -1 / 100.0f;

  file << "Budget Allocation----: $" << std::fixed << std::setprecision(PRECISION) << std::left << std::setw(LEFTW - 11) << budgetToday << std::endl;
  file << "Total Amount Spent---: $" << std::fixed << std::setprecision(PRECISION) << std::left << std::setw(LEFTW - 11) << spentToday << std::endl;

  if (budgetToday < spentToday)
  {
    file << "OVER BUDGET BY-------: $" << std::fixed << std::setprecision(PRECISION) << std::left << std::setw(LEFTW - 36) << budgetToday - spentToday << std::endl;
    file << "Analysis: OVER BUDGET THIS WEEK.\n";
  }
  else
  {
    file << "UNDER BUDGET BY------: $" << std::fixed << std::setprecision(PRECISION) << std::left << std::setw(LEFTW - 36) << budgetToday - spentToday << std::endl;
    file << "Analysis: UNDER BUDGET THIS WEEK! GOOD JOB!\n";
  }

  file.close();

  //Reset for next user.
  negativeLabels = 0;
}

void application::AlexReport(std::ofstream &file)
{
  file << "Date: " << month.GetINTMonth() << "/" << month.currentDay << std::endl;
  file << "USER: Alex                                   Time Period: DAILY\n";
  file << "WHAT WE CAN SPEND TODAY:            Days Remaining In Month: " << std::right << std::setw(2) << month.DaysRemainingInMonth() << std::endl;
  file << "---------------------------------------------------------------\n";
  file << std::left << std::setw(LEFTW) << "Label:" << std::right << std::setw(RIGHTW) << "Amount ($):" << std::endl;

  for (auto entry : budgetForToday)
  {
    
    int num = entry.second;
    std::string name = entry.first;

    auto it = Spent.spent.find(name);
    int spent = it->second;

    float output = AppMath::BudgetOutput(num);
    float labelSpent = AppMath::BudgetOutput(spent);

    if (labelSpent < 0) labelSpent *= -1;

    if (output < 0)
      ++negativeLabels;
   
    if (output < 10.0f && name != "Entertainment - Books")
    {
      if (output < 0.0f)
        file << std::left << std::setw(LEFTW) << entry.first << std::right << std::setw(RIGHTW - 5) << std::fixed << std::setprecision(PRECISION) << "$" << output << std::endl;
      else if (output < -10.0f)
        file << std::left << std::setw(LEFTW) << entry.first << std::right << std::setw(RIGHTW - 6) << std::fixed << std::setprecision(PRECISION) << "$" << output << std::endl;
      else if (output < -100.0f)
        file << std::left << std::setw(LEFTW) << entry.first << std::right << std::setw(RIGHTW - 7) << std::fixed << std::setprecision(PRECISION) << "$" << output << std::endl;
      else
        file << std::left << std::setw(LEFTW) << entry.first << std::right << std::setw(RIGHTW - 4) << std::fixed << std::setprecision(PRECISION) << "$" << output << std::endl;
    }
    else if(name == "Entertainment - Books" || output < 100.0f)
      file << std::left << std::setw(LEFTW) << entry.first << std::right << std::setw(RIGHTW - 5) << std::fixed << std::setprecision(PRECISION) << "$" << output << std::endl;
    else
      file << std::left << std::setw(LEFTW) << entry.first << std::right << std::setw(RIGHTW - 6) << std::fixed << std::setprecision(PRECISION) << "$" << output << std::endl;
  }

  file << "---------------------------------------------------------------\n";
  file << "Financial Data as of day: " << month.currentDay << std::endl;
  file << "\n";
  file << "Labels over budget (GOAL: 5):" << std::right << std::setw(2) << negativeLabels << std::endl;
  file << "Total Amount Budgeted: $" << std::fixed << std::setprecision(PRECISION) << totalBudgetAmount / 100.0f << std::endl;
  file << "Total Amount Spent---: $" << std::fixed << std::setprecision(PRECISION) << totalSpentAmount * -1 / 100.0f << std::endl;

  int netIncome = totalBudgetAmount + totalSpentAmount;
  file << "Remaining Budget-----: $" << std::fixed << std::setprecision(PRECISION) << netIncome / 100.00f << std::endl;

  file << "---------------------------------------------------------------\n";

  float budgetToday = AppMath::BudgetOutput(totalBudgetAmount);

  float spentToday = totalSpentAmount * -1 / 100.0f;

  file << "Budget Allocation----: $" << std::fixed << std::setprecision(PRECISION) << std::left << std::setw(9) << budgetToday << std::endl;
  file << "Total Amount Spent---: $" << std::fixed << std::setprecision(PRECISION) << std::left << std::setw(9) << spentToday << std::endl;

  if (budgetToday < spentToday)
  {
    file << "OVER BUDGET BY-------: $" << std::fixed << std::setprecision(PRECISION) << std::left << std::setw(7) << budgetToday - spentToday << std::endl;
    file << "Analysis: OVER BUDGET TODAY.\n";
  }
  else
  {
    file << "UNDER BUDGET BY------: $" << std::fixed << std::setprecision(PRECISION) << std::left << std::setw(7) << budgetToday - spentToday << std::endl;
    file << "Analysis: UNDER BUDGET TODAY! GOOD JOB!\n";
  }
  file << "\n";

  float ProjectedSpent = (spentToday / (month.currentDay - 1)) * month.totalDays;
  float ProjectedEndOfMonth = (totalBudgetAmount / 100.0f) - ProjectedSpent;

  file << "Estimated Total Spent: $" << std::fixed << std::setprecision(PRECISION) << std::left << std::setw(7) << ProjectedSpent << std::endl;
  
  if (ProjectedEndOfMonth < 0.0f)
    file << "Est. over budget by--: $" << std::fixed << std::setprecision(PRECISION) << std::left << std::setw(7) << ProjectedEndOfMonth << std::endl;
  else
    file << "Est. under budget by: $" << std::fixed << std::setprecision(PRECISION) << std::left << std::setw(7) << ProjectedEndOfMonth << std::endl;

  file.close();

  //Reset for next user.
  negativeLabels = 0;
}

void application::OpenFile(std::string filename)
{
  std::string command = "code " + filename;
  std::system(command.c_str());

  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  //pause();
}

void application::OpenFileExcel(std::string filename)
{
  std::string command = "start excel \"" + filename;
  std::system(command.c_str());

  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  //pause();
}

void application::openFileInNotepad(const char* filename)
{
  // Build the command to open the file in Notepad
  std::string command = "notepad.exe ";
  command += filename;

  // Execute the command
  system(command.c_str());
}

#include <windows.h>
void application::OpenFileWord(const char* filePath)
{
  ShellExecuteA(NULL, "open", filePath, NULL, NULL, SW_SHOWNORMAL);
}

void application::pause()
{
  std::cout << "Enter any key to continue.";
  std::cin.get();
}

void application::openWebsite(const std::string& url)
{
  std::string command;
#ifdef _WIN32 // Windows
  command = "start " + url;
#else // macOS and Linux
  command = "open " + url;
#endif
  system(command.c_str());
}

void application::RUNAlexReport()
{
  std::ofstream outfile;

#if _DEBUG
  outfile.open("data/report.doc");
#else
  outfile.open("x64/Release/data/report.doc");
#endif

  if (!outfile.is_open()) return;

  AlexReport(outfile);


#if _DEBUG
  OpenFileWord("C:/PersonalProjects/dailybudget/DailyBudget/data/report.doc");
#else
  OpenFileWord("C:/PersonalProjects/dailybudget/DailyBudget/x64/Release/data/report.doc");
#endif
}

/******************************************************************************
 * Runs the report for Allie based on a weekly frequency.
 *****************************************************************************/
void application::RUNAllieReport()
{
  if (month.currentDay != 1 && month.currentDay % 7 != 0) return;

  std::ofstream outfile;

#if _DEBUG
  outfile.open("data/AllieReport.doc");
#else
  outfile.open("x64/Release/data/AllieReport.doc");
#endif

  if (!outfile.is_open()) return;

  AllieReport(outfile);


#if _DEBUG
  OpenFileWord("C:/PersonalProjects/dailybudget/DailyBudget/data/AllieReport.doc");
#else
  OpenFileWord("C:/PersonalProjects/dailybudget/DailyBudget/x64/Release/data/AllieReport.doc");
#endif
}

/******************************************************************************
 * Calculates the day multiplier for Allie's report.
 *  0) Daily. (default)
 *  1) weekly.
 *  2) bi-weekly.
 * 
 * 
 * \param DayOfTheMonth
 * \param frequency
 * \return 
 *****************************************************************************/
int AppMath::GetDayMultiplier(const int frequency)
{
  int mul = 0;
  months month;
  month.Init();

  switch (frequency)
  {
    case 1: //Weekly
    {
      mul = 7 * ((month.currentDay / 7) + 1);

      if (mul > month.totalDays)
        mul = month.totalDays;

      return mul;
    }
    case 2: //Bi-Weekly
    {
      mul = 14 * ((month.currentDay / 14) + 1);

      if (mul > month.totalDays)
        mul = month.totalDays;

      return mul;
    }
    default: //Daily
    {
      return month.currentDay;
    }
  }

  return 0;
}

/******************************************************************************
 * Calculates the daily budget.
 *  0) daily - default.
 *  1) weekly.
 *  2) bi-weekly.
 * 
 * \param BudgetToday
 * \param frequency
 * \return 
 *****************************************************************************/
float AppMath::BudgetOutput(const int BudgetToday, const int frequency)
{
  months month;
  month.Init();

  int DayMultiplier = AppMath::GetDayMultiplier(frequency);

  return ((BudgetToday * DayMultiplier) / month.totalDays) / 100.0f;
}
