#include <array>
#include <stdexcept>
#include <exception>

template <typename T, size_t N>
class StackVector {
public:
    explicit StackVector(size_t a_size = 0) {
        if(a_size > Capacity()) throw std::invalid_argument("No capacity");

        capacity_ = a_size;
    }

    T& operator[](size_t index) {
        return arr_.at(index);
    }
    const T& operator[](size_t index) const {
        return arr_.at(index);
    }

    std::array<T, N>::iterator begin() {
        return arr_.begin();
    }
    std::array<T, N>::iterator end() {
        return arr_.end();
    }
    std::array<T, N>::const_iterator begin() const {
        return arr_.cbegin();
    }
    std::array<T, N>::const_iterator end() const {
        return arr_.cend();
    }

    size_t Size() const {
        return capacity_;
    }
    size_t Capacity() const {
        return arr_.size();
    }

    void PushBack(const T& value) {
        if (arr_.size() == capacity_) throw std::overflow_error("no capacity");

        arr_[capacity_] = value;
        ++capacity_;
    }
    T PopBack() {
        if (capacity_ == 0) throw std::underflow_error("Empty array");
        --capacity_;
        return arr_[capacity_];
    }

private:
    std::array<T, N> arr_;
    size_t capacity_;
};