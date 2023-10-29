#include <boost/ut.hpp>
#include <flagpp/flags.hpp>

using namespace boost::ut;
using namespace boost::ut::bdd;

enum class my_enum
{
    none,
    a = 1 << 0,
    b = 1 << 1,
    c = 1 << 2,
};

template <>
constexpr bool flagpp::enabled<my_enum> = true;

// NOLINTNEXTLINE
suite<"basic"> basic_suite = []()
{
    auto flag = my_enum::a;
    flag |= my_enum::b;
    flag |= my_enum::c;

    expect(flag & my_enum::a);
    expect(flag & my_enum::b);
    expect(flag & my_enum::c);

    flag &= ~my_enum::c;

    expect(flag & my_enum::a);
    expect(flag & my_enum::b);
    expect(not(flag & my_enum::c));

    flag &= ~my_enum::b;

    expect(flag & my_enum::a);
    expect(not(flag & my_enum::b));
    expect(not(flag & my_enum::c));

    flag = flag & ~my_enum::a;

    expect(not(flag & my_enum::a));
    expect(not(flag & my_enum::b));
    expect(not(flag & my_enum::c));

    flag |= my_enum::a | my_enum::b | my_enum::c;

    expect(flag & my_enum::a);
    expect(flag & my_enum::b);
    expect(flag & my_enum::c);

    expect(flag & my_enum::a);
    expect(flag & my_enum::b);
    expect(flag & my_enum::c);
};

// NOLINTNEXTLINE
suite<"wrapped"> wrapped_suite = []()
{
    auto flag = my_enum::a | my_enum::b;
    flag |= my_enum::c;

    expect(flag & my_enum::a);
    expect(flag & my_enum::b);
    expect(flag & my_enum::c);

    flag &= ~my_enum::c;

    expect(flag & my_enum::a);
    expect(flag & my_enum::b);
    expect(not(flag & my_enum::c));

    flag &= ~my_enum::b;

    expect(flag & my_enum::a);
    expect(not(flag & my_enum::b));
    expect(not(flag & my_enum::c));

    flag = flag & ~my_enum::a;

    expect(not(flag & my_enum::a));
    expect(not(flag & my_enum::b));
    expect(not(flag & my_enum::c));

    flag |= my_enum::a | my_enum::b | my_enum::c;

    expect(flag & my_enum::a);
    expect(flag & my_enum::b);
    expect(flag & my_enum::c);

    expect(flag & my_enum::a);
    expect(flag & my_enum::b);
    expect(flag & my_enum::c);
};
