#include "consteval_mutable.hpp"

namespace mutable_test1{
    template<auto>
    struct storage;
    using mut = consteval_mutable<^^storage>;

    static_assert(!mut::check());
    static_assert(!mut::check_is_value());
    static_assert(!mut::check_is_type());
    
    consteval{
        mut::put<123>();
    }

    static_assert(mut::check());
    static_assert(mut::check_is_value());
    static_assert(!mut::check_is_type());
    static_assert(mut::get_v<> == 123);

    consteval{
        mut::put<124>();
    }
    static_assert(mut::get_v<> == 124);

    consteval{
        mut::put<125>();
    }
    static_assert(mut::get_v<> == 125);

    consteval{
        mut::put<125>();
    }
    static_assert(mut::get_v<> == 125);

    consteval{
        mut::put<int>();
    }
    static_assert(!mut::check_is_value());
    static_assert(mut::check_is_type());
    static_assert(std::is_same_v<mut::get_t<>,int>);

    consteval{
        mut::put<125>();
    }
    static_assert(mut::get_v<> == 125);

    consteval{
        mut::put<void>();
    }
    static_assert(std::is_same_v<mut::get_t<>,void>);

    using func1 = decltype([](){return 1;});

    consteval{
        mut::put<func1>();
    }

    static_assert(mut::get_t<>{}() == 1);
}

namespace mutable_test2{
    template<auto>
    struct storage;
    using mut = consteval_mutable<^^storage>;

    consteval{
        mut::put<124>();
    }
    static_assert(mut::get_v<> == 124);

    const int i = 125;
    consteval{
        mut::put<i>();
    }
    static_assert(mut::get_v<> == i);
    static_assert(mut::get_v<> == 125);

    consteval{
        mut::put<double>();
    }
    static_assert(std::is_same_v<mut::get_t<>,double>);

    using func1 = decltype([](){return 1;});
    consteval{
        mut::put<func1>();
    }

    static_assert(mut::get_t<>{}() == 1);

}