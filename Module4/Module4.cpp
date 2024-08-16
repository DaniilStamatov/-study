
#include <string>
#include <stdio.h>
#include <vector>
#include <set>
#include <iostream>
#include <algorithm>
using namespace std;
void PrintSpacesPositions(std::string& str) {
    for (std::string::iterator it = std::find(str.begin(), str.end(), ' '); it != str.end();) {
        std::cout << std::distance(str.begin(), it)<<std::endl;
        it = std::find(std::next(it), str.end(), ' ');
    }
}

int binary_search(const std::vector<int>& vec, int target) {
    int left = 0;
    int right = vec.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (vec[mid] == target) {
            return mid;
        }
        else if (vec[mid] > target) {
            right = mid - 1;
        }
        else {
            left = mid + 1;
        }

    }
    return -1;
}

std::set<int>::iterator FindNearestElement(const std::set<int>& numbers, int border) {
    if (numbers.empty()) {
        return numbers.end();
    }

    auto lower = numbers.lower_bound(border);
    auto upper = lower;
    if (lower != numbers.begin()) {
        --upper;
    }
    else {
        return lower;  // If lower is at the beginning, return it
    }

    if (lower == numbers.end()) {
        return upper;
    }
    else if (*lower - border >= border - *upper) {
        return upper;
    }
    else {
        return lower;
    }
}

template<typename I>
std::pair<I, I> FindStartsWith(I range_begin, I range_end, const std::string& prefix) {
   /* I first = range_begin;
    while (first != range_end && (*first)[0] < prefix) {
        ++first;
    }

    I last = first;
    while (last != range_end && (*last)[0] == prefix) {
        ++last;
    }

    return std::make_pair(first, last);*/
   /* auto first = std::lower_bound(range_begin, range_end, prefix, [&prefix](const std::string& a, const std::string& b) {
        return a.compare(0, prefix.size(), b.substr(0, prefix.size())) < 0;
        });

    auto last = std::upper_bound(range_begin, range_end, prefix + std::string(prefix.size(), '\xff'), [&prefix](const std::string& a, const std::string& b) {
        return a.compare(0, prefix.size(), b.substr(0, prefix.size())) < 0;
        });

    return std::make_pair(first, last);*/
    return std::equal_range(range_begin, range_end, prefix, [&prefix](const std::string& a, const std::string& b) {
        return a.compare(0, prefix.size(), b.substr(0, prefix.size())) < 0;
        });
}
//int main() {
//    const std::vector<string> sorted_strings = {"daormansk","moscow", "motovilikha"};
//
//    const auto m_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "mo");
//    for (auto it = m_result.first; it != m_result.second; ++it) {
//        cout << *it << " ";
//    }
//    cout << endl;
//
//
//    const auto p_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "mt");
//    cout << (p_result.first - begin(sorted_strings)) << " " << (p_result.second - begin(sorted_strings)) << endl;
//
//    const auto z_result = FindStartsWith(begin(sorted_strings), end(sorted_strings), "na");
//    cout << (z_result.first - begin(sorted_strings)) << " " << (z_result.second - begin(sorted_strings)) << endl;
//
//    return 0;
//   /* set<int> numbers = { 1, 4, 6 };
//    cout << *FindNearestElement(numbers, 0) << " " << *FindNearestElement(numbers, 3) << " "
//        << *FindNearestElement(numbers, 5) << " " << *FindNearestElement(numbers, 6) << " "
//        << *FindNearestElement(numbers, 100) << endl;
//
//    set<int> empty_set;
//
//    cout << (FindNearestElement(empty_set, 8) == end(empty_set)) << endl;
//    return 0;*/
//}