#include <iostream>
#include <algorithm>

template <typename ValueType>
class Array_ptr {
  private:
    ValueType* ptr_to_{nullptr};
  public:
    Array_ptr() = default;

    explicit Array_ptr(const size_t size) : ptr_to_(size > 0 ? new ValueType[size] : nullptr) {}

    explicit Array_ptr(ValueType* value_ptr_) noexcept : ptr_to_(value_ptr_) {}; 

    explicit Array_ptr(Array_ptr&& other) noexcept : ptr_to_(std::exchange(other.ptr_to_, nullptr)) {}

    Array_ptr(const Array_ptr&) = delete;

    Array_ptr& operator=(const Array_ptr&) = delete;

    Array_ptr& operator=(Array_ptr&& other) noexcept {
        if (this != &other)
            ptr_to_ = std::exchange(other.ptr_to_, nullptr);
        return *this;
    }

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

    void swap(Array_ptr& other) noexcept {
        std::swap(ptr_to_, other.ptr_to_);
    }

    ~Array_ptr() {
        delete[] ptr_to_;
    }

  };