#include "SearchServer.h"


SearchServer::SearchServer(const std::string& stop_words_text): stop_words_(MakeUniqueNonEmptyStrings(SplitIntoWords(stop_words_text)))
{
}

void SearchServer::AddDocument(int document_id, const std::string& document, DocumentStatus status, const std::vector<int>& ratings) {
    if ((document_id < 0) || (documents_.count(document_id) > 0)) {
        throw std::invalid_argument("Invalid document_id");
    }
    const auto words = SplitIntoWordsNoStop(document);

    const double inv_word_count = 1.0 / words.size();
    for (const std::string& word : words) {
        word_to_document_freqs_[word][document_id] += inv_word_count;
    }

    for (const std::string& word : words) {
        document_to_word_freqs_[document_id][word] += inv_word_count;
    }
    documents_.emplace(document_id, DocumentData{ ComputeAverageRating(ratings), status });
    document_ids_.emplace(document_id);
}

std::tuple<std::vector<std::string>, DocumentStatus> SearchServer::MatchDocument(const std::string& raw_query, int document_id) const
{
    {
        LOG_DURATION_STREAM("Operation time: ", std::cout);
        const auto query = ParseQuery(raw_query);

        std::vector<std::string> matched_words;
        for (const std::string& word : query.plus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.push_back(word);
            }
        }
        for (const std::string& word : query.minus_words) {
            if (word_to_document_freqs_.count(word) == 0) {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id)) {
                matched_words.clear();
                break;
            }
        }
        return { matched_words, documents_.at(document_id).status };
    }
    
}

const std::map<std::string, double>& SearchServer::GetWordFrequencies(int document_id) const
{
    static const std::map<std::string, double> empty;
    if (document_to_word_freqs_.count(document_id) == 0) {
        return empty;
    }
    else {
        return document_to_word_freqs_.at(document_id);
    }
}

void SearchServer::RemoveDocument(int document_id)
{
    return RemoveDocument(std::execution::seq, document_id);
}

void SearchServer::RemoveDocument(const std::execution::sequenced_policy&, int document_id)
{
    if (!document_ids_.count(document_id)) {
        throw std::invalid_argument("Invalid document ID to remove");
    }

    document_ids_.erase(document_id);

    if (documents_.count(document_id)) {
        documents_.erase(document_id);
    }

    for (auto& element : SearchServer::word_to_document_freqs_) {
        if (element.second.count(document_id)) {
            element.second.erase(document_id);
        }
    }

    if (document_to_word_freqs_.count(document_id)) {
        document_to_word_freqs_.erase(document_id);
    }
}

void SearchServer::RemoveDocument(const std::execution::parallel_policy&, int document_id)
{
    if (!document_ids_.count(document_id)) {
        throw std::invalid_argument("Invalid document ID to remove");
    }

    std::map<std::string, double> word_freqs = document_to_word_freqs_.at(document_id);
    std::vector<const std::string*> words_to_erase(word_freqs.size());

    std::transform(std::execution::par, word_freqs.begin(), word_freqs.end(),
        words_to_erase.begin(),
        [](const auto& words_freq) { return &words_freq.first; });

    std::for_each(std::execution::par, words_to_erase.begin(), words_to_erase.end(),
        [this, document_id](const auto& word) {word_to_document_freqs_.at(*word).erase(document_id); });

    documents_.erase(document_id);
    document_ids_.erase(document_id);
    document_to_word_freqs_.erase(document_id);
}


bool SearchServer::IsValidWord(const std::string& word)
{
    // A valid word must not contain special characters
    return std::none_of(word.begin(), word.end(), [](char c) {
        return c >= '\0' && c < ' ';
        });
}

std::vector<std::string> SearchServer::SplitIntoWordsNoStop(const std::string& text) const
{
    std::vector<std::string> words;
    for (const std::string& word : SplitIntoWords(text)) {
        if (!IsValidWord(word)) {
            throw std::invalid_argument("Word " + word + " is invalid");
        }
        if (!IsStopWord(word)) {
            words.push_back(word);
        }
    }
    return words;
}

int SearchServer::ComputeAverageRating(const std::vector<int>& ratings)
{
    if (ratings.empty()) {
        return 0;
    }
    int rating_sum = 0;
    for (const int rating : ratings) {
        rating_sum += rating;
    }
    return rating_sum / static_cast<int>(ratings.size());
}

SearchServer::QueryWord SearchServer::ParseQueryWord(const std::string& text) const
{
    if (text.empty()) {
        throw std::invalid_argument("Query word is empty");
    }
    std::string word = text;
    bool is_minus = false;
    if (word[0] == '-') {
        is_minus = true;
        word = word.substr(1);
    }
    if (word.empty() || word[0] == '-' || !IsValidWord(word)) {
        throw std::invalid_argument("Query word " + text + " is invalid");
    }

    return { word, is_minus, IsStopWord(word) };
}

SearchServer::Query SearchServer::ParseQuery(const std::string& text) const
{
    Query result;
    for (const std::string& word : SplitIntoWords(text)) {
        const auto query_word = ParseQueryWord(word);
        if (!query_word.is_stop) {
            if (query_word.is_minus) {
                result.minus_words.insert(query_word.data);
            }
            else {
                result.plus_words.insert(query_word.data);
            }
        }
    }
    return result;
}

SearchServer::Query SearchServer::ParseQueryParallel(const std::string& text) const
{
   
}


double SearchServer::ComputeWordInverseDocumentFreq(const std::string& word) const {
    return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
}

