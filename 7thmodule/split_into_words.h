#include <string_view>
#include <string>
#include <vector>
using namespace std;
vector<string_view> SplitIntoWordsView(string_view str) {
    vector<string_view> result;
   
    str.remove_prefix(std::min(str.size(), str.find_first_not_of(' ')));
    const int64_t end = str.npos;
    while (!str.empty()) {
        int64_t space = str.find(' ');
        result.push_back(space == end ? str.substr(0, str.size()) : str.substr(0, space));
        str.remove_prefix(std::min(str.size(), str.find_first_not_of(' ')));
    }
    return result;
}