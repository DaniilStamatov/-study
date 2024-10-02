#include "budget_manager.h"

#include <iostream>
#include <string_view>
#include "parser.h"

using namespace std;

void ParseAndProcessQuery(BudgetManager& manager, string_view line) {
    std::string_view query_type = line.substr(0, line.find_first_of(' '));
    if (query_type == "Earn"sv) {
        Request_Modify query(line);

        manager.Earn(query.GetDateFrom(), query.GetDateTo(), query.GetEarnings());
    }
    else if (query_type == "ComputeIncome"sv) {
        Request_Read query(line);
        manager.ComputeIncome(query.GetDateFrom(), query.GetDateTo());
    }
    else if (query_type == "PayTax"sv){
        Request_Modify query(line);

        manager.PayTax(query.GetDateFrom(), query.GetDateTo(), query.GetTax());
    }
    else if (query_type == "Spend") {
        Request_Modify query(line);

        manager.PayTax(query.GetDateFrom(), query.GetDateTo(), query.GetSpendings());
    }
    else{
        invalid_argument("Wrong query");
    }
}

int ReadNumberOnLine(istream& input) {
    std::string line;
    std::getline(input, line);
    return std::stoi(line);
}
//
//int main() {
//    BudgetManager manager;
//
//    const int query_count = ReadNumberOnLine(cin);
//
//    for (int i = 0; i < query_count; ++i) {
//        std::string line;
//        std::getline(cin, line);
//        ParseAndProcessQuery(manager, line);
//    }
//}
