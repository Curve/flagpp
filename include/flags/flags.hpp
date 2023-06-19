#pragma once
#include <concepts>

namespace flags
{
    template <typename From, typename To>
    concept castable_to = requires(From from) { static_cast<To>(from); };

    template <class T>
    concept scoped_enum = requires() {
        requires std::is_enum_v<T>;
        requires not std::convertible_to<T, std::underlying_type_t<T>>;
    };

    template <class T>
        requires scoped_enum<T>
    class enum_wrapper;

    template <class T>
    concept is_enum_wrapper = requires(T value) {
        []<typename O>(enum_wrapper<O> &) {
        }(value);
    };

    template <class T>
        requires scoped_enum<T>
    static constexpr bool enabled = false;

    template <class T>
        requires enabled<T>
    static constexpr bool strict = false;

    template <typename L, typename R>
    consteval auto is_allowed()
    {
        constexpr auto enabled_l = requires { requires enabled<L>; };
        constexpr auto enabled_r = requires { requires enabled<R>; };

        if constexpr (not(enabled_l or std::integral<L>))
        {
            return false;
        }

        if constexpr (not(enabled_r or std::integral<R> or is_enum_wrapper<R>))
        {
            return false;
        }

        constexpr auto strict_l = requires { requires strict<L>; };
        constexpr auto strict_r = requires { requires strict<R>; };

        if constexpr (strict_l or strict_r)
        {
            constexpr auto is_wrapper_of = requires { requires std::same_as<enum_wrapper<L>, R>; };
            return std::same_as<L, R> or is_wrapper_of;
        }

        return true;
    }

    template <typename L, typename R>
    concept allowed = requires() { requires is_allowed<L, R>(); };
} // namespace flags

template <typename L, typename R>
    requires flags::allowed<L, R>
constexpr auto operator&(L left, R right)
{
    return flags::enum_wrapper{left} & flags::enum_wrapper{right};
}

template <typename L, typename R>
    requires flags::allowed<L, R>
constexpr auto operator|(L left, R right)
{
    return flags::enum_wrapper{left} | flags::enum_wrapper{right};
}

template <typename L, typename R>
    requires flags::allowed<L, R>
constexpr auto operator^(L left, R right)
{
    return flags::enum_wrapper{left} ^ flags::enum_wrapper{right};
}

template <typename L, typename R>
    requires flags::allowed<L, R>
constexpr auto operator<<(L left, R right)
{
    return flags::enum_wrapper{left} << flags::enum_wrapper{right};
}

template <typename L, typename R>
    requires flags::allowed<L, R>
constexpr auto operator>>(L left, R right)
{
    return flags::enum_wrapper{left} >> flags::enum_wrapper{right};
}

template <class Enum>
    requires flags::enabled<Enum>
constexpr auto operator~(Enum value)
{
    return ~flags::enum_wrapper{value};
}

template <typename L, typename R>
    requires flags::allowed<L, R>
constexpr auto &operator&=(L &left, R right)
{
    left = flags::enum_wrapper{left} & flags::enum_wrapper{right};
    return left;
}

template <typename L, typename R>
    requires flags::allowed<L, R>
constexpr auto &operator|=(L &left, R right)
{
    left = flags::enum_wrapper{left} | flags::enum_wrapper{right};
    return left;
}

template <typename L, typename R>
    requires flags::allowed<L, R>
constexpr auto &operator^=(L &left, R right)
{
    left = flags::enum_wrapper{left} ^ flags::enum_wrapper{right};
    return left;
}

#include "flags.inl"