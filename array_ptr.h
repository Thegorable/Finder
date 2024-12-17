#include <cassert>
#include <cstdlib>
#include<algorithm>

template <typename Type>
class ArrayPtr {
public:

    ArrayPtr() = default;

    explicit ArrayPtr(int8_t s) : size_(s) {
        if (s) {
            raw_ptr_ = new Type[s];
        }
    }

    explicit ArrayPtr(Type* raw_ptr) noexcept : raw_ptr_(raw_ptr) {}

    ArrayPtr(const ArrayPtr&) = delete;
    ArrayPtr& operator=(const ArrayPtr&) = delete;
    
    ArrayPtr(ArrayPtr&& other) : size_(0), raw_ptr_(nullptr) {
        if (raw_ptr_ != other.raw_ptr_) {
            this->swap(other);
        }
    }

    ArrayPtr& operator =(ArrayPtr&& other) noexcept {
        if (raw_ptr_ != other.raw_ptr_) {
            this->swap(other);
            delete[] other.raw_ptr_;
            other.raw_ptr_ = nullptr;
            other.size_ = 0;
        }

        return *this;
    }

    Type* Release() noexcept {
        Type* temp = raw_ptr_;
        raw_ptr_ = nullptr;
        size_ = 0;
        return temp;
    }

    Type& operator[](size_t index) noexcept {
        return raw_ptr_[index];
    }

    const Type& operator[](size_t index) const noexcept {
        return raw_ptr_[index];
    }

    explicit operator bool() const {
        return (bool)size_;
    }

    Type* Get() const noexcept {
        return raw_ptr_;
    }

    inline uint8_t size() const noexcept;

    void swap(ArrayPtr& other) noexcept {
        std::swap(other.raw_ptr_, raw_ptr_);
        std::swap(other.size_, this->size_);
    }

    ~ArrayPtr() {
        delete[] raw_ptr_;
        raw_ptr_ = nullptr;
    }

private:
    Type* raw_ptr_ = nullptr;
    uint8_t size_ = 0;
};

template<typename Type>
inline uint8_t ArrayPtr<Type>::size() const noexcept {
    return this->size_;
}
