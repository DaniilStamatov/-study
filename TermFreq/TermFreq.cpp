#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "TermFreq.h"
#include "math.h"
#include <set>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <stack>

#include <algorithm>
using namespace std;
struct Animal {
    string name;
    int age;
    double weight;
};


template <typename Term>
map<Term, int> ComputeTermFreqs(const vector<Term>& terms) {
    map<Term, int> term_freqs;
    for (const Term& term : terms) {
        ++term_freqs[term];
    }
    return term_freqs;
}

pair<string, int> FindMaxFreqAnimal(const vector<pair<string, int>>& animals) {
    int max_freq = 0;
    pair<string, int> max;
    for (const auto& freq : ComputeTermFreqs(animals)) {
        if (freq.second > max_freq) {
            max_freq = freq.second;
            max = freq.first;
        }
    }

    return max;
    // верните животного с максимальной частотой
}
template <typename Document, typename Term>
std::vector<double> ComputeTfIdfs(const Document& documents, const Term& term) {
    int document_freq = 0;

    std::vector<double> tfs;
    for (const auto& document : documents) {
        int freq = std::count(document.begin(), document.end(), term);
        if (freq > 0) {
            ++document_freq;
        }
        tfs.push_back(static_cast<double>(freq) / document.size());
    }

    double idf = log(static_cast<double>(documents.size()) / document_freq);
    for (double& tf : tfs) {
        tf *= idf;
    }

    return tfs;
}

ostream& operator<<(ostream& out, const pair<string, int>& container) {
    out << "(" << container.first << ", " << container.second << ")";
    return out;
}

#include <cstdlib>
#include <string>

using namespace std;



template<typename T>
void Print(ostream& out, const T container) {
    bool isFirst = true;
    for (const auto& element : container) {
        if (!isFirst) {
            out << ", " << element;
        }
        else {
            out << element;
            isFirst = false;
        }
    }
}
template<typename Key, typename Value>
ostream& operator<<(ostream& out, const map<Key, Value>& container) {
    out << "<";
    Print(out, container);
    out << ">";
    return out;
}

template<typename Element>
ostream& operator<<(ostream& out, const set<Element>& container) {
    out << "{";
    Print(out, container);
    out << "}";
    return out;
}

//template<typename Element>
//ostream& operator<<(ostream& out, const vector<Element>& container) {
//    out << "[";
//    Print(out, container);
//    out << "]";
//    return out;
//}

template <typename Container, typename KeyMapper>
void SortBy(Container& container, KeyMapper key_mapper, bool reverse = false) {
    // теперь можно сортировать контейнер
    sort(container.begin(), container.end(), [key_mapper, reverse](const auto& lhs, const auto& rhs) {
        if (reverse) {
            return key_mapper(lhs) > key_mapper(rhs);
        }
        return key_mapper(lhs) < key_mapper(rhs); });
}

void PrintNames(const vector<Animal>& animals) {
    for (const Animal& animal : animals) {
        cout << animal.name << ' ';
    }
    cout << endl;
}

// Определяет, будет ли слово палиндромом
// text может быть строкой, содержащей строчные символы английского алфавита и пробелы
// Пустые строки и строки, состоящие только из пробелов, — это не палиндромы
bool IsPalindrome(const string& s) {
    if (s.empty()) {
        return true;
    }

    int start = 0;
    int end = s.size() - 1;

    while (start <= end) {
        if (!isalnum(s[start])) {
            start++;
            continue;
        }
        if (!isalnum(s[end])) {
            end--;
            continue;
        }
        if (tolower(s[start]) != tolower(s[end])) return false;

        else {
            start++;
            end--;
        }
    }

    return true;
}

class Solution {
public:
    bool isPalindrome(string& s) {
        if (s.empty()) {
            return true;
        }

        int start = 0;
        int end = s.size() - 1;

        while (start <= end) {
            if (!isalnum(s[start])) {
                start++;
                continue;
            }
            if (!isalnum(s[end])) {
                end--;
                continue;
            }
            if (tolower(s[start]) != tolower(s[end])) return false;

            else {
                start++;
                end--;
            }
        }

        return true;
    }

    bool hasDuplicate(vector<int>& nums) {
        std::vector<int> second;
        for (int num : nums) {
            if (count(nums.begin(), nums.end(), num) > 1) {
                return true;
            }
        }
        return false;
    }

