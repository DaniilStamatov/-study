#pragma once
#include <deque>
#include "SearchServer.h"

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server) : search_server_(search_server) {

        // �������� ����������
    }
    // ������� "������" ��� ���� ������� ������, ����� ��������� ���������� ��� ����� ����������
    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
        std::vector<Document> results = search_server_.FindTopDocuments(raw_query, document_predicate);
        requests_.emplace_back(QueryResult{ raw_query, results.empty() });
        RemoveOldRequests();
        return results;
        // �������� ����������
    }

    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);

    std::vector<Document> AddFindRequest(const std::string& raw_query);

    int GetNoResultRequests() const;
private:
    struct QueryResult {
        std::string query;
        bool no_result_;
        time_t timestamp;
        // ����������, ��� ������ ���� � ���������
    };
    std::deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    const SearchServer& search_server_;

    void RemoveOldRequests();
};