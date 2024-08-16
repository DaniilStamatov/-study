#pragma once

#include <assert.h>
template<typename Type>
class ArrayPtr {
public:
    ArrayPtr() = default;


    // Создаёт в куче массив из size элементов типа Type.
    // Если size == 0, поле raw_ptr_ должно быть равно nullptr
    explicit ArrayPtr(size_t size) {
        if (size == 0) {
            raw_ptr_ = nullptr;
        }
        else {
            raw_ptr_ = new Type[size];
        }
        // Реализуйте конструктор самостоятельно
    }

    // Конструктор из сырого указателя, хранящего адрес массива в куче либо nullptr
    explicit ArrayPtr(Type* raw_ptr) noexcept : raw_ptr_(raw_ptr){
    }

    // Запрещаем копирование
    ArrayPtr(const ArrayPtr&) = delete;

    ~ArrayPtr() {
        delete[] raw_ptr_;
    }

    // Запрещаем присваивание
    ArrayPtr& operator=(const ArrayPtr&) = delete;

    // Прекращает владением массивом в памяти, возвращает значение адреса массива
    // После вызова метода указатель на массив должен обнулиться
    [[nodiscard]] Type* Release() noexcept {
        Type* temp = raw_ptr_;
        raw_ptr_ = nullptr;
        return temp;
        // Заглушка. Реализуйте метод самостоятельно
    }

    // Возвращает ссылку на элемент массива с индексом index
    Type& operator[](size_t index) noexcept {
        return *(raw_ptr_ + index); // or return raw_ptr_[index];
        // Реализуйте операцию самостоятельно
    }

    // Возвращает константную ссылку на элемент массива с индексом index
    const Type& operator[](size_t index) const noexcept {
        return *(raw_ptr_ + index); // or return raw_ptr_[index];
        // Реализуйте операцию самостоятельно
    }

    // Возвращает true, если указатель ненулевой, и false в противном случае
    explicit operator bool() const {
        return raw_ptr_ != nullptr;
        // Заглушка. Реализуйте операцию самостоятельно
    }

    // Возвращает значение сырого указателя, хранящего адрес начала массива
    Type* Get() const noexcept {
        return raw_ptr_;

        //or return &raw_ptr_[0];
        // Заглушка. Реализуйте метод самостоятельно
    }

    // Обменивается значениям указателя на массив с объектом other
    void swap(ArrayPtr& other) noexcept {
        Type* temp = raw_ptr_;
        raw_ptr_ = other.Get();
        other.raw_ptr_ = temp;
        // Реализуйте метод самостоятельно
    }

private:

	Type* raw_ptr_ = nullptr;
};