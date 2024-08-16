#include <numeric>
#include <ostream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
using namespace std;

class Rational {
public:
    Rational() = default;

    Rational(int numerator)
        : numerator_(numerator)
        , denominator_(1) {
    }

    Rational(int numerator, int denominator)
        : numerator_(numerator)
        , denominator_(denominator) {
        if (denominator_ == 0) throw domain_error("Знаменатель не должен быть нулевым");

        Normalize();
    }

    int Numerator() const {
        return numerator_;
    }

    int Denominator() const {
        return denominator_;
    }

    Rational& operator+=(Rational right) {
        const int numerator = numerator_ * right.Denominator() + right.Numerator() * denominator_;
        const int denumerator = denominator_ * right.Denominator();
        numerator_ = numerator;
        denominator_ = denumerator;
        Normalize();
        return *this;
    }

    Rational& operator-=(Rational right) {
        const int numerator = numerator_ * right.Denominator() - right.Numerator() * denominator_;
        const int denumerator = denominator_ * right.Denominator();
        numerator_ = numerator;
        denominator_ = denumerator;
        Normalize();
        return *this;
    }


    bool operator==(const Rational& right) const{

        return numerator_ == right.numerator_ && denominator_ == right.denominator_;
    }
    Rational& operator/=(Rational right) {
        if (right == Rational(0, 1)) throw std::runtime_error("Division by zero");
        numerator_ *= right.denominator_;
        denominator_ *= right.numerator_;
        Normalize();
        return *this;
    }

    Rational& operator*=(Rational right) {
        const int numerator = numerator_ * right.Numerator();
        const int denumerator = denominator_* right.Denominator();
        numerator_ = numerator;
        denominator_ = denumerator;
        Normalize();
        return *this;
    }

private:
    void Normalize() {
        if (denominator_ < 0) {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
        const int divisor = gcd(numerator_, denominator_);
        numerator_ /= divisor;
        denominator_ /= divisor;
    }

    int gcd(int a, int b) {
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    int numerator_ = 0;
    int denominator_ = 1;
};

ostream& operator<<(ostream& os, Rational rational) {
    os << rational.Numerator() << "/" << rational.Denominator() << endl;
    return os;
}

istream& operator>>(istream& is, Rational& rational) {
    int num;
    int denum;
    is >> num >> denum;
    rational = Rational{ num, denum };
    return is;
}

Rational operator+(Rational left, Rational right) {
    return left+=right;
}

Rational operator-(Rational left, Rational right) {
    return left -= right;
}


Rational operator/(Rational left, Rational right) {
    return left /= right;
}


Rational operator*(Rational left, Rational right) {
    return left *= right;
}


struct Lang {
    string name;
    int age;
};

ostream& operator<<(ostream& os, Lang rational) {
    os << rational.name << ", " << rational.age << endl;
    return os;
}
template<typename Iterator>
void PrintRange(Iterator begin, Iterator end) {
    for (auto it = begin; it != end; ++it) {
        cout << *it << " ";
    }
    cout << endl;
}


template<typename Container>
void FindAndPrint(Container c, int i) {
    auto it = find_if(c.begin(), c.end(), [i](const int& j) {return j == i;});

    if (it == c.end())
        PrintRange(c.begin(), c.end());
    else {
        PrintRange(c.begin(), it);
        PrintRange(it, c.end());
    }
}

template<typename Iterator>
auto MakeVector(Iterator begin, Iterator end) {
   return vector<typename Iterator::value_type>(begin, end);
}



template<typename Container>
void EraseAndPrint(Container& c, int it) {
    if (c.size() == 0) return;
    int itCopy = it;
    PrintRange(c.begin(), c.begin()+it);
    PrintRange(c.begin()+it, c.end());
    c.erase(c.begin() + it);
}
template<typename Container>
void EraseAndPrint(Container& c, int it, int beg, int end) {
    if (c.size() == 0) return;
    int itCopy = it;

    c.erase(c.begin() + it);

    PrintRange(c.begin(), c.begin() + it);
    PrintRange(c.begin() + it, c.end());

    c.erase(c.begin() + beg, c.begin() + end);
    PrintRange(c.begin(), c.end());
}


int Fibonacci(int n) {
    int fibo = 0;
    if (n <= 1) return n;

    fibo = Fibonacci(n - 1) + Fibonacci(n - 2);

    return fibo;
}

bool IsPowOfTwo(int number) {
    if (number <= 0) return false;
    if (number == 1) return true;
    
    if (number % 2 > 0) return false;
    return  IsPowOfTwo(number / 2);
}





void hanoi(int n, std::vector<int>& from, std::vector<int>& to, std::vector<int>& aux) {
    if (n == 1) {
        // Move the top disk from 'from' to 'to'
        to.push_back(from.back());
        from.pop_back();
        std::cout << "Move disk " << n << " from rod " << "A" << " to rod " << "C" << std::endl;
    }
    else {
        // Move n-1 disks from 'from' to 'aux', using 'to' as an auxiliary rod
        hanoi(n - 1, from, aux, to);

        // Move the nth disk from 'from' to 'to'
        to.push_back(from.back());
        from.pop_back();
        std::cout << "Move disk " << n << " from rod " << "A" << " to rod " << "C" << std::endl;

        // Move n-1 disks from 'aux' to 'to', using 'from' as an auxiliary rod
        hanoi(n - 1, aux, to, from);
    }
}
//
//int main() {
//    int n = 4; // Number of disks
//    std::vector<int> rodA(n);
//    std::vector<int> rodB;
//    std::vector<int> rodC;
//
//    // Initialize rodA with disks in decreasing order of size
//    for (int i = n; i > 0; --i) {
//        rodA[i - 1] = i;
//    }
//
//    hanoi(n, rodA, rodC, rodB);
//
//    return 0;
//}
//int main() {
//    set<int> test = { 1, 1, 1, 2, 3, 4, 5, 5 };
//    FindAndPrint(test, 3);
//    FindAndPrint(test, 0);
//
//    set<string> unique_langs = { "Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s };
//    vector<string> langs = MakeVector(unique_langs.begin(), unique_langs.end());
//    
//    PrintRange(langs.begin(), langs.end());
//}
//int main() {
//    std::vector<Lang> langs = { {"Python"s, 29}, {"Java"s, 24}, {"C#"s, 20}, {"Ruby"s, 25}, {"C++"s, 37} };
//
//    auto it = find_if(langs.begin(), langs.end(), [](const Lang& lang) {return lang.name[0] == 'J'; });
//    ++it;
//    cout << *it << std::endl;
//}
    // Выведите первый язык, начинающийся на J, используя алгоритм find_if
    // 
//}
//
//int main() {
//    vector<string> langs = { "Python"s, "Java"s, "C#"s, "Ruby"s, "C++"s };
//    auto res = find_if(langs.begin(), langs.end(), [](const string& lang) {return lang[0] == 'C'; });//returns iterator to 
//    // Выведите первый язык, начинающийся на C, используя алгоритм find_if
//
//    cout << *res << std::endl;
//}