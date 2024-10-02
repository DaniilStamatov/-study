#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

using namespace std;

string GetLine(istream& in);
// реализуйте эту функцию:
void CreateFiles(const string& config_file) {
    std::ifstream in(config_file);
    std::string line;
    std::string filename;
    std::ofstream of;

    while (in) {
        string str = GetLine(in);
        if (str.empty()) return;
        else if (str[0] != '>') {
            of.close();
            of.open(str);
        }
        else {
            str.erase(0, 1);
            of << str << endl;
        }
    }

    in.close();
}

string GetLine(istream& in) {
    string s;
    getline(in, s);
    return s;
}
//
//int main() {
//    ofstream("test_config.txt"s) << "a.txt\n"
//        ">10\n"
//        ">abc\n"
//        "b.txt\n"
//        ">123"sv;
//
//    CreateFiles("test_config.txt"s);
//    ifstream in_a("a.txt"s);
//    assert(GetLine(in_a) == "10"s && GetLine(in_a) == "abc"s);
//
//    ifstream in_b("b.txt"s);
//    assert(GetLine(in_b) == "123"s);
//}
