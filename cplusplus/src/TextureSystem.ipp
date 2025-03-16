#include "TextureSystem.hpp"

template <typename Tout, typename Tin>
std::vector<Tout> cs::ConvertVector(const std::vector<Tin> &vec) {
    std::vector<Tout> out;
    for (auto &&t : vec) out.push_back((Tout)t);
    return out;
}

template <typename Tout, typename Tin>
std::vector<std::vector<Tout>> cs::Convert2dVector(const std::vector<std::vector<Tin>> &vec2d) {
    std::vector<std::vector<Tout>> out;
    for (auto &&vt : vec2d) out.push_back(ConvertVector<Tout,Tin>(vt));
    return out;
}

namespace cs {
    inline std::vector<std::vector<smart_ref<Console::Symbol> > > PtrToTexture(void* ptr, bool direct = false) {
        auto sym = std::vector<std::vector<smart_ref<Console::Symbol> > >();

        const int int32_size = sizeof(int32_t);
        const int intptr_size = sizeof(void*);

        void* now_ptr;

        int32_t height = System::ReadPointer<int32_t>(ptr);
        now_ptr = System::MovePointer(ptr, int32_size);

        for (int32_t i = 0; i < height; i++) {
            int32_t width = System::ReadPointer<int32_t>(now_ptr);
            now_ptr = System::MovePointer(now_ptr, int32_size);
            std::vector<smart_ref<Console::Symbol> > now;

            for (int32_t j = 0; j < width; j++) {
                Console::Symbol* sym = (Console::Symbol*)System::ReadPointer<nint>(now_ptr);
                if (direct) now.push_back(smart_ref(sym));
                else now.push_back(smart_ref(Console::Symbol(*sym)));
                now_ptr = System::MovePointer(now_ptr, intptr_size);
            }

            if (direct) sym.push_back(now);
            else sym.push_back(now);
        }

        System::FreeMemory(ptr);
        
        return sym;
    }

    inline void* TextureToPtr(std::vector<std::vector<Console::Symbol> > &texture) {
        const int int32_size = sizeof(int32_t);
        const int intptr_size = sizeof(void*);
        int32_t size, count;

        size = texture.size();
        count = 0;
        for (int32_t i = 0; i < size; i++) {
            count += texture[i].size();
        }

        void* ret = System::AllocateMemory((size + 1) * int32_size + count * intptr_size);

        count = 0;
        void* where;
        System::WritePointer<int32_t>(ret, size);
        where = System::MovePointer(ret, int32_size);
        for (int32_t i = 0; i < size; i++) {
            System::WritePointer<int32_t>(where,texture[i].size());
            where = System::MovePointer(where, int32_size);
            for (size_t j = 0; j < texture[i].size(); j++) {
                System::WritePointer<nint>(where, &texture[i][j]);
                where = System::MovePointer(where, intptr_size);
            }
        }

        return ret;
    }
}