#include <iostream>

using namespace std;

namespace ustc {

template<typename T>
class shared_ptr {
public:
    constexpr shared_ptr() noexcept : ptr_(nullptr), cnt_(nullptr) {}
    explicit shared_ptr(T *ptr) : ptr_(ptr) {
        if(ptr != nullptr) {
            cnt_ = new int(1);
        }
    }
    shared_ptr(const shared_ptr<T>& p) noexcept : ptr_(p.ptr_), cnt_(p.cnt_) {
        if(ptr_ != nullptr) {
            ++ (*cnt_);
        }
    }
    shared_ptr(shared_ptr<T>&& p) noexcept : ptr_(p.ptr_), cnt_(p.cnt_) {
        p.ptr_ = nullptr;
        p.cnt_ = nullptr;
    }
    ~shared_ptr() {
        if(ptr_ != nullptr) {
            -- (*cnt_);
            if(*cnt_ <= 0) {
                cout << "deletor" << endl;
                delete ptr_;
                delete cnt_;
            }
            ptr_ = nullptr;
            cnt_ = nullptr;
        }
    }

    T *get() const noexcept {
        return ptr_;
    }
    void reset(T *ptr = nullptr) {
        if(ptr_ == ptr) {
            return;
        }
        this->~shared_ptr();
        if(ptr != nullptr) {
            ptr_ = ptr;
            cnt_ = new int(1);
        }
    }
    void swap(shared_ptr<T>& p) noexcept {
        auto ptr = ptr_;
        auto cnt = cnt_;
        ptr_ = p.ptr_;
        cnt_ = p.cnt_;
        p.ptr_ = ptr;
        p.cnt_ = cnt;
    }
    int use_count() const noexcept {
        if(cnt_ == nullptr) {
            return 0;
        }
        return *cnt_;
    }
    bool unique() const noexcept {
        if(ptr_ == nullptr) {
            return false;
        }
        return *cnt_ == 1;
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

    shared_ptr& operator= (const shared_ptr<T>& p) noexcept {
        if(ptr_ == p.ptr_) {
            return *this;
        }
        this->~shared_ptr();
        if(p.ptr_ != nullptr) {
            ptr_ = p.ptr_;
            cnt_ = p.cnt_;
            ++ (*cnt_);
        }
        return *this;
    }
    shared_ptr& operator= (shared_ptr<T>&& p) noexcept {
        if(ptr_ == p.ptr_) {
            return *this;
        }
        this->~shared_ptr();
        ptr_ = p.ptr_;
        cnt_ = p.cnt_;
        p.ptr_ = nullptr;
        p.cnt_ = nullptr;
        return *this;
    }

private:
    T *ptr_;
    int *cnt_;

};

} // namespace ustc

int main() {
    int *a = new int(10);
    ustc::shared_ptr<int> p(a);
    ustc::shared_ptr<int> q;
    q = p;
    cout << q.use_count() << endl;
    ustc::shared_ptr<int> s;
    q = s;
    cout << p.use_count() << endl;
    return 0;
}
