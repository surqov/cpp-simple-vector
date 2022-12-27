#include <iostream>
#include <algorithm>

template <typename ValueType>
class ArrayPtr {
  private:
    ValueType* ptr_to_{nullptr};
  public:
    ArrayPtr() = default;

    explicit ArrayPtr(const size_t size) : ptr_to_(size > 0 ? new ValueType[size] : nullptr) {
        std::generate(ptr_to_, ptr_to_ + size, 
        [] { return ValueType(); }
        );
    }

    explicit ArrayPtr(ValueType* value_ptr_) noexcept : ptr_to_(value_ptr_) {}; 

    explicit ArrayPtr(ArrayPtr&& other) noexcept : ptr_to_(std::exchange(other.ptr_to_, nullptr)) {}

    ArrayPtr(ArrayPtr&) = delete;

    ArrayPtr(const ArrayPtr&) = delete;

    ArrayPtr& operator=(ArrayPtr&& other) noexcept {
        if (this != &other)
            ptr_to_ = std::swap(other.ptr_to_, nullptr);
        return *this;
    }

    ArrayPtr& operator++() noexcept {
        ++ptr_to_;
        return *this;
    }

    ArrayPtr operator++(int) noexcept {
        ArrayPtr temp(*this);
        ++(*this);
        return temp;
    }

    ValueType* Release() noexcept {
        ValueType* tmp = ptr_to_;
        ptr_to_ = nullptr;
        return tmp;
    }

    ValueType& operator[](size_t index) noexcept {
        return *(ptr_to_ + index);
    }

    const ValueType& operator[](size_t index) const noexcept {
        return *(ptr_to_ + index);
    }

    explicit operator bool() const {
        return ptr_to_;
    }

    ValueType* Get() const noexcept {
        return ptr_to_;
    }

    ValueType& operator*() const noexcept {
        return *ptr_to_;
    }

    void swap(ArrayPtr& other) noexcept {
        std::swap(ptr_to_, other.ptr_to_);
    }

    ~ArrayPtr() {
        delete[] ptr_to_;
    }

  };

template <typename ValueType>
bool operator==(const ArrayPtr<const ValueType>& lhs, const ArrayPtr<const ValueType>& rhs) noexcept {
        return lhs.ptr_to_ == rhs.ptr_to_;
}

template <typename ValueType>
bool operator!=(const ArrayPtr<const ValueType>& lhs, const ArrayPtr<const ValueType>& rhs) noexcept {
        return !(lhs == rhs);
}

template <typename ValueType>
bool operator==(const ArrayPtr<ValueType>& lhs, const ArrayPtr<ValueType>& rhs) noexcept {
        return lhs.ptr_to_ == rhs.ptr_to_;
}

template <typename ValueType>
bool operator!=(const ArrayPtr<ValueType>& lhs, const ArrayPtr<ValueType>& rhs) noexcept {
        return !(lhs == rhs);
}

