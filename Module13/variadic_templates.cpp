#include <cassert>
#include <string>
#include <string_view>
/* Напишите вашу реализацию EqualsToOneOf здесь*/

using namespace std::literals;

template<typename T0, typename ...Values>
bool EqualsToOneOf(const T0& ans, const Values&... values) {
    if constexpr(sizeof...(values) == 0) {
        return false;
    }
    return ((ans == values) || ...);
}

//int main() {
//    assert(EqualsToOneOf("hello"sv, "hi"s, "hello"s));
//    assert(!EqualsToOneOf(1, 10, 2, 3, 6));
//    assert(!EqualsToOneOf(8));
//}
