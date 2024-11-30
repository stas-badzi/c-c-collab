#pragma once

template<typename EnumType, EnumType UNDEFINED, EnumType... Values>
class EnumCheck;

template<typename EnumType, EnumType UNDEFINED>
class EnumCheck<EnumType, UNDEFINED>
{
public:
    static bool constexpr ContainsValue(std::underlying_type_t<EnumType>) {
    static_assert(std::is_enum_v<EnumType>);
        return false;
    }
    static EnumType constexpr ToEnum(std::underlying_type_t<EnumType>) {
    static_assert(std::is_enum_v<EnumType>);
        return UNDEFINED;
    }
};

template<typename EnumType, EnumType UNDEFINED, EnumType V, EnumType... Next>
class EnumCheck<EnumType, UNDEFINED, V, Next...> : private EnumCheck<EnumType, UNDEFINED, Next...> {
    using super = EnumCheck<EnumType, UNDEFINED, Next...>;
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

class Key {
public:
    enum class Enum : unsigned short {
        NONE = 0xffff, /* only in KeyPressed when no key is pressed */

        NUL = 0, /* ␀ */
        };
    using Class = EnumCheck<Enum, Enum::NUL, Enum::NONE, Enum::NUL>;
};