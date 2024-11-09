#pragma once

#include <windows.h>
#include <limits>

template<typename EnumType, EnumType... Values>
class EnumCheck;

template<typename EnumType>
class EnumCheck<EnumType>
{
public:
    static bool constexpr ContainsValue(std::underlying_type_t<EnumType>) {
    static_assert(std::is_enum_v<EnumType>);
        return false;
    }
    static EnumType constexpr ToEnum(std::underlying_type_t<EnumType>) {
    static_assert(std::is_enum_v<EnumType>);
        return static_cast<EnumType>(std::numeric_limits<std::underlying_type_t<EnumType>>::max());
    }
};

template<typename EnumType, EnumType V, EnumType... Next>
class EnumCheck<EnumType, V, Next...> : private EnumCheck<EnumType, Next...> {
    using super = EnumCheck<EnumType, Next...>;
public:
    static bool constexpr ContainsValue(std::underlying_type_t<EnumType> v) {
        static_assert(std::is_enum_v<EnumType>);
        return (v == static_cast<std::underlying_type_t<EnumType>>(V) || super::ContainsValue(v));
    }
    static EnumType constexpr ToEnum(std::underlying_type_t<EnumType> v) {
        static_assert(std::is_enum_v<EnumType>);
        return (v == static_cast<std::underlying_type_t<EnumType>>(V)) ? static_cast<EnumType>(v) : super::ToEnum(v);
    }
};

struct ToggledKeys {
    bool CapsLock, NumLock, ScrollLock;
};