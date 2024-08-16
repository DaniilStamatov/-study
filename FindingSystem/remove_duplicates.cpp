#include "remove_duplicates.h"

void RemoveDuplicates(SearchServer& search_server)
{
	std::map<std::vector<std::string>, int> doc_words;
	std::vector<int> duplicate_ids;
	

	for (const int document_id : search_server) {
		auto word_freqs = search_server.GetWordFrequencies(document_id);

		std::vector<std::string> words;
		for (const auto& word_freq : word_freqs) {
			words.push_back(word_freq.first);
		}

		auto [word, emplaced] = doc_words.emplace(words, document_id);
		if (!emplaced) {
			duplicate_ids.push_back(document_id);
		}
	}

	for (const int document_id : duplicate_ids) {
		std::cout << "Found duplicate document id " << document_id << std::endl;
		search_server.RemoveDocument(document_id);
	}
}
