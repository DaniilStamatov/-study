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

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size) : size_(size), capacity_(size){
       items_ = size != 0 ? new Type[size] : nullptr;
       std::fill(items_, items_ + size, Type());
    }

        // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value) : SimpleVector(size){
        if(size!=0)
            std::fill(items_, items_ + size, value);
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) : SimpleVector(init.size()){
        std::copy(init.begin(), init.end(), items_);
        // Напишите тело конструктора самостоятельно
    }

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        return size_;
        // Напишите тело самостоятельно
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        return items_[index];
        // Напишите тело самостоятельно
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        return items_[index];
        // Напишите тело самостоятельно
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

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
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
        // Напишите тело самостоятельно
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
        // Напишите тело самостоятельно
    }

    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
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

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
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

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        assert(pos != this->end());
        size_t count = pos - items_;
        std::move(items_ + count + 1, items_ + size_, items_ + count);
        --size_;

        return &items_[count];
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        std::swap(capacity_, other.capacity_);
        std::swap(size_, other.size_);
        std::swap(items_, other.items_);
        // Напишите тело самостоятельно
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (index >= size_) throw std::out_of_range("out of range");
        return items_[index];
        // Напишите тело самостоятельно
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if (index >= size_) throw std::out_of_range("out of range");
        return items_[index];
        // Напишите тело самостоятельно
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        size_ = 0;
        // Напишите тело самостоятельно
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
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
        // Напишите тело самостоятельно
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        return items_;
        // Напишите тело самостоятельно
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        return items_ + size_;
        // Напишите тело самостоятельно
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        return items_;
        // Напишите тело самостоятельно
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        return items_ + size_;
        // Напишите тело самостоятельно
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        return begin();
        // Напишите тело самостоятельно
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        return end();
        // Напишите тело самостоятельно
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