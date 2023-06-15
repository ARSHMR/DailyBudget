#include "months.h"
#include "stream.h"

void months::Init()
{
  LoadMonths();
  AssignCurrentDay();
  AssignTotalDays();
}

int months::DaysRemainingInMonth()
{
  return totalDays - currentDay;
}

#pragma warning(push)
#pragma warning(disable: 4996)
void months::AssignCurrentDay()
{
  std::time_t currentTime = std::time(nullptr);
  std::tm* localTime = std::localtime(&currentTime);

  currentDay = localTime->tm_mday;
}

void months::AssignTotalDays()
{
  std::time_t currentTime = std::time(nullptr);
  std::tm* localTime = std::localtime(&currentTime);

  int monthNum = localTime->tm_mon + 1;

  this->totalDays = months.find(monthNum)->second;
}

int months::GetINTMonth()
{
  std::time_t currentTime = std::time(nullptr);
  std::tm* localTime = std::localtime(&currentTime);

  return localTime->tm_mon + 1;
}
#pragma warning(pop)

void months::LoadMonths()
{
  str monthstream;

  monthstream.LoadJSONData("data/months.json");

  auto begin = monthstream.begin();
  auto end = monthstream.end();

  while (begin != end)
  {
    std::string num = begin.key();
    int NUM = std::stoi(num);

    months.emplace(NUM, begin.value());

    begin++;
  }


}