    bool isAnagram(string s, string t) {
        sort(s.begin(), s.end());
        sort(t.begin(), t.end());
        return s == t;
    }

    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map<int, int> temp;

        for (int i = 0; i < nums.size(); i++) {
            if (temp.find(target - nums[i]) != temp.end()) {
                temp[nums[i]] = i;
            }
            else {
                return { temp[target - nums[i]], i };
            }
        }
        return { -1, -1 };
    }

    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        std::unordered_map<std::string, std::vector<std::string>> map;

        for (std::string str: strs) {
            std::string s = str;
            sort(s.begin(), s.end());
            map[str].push_back(s);
        }
        


        std::vector<std::vector<std::string>> result;

        for (auto pair : map) {
            result.push_back(pair.second);
        }

        return result;
    }

    static bool compare(pair<int, int> a, pair<int, int> b) {
        return a.first > b.first;
    }
    vector<int> topKFrequent(vector<int>& nums, int k) {
        unordered_map<int, int> freqs;
        vector<int> result;

        for (int n : nums) {
            freqs[n] ++;
        }
        vector<pair<int, int>> vec;
        for (auto freq : freqs) {
            vec.push_back(make_pair(freq.second, freq.first));
        }
        sort(vec.begin(), vec.end(), compare);

        for (auto pair : vec) {
            if (k == 0) return result;
            result.push_back(pair.second);
            k--;
        }
        return result;
    }

    static vector<int> productExceptSelf(vector<int>& nums) {
        std::vector<int> result(nums.size());

        int res = 1;
        int flag = 0;
        for (int i = 0; i < nums.size(); i++) {
            if (nums[i] == 0) {
                flag += 1;
                continue;
            }
            res *= nums[i];
        }
        for (int i = 0; i < nums.size(); i++) {
            if (flag == 0) {
                result[i] = res / nums[i];
            }
            else if (flag == 1) {
                if (nums[i] != 0) {
                    result[i] = 0;
                }
                else {
                    result[i] = res;
                }
            }
            else {
                result[i] = 0;
            }
        }
        
        return result;
    }

    

    bool isValidSudoku(vector<vector<char>>& board) {
        int n = board.size();
        std::unordered_map<int, std::unordered_set<char>> lines;
        std::unordered_map<int, std::unordered_set<char>> columns;
        std::unordered_map<int, std::unordered_set<char>> squares;


        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                char symbol = board[i][j];
                if (symbol == '.') {
                    continue;
                }
                if (lines[i].count(symbol) || columns[j].count(symbol) || squares[(i / 3) * 3 + j / 3].count(symbol)) {
                    return false;
                }

                lines[i].insert(symbol);
                columns[j].insert(symbol);
                squares[(i / 3) * 3 + j / 3].insert(symbol);
            }
        }

        return true;
    }
    static int longestConsecutive(vector<int>& nums) {
        if (nums.size() == 0) {
            return 0;
        }
        int count = 1;
        int longest = 1;

        sort(nums.begin(), nums.end());
        for (int i = 1; i < nums.size() - 1; i++) {
            if (nums[i -1] != nums[i]) {
                if (nums[i] -nums[i - 1] == 1) {
                    count++;
                }
                else {
                    longest = max(count, longest);
                    count = 1;
                }
            }
        }
        return max(longest, count);
    }

    bool isValid(string s) {
        unordered_map<char, char> brackets = { {')', '('}, {']','['}, {'}', '{'} };
        std::stack<char> st;

        for (const char& c : s) {
            if (brackets.find(c) != brackets.end()) {
                if (st.empty()) return false;

                if (st.top() != brackets.at(c)) {
                    return false;
                }

                st.pop();
            }
            else {
                st.push(c);
            }
        }
        return st.empty();
    }
    int evalRPN(vector<string>& tokens) {
        stack<int> st;
        for (const string& c : tokens) {
            if (c == "+") {
                int a = st.top();
                st.pop();
                int b = st.top();
                st.pop();
                st.push(a + b);
            }
            else if (c == "/") {
                int a = st.top();
                st.pop();
                int b = st.top();
                st.pop();
                st.push(static_cast<int>(static_cast<double>(b) / a));
            }
            else if (c == "*") {
                int a = st.top();
                st.pop();
                int b = st.top();
                st.pop();
                st.push(a * b);
            }
            else if (c == "-") {
                int a = st.top();
                st.pop();
                int b = st.top();
                st.pop();
                st.push(a - b);
            }
            else {
                st.push(stoi(c));
            }
        }
        return st.top();
    }
};

