#include <numeric>
#include <ostream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <string>

using namespace std;
template<typename Container>
void Permute(Container& container, int start, std::vector<std::string>& permutations) {
    if (start == container.size()) {
        std::string current;
        for (auto c : container) {
            current += std::to_string(c) + " ";
        }
        permutations.push_back(current);
    }

    for (int i = start; i < container.size(); i++) {
        std::swap(container[i], container[start]);
        Permute(container, start + 1, permutations);
        std::swap(container[i], container[start]);
    }
}

template<typename Iterator>
vector<string> GetPermutations(Iterator begin, Iterator end) {
    std::vector<std::string> permutations;

    std::vector<typename std::iterator_traits<Iterator>::value_type> container(begin, end);
    Permute(container, 0, permutations);

    return permutations;
}




//int main() {
//    vector<int> permutation(5);
//    // iota             -> http://ru.cppreference.com/w/cpp/algorithm/iota
//    // Заполняет диапазон последовательно возрастающими значениями
//    iota(permutation.begin(), permutation.end(), 1);
//
//    auto result = GetPermutations(permutation.begin(), permutation.end());
//    for (const auto& s : result) {
//        cout << s << endl;
//    }
//    return 0;
//}