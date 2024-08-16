#pragma once
#include <iostream>
template<typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end) :
        start_(begin), end_(end), size_(std::distance(start_, end_)) {
    }

    Iterator Begin() const {
        return start_;
    }

    Iterator End() const {
        return end_;
    }

    size_t Size() const {
        return size_;
    }


private:
    Iterator start_, end_;
    size_t size_;
};


template<typename Iterator>
std::ostream& operator<<(std::ostream& os, const IteratorRange<Iterator>& itRange) {
    for (Iterator it = itRange.Begin(); it != itRange.End(); it++) {
        cout << *it;
    }
    return os;
}


template<typename Iterator>
class Paginator {
public:
    Paginator(Iterator begin, Iterator end, size_t page_size) {
        size_t left = std::distance(begin, end);
        while (left > 0) {
            const size_t current_page_size = min(left, page_size);
            const Iterator current_page_end = begin + current_page_size;
            pages_.push_back({ begin, current_page_end });
            left -= current_page_size;
            begin = current_page_end;
        }
    }

    auto Begin() const {
        return pages_.begin();
    }

    auto End() const {
        return pages_.end();
    }

    size_t Size() const {
        return pages_.size();
    }
private:
    vector<IteratorRange<Iterator>> pages_;
};
template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator<typename Container::const_iterator>(begin(c), end(c), page_size);
}



