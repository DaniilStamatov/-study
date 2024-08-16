#include "RequestQueue.h"

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentStatus status)
{
    std::vector<Document> results = search_server_.FindTopDocuments(raw_query, status);
    requests_.emplace_back(QueryResult{ raw_query, results.empty() });
    RemoveOldRequests();
    return results;
}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query)
{
    std::vector<Document> results = search_server_.FindTopDocuments(raw_query);
    requests_.emplace_back(QueryResult{ raw_query, results.empty() });
    RemoveOldRequests();
    return results;
    // напишите реализацию
}

int RequestQueue::GetNoResultRequests() const
{
    int count = 0;
    for (const auto& request : requests_) {
        if (request.no_result_) {
            count++;
        }
    }
    return count;
    // напишите реализацию
}

void RequestQueue::RemoveOldRequests()
{
    while (requests_.size() > 1440) {
        requests_.pop_front();
    }
}