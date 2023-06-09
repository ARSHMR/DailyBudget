#include "budgetamount.h"

void BudgetAmount::LoadBudgetAmount()
{
  str spentstream;
  spentstream.LoadJSONData("data/budget.json");
  auto begin = spentstream.begin();
  auto end = spentstream.end();

  while (begin != end)
  {
    int value = (float)begin.value() * 100;

    std::string name = begin.key();

    budgetAmount.emplace(name, value);

    ++begin;
  }
}

int BudgetAmount::CalculateRemaingBudget()
{
  return this->monthlyBudgetForMonth - this->amountSpentInMonth;
}
