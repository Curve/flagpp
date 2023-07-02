#include <catch2/catch_test_macros.hpp>
#include <flags/flags.hpp>

enum class my_enum
{
    none,
    a = 1 << 0,
    b = 1 << 1,
    c = 1 << 2,
};

enum class my_enum_2
{
    none,
    a = 1 << 0,
    b = 1 << 1,
    c = 1 << 2,
};

template <>
constexpr bool flags::enabled<my_enum> = true;

template <>
constexpr bool flags::enabled<my_enum_2> = true;

TEST_CASE("Basic bit-wise operations are tested [plain]", "[plain]")
{
    auto flag = my_enum::a;
    flag |= my_enum::b;
    flag |= my_enum::c;

    REQUIRE(flag & my_enum::a);
    REQUIRE(flag & my_enum::b);
    REQUIRE(flag & my_enum::c);

    flag &= ~my_enum::c;

    REQUIRE(flag & my_enum::a);
    REQUIRE(flag & my_enum::b);
    REQUIRE(not(flag & my_enum::c));

    flag &= ~my_enum::b;

    REQUIRE(flag & my_enum::a);
    REQUIRE(not(flag & my_enum::b));
    REQUIRE(not(flag & my_enum::c));

    flag = flag & ~my_enum::a;

    REQUIRE(not(flag & my_enum::a));
    REQUIRE(not(flag & my_enum::b));
    REQUIRE(not(flag & my_enum::c));

    flag |= my_enum::a | my_enum::b | my_enum::c;

    REQUIRE(flag & my_enum::a);
    REQUIRE(flag & my_enum::b);
    REQUIRE(flag & my_enum::c);

    flag = my_enum::none | my_enum_2::a;
    flag |= my_enum_2::b | my_enum_2::c;

    REQUIRE(flag & my_enum::a);
    REQUIRE(flag & my_enum::b);
    REQUIRE(flag & my_enum::c);
}

TEST_CASE("Basic bit-wise operations are tested [wrapped]", "[wrapped]")
{
    auto flag = my_enum::a | my_enum::b;
    flag |= my_enum::c;

    REQUIRE(flag & my_enum::a);
    REQUIRE(flag & my_enum::b);
    REQUIRE(flag & my_enum::c);

    flag &= ~my_enum::c;

    REQUIRE(flag & my_enum::a);
    REQUIRE(flag & my_enum::b);
    REQUIRE(not(flag & my_enum::c));

    flag &= ~my_enum::b;

    REQUIRE(flag & my_enum::a);
    REQUIRE(not(flag & my_enum::b));
    REQUIRE(not(flag & my_enum::c));

    flag = flag & ~my_enum::a;

    REQUIRE(not(flag & my_enum::a));
    REQUIRE(not(flag & my_enum::b));
    REQUIRE(not(flag & my_enum::c));

    flag |= my_enum::a | my_enum::b | my_enum::c;

    REQUIRE(flag & my_enum::a);
    REQUIRE(flag & my_enum::b);
    REQUIRE(flag & my_enum::c);

    flag = my_enum::none | my_enum_2::a;
    flag |= my_enum_2::b | my_enum_2::c;

    REQUIRE(flag & my_enum::a);
    REQUIRE(flag & my_enum::b);
    REQUIRE(flag & my_enum::c);
}

enum class my_enum_3
{
    none,
    a = 1 << 0,
    b = 1 << 1,
    c = 1 << 2,
};

template <>
constexpr bool flags::enabled<my_enum_3> = true;

template <>
constexpr bool flags::strict<my_enum_3> = true;

template <auto Left, auto Right>
concept check_bit_or = requires() { Left | Right; };

template <auto Left, auto Right>
concept check_bit_or_assign = requires() { Left |= Right; };

template <typename Left, auto Right>
concept check_bit_or_assign_wrapper = requires(flags::enum_wrapper<Left> left) { left |= Right; };

template <typename Left, typename Right>
concept check_bit_or_assign_wrapper_edge =
    requires(flags::enum_wrapper<Left> left, flags::enum_wrapper<Right> right) { left |= right; };

TEST_CASE("Check if strict works properly", "[struct]")
{
    REQUIRE(check_bit_or<my_enum_3::a, my_enum_3::b>);

    REQUIRE(not check_bit_or<my_enum_3::a, my_enum::b>);
    REQUIRE(not check_bit_or<my_enum::a, my_enum_3::b>);

    REQUIRE(not check_bit_or_assign<my_enum::a, my_enum_3::b>);
    REQUIRE(not check_bit_or_assign<my_enum_3::a, my_enum::b>);

    REQUIRE(not check_bit_or_assign_wrapper<my_enum, my_enum_3::b>);
    REQUIRE(not check_bit_or_assign_wrapper<my_enum_3, my_enum::b>);

    REQUIRE(not check_bit_or_assign_wrapper_edge<my_enum, my_enum_3>);
    REQUIRE(not check_bit_or_assign_wrapper_edge<my_enum_3, my_enum>);
}

enum class my_enum_4
{
    none,
    a = 1 << 0,
    b = 1 << 1,
    c = 1 << 2,
};

template <auto Left, auto Right>
concept would_be_valid = requires() { Left | Right; };

TEST_CASE("Check compile time constraints", "[constraints]")
{
    REQUIRE(would_be_valid<my_enum::a, my_enum::b>);
    REQUIRE(not would_be_valid<my_enum_4::a, my_enum_4::b>);
}