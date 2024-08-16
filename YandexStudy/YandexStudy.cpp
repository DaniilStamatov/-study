#include <cstdlib>
#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <deque>
#include <tuple>

using namespace std;
//#define TIME_BLOCK(label, code)\
//{\
//    auto start_time = chrono::steady_clock::now();\
//    code;\
//    auto end_time = chrono::steady_clock::now();\
//    auto duration = end_time - start_time;\
//    std::cerr << label << chrono::duration_cast<chrono::microseconds>(duration).count() << " ms" << std::endl;\
//}
#define PROFILE_CONCAT_INTERNAL(X, Y) X ## Y
#define PROFILE_CONCAT(X, Y) PROFILE_CONCAT_INTERNAL(X, Y)
#define UNIQUE_VAR_NAME_PROFILE PROFILE_CONCAT(profile_guard_, __LINE__)
#define LOG_DURATION(x) LogDuration UNIQUE_VAR_NAME_PROFILE(x)

struct Ticket {
    int id;
    std::string name;
};
class TicketOffice {
public:
    void PushTicket(const string& name) {
        tickets_.push_back(Ticket{ last_id_, name });
        last_id_++;
    }
    
    int GetAvailable() const {
        return tickets_.size();
    }

    int GetAvailable(const std::string& name) const {
        return std::count_if(tickets_.begin(), tickets_.end(), [&name](const Ticket& ticket) { return ticket.name == name; });
    }

    void Invalidate(int minimum) {
        while (!tickets_.empty() && tickets_.front().id < minimum) {
            tickets_.pop_front();
        }
    }
private:
    int last_id_ = 0;
    std::deque<Ticket> tickets_;
};
class LogDuration {
public:
    LogDuration(std::string name) : name_(name) {

    }

    ~LogDuration() {
        const auto end_time = chrono::steady_clock::now();
        const auto duration = end_time - start_time;
        std::cerr << name_ << chrono::duration_cast<chrono::milliseconds>(duration).count() << " ms" << std::endl;
    }
private:
    std::string name_;
    const chrono::steady_clock::time_point start_time = chrono::steady_clock::now();
};

vector<int> ReverseVector(const vector<int>& source_vector) {
    vector<int> res;
    for (int i : source_vector) {
        res.insert(res.begin(), i);
    }

    return res;
}

vector<int> ReverseVector2(const vector<int>& source_vec) {
    return {source_vec.rbegin(), source_vec.rend()};
}

vector<int> ReverseYour(const vector<int>& source) {
    vector<int> res(source.size());
    auto it = res.rbegin();
    for (auto elem : source) {
        *it++ = elem;
    }
    return res;
}

int CountPops(const vector<int>& source_vector, int begin, int end) {

    int res = 0;

    for (int i = begin; i < end; ++i) {
        if (source_vector[i]) {
            ++res;
        }
    }

    return res;
}

template<typename T> 
bool TestPermut(const std::vector<T>& vector1, const std::vector<T>& vector2) {
    if (vector1.size() != vector2.size()) return false;
    std::vector<T> v1 = vector1;
    std::vector<T> v2 = vector2;
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    return v1 == v2;
}


template<typename T>
bool TestPermut2(const std::vector<T>& vector1, const std::vector<T>& vector2) {
    if (vector1.size() != vector2.size()) return false;
    std::vector<T> v1 = vector1;
    std::vector<T> v2 = vector2;
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    return v1 == v2;
}

void AppendRandom(vector<int>& v, int n) {
    for (int i = 0; i < n; ++i) {
        // получаем случайное число с помощью функции rand.
        // с помощью (rand() % 2) получим целое число в диапазоне 0..1.
        // в C++ имеются более современные генераторы случайных чисел,
        // но в данном уроке не будем их касаться
        v.push_back(rand() % 2);
    }
}

