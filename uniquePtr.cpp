#include <iostream>

using namespace std;

namespace ustc {

template<typename T>
class unique_ptr {
public:
    constexpr unique_ptr() noexcept : ptr_(nullptr) {}
    explicit unique_ptr(T *ptr) noexcept : ptr_(ptr) {}

    unique_ptr(const unique_ptr<T> &) = delete;
    unique_ptr(unique_ptr<T> && p) noexcept : ptr_(p.ptr_) {
        p.ptr_ = nullptr;
    }

    unique_ptr& operator= (const unique_ptr<T>&) = delete;
    unique_ptr& operator= (unique_ptr<T>&& p) noexcept {
        reset(p.release());
        return *this;
    }

    T *release() noexcept {
        T *ret = ptr_;
        ptr_ = nullptr;
        return ret;
    }
    void reset(T *ptr = nullptr) noexcept {
        if(ptr_ != ptr) {
            delete ptr_;
            ptr_ = ptr;
        }
    }
    void swap(unique_ptr<T>& p) noexcept {
        T *tmp = ptr_;
        ptr_ = p.ptr_;
        p.ptr_ = tmp;
    }
    T *get() const noexcept {
        return ptr_;
    }
    
    T * operator -> () const noexcept {
        return ptr_;
    }
    T & operator * () const noexcept {
        return *ptr_;
    }
    explicit operator bool() const noexcept {
        return ptr_ != nullptr;
    }
    ~unique_ptr() {
        if(ptr_ != nullptr) {
            delete ptr_;
            ptr_ = nullptr;
        }
    }

private:
    T *ptr_;

};

} // namespace ustc

int main() {
    int *a = new int(10);
    ustc::unique_ptr<int> p(a);
    ustc::unique_ptr<int> q(std::move(p));
    cout << *q << endl;
    q.swap(p);
    cout << *p << endl;
    return 0;
}
