#include "dailyspending.h"


void SpendingPerDay::LoadTotalSpendingAsOfYesterday()
{
  auto SpentList = Spent.spent;
  int SumSpent = 0;

  for (auto SpentAmount : SpentList)
  {
    SumSpent += SpentAmount.second;
  }

  SpentPerDay.insert(CurrentDay - 1, SumSpent);
}

void SpendingPerDay::StoreCurrentDay()
{
  CurrentDay = Month.currentDay;
}

void SpendingPerDay::StoreCurrentMonth()
{
  CurrentMonth = Month.GetINTMonth();
}
