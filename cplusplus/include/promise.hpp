#include <mutex>
#include <optional>
#include <memory>

namespace stsb {
    template<typename T>
    class promise {
    private:
        std::shared_ptr<T> storage;
        std::shared_ptr<bool> ready;
        mutable std::mutex mtx;
    public:
        inline promise(T& storage) : storage(std::make_shared<T>(storage)), ready(new bool(false)), mtx() {}
        inline promise(void) : storage(new T()), ready(new bool(false)), mtx() {}
        inline promise(const promise& cp) : storage(cp.storage), ready(cp.ready), mtx() {}
        inline promise(promise&& cp) : storage(cp.storage), ready(cp.ready), mtx() {}
        inline promise<T>& operator=(const promise<T>& cp) { storage = cp.storage; ready = cp.ready; return *this; }
        inline promise<T>& operator=(promise<T>&& cp) { storage = cp.storage; ready = cp.ready; return *this; }
        inline promise<T>& operator=(const T& value) { mtx.lock(); *(storage.get()) = value; mtx.unlock(); *(ready.get()) = true; return *this; }
        inline promise<T>& set(const T& value) { mtx.lock(); *(storage.get()) = value; mtx.unlock(); *(ready.get()) = true; return *this; }
        inline std::optional<T> operator()() const { mtx.lock(); if (!(*(ready.get()))) { mtx.unlock(); return std::nullopt; } auto tmp = *(storage.get()); mtx.unlock(); return tmp; }
        inline std::optional<T> get() const { mtx.lock(); if (!(*(ready.get()))) { mtx.unlock(); return std::nullopt; } auto tmp = *(storage.get()); mtx.unlock(); return tmp; }
        inline ~promise() {}
    };
}