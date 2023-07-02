<div align="center">

# 🏴‍☠️ flags

A C++20 library that provides (opt-in) bit-wise operations for arbitrary `enum class`es

</div>

# ⚙️ Configuration

## Tests

```cmake
set(flags_tests OFF)
```
> If set to `ON`, *flags* will build tests.

# 📦 Installation

> **Note**  
> This library requires a C++20 capable compiler.)

- FetchContent
    ```cmake
    include(FetchContent)
    FetchContent_Declare(flags GIT_REPOSITORY "https://github.com/Curve/flags")

    FetchContent_MakeAvailable(flags)
    target_link_libraries(<YourLibrary> cr::flags)
    ```
- Git Submodule
    ```bash
    git submodule add "https://github.com/Curve/flags"
    ```
    ```cmake
    # Somewhere in your CMakeLists.txt
    add_subdirectory("<path_to_flags>")
    target_link_libraries(<YourLibrary> cr::flags)
    ```

# 📒 Examples

```cpp
#include <flags/flags.hpp>

enum class my_enum
{
    none,
    a = 1 << 0,
    b = 1 << 1,
    c = 1 << 2,
};

template <>
constexpr bool flags::enabled<my_enum> = true;

// You can now use `my_enum` for bit-wise operations!

auto flag = my_enum::a;
flag |= my_enum::b;
flag |= my_enum::c;

if (flag & my_enum::b)
{
    flag &= ~my_enum::b;
}
```

> For more examples see [tests](tests/)