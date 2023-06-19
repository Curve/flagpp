#pragma once
#include "flags.hpp"

namespace flags
{
    template <typename T>
    concept strict_wrapper = requires(T value) {
        []<typename O>(enum_wrapper<O> &)
            requires strict<O>
        {
        }(value);
    };

    template <typename Enum, typename T>
    consteval auto valid_argument()
    {
        if constexpr (not castable_to<T, std::underlying_type_t<Enum>>)
        {
            return false;
        }

        constexpr auto is_strict_wrapper = requires { requires strict_wrapper<T>; };
        constexpr auto is_strict_type = requires { requires strict<T>; };
        constexpr auto self_strict = requires { requires strict<Enum>; };

        if constexpr (self_strict or is_strict_type or is_strict_wrapper)
        {
            constexpr auto is_wrapper_of = requires { requires std::same_as<enum_wrapper<Enum>, T>; };
            return std::same_as<T, Enum> or is_wrapper_of;
        }

        return true;
    }

    template <typename Enum, typename T>
    concept valid = requires() { requires valid_argument<Enum, T>(); };

    template <class T>
        requires scoped_enum<T>
    class enum_wrapper
    {
        using underlying_t = std::underlying_type_t<T>;

      private:
        T m_value;

      public:
        constexpr enum_wrapper(T value) : m_value(value) {}
        constexpr enum_wrapper(underlying_t value) : m_value(static_cast<T>(value)) {}

      public:
        constexpr auto underlying() const
        {
            return static_cast<underlying_t>(m_value);
        }

        constexpr auto value() const
        {
            return m_value;
        }

      public:
        constexpr operator underlying_t() const
        {
            return underlying();
        }

        constexpr operator T() const
        {
            return value();
        }

      public:
        template <typename O>
            requires valid<T, O>
        constexpr auto operator==(O other) const
        {
            return underlying() == static_cast<underlying_t>(other);
        }

      public:
        template <typename O>
            requires valid<T, O>
        constexpr enum_wrapper operator&(O other) const
        {
            return underlying() & static_cast<underlying_t>(other);
        }

        template <typename O>
            requires valid<T, O>
        constexpr enum_wrapper operator|(O other) const
        {
            return underlying() | static_cast<underlying_t>(other);
        }

        template <typename O>
            requires valid<T, O>
        constexpr enum_wrapper operator^(O other) const
        {
            return underlying() ^ static_cast<underlying_t>(other);
        }

        template <typename O>
            requires valid<T, O>
        constexpr enum_wrapper operator<<(O other) const
        {
            return underlying() << static_cast<underlying_t>(other);
        }

        template <typename O>
            requires valid<T, O>
        constexpr enum_wrapper operator>>(O other) const
        {
            return underlying() >> static_cast<underlying_t>(other);
        }

      public:
        constexpr enum_wrapper operator~() const
        {
            return ~underlying();
        }

      public:
        template <typename O>
            requires valid<T, O>
        constexpr enum_wrapper &operator&=(O other)
        {
            m_value = (*this) & other;
            return *this;
        }

        template <typename O>
            requires valid<T, O>
        constexpr enum_wrapper &operator|=(O other)
        {
            m_value = (*this) | other;
            return *this;
        }

        template <typename O>
            requires valid<T, O>
        constexpr enum_wrapper &operator^=(O other)
        {
            m_value = (*this) ^ other;
            return *this;
        }
    };
} // namespace flags