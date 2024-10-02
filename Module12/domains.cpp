#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

using namespace std;

class Domain {
private:
    std::string domain_;
public:
    Domain(const std::string& domain) : domain_(domain) {

    }

    bool IsSubdomain(const Domain& other) const {
        if (domain_.size() > other.domain_.size()) return false;
        auto res = std::mismatch(other.domain_.begin(), other.domain_.end(), domain_.begin());
        return res.first == other.domain_.end();
    }

    bool operator==(const Domain& other) const {
        return domain_ == other.domain_;
    }

    std::string GetDomain() const {
        return domain_;
    }
};


class DomainChecker {
private:
    std::vector<Domain> forbidden_domains_;

public:
    template <typename Iterator>
    DomainChecker(Iterator begin, Iterator end) : forbidden_domains_(begin, end) {
        std::sort(forbidden_domains_.begin(), forbidden_domains_.end(), [](const Domain& lhs, const Domain& rhs) {
            return lhs.GetDomain() < rhs.GetDomain();
            });
        end = std::unique(forbidden_domains_.begin(), forbidden_domains_.end(), [](const Domain& lhs, const Domain& rhs) {
            return rhs.IsSubdomain(lhs);
            });

        forbidden_domains_.erase(end, forbidden_domains_.end());

    }

    // конструктор должен принимать список запрещённых доменов через пару итераторов

    bool IsForbidden(const Domain& domain) {
        auto it = upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(), domain, [](const Domain& lhs, const Domain& rhs) {
            return lhs.GetDomain() < rhs.GetDomain();
            });

        if (it == forbidden_domains_.begin()) return false;
        else return domain.IsSubdomain(*prev(it));
    }
};



template <typename Number>
vector<Domain> ReadDomains(istream& in, Number num) {
    vector<Domain> domains;
    domains.reserve(num);
    string domain;
    for (size_t i = 0; i < num; ++i) {
        getline(in, domain);
        domains.push_back(domain);
    }
    return domains;
}


template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}
//
//int main() {
//    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
//    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());
//
//    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
//    for (const Domain& domain : test_domains) {
//        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
//    }
//}