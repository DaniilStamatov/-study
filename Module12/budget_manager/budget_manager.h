#pragma once
#include "date.h"
#include <iostream>

struct DayInfo {
    double earnings;
};

class BudgetManager {
private:
    std::vector<DayInfo> all_days_;
public:
    inline static const Date START_DATE{ 2000, 1, 1 };
    inline static const Date END_DATE{ 2100, 1, 1 };
    BudgetManager() : all_days_(START_DATE.ComputeDistance(START_DATE, END_DATE), DayInfo()){}
    void ComputeIncome(const Date start, const Date end);
    void Earn(const Date start, const Date end, double earning);
    void PayTax(const Date start, const Date end, int tax);
    void Spend(const Date start, const Date end, double spendings);
    // разработайте класс BudgetManager
};