void AppendRandom2(vector<int>& v, int n) {
    v.reserve(v.size() + n);
    for (int i = 0; i < n; i += 15) {
        int number = rand();

        // мы можем заполнить 15 элементов вектора,
        // но не более, чем нам осталось до конца:
        int count = min(15, n - i);

        for (int j = 0; j < count; ++j)
            // таким образом, получим j-й бит числа.
            // операцию побитового сдвига вы уже видели в этой программе
            // на этот раз двигаем вправо, чтобы нужный бит оказался самым последним
            v.push_back((number >> j) % 2);
    }
}

std::vector<float> ComputeAvgTemp(std::vector<std::vector<float>>& mes) {
    if (mes.empty()) return{};
    size_t num_mes_per_day = mes[0].size();

    std::vector<float> avgTemps(num_mes_per_day, 0.0f);
    std::vector<size_t> numPosTemps(num_mes_per_day, 0);

    for (const auto& day : mes) {
        for (size_t i = 0; i < num_mes_per_day; ++i) {
            avgTemps[i] += (day[i] > 0.0f) ? day[i] : 0.0f;
            numPosTemps[i] += (day[i] > 0.0f) ? 1 : 0;
        }
    }

    for (size_t i = 0; i < num_mes_per_day; ++i) {
        avgTemps[i] = (numPosTemps[i] > 0) ? avgTemps[i] / static_cast<float>(numPosTemps[i]) : 0.0f;
    }

    return avgTemps;

}


int EffectiveCount(std::vector<int>& v, int n, int i) {
    int threshold = std::log2(v.size());
    int count;

    if (i <= threshold) {
        std::cout << "Using find if" << std::endl;
        auto it = std::find_if(v.begin(), v.end(), [&i](int x) {return x > i; });
        count = std::distance(v.begin(), it);
    }

    else {
        std::cout << "Using upper bound" << std::endl;
        auto it = std::upper_bound(v.begin(), v.end(), i);
        count = std::distance(v.begin(), it);
    }

    return count;
}
void Operate() {
    vector<int> random_bits;
    
    // операция << для целых чисел это сдвиг всех бит в двоичной
    // записи числа. Запишем с её помощью число 2 в степени 17 (131072)
    static const int N = 1 << 25;
    std::vector<int> v1(100000);
    std::vector<int> v2(100000);
    std::iota(v1.begin(), v1.end(), 0);
    std::iota(v2.begin(), v2.end(), 0);
    std::random_shuffle(v2.begin(), v2.end());

    {
        LOG_DURATION("TestPermut ");
        TestPermut(v1, v2);
    }

    // заполним вектор случайными числами 0 и 1
    {
        LOG_DURATION("AppendRandom ");
        AppendRandom2(random_bits, N);
    }
   
    // перевернём вектор задом наперёд
  /*  {
        LOG_DURATION("REVERSE ");
        ReverseVector(reversed_bits);
    }*/
    
    {
        LOG_DURATION("REVERSE2 ");
        ReverseVector2(random_bits);
    }
    vector<int> reversed_bits = random_bits;

    //// посчитаем процент единиц на начальных отрезках вектора
    {
        LOG_DURATION("COUNTPOPS ");
        for (int i = 1, step = 1; i <= N; i += step, step *= 2) {
            // чтобы вычислить проценты, мы умножаем на литерал 100. типа double;
            // целочисленное значение функции CountPops при этом автоматически
            // преобразуется к double, как и i
            double rate = CountPops(reversed_bits, 0, i) * 100. / i;
            cout << "After "s << i << " bits we found "s << rate << "% pops"s
                << endl;
        };
    }
       
}


#include <iostream>

using namespace std;

template <typename F>
int FindFloor(int n, F drop) {
    int left = 0;
    int right = n;

    while (left < right) {
        int mid = (left + right) / 2;

        if (drop(mid)) {
            right = mid;
        }
        else {
            left = mid + 1;
        }
    }

    return left;
}

int64_t T(int i) {
    if (i == 0 || i == 1) return 0;
    if (i == 2) return 1;
    int prev_0 = 0, prev_1 = 0, prev_2 = 1;
    for (int t = 2; t < i; ++t) {
        int next = prev_0 + prev_1 + prev_2;
        prev_0 = prev_1;
        prev_1 = prev_2;
        prev_2 = next;
    }

    return prev_2;
    //return T(i - 1) + T(i - 2) + T(i - 3);
}

