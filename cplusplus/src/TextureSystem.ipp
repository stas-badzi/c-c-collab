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