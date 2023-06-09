#include "spent.h"
#include "application.h"

void spent::LoadAmountSpent()
{
  str spentstream;
  spentstream.LoadJSONData("data/budget.json");
  auto begin = spentstream.begin();
  auto end = spentstream.end();

  tran spending;

  while (begin != end)
  {
    std::string name = begin.key();
    int value = spending.GetCatagoryTotal("data/Transactions.csv", name) * 100;

    spent.emplace(name, value);

    ++begin;
  }
}