template <typename F>
string BruteForceCicle(F check) {
    for (char c1 = 'A'; c1 <= 'Z'; ++c1) {
        for (char c2 = 'A'; c2 <= 'Z'; ++c2) {
            for (char c3 = 'A'; c3 <= 'Z'; ++c3) {
                for (char c4 = 'A'; c4 <= 'Z'; ++c4) {
                    for (char c5 = 'A'; c5 <= 'Z'; ++c5) {
                        std::string password = { c1, c2, c3, c4, c5 };
                        if (check(password)) {
                            std::cout << "Password found: " << password << std::endl;
                            return password;
                        }
                    }
                }
            }
        }
    }

    return "Password not found";
    // верните строку str, для которой check(str) будет true
}

template <typename F>
string BruteForceRecursion(F check) {
    return BruteForceInternal(check, "", 0);
}

template <typename F>
string BruteForceInternal(F check, const string& begin, int n) {
    if (n == 5) {
        if (check(begin)) {
            return begin;
        }
        return "";
    }

    for (char c = 'A'; c <= 'Z'; ++c) {
        std::string res = BruteForceInternal(check, begin + c, n + 1);
        if (!res.empty()) {
            return res;
        }
    }

    return "";
}

enum class CatBreed {
    Bengal,
    Balinese,
    Persian,
    Siamese,
    Siberian,
    Sphynx,
};

// Пол
enum class Gender {
    Male,
    Female,
};

struct Cat {
    string name;
    Gender gender;
    CatBreed breed;
    int age;
};


string CatBreedToString(CatBreed breed) {
    switch (breed) {
    case CatBreed::Bengal:
        return "Bengal"s;
    case CatBreed::Balinese:
        return "Balinese"s;
    case CatBreed::Persian:
        return "Persian"s;
    case CatBreed::Siamese:
        return "Siamese"s;
    case CatBreed::Siberian:
        return "Siberian";
    case CatBreed::Sphynx:
        return "Sphynx"s;
    default:
        throw invalid_argument("Invalid cat breed"s);
    }
}


template <typename Comparator>
vector<const Cat*> GetSortedCats(const vector<Cat>& cats, const Comparator& comp) {
    vector<const Cat*> sorted_cats;
    for (const auto& cat : cats) {
        sorted_cats.push_back(&cat);
    }

    std::sort(sorted_cats.begin(), sorted_cats.end(), comp);
    return sorted_cats;
}

void PrintCatPointerValues(const vector<const Cat*>& cat_pointers, ostream& out) {
    for (const auto& cat : cat_pointers) {
        cout << cat->name << " " << CatBreedToString(cat->breed) << endl;
    }
}

int main() {
    const vector<Cat> cats = {
        {"Tom"s, Gender::Male, CatBreed::Bengal, 2},
        {"Leo"s, Gender::Male, CatBreed::Siberian, 3},
        {"Luna"s, Gender::Female, CatBreed::Siamese, 1},
        {"Charlie"s, Gender::Male, CatBreed::Balinese, 7},
        {"Ginger"s, Gender::Female, CatBreed::Sphynx, 5},
        {"Tom"s, Gender::Male, CatBreed::Siamese, 2},
    };
   
    vector<const Cat*> sorted_cats_by_breed_name = GetSortedCats(cats, [](const Cat* cat1, const Cat* cat2) {

        return std::tie(cat1->breed, cat1->name) < std::tie(cat2->breed, cat2->name); }
    );

    vector<const Cat*> sorted_cats_by_breed_gender = GetSortedCats(cats, [](const Cat* cat1, const Cat* cat2) {
        /*if (cat1.breed != cat2.breed
            return cat1.breed < cat2.breed;
        else {
            return cat1.name < cat2.name;

        */
        return std::tie(cat1->gender, cat1->breed) < std::tie(cat2->gender, cat2->breed);
        }
    );


    PrintCatPointerValues(sorted_cats_by_breed_name, std::cout);
    PrintCatPointerValues(sorted_cats_by_breed_gender, std::cout);


}