#ifndef SMART_REF_HPP
#define SMART_REF_HPP

namespace {
    template<typename T>
    class smart_ref {
    private:
        bool alloc;
        T& ref;
    public:
        smart_ref(T* pointer);
        smart_ref(T& lvalue);
        smart_ref(T&& rvalue);
        smart_ref(const smart_ref& l_ref);
        smart_ref(smart_ref&& r_ref);
        smart_ref<T>& operator=(const smart_ref<T>& lother);
        smart_ref<T>& operator=(smart_ref<T>&& rother) noexcept;
        explicit operator T() const;
        T* ptr(void);
        ~smart_ref();
    };

}

#include "smart_ref.ipp"
#endif // SMART_REF_HPP