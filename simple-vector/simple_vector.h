#pragma once

#include "ArrayPtr.h"

#include <initializer_list>
#include <array>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std::literals;

struct ReserveProxyObj {
    size_t capacity_to_reserve;
    explicit ReserveProxyObj(size_t capacity) : capacity_to_reserve(capacity) {}
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}

template <typename Type>
class SimpleVector {
  public:
    using Iterator = Type*;
    using ConstIterator = const Type*;
    using SmartPtr = ArrayPtr<Type>;
  private:
    size_t size_ = 0;
    size_t capacity_ = 0;
    SmartPtr array_{nullptr};

    template <class InputIterator>
    void AssignByMove(InputIterator begin_, InputIterator end_) {
        size_t size = std::distance(begin_, end_);
        SimpleVector<Type> temp_(size);
        std::move(begin_, end_, temp_.begin());
        swap(temp_);
    }

    template <class InputIterator>
    void AssignByCopy(InputIterator begin_, InputIterator end_) {
        size_t size = std::distance(begin_, end_);
        SimpleVector<Type> temp_(size);
        std::copy(begin_, end_, temp_.begin());
        swap(temp_);
    }

    void ResizeByMove(size_t new_capacity_) {
        SmartPtr new_array_(new_capacity_);
        size_t _count = std::min(size_, new_capacity_);
        std::move(array_.Get(), array_.Get() + _count, new_array_.Get());
        array_.swap(new_array_);
        capacity_ = new_capacity_;
    }
  public:
    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size) 
        : size_(size), 
        capacity_(size), 
        array_(size) {};

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value) 
        : size_(size), 
        capacity_(size), 
        array_(size) { 
            std::fill(array_.Get(), array_.Get() + size_, value);
    }

    // Перемещает значения из std::initializer_list 
    SimpleVector(std::initializer_list<Type> init) noexcept {
        AssignByMove(init.begin(), init.end());
    }

    // Копирует значения вектора other в *this
    SimpleVector(const SimpleVector<Type>& other) noexcept {
        AssignByCopy(other.begin(), other.end());
    }

    // Инициализирует объект по rvalue ref, только зачем?
    SimpleVector(SimpleVector<Type>&& other) noexcept
        : size_(std::exchange(other.size_, 0)), 
        capacity_(std::exchange(other.capacity_, 0)),
        array_(std::move(other.array_)) {}

    // 
    explicit SimpleVector(ReserveProxyObj wrapper) {
        capacity_ = wrapper.capacity_to_reserve;
        Reserve(capacity_);
    }    

    // Меняет значения полей двух векторов
    void swap(SimpleVector<Type>& other) noexcept {
        array_.swap(other.array_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
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

    // Оператор присваивания
    SimpleVector& operator=(const SimpleVector& other) {
        if (this != &other) {
            auto temp_(other);
            swap(temp_);
        }
        return *this;
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        assert(index < size_);
        return array_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        assert(index < size_);
        return array_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (index < size_) {
            return array_[index];
        } else {
            throw std::out_of_range("Out of range"s);
        }
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if (index < size_) {
            return array_[index];
        } else {
            throw std::out_of_range("Out of range"s);
        }
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        size_ = 0;
    }

    Iterator Erase(Iterator position) {
        assert(position >= begin() && position <= end());
        std::move(std::next(position), end(), position);
        --size_;
        return position;
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
        if (new_size > capacity_){
            size_t new_capacity_ = std::max(capacity_ * 2, new_size);
            ResizeByMove(new_capacity_);
        } else if (new_size > size_) {
            std::generate(array_.Get() + size_, array_.Get() + new_size, [] {return Type();});
        }
        size_ = new_size;
    }

    // Вставляет элемент в конец массива и увеличивает вместимость, если требуется
    void PushBack(const Type& value) {
        size_t new_size = size_ + 1;
        if (new_size > capacity_) {
            size_t new_capacity_ = (capacity_ != 0) ? std::max(new_size, 2 * capacity_) : new_size;
            ResizeByMove(new_capacity_);
        }
        array_[size_] = value;
        ++size_;
    }

    // Вставляет по rvalue в конец массива
    void PushBack(Type&& value) {
        size_t new_size = size_ + 1;
        if (new_size > capacity_) {
            size_t new_capacity_ = (capacity_ != 0) ? std::max(new_size, 2 * capacity_) : new_size;
            ResizeByMove(new_capacity_);
        }
        array_[size_] = std::move(value);
        ++size_;
    }

    // Удаляет последний элемент
    void PopBack() noexcept {
        if (!IsEmpty()) {
            --size_;
        } 
    }

    void Reserve(size_t size) {
        if (size > capacity_) {
            ResizeByMove(size);
        }
    }

    //Вставляет позицию в произвольное место (копирование, доступ через ref)
    Iterator Insert(ConstIterator position, const Type& value) {
        assert(position >= begin() && position <= end());
        size_t position_offset = position - array_.Get();
        PushBack(value);
        std::rotate(array_.Get() + position_offset, end() - 1, end());
        return array_.Get() + position_offset;
    }

    //Вставляет позицию в произвольное место (перемещение, доступ через rvalue)
    Iterator Insert(ConstIterator position, Type&& value) {
        assert(position >= begin() && position <= end());
        size_t position_offset = position - array_.Get();
        PushBack(std::move(value));
        std::rotate(array_.Get() + position_offset, end() - 1, end());
        return array_.Get() + position_offset;
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        return size_ > 0 ? array_.Get() : nullptr;
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        return size_ > 0 ? &array_[size_] : nullptr;
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        return size_ > 0 ? array_.Get() : nullptr;
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        return size_ > 0 ? &array_[size_] : nullptr;
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        return size_ > 0 ? array_.Get() : nullptr;
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        return size_ > 0 ? &array_[size_] : nullptr;
    }

    void Print() {
        for (const Type& i : *this) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }
};

template <typename Type>
bool operator==(const SimpleVector<Type>& left, const SimpleVector<Type>& right) {
    if (&left == &right)
        return true;
    return left.GetSize() == right.GetSize() && std::equal(left.begin(), left.end(), right.begin());
}

template <typename Type>
bool operator!=(const SimpleVector<Type>& left, const SimpleVector<Type>& right) {
    return !(left == right);
}

template <typename Type>
bool operator<(const SimpleVector<Type>& left, const SimpleVector<Type>& right) {
    return std::lexicographical_compare(left.begin(), left.end(), right.begin(), right.end());
}

template <typename Type>
bool operator<=(const SimpleVector<Type>& left, const SimpleVector<Type>& right) {
    return (left < right) || (left == right);
}

template <typename Type>
bool operator>(const SimpleVector<Type>& left, const SimpleVector<Type>& right) {
    return right < left;
}

template <typename Type>
bool operator>=(const SimpleVector<Type>& left, const SimpleVector<Type>& right) {
    return (right < left) || (right == left);
}
