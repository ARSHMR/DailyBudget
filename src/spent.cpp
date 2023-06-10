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
    auto BeginL2 = begin.value().begin();
    auto EndL2 = begin.value().end();

    while (BeginL2 != EndL2)
    {
      std::string name = BeginL2.key();
      int value = spending.GetCatagoryTotal("data/Transactions.csv", name) * 100;

      spent.emplace(name, value);

      ++BeginL2;
    }

    ++begin;
  }
}
