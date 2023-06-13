#pragma once
#include <ostream>
#include <concepts>
#include <type_traits>

namespace flags
{
    template <class T>
    concept scoped_enum = requires() {
        requires std::is_enum_v<T>;
        requires not std::is_convertible_v<T, std::underlying_type_t<T>>;
    };

    template <typename T>
    concept stream = requires(T &value) {
        []<typename Char, typename Traits>(std::basic_ios<Char, Traits> &) {
        }(value);
    };

    template <class T>
        requires scoped_enum<T>
    static constexpr bool enabled = false;

    template <class T>
        requires enabled<T>
    static constexpr bool no_underlying = false;

    template <class T>
    concept allowed = requires() {
        requires enabled<T>;
        requires scoped_enum<T>;
        requires std::integral<std::underlying_type_t<T>>;
    };

    template <typename T>
    constexpr auto underlying(T value)
    {
        if constexpr (scoped_enum<T>)
        {
            return static_cast<std::underlying_type_t<T>>(value);
        }
        else
        {
            return value;
        }
    }

    template <typename Enum, typename T>
    constexpr auto cast(T value)
    {
        if constexpr (no_underlying<Enum>)
        {
            return static_cast<Enum>(value);
        }
        else
        {
            return value;
        }
    }
} // namespace flags

template <typename T>
    requires flags::allowed<T>
constexpr auto operator~(T value)
{
    return flags::cast<T>(~flags::underlying(value));
}

template <typename L, typename R>
    requires flags::allowed<L> or flags::allowed<R>
constexpr auto operator&(L left, R right)
{
    using enum_t = std::conditional_t<flags::allowed<L>, L, R>;
    return flags::cast<enum_t>(flags::underlying(left) & flags::underlying(right));
}

template <typename L, typename R>
    requires flags::allowed<L> or flags::allowed<R>
constexpr auto operator|(L left, R right)
{
    using enum_t = std::conditional_t<flags::allowed<L>, L, R>;
    return flags::cast<enum_t>(flags::underlying(left) | flags::underlying(right));
}

template <typename L, typename R>
    requires flags::allowed<L> or flags::allowed<R>
constexpr auto operator^(L left, R right)
{
    using enum_t = std::conditional_t<flags::allowed<L>, L, R>;
    return flags::cast<enum_t>(flags::underlying(left) ^ flags::underlying(right));
}

template <typename L, typename R>
    requires(not flags::stream<L>) and (flags::allowed<L> or flags::allowed<R>)
constexpr auto operator<<(L left, R right)
{
    using enum_t = std::conditional_t<flags::allowed<L>, L, R>;
    return flags::cast<enum_t>(flags::underlying(left) << flags::underlying(right));
}

template <typename L, typename R>
    requires flags::allowed<L> or flags::allowed<R>
constexpr auto operator>>(L left, R right)
{
    using enum_t = std::conditional_t<flags::allowed<L>, L, R>;
    return flags::cast<enum_t>(flags::underlying(left) >> flags::underlying(right));
}

template <typename L, typename R>
    requires flags::allowed<L> or flags::allowed<R>
constexpr auto &operator&=(L &left, R right)
{
    using enum_t = std::conditional_t<flags::allowed<L>, L, R>;
    return left = flags::cast<enum_t>(left & right);
}

template <typename L, typename R>
    requires flags::allowed<L> or flags::allowed<R>
constexpr auto &operator|=(L &left, R right)
{
    using enum_t = std::conditional_t<flags::allowed<L>, L, R>;
    return left = flags::cast<enum_t>(left | right);
}

template <typename L, typename R>
    requires flags::allowed<L> or flags::allowed<R>
constexpr auto &operator^=(L &left, R right)
{
    using enum_t = std::conditional_t<flags::allowed<L>, L, R>;
    return left = flags::cast<enum_t>(left ^ right);
}