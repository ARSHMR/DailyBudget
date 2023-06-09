#pragma once

#include "headers.h"

class months
{
public:
  std::map<int, int> months;
  int currentDay = 0;
  int totalDays = 0;

  void Init();
  int DaysRemainingInMonth();
  int GetINTMonth();

private:
  void AssignCurrentDay();
  void AssignTotalDays();
  void LoadMonths();
};
