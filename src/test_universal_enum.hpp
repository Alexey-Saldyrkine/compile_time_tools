#include "universal_enum.hpp"

namespace uni_enum_test1{
    template<auto>
    struct storage;

    using uni_enum = universal_enum<^^storage>;

    consteval{
        uni_enum::put<"alpha">();
        uni_enum::put<"a",123>();
        uni_enum::put<"a-T",int>();
    }
    static_assert(std::is_same_v<uni_enum::get_t<"a-T">,int>);
    static_assert(uni_enum::get_v<"a"> == 123);
    consteval{
        uni_enum::put<"beta">();
    }
    consteval{
        uni_enum::put<"gamma">();
    }

    static_assert(uni_enum::get<"alpha">() == 0);
    static_assert(uni_enum::get<"beta">() == 1);
    static_assert(uni_enum::get<"gamma">() == 2);
}