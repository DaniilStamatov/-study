#include "parser.h"
#include <string_view>

void Request::SetDateFrom(const Date from)
{
	from_ = from;
}

void Request::SetDateTo(const Date to)
{
	to_ = to;
}

void Request::SetEarnings(const double earnings)
{
	earnings_ = earnings;
}

void Request::SetSpendings(const double spendings) {
	spendings_ = spendings;
}

void Request::SetTax(const double tax)
{
	tax_ = tax;
}

const Date Request::GetDateFrom() const
{
	return from_;
}

const Date Request::GetDateTo() const
{
	return to_;
}

const double Request::GetEarnings() const
{
	return earnings_;
}

const double Request::GetSpendings() const{
	return spendings_;
}

const double Request::GetTax() const
{
	return tax_;
}

void Request_Read::ParseQuery(std::string_view query)
{
	SetDateFrom(Date::FromString(query.substr(0, query.find_first_of(' '))));
	SetDateTo(Date::FromString(query.substr(query.find_first_of(' ') + 1, query.npos - query.find_first_of(' ') - 1)));

}

void Request_Modify::ParseQuery(std::string_view query)
{
	std::string_view query_type = query.substr(0, query.find_first_of(' '));
	if (query_type == "Earn") {
		SetEarnings(std::stod(std::string(query.substr(query.find_last_of(' ') + 1, query.npos))));
	}
	else if (query_type == "Spend") {
		SetSpendings(std::stod(std::string(query.substr(query.find_last_of(' ') + 1, query.npos))));
	}
	else if (query_type == "PayTax") {
		SetTax(std::stod(std::string(query.substr(query.find_last_of(' ') + 1, query.npos))));
	}

	query.remove_prefix(query_type.size() + 1);
	SetDateFrom(Date::FromString(query.substr(0, query.find_first_of(' '))));
	SetDateTo(Date::FromString(query.substr(query.find_first_of(' ') + 1, query.npos - query.find_first_of(' ') - 1)));
}


