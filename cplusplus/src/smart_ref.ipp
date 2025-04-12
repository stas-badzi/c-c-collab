#ifndef SMART_REF_HPP
#error "smart_ref.ipp must be included from smart_ref.hpp"
#else

namespace {
    template<typename T> smart_ref<T>::smart_ref(T* pointer) : alloc(0), ref(*pointer) {}
    template<typename T> smart_ref<T>::smart_ref(T& lvalue) : alloc(0), ref(lvalue) {}
    template<typename T> smart_ref<T>::smart_ref(T&& rvalue) : alloc(1), ref(*(new T(rvalue))) {}
    template <typename T> smart_ref<T>::smart_ref(const smart_ref& l_ref) : alloc(l_ref.alloc), ref(alloc ? *(new T(l_ref.ref)) : l_ref.ref) {}
    template <typename T> smart_ref<T>::smart_ref(smart_ref&& r_ref) : alloc(r_ref.alloc), ref(r_ref.ref) {
        r_ref.alloc = false;
    }
    template <typename T> smart_ref<T> &smart_ref<T>::operator=(const smart_ref<T>& lother) {
        if (this == &lother) return *this;
        if (this->alloc) delete &(this->ref);
        if ((this->alloc = lother.alloc)) this->ref = new T(lother.ref);
        else this->ref = lother.ref;
        return *this;
    }
    template <typename T> smart_ref<T> &smart_ref<T>::operator=(smart_ref<T>&& rother) noexcept {
        if (this == &rother) return *this;
        if (this->alloc) delete &(this->ref);
        this->alloc = rother.alloc;
        this->ref = rother.ref;
        rother.alloc = false;
        rother.ref = nullptr;
        return *this;
    }
    template <typename T> smart_ref<T>::operator T() const { return this->ref; }
    template <typename T> smart_ref<T>::~smart_ref() {if (alloc) delete &ref;}
}
#endif // SMART_REF_HPP