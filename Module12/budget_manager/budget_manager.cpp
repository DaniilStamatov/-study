#include "budget_manager.h"
void BudgetManager::ComputeIncome(const Date start, const Date end)
{
	double income = 0.0;
	int from = start.ComputeDistance(START_DATE, start);
	int to = end.ComputeDistance(START_DATE, end);

	for (int i = from; i <= to; i++) {
		income += all_days_[i].earnings;
	}

	std::cout << income << std::endl;
}

void BudgetManager::Earn(const Date start, const Date end, double earning)
{
	int from = start.ComputeDistance(START_DATE, start);
	int to = end.ComputeDistance(START_DATE, end);
	double avg = earning / static_cast<double>(to - from + 1);

	for (int i = from; i <= to; i++) {
		all_days_[i].earnings += avg;
	}
}

void BudgetManager::PayTax(const Date start, const Date end, int tax)
{
	int from = start.ComputeDistance(START_DATE, start);
	int to = end.ComputeDistance(START_DATE, end);

	for (int i = from; i <= to; i++) {
		all_days_[i].earnings -= all_days_[i].earnings * tax;
	}
}

void BudgetManager::Spend(const Date start, const Date end, double spendings)
{
	int from = start.ComputeDistance(START_DATE, start);
	int to = end.ComputeDistance(START_DATE, end);
	double avg = spendings / static_cast<double>(to - from + 1);

	for (int i = from; i <= to; i++) {
		all_days_[i].earnings -= avg;
	}
}
