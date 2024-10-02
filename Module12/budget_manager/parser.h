#pragma once
#include "date.h"

// напишите в этом классе код, ответственный за чтение запросов

class Request {
private:
	Date from_;
	Date to_;
	double earnings_;
	double spendings_;
	double tax_;
public:
	virtual void ParseQuery(std::string_view query) = 0;
	void SetDateFrom(const Date from);
	void SetDateTo(const Date to);
	void SetEarnings(const double earnings);
	void SetSpendings(const double spendings);
	void SetTax(const double tax);
	const Date GetDateFrom() const;
	const Date GetDateTo() const;
	const double GetEarnings() const;
	const double GetSpendings() const;
	const double GetTax() const;

};

class Request_Read : public Request {
public:
	Request_Read(std::string_view query)  {
		ParseQuery(query);
	}

	void ParseQuery(std::string_view query) override;
};


class Request_Modify : public Request {
public:
	Request_Modify(std::string_view query) {
		ParseQuery(query);
	}

	void ParseQuery(std::string_view query) override;
};

