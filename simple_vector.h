#pragma once

#include <cassert>
#include <initializer_list>
#include <array>
#include <stdexcept>
#include <iostream>

using namespace std::literals;

template <typename Type>
class SimpleVector {
  private:
  size_t size_ = 0;
  size_t capacity_ = 0;
  Type* array_ = nullptr;
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size) 
    : size_(size), capacity_(size), array_(size > 0 ? new Type[size] : nullptr) { }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value) 
    : size_(size), capacity_(size), array_(size > 0 ? new Type[size] : nullptr) { 
        std::fill(array_, array_ + size_, value);
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) 
    : size_(init.size()), capacity_(init.size()), array_(size_ > 0 ? new Type[size_] : nullptr) {
        std::copy(init.begin(), init.end(), array_);
    }

    ~SimpleVector() {
        delete[] array_;
    }

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return (size_ == 0);
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        return *(array_ + index);
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        return *(array_ + index);
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (index < size_) {
            return array_[index];
        } else {
            throw std::out_of_range("Out of range");
        }
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if (index < size_) {
            return array_[index];
        } else {
            throw std::out_of_range("Out of range");
        }
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        size_ = 0;
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
        if (size_ > new_size) {
            size_ = new_size;
        } else if (size_ == new_size) {
            // pass
        } else {
            size_t new_capacity_ = std::max(capacity_ * 2, new_size);
            Type* temp_array_ = new Type[new_capacity_];
            std::copy(array_, array_ + size_, temp_array_);
            size_ = capacity_ = new_capacity_;
            array_ = temp_array_;
        }
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        return size_ > 0 ? array_ : nullptr;
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        return size_ > 0 ? &array_[size_] : nullptr;
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        return size_ > 0 ? array_ : nullptr;
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        return size_ > 0 ? &array_[size_]: nullptr;
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        return size_ > 0 ? array_ : nullptr;
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        return size_ > 0 ? &array_[size_]: nullptr;
    }

    void Print() {
        for (const Type& i : *this) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }

};
