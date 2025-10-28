#include "consteval_counter.hpp"

#define test(x) consteval{ \
        counter::increment();\
    }\
static_assert(counter::get() == x+1);

namespace counter_test1{
    template<auto>
    struct storage;

    using counter = consteval_counter<^^storage>;

    static_assert(counter::get() == 0);
    test(0)
    test(1)
    test(2)
    test(3)
    test(4)
    test(5)
    test(6)
    test(7)
    test(8)
    test(9)
    test(10)
    test(11)
    test(12)
    test(13)
    test(14)
    test(15)
    test(16)
}

#undef test

namespace counter_test2{
    template<auto>
    struct storage;

    using counter = consteval_counter<^^storage>;

    static_assert(counter::get() == 0);
    consteval{
        counter::increment(2);
    }
    static_assert(counter::get()==2);

    consteval{
        counter::increment(7);
    }
    static_assert(counter::get()==9);

    consteval{
        counter::increment(10);
    }
    static_assert(counter::get()==19);
}