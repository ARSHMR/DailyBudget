#include "transactions.h"
#include <string_view>

double tran::GetCatagoryTotal(const std::string& filename, const std::string& category)
{
  std::ifstream file(filename);
  if (!file) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return 0.0;
  }

  std::string line;
  double totalAmount = 0.0;

  while (getline(file, line)) {
    std::istringstream iss(line);
    std::string holder = iss.str();
    std::string transaction, transCategory, amountStr, group, type, date, merchant;
    double amount = 0;

    if (getline(iss, group,',') && getline(iss, transCategory, ',') && getline(iss, type, ',') &&getline(iss, date, ',') && getline(iss, merchant,',') && getline(iss, amountStr)) {

      size_t size = transCategory.size();
      std::string categoryName = transCategory.substr(1, size - 2);

      size_t amountSize = amountStr.size();
      std::string numAmount = amountStr.substr(1, amountSize - 2);

      if (category == "Home Maintenance")
      {
        int x = 10;
      }


      if (categoryName == category) {
        try
        {
          amount = std::stod(numAmount);

          totalAmount += amount;
          
        }
        catch (const std::exception& e)
        {
          std::cerr << "Error:" << e.what() << std::endl;
          std::cerr << "Error parsing amount: " << amountStr << std::endl;
        }
      }
    }
  }

  return totalAmount;
}
