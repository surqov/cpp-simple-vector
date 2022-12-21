#include "simple_vector.h"

#include <iostream>

template <typename ValueType>
class Array_ptr {
  private:
	friend class SimpleVector;
    ValueType* ptr_to_ = nullptr;
  public:
    using iterator_category = std::forward_iterator_tag ; // Категория итератора — forward iterator
    using value_type = ValueType; // Тип элементов, по которым перемещается итератор
    using difference_type = std::ptrdiff_t; // Тип, используемый для хранения смещения между итераторами
    using pointer = ValueType*; // Тип указателя на итерируемое значение
    using reference = ValueType&; // Тип ссылки на итерируемое значение 

Array_ptr() = default;

Array_ptr(const Array_ptr<ValueType>& other) noexcept : ptr_to_(other.ptr_to_) { }

Array_ptr& operator=(const Array_ptr& rhs) = default;

[[nodiscard]] bool operator==(const Array_ptr<const ValueType>& rhs) const noexcept {
    return ptr_to_ == rhs.ptr_to_;
}

[[nodiscard]] bool operator!=(const Array_ptr<const ValueType>& rhs) const noexcept {
    return !(*this == rhs);
}

[[nodiscard]] bool operator==(const Array_ptr<ValueType>& rhs) const noexcept {
    return ptr_to_ == rhs.ptr_to_;
}

[[nodiscard]] bool operator!=(const Array_ptr<ValueType>& rhs) const noexcept {
    return !(*this == rhs);
}

Array_ptr& operator++() noexcept {
    ++ptr_to_;
    return *this;
}

Array_ptr operator++(int) noexcept {
    Array_ptr temp(*this);
    ++(*this);
    return temp;
}

[[nodiscard]] reference operator*() const noexcept {
    return *ptr_to_;
}

[[nodiscard]] pointer operator->() const noexcept {
    return &ptr_to_;
}
  };