class MinStack {
public:
    vector<pair<int, int>> st;
    MinStack() {

    }

    void push(int val) {
        if (st.empty()) {
            std::pair<int, int> p;
            p.first = val;
            p.second = val;
            st.push_back(p);
        }
        else {
            std::pair<int, int> p;
            p.first = val;
            p.second = min(val, st.back().second);
            st.push_back(p);
        }
    }

    void pop() {
        st.pop_back();
    }

    int top() {
        return st.back().first;
    }

    int getMin() {
        return st.back().second;
    }
};

#define MACCROS(a, b) cout << a+b << endl;
#define PRINT_EXPRESSION(exp) cout << (#exp) << " = " << exp << endl;

void LogImpl(const string& str, const string& funcName, const string& fileName, int lineNumber) {
    cout << fileName << "(" << lineNumber << "): ";
    cout << funcName << ": " << str << endl;
}

template <typename T>
ostream& operator<<(ostream& os, const vector<T>& v) {
    os << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i != v.size() - 1) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}


template <typename K, typename V>
ostream& operator<<(ostream& os, const map<K, V>& m) {
    os << "[";
    for (const auto& pair : m) {
        os << pair.first << ": " << pair.second;
        if (&pair != &*m.rbegin()) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

template <typename T>
ostream& operator<<(ostream& os, const set<T>& s) {
    os << "[";
    for (const auto& elem : s) {
        os << elem;
        if (&elem != &*s.rbegin()) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}


#define LOG(expr) LogImpl(#expr, __FUNCTION__, __FILE__, __LINE__);
void Assert(bool value, const string& hint, const string& fileName, const string& funcName, unsigned int line) {
    if (value != true) {
        cout << "Assertion failed";
        cout << fileName << "("s << line << "): "s << funcName << ": "s;
        if(!hint.empty())
            cout << hint;
       
        abort();
    }

    // Реализуйте тело функции Assert
}

template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const string& t_str, const string& u_str,
    const string& fileName, const string& funcName, unsigned int line, const string& hint) {
    if (t != u) {
        cout << boolalpha;
        cout << fileName << "("s << line << "): "s << funcName << ": "s;
        cout<< "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
        cout << t << " != "s << u << "."s;
        if (!hint.empty()) {
            cout << " Hint: "s << hint;
        }
        cout << endl;
        abort();
    }
}


#define ASSERT(expr, hint) Assert(expr, hint, __FILE__, __FUNCTION__, __LINE__)
#define ASSERT_EQUAL(a, b) AssertEqualImpl(a, b, #a, #b,__FILE__, __FUNCTION__, __LINE__, "")
#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl(a, b, #a, #b,__FILE__, __FUNCTION__, __LINE__, hint)


vector<int> TakeEvens(const vector<int>& numbers) {
    vector<int> evens;
    for (int x : numbers) {
        if (x % 2 == 0) {
            evens.push_back(x);
        }
    }
    return evens;
}

vector<int> TakePositives(const vector<int>& numbers) {
    vector<int> positives;
    for (int x : numbers) {
        // Ошибка допущена намеренно, чтобы продемонстрировать вывод при несработавшем AssertEqual
        if (x >= 0) {
            positives.push_back(x);
        }
    }
    return positives;
}

int main() {
    const string greeting = "Hello"s;

    const vector<int> numbers = { 1, 0, 2, -3, 6, 2, 4, 3 };
    const vector<int> expected_evens = { 0, 2, 6, 2, 4 };
    ASSERT_EQUAL(TakeEvens(numbers), expected_evens);

    const vector<int> expected_positives = { 1, 2, 6, 2, 4, 3 };
    ASSERT_EQUAL(TakePositives(numbers), expected_positives);

    /*MACCROS(1, 2);
    PRINT_EXPRESSION(2 * 8 - 1);
    ASSERT_EQUAL(greeting.length(), 5);
    ASSERT_EQUAL_HINT(greeting.length(), 6, "length must be equal");

    LOG(13211);
    LogImpl("1 + 2", __FUNCTION__, __FILE__, __LINE__);*/
    // Намеренная ошибка в условии, чтобы показать работу Assert
    ASSERT(2+2 == 4, "THIS IS FAIL"s);
    // Следующая строка не должна выполниться, так как Assert аварийно завершит работу программы
    cout << "This line will not be printed"s << endl;
}
