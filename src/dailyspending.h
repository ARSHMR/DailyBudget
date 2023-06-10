/*****************************************************************//**
 * \file   dailyspending.h
 * \brief  Contains the method declerations that keeps track of daily
 *         spending and exports information related to daily spending.
 * 
 * \brief  Copyright ©Alex Schumer 2023
 * 
 * \author Alex Schumer
 * \date   June 2023
 *********************************************************************/
#pragma once

#include "headers.h" /* external dependencies */
#include "stream.h"
#include "spent.h"
#include "months.h"

class SpendingPerDay
{
private:
  // variable to hold current day.
  int CurrentDay = 0;
  int CurrentMonth = 0;
  // variable to hold how many days are in the month.
  int TotalDaysInMonth = 0;
  // data structure to hold how much we spend per day.
  std::map<int, int> SpentPerDay;

  spent Spent;
  months Month;

private:
  //Helper functions:
  void LoadTotalSpendingAsOfYesterday();
  void StoreCurrentDay();
  void StoreCurrentMonth();
  //Create report.
  //Open report in word.

public:
  //Init function to load in varaible data.
  void INIT();
  //Update function to bring total spending current.
};