//#pragma once
//#include <cassert>
//#include <filesystem>
//#include <fstream>
//#include <iostream>
//#include <sstream>
//#include <string>
//#include <string_view>
//
//using namespace std;
//using filesystem::path;
//
//path operator""_p(const char* data, std::size_t sz) {
//    return path(data, data + sz);
//}
//
//void PrintTree(ostream& dst, const path& p, const filesystem::file_status& status, int offset) {
//    dst << string(offset, ' ') << p.filename().string() << std::endl;
//    if (status.type() == filesystem::file_type::directory) {
//        std::vector<filesystem::directory_entry> entries;
//        for (const auto& dir_entry : filesystem::directory_iterator(p)) {
//            entries.push_back(dir_entry);
//        }
//        std::sort(entries.begin(), entries.end(), [](const auto& lhs, const filesystem::directory_entry& rhs) {
//            return lhs.path().filename() > rhs.path().filename();
//            });
//        for (const auto& dir_entry : entries) {
//            PrintTree(dst, dir_entry.path(), dir_entry.status(), offset + 2);
//        }
//    }
//}
//
//
//
//// �������� ��� �������
//void PrintTree(ostream& dst, const path& p) {
//    error_code err;
//    auto status = filesystem::status(p, err);
//    if (err) {
//        return;
//    }
//
//    PrintTree(dst, p, status, 0);
//}
//
//
////int main() {
////    error_code err;
////    filesystem::remove_all("test_dir", err);
////    filesystem::create_directories("test_dir"_p / "a"_p, err);
////    filesystem::create_directories("test_dir"_p / "b"_p, err);
////
////    ofstream("test_dir"_p / "b"_p / "f1.txt"_p);
////    ofstream("test_dir"_p / "b"_p / "f2.txt"_p);
////    ofstream("test_dir"_p / "a"_p / "f3.txt"_p);
////    ostringstream out;
////    PrintTree(out, "test_dir"_p);
////    std::string output = out.str();
////    std::cout << output;
////    assert(out.str() ==
////        "test_dir\n"
////        "  b\n"
////        "    f2.txt\n"
////        "    f1.txt\n"
////        "  a\n"
////        "    f3.txt\n"sv
////    );
////}
