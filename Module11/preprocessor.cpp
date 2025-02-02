#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

using namespace std;
using filesystem::path;


path operator""_p(const char* data, std::size_t sz) {
    return path(data, data + sz);
}

bool FindIncludes(const path& file_path, size_t line_number, const path& in_file, const path& out_file, const vector<path>& include_directories);


// �������� ��� �������
bool Preprocess(const path& in_file, const path& out_file, const vector<path>& include_directories) {
    ifstream in(in_file);
    if (!in) {
        return false;
    }

    ofstream out(out_file, ios::out | ios::app);
    if (!out) {
        return false;
    }

    static regex include_regex(R"/(\s*#\s*include\s*"([^"]*)"\s*)/");
    static regex include_lib_regex(R"/(\s*#\s*include\s*<([^>]*)>\s*)/");

    smatch match;
    string line;
    size_t line_number = 0;


    while (getline(in, line)) {
        ++line_number;
        if (regex_match(line, match, include_regex)) {
            path p = string(match[1]);
            path full_path = in_file.parent_path() / p;
            ifstream full_in;
            full_in.open(full_path);
            if (full_in.is_open()) {
                Preprocess(full_path, out_file, include_directories);
                full_in.close();
            }
            else {
                bool found =  FindIncludes(p, line_number, in_file, out_file, include_directories);
                if (!found) return false;
            }
        }
        else if (regex_match(line, match, include_lib_regex)) {
            path p = string(match[1]);
            bool found = FindIncludes(p, line_number, in_file, out_file, include_directories);
            if (!found) return false;
        }
        else {
            out << line << endl;
        }
    }

    return true;
}

bool FindIncludes(const path& file_path, size_t line_number, const path& in_file, const path& out_file, const vector<path>& include_directories) {
    bool found = false;
    for (const auto& file : include_directories) {
        if (exists(path(file / file_path))) {
            found = true;
            Preprocess(file / file_path, out_file, include_directories);
            break;
        }
    }
    if (!found) {
        cout << "unknown include file "s << file_path.filename().string() << " at file "s << in_file.string() << " at line "s << line_number << endl;
    }
    return found;
}

string GetFileContents(string file) {
    ifstream stream(file);

    // ������������ string �� ���� ����������
    return { (istreambuf_iterator<char>(stream)), istreambuf_iterator<char>() };
}

void Test() {
    error_code err;
    filesystem::remove_all("sources"_p, err);
    filesystem::create_directories("sources"_p / "include2"_p / "lib"_p, err);
    filesystem::create_directories("sources"_p / "include1"_p, err);
    filesystem::create_directories("sources"_p / "dir1"_p / "subdir"_p, err);

    {
        ofstream file("sources/a.cpp");
        file << "// this comment before include\n"
            "#include \"dir1/b.h\"\n"
            "// text between b.h and c.h\n"
            "#include \"dir1/d.h\"\n"
            "\n"
            "int SayHello() {\n"
            "    cout << \"hello, world!\" << endl;\n"
            "#   include<dummy.txt>\n"
            "}\n"sv;
    }
    {
        ofstream file("sources/dir1/b.h");
        file << "// text from b.h before include\n"
            "#include \"subdir/c.h\"\n"
            "// text from b.h after include"sv;
    }
    {
        ofstream file("sources/dir1/subdir/c.h");
        file << "// text from c.h before include\n"
            "#include <std1.h>\n"
            "// text from c.h after include\n"sv;
    }
    {
        ofstream file("sources/dir1/d.h");
        file << "// text from d.h before include\n"
            "#include \"lib/std2.h\"\n"
            "// text from d.h after include\n"sv;
    }
    {
        ofstream file("sources/include1/std1.h");
        file << "// std1\n"sv;
    }
    {
        ofstream file("sources/include2/lib/std2.h");
        file << "// std2\n"sv;
    }

    assert((!Preprocess("sources"_p / "a.cpp"_p, "sources"_p / "a.in"_p,
        { "sources"_p / "include1"_p,"sources"_p / "include2"_p })));

    ostringstream test_out;
    test_out << "// this comment before include\n"
        "// text from b.h before include\n"
        "// text from c.h before include\n"
        "// std1\n"
        "// text from c.h after include\n"
        "// text from b.h after include\n"
        "// text between b.h and c.h\n"
        "// text from d.h before include\n"
        "// std2\n"
        "// text from d.h after include\n"
        "\n"
        "int SayHello() {\n"
        "    cout << \"hello, world!\" << endl;\n"sv;

    assert(GetFileContents("sources/a.in"s) == test_out.str());
}

//int main() {
//    Test();
//}
