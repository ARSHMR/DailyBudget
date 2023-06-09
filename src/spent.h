#pragma once

#include "headers.h"
#include "stream.h"
#include "transactions.h"

struct spent
{
  std::map<std::string, int> spent;

  void LoadAmountSpent();
};
