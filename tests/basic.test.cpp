#include <catch2/catch_test_macros.hpp>
#include <flags/flags.hpp>

enum class my_enum
{
    a = 1 << 0,
    b = 1 << 1,
    c = 1 << 2,
};

template <>
constexpr bool flags::enabled<my_enum> = true;

TEST_CASE("Basic bit-wise operations are tested", "[Basic]")
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

    flag &= ~my_enum::a;

    REQUIRE(not(flag & my_enum::a));
    REQUIRE(not(flag & my_enum::b));
    REQUIRE(not(flag & my_enum::c));
}

enum class my_enum_2
{
    a = 1 << 0,
    b = 1 << 1,
    c = 1 << 2,
};

template <>
constexpr bool flags::enabled<my_enum_2> = true;
template <>
constexpr bool flags::no_underlying<my_enum_2> = true;

TEST_CASE("Basic bit-wise operations are tested [no_underlying]", "[Basic-No-Underlying]")
{
    auto flag = my_enum_2::a | my_enum_2::b;
    flag |= my_enum::c;

    REQUIRE((flag & my_enum_2::a) == my_enum_2::a);
    REQUIRE((flag & my_enum_2::b) == my_enum_2::b);
    REQUIRE((flag & my_enum_2::c) == my_enum_2::c);

    flag &= ~my_enum_2::c;

    REQUIRE((flag & my_enum_2::a) == my_enum_2::a);
    REQUIRE((flag & my_enum_2::b) == my_enum_2::b);
    REQUIRE((flag & my_enum_2::c) != my_enum_2::c);

    flag &= ~my_enum_2::b;

    REQUIRE((flag & my_enum_2::a) == my_enum_2::a);
    REQUIRE((flag & my_enum_2::b) != my_enum_2::b);
    REQUIRE((flag & my_enum_2::c) != my_enum_2::c);

    flag &= ~my_enum_2::a;

    REQUIRE((flag & my_enum_2::a) != my_enum_2::a);
    REQUIRE((flag & my_enum_2::b) != my_enum_2::b);
    REQUIRE((flag & my_enum_2::c) != my_enum_2::c);
}