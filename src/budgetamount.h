#pragma once

#include "headers.h"
#include "stream.h"

class BudgetAmount
{
private:
  int monthlyBudgetForMonth = 0;
  int amountSpentInMonth = 0;

  
public:
  void LoadBudgetAmount();
  int CalculateRemaingBudget();

  std::map<std::string, int> budgetAmount;

};
