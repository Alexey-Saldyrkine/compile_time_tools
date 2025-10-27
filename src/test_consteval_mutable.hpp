#include "consteval_mutable.hpp"

namespace mutable_test1{
    template<auto>
    struct storage;
    using mut = consteval_mutable<^^storage>;

    static_assert(!mut::check());
    static_assert(!mut::checkV());
    static_assert(!mut::checkT());
    
    consteval{
        mut::putV<123>();
    }

    static_assert(mut::check());
    static_assert(mut::checkV());
    static_assert(!mut::checkT());
    static_assert(mut::get_v<> == 123);

    consteval{
        mut::putV<124>();
    }
    static_assert(mut::get_v<> == 124);

    consteval{
        mut::putV<125>();
    }
    static_assert(mut::get_v<> == 125);

    consteval{
        mut::putT<int>();
    }
    static_assert(!mut::checkV());
    static_assert(mut::checkT());
    static_assert(std::is_same_v<mut::get_t<>,int>);

    consteval{
        mut::putV<125>();
    }
    static_assert(mut::get_v<> == 125);

    consteval{
        mut::putT<void>();
    }
    static_assert(std::is_same_v<mut::get_t<>,void>);

    using func1 = decltype([](){return 1;});

    consteval{
        mut::putT<func1>();
    }

    static_assert(mut::get_t<>{}() == 1);
}

namespace mutable_test2{
    template<auto>
    struct storage;
    using mut = consteval_mutable<^^storage>;

    consteval{
        mut::putV<124>();
    }
    static_assert(mut::get_v<> == 124);

    const int i = 125;
    consteval{
        mut::putV<i>();
    }
    static_assert(mut::get_v<> == i);
    static_assert(mut::get_v<> == 125);

    consteval{
        mut::putT<double>();
    }
    static_assert(std::is_same_v<mut::get_t<>,double>);

    using func1 = decltype([](){return 1;});
    consteval{
        mut::putT<func1>();
    }

    static_assert(mut::get_t<>{}() == 1);

}