//#include "consteval_mutable.hpp"

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

namespace mutable_test3{
template<auto>
struct storage; // the storage type

using mut = consteval_mutable<^^storage>; // the mutable type

static_assert(mut::check() == false); // the mutabel is empty

consteval{
    mut::put<54321>(); // put a value into the mutable
}

static_assert(mut::get_v<> == 54321); // get the current stored element as a value

consteval{
    mut::put<int>(); // put a type into the mutable
}

static_assert(std::is_same_v< mut::get_t<>, int>); // get the current stored element as a type

template<int i>
constexpr int templateExample = i;

consteval{
    mut::put_refl<^^templateExample>(); // put a reflextion of a tempalte, which is niether a value or type
}

static_assert( template[:mut::get_refl():]<5> == 5 ); // get the current stored reflection and splice it into back into a tempalte

template<int i>
constexpr int templateExample2 = i+1;

consteval{
    mut::put<^^templateExample2>(); // alternatively you can put the reflection of a template as a value
}

static_assert( template[:mut::get_v<>:]<5> == 6 ); // get the current stored element as a value, which is a reflection, and splice it into back into a tempalte

static_assert(mut::get_v<0> == 54321); // get first element stored
static_assert(std::is_same_v< mut::get_t<1>, int>); // get the second element stored
}