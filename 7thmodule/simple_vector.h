#pragma once

#include <cassert>
#include <initializer_list>
#include <stdexcept>


class ReserveProxyObj {
public:
    ReserveProxyObj(size_t capacity)
        : capacity_(capacity)
    {
    }

    size_t GetCapacity() {
        return capacity_;
    }

private:
    size_t capacity_;
};

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept : size_(0), capacity_(0), items_(nullptr){

    }

    SimpleVector(const SimpleVector& other): capacity_(other.capacity_), size_(other.size_) {
        items_ = size_ != 0 ? new Type[other.size_] : nullptr;
        std::copy(other.begin(), other.end(), items_);
    }

    SimpleVector(SimpleVector&& other) {
        items_ = size_ != 0 ? new Type[other.size_] : nullptr;
        swap(other);
    }

    // ������ ������ �� size ���������, ������������������ ��������� �� ���������
    explicit SimpleVector(size_t size) : size_(size), capacity_(size){
       items_ = size != 0 ? new Type[size] : nullptr;
       std::fill(items_, items_ + size, Type());
    }

        // ������ ������ �� size ���������, ������������������ ��������� value
    SimpleVector(size_t size, const Type& value) : SimpleVector(size){
        if(size!=0)
            std::fill(items_, items_ + size, value);
    }

    // ������ ������ �� std::initializer_list
    SimpleVector(std::initializer_list<Type> init) : SimpleVector(init.size()){
        std::copy(init.begin(), init.end(), items_);
        // �������� ���� ������������ ��������������
    }

    // ���������� ���������� ��������� � �������
    size_t GetSize() const noexcept {
        return size_;
        // �������� ���� ��������������
    }

    // ���������� ����������� �������
    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    // ��������, ������ �� ������
    bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    // ���������� ������ �� ������� � �������� index
    Type& operator[](size_t index) noexcept {
        return items_[index];
        // �������� ���� ��������������
    }

    // ���������� ����������� ������ �� ������� � �������� index
    const Type& operator[](size_t index) const noexcept {
        return items_[index];
        // �������� ���� ��������������
    }

    SimpleVector& operator=(const SimpleVector& rhs) {
        if (&items_ != &rhs.items_) {
            Type* new_items = new Type[rhs.GetCapacity()];
            std::copy(rhs.begin(), rhs.end(), new_items);
            std::swap(items_, new_items);
            size_ = rhs.size_;
            capacity_ = rhs.capacity_;
        }
        return *this;
    }

    // ��������� ������� � ����� �������
    // ��� �������� ����� ����������� ����� ����������� �������
    void PushBack(const Type& item) {
        if (size_ + 1 > capacity_) {
            size_t new_capacity = std::max(size_ + 1, capacity_ * 2);
            Type* new_items = new Type[new_capacity];
            std::fill(new_items, new_items + new_capacity, Type());
            std::copy(items_, items_ + capacity_, new_items);
            std::swap(items_, new_items);
            capacity_ = new_capacity;
        }
        items_[size_] = item;
        ++size_;
        // �������� ���� ��������������
    }

    void PushBack(Type&& item) {
        if (size_ + 1 > capacity_) {
            size_t new_capacity = std::max(size_ + 1, capacity_ * 2);
            Type* new_items = new Type[new_capacity];
            std::move(items_, items_ + size_, new_items);
            std::swap(items_, new_items);
            capacity_ = new_capacity;
        }
        items_[size_] = std::move(item);
        ++size_;
        // �������� ���� ��������������
    }

    // ��������� �������� value � ������� pos.
    // ���������� �������� �� ����������� ��������
    // ���� ����� �������� �������� ������ ��� �������� ���������,
    // ����������� ������� ������ ����������� �����, � ��� ������� ������������ 0 ����� ������ 1
    Iterator Insert(ConstIterator pos, const Type& value) {
        size_t count = pos - items_;
        if (capacity_ == 0) {
            Type* new_items = new Type[1];
            new_items[count] = value;
            std::swap(items_, new_items);
            size_ = 1;
            ++capacity_;
        }
        else if(size_ < capacity_)
        {
            std::copy_backward(items_ + count, items_ + size_, items_ + size_ + 1);
            items_[count] = value;
        }
        else {
            size_t new_capacity = std::max(size_ + 1, capacity_ * 2);
            Type* new_items = new Type[new_capacity];
            std::copy(items_, items_ + capacity_, new_items);
            std::copy_backward(items_ + count, items_ + size_, new_items + size_ + 1);
            new_items[count] = value;
            std::swap(items_, new_items);
            capacity_ = new_capacity;
        }

        ++size_;

        return &items_[count];
    }

    Iterator Insert(ConstIterator pos, Type&& value) {
        size_t count = pos - items_;
        if (capacity_ == 0) {
            Type* new_items = new Type[1];
            new_items[count] = std::move(value);
            std::swap(items_, new_items);
            size_ = 1;
            ++capacity_;
        }
        else if (size_ < capacity_)
        {
            std::move_backward(items_ + count, items_ + size_, items_ + size_ + 1);
            items_[count] = std::move(value);
        }
        else {
            size_t new_capacity = std::max(size_ + 1, capacity_ * 2);
            Type* new_items = new Type[new_capacity];
            std::move(items_, items_ + capacity_, new_items);
            std::move_backward(items_ + count, items_ + size_, new_items + size_ + 1);
            new_items[count] = std::move(value);
            std::swap(items_, new_items);
            capacity_ = new_capacity;
        }

        ++size_;

        return &items_[count];
    }

    // "�������" ��������� ������� �������. ������ �� ������ ���� ������
    void PopBack() noexcept {
        if (items_) --size_;
    }


    void Reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            Type* temp = new Type[new_capacity];
            std::fill(temp, temp + new_capacity, Type());
            std::copy(items_, items_ + size_, temp);
            items_.swap(temp);
            capacity_ = new_capacity;
        }
    }

    // ������� ������� ������� � ��������� �������
    Iterator Erase(ConstIterator pos) {
        assert(pos != this->end());
        size_t count = pos - items_;
        std::move(items_ + count + 1, items_ + size_, items_ + count);
        --size_;

        return &items_[count];
    }

    // ���������� �������� � ������ ��������
    void swap(SimpleVector& other) noexcept {
        std::swap(capacity_, other.capacity_);
        std::swap(size_, other.size_);
        std::swap(items_, other.items_);
        // �������� ���� ��������������
    }

    // ���������� ����������� ������ �� ������� � �������� index
    // ����������� ���������� std::out_of_range, ���� index >= size
    Type& At(size_t index) {
        if (index >= size_) throw std::out_of_range("out of range");
        return items_[index];
        // �������� ���� ��������������
    }

    // ���������� ����������� ������ �� ������� � �������� index
    // ����������� ���������� std::out_of_range, ���� index >= size
    const Type& At(size_t index) const {
        if (index >= size_) throw std::out_of_range("out of range");
        return items_[index];
        // �������� ���� ��������������
    }

    // �������� ������ �������, �� ������� ��� �����������
    void Clear() noexcept {
        size_ = 0;
        // �������� ���� ��������������
    }

    // �������� ������ �������.
    // ��� ���������� ������� ����� �������� �������� �������� �� ��������� ��� ���� Type
    void Resize(size_t new_size) {
        if (new_size <= size_) {
            size_ = new_size;
        }
        if (new_size <= capacity_) {
            std::fill(end(), end() + new_size, Type());
        }
        if (new_size > capacity_) {
            size_t new_capacity = std::max(new_size, capacity_ * 2);
            Type* new_items = new Type[new_capacity];
            std::fill(new_items, new_items + new_capacity, Type());
            std::copy(items_, items_ + capacity_, new_items);
            std::swap(items_, new_items);

            size_ = new_size;
            capacity_ = new_capacity;
        }
        // �������� ���� ��������������
    }

    // ���������� �������� �� ������ �������
    // ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
    Iterator begin() noexcept {
        return items_;
        // �������� ���� ��������������
    }

    // ���������� �������� �� �������, ��������� �� ���������
    // ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
    Iterator end() noexcept {
        return items_ + size_;
        // �������� ���� ��������������
    }

    // ���������� ����������� �������� �� ������ �������
    // ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
    ConstIterator begin() const noexcept {
        return items_;
        // �������� ���� ��������������
    }

    // ���������� �������� �� �������, ��������� �� ���������
    // ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
    ConstIterator end() const noexcept {
        return items_ + size_;
        // �������� ���� ��������������
    }

    // ���������� ����������� �������� �� ������ �������
    // ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
    ConstIterator cbegin() const noexcept {
        return begin();
        // �������� ���� ��������������
    }

    // ���������� �������� �� �������, ��������� �� ���������
    // ��� ������� ������� ����� ���� ����� (��� �� �����) nullptr
    ConstIterator cend() const noexcept {
        return end();
        // �������� ���� ��������������
    }

private:
    Type* items_;
    size_t size_;
    size_t capacity_;
};


ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}


template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::equal(lhs.begin(), lhs.end(),
        rhs.begin());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !std::equal(lhs.begin(), lhs.end(),
        rhs.begin());
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
        rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return (lhs < rhs || lhs == rhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs <= rhs);
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs < rhs);
}