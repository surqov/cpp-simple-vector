#pragma once

#include "array_ptr.h"

#include <array>
#include <cassert>
#include <initializer_list>

template <typename Type>
class SimpleVector {
  private:
    size_t size_ = 0;
    size_t capacity_ = 0;
    Array_ptr<Type> first_element = nullptr;
    void reassign(I begin_, I end_) {
		std::array<Type> curr;



        Node** ptr = &curr.head_.next_node;
        while (begin_ != end_) {
            *ptr = new Node(*begin_, nullptr);
            ptr = &((*ptr)->next_node);
			++curr.size_;
            ++begin_;
        }
        swap(curr);
	}
  public:
    using value_type = Type;
	using reference = value_type&;
	using const_reference = const value_type&;
	using Iterator = Array_ptr<Type>;
	using ConstIterator = Array_ptr<const Type>;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size) {
        if (size > 0) {
            size_ = size;
            capacity_ = size;
            first_element = new std::array<Type>[size];
        }
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value) {
        if (size > 0) {
            size_ = size;
            capacity_ = size;
            first_element = new std::array<Type>[size];
        }
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) {
        
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
        return (size_ == 0)
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
        
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        
    }
};