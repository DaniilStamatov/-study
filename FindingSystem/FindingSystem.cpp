﻿
#include "RequestQueue.h"
#include <iostream>
#include <chrono>
#include "remove_duplicates.h"
#include <random>
std::ostream& operator<<(std::ostream& out, const Document& document) {
    out << "{ "
        << "document_id = " << document.id << ", "
        << "relevance = " << document.relevance << ", "
        << "rating = " << document.rating << " }";
    return out;
}


void AddDocument(SearchServer& search_server, int document_id, const std::string& document, DocumentStatus status, const std::vector<int>& ratings) {
    search_server.AddDocument(document_id, document, status, ratings);
}
using namespace std;
//string GenerateWord(mt19937& generator, int max_length) {
//    const int length = uniform_int_distribution(1, max_length)(generator);
//    string word;
//    word.reserve(length);
//    for (int i = 0; i < length; ++i) {
//        word.push_back(uniform_int_distribution('a', 'z')(generator));
//    }
//    return word;
//}
//
//vector<string> GenerateDictionary(mt19937& generator, int word_count, int max_length) {
//    vector<string> words;
//    words.reserve(word_count);
//    for (int i = 0; i < word_count; ++i) {
//        words.push_back(GenerateWord(generator, max_length));
//    }
//    sort(words.begin(), words.end());
//    words.erase(unique(words.begin(), words.end()), words.end());
//    return words;
//}
//
//string GenerateQuery(mt19937& generator, const vector<string>& dictionary, int max_word_count) {
//    const int word_count = uniform_int_distribution(1, max_word_count)(generator);
//    string query;
//    for (int i = 0; i < word_count; ++i) {
//        if (!query.empty()) {
//            query.push_back(' ');
//        }
//        query += dictionary[uniform_int_distribution<int>(0, dictionary.size() - 1)(generator)];
//    }
//    return query;
//}
//
//vector<string> GenerateQueries(mt19937& generator, const vector<string>& dictionary, int query_count, int max_word_count) {
//    vector<string> queries;
//    queries.reserve(query_count);
//    for (int i = 0; i < query_count; ++i) {
//        queries.push_back(GenerateQuery(generator, dictionary, max_word_count));
//    }
//    return queries;
//}
//
//template <typename QueriesProcessor>
//void Test(string_view mark, QueriesProcessor processor, const SearchServer& search_server, const vector<string>& queries) {
//    LOG_DURATION(mark);
//    const auto documents_lists = processor(search_server, queries);
//}
//
//#define TEST(processor) Test(#processor, processor, search_server, queries)


#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    SearchServer search_server("and with"s);

    int id = 0;
    for (
        const string& text : {
            "funny pet and nasty rat"s,
            "funny pet with curly hair"s,
            "funny pet and not very nasty rat"s,
            "pet with rat and rat and rat"s,
            "nasty rat with curly hair"s,
        }
        ) {
        search_server.AddDocument(++id, text, DocumentStatus::ACTUAL, { 1, 2 });
    }

    const string query = "curly and funny"s;

    auto report = [&search_server, &query] {
        cout << search_server.GetDocumentCount() << " documents total, "s
            << search_server.FindTopDocuments(query).size() << " documents for query ["s << query << "]"s << endl;
        };

    report();
    // однопоточная версия
    search_server.RemoveDocument(5);
    report();
    // однопоточная версия
    search_server.RemoveDocument(execution::seq, 1);
    report();
    // многопоточная версия
    search_server.RemoveDocument(execution::par, 2);
    report();

    return 0;
}