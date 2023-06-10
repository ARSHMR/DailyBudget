#include "budgetamount.h"

void BudgetAmount::LoadBudgetAmount()
{
  str spentstream;
  spentstream.LoadJSONData("data/budget.json");
  auto begin = spentstream.begin();
  auto end = spentstream.end();

  while (begin != end)
  {
    std::string bname = begin.key();

    auto begin2 = begin.value().begin();
    auto end2 = begin.value().end();

    while (begin2 != end2)
    {
      std::string name = begin2.key();
      int value = (float)begin2.value() * 100;

      budgetAmount.emplace(name, value);

      ++begin2;
    }


    ++begin;
  }
}

int BudgetAmount::CalculateRemaingBudget()
{
  return this->monthlyBudgetForMonth - this->amountSpentInMonth;
}